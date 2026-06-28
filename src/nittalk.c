#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include "shell.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>

#define VERIFICATION_STR "NIT"
#define PORT 8080

// Force exactly 72 bytes structural layout
struct __attribute__((packed)) RadioHeader
{
    char verification[4];  // "NIT\x00" (4 Bytes)
    char filename[64];     // Target file name (64 Bytes)
    uint32_t payload_size; // Data size in Network Byte Order (4 Bytes)
};

void handle_nittalk(char **args)
{
    if (args[1] == NULL)
    {
        printf("Usage:\n  nittalk -listen\n  nittalk -s <IP> <filepath>\n");
        return;
    }

    if (strcmp(args[1], "-listen") == 0)
    {
        int server_fd, client_fd;
        struct sockaddr_in address;
        int opt = 1;
        int addrlen = sizeof(address);
        struct RadioHeader header;

        printf("[📻] Initializing covert listening post on port 8080...\n");

        // Step 1: Create the Socket
        if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        {
            perror("Socket creation failed");
            return;
        }

        // Avoid "Address already in use" errors
        setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

        // Define the Address Configuration
        address.sin_family = AF_INET;         // IPv4
        address.sin_addr.s_addr = INADDR_ANY; // Accept connections from any interface
        address.sin_port = htons(8080);       // Host-To-Network Short

        // Step 2: Bind the Socket
        if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
        {
            perror("Bind failed");
            close(server_fd);
            return;
        }

        // Step 3: Listen for Inbound Connections
        if (listen(server_fd, 3) < 0)
        {
            perror("Listen failed");
            close(server_fd);
            return;
        }

        printf("[📡] Beacon online. Awaiting inbound transmission...\n");

        // Step 4: Accept an incoming connection (Blocks here)
        if ((client_fd = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
        {
            perror("Accept failed");
            close(server_fd);
            return;
        }

        printf("[🔓] Target connected! Secure link established.\n");

        // --- PASTE THE NEW SYSTEM STREAM HANDLING CODE HERE ---
        int bytes_received = 0;
        int expected_header_size = sizeof(struct RadioHeader);
        char *header_ptr = (char *)&header;

        printf("[*] Synchronizing stream windows... Awaiting 72-byte header...\n");

        // Loop until we have accumulated exactly 72 bytes
        while (bytes_received < expected_header_size)
        {
            int n = recv(client_fd, header_ptr + bytes_received, expected_header_size - bytes_received, 0);
            if (n <= 0)
            {
                if (n < 0)
                    perror("[!] Header read failure");
                else
                    printf("[!] Connection severed by remote peer before header sync.\n");
                close(client_fd);
                close(server_fd);
                return;
            }
            bytes_received += n;
        }

        // Phase 1: Security Validation Check
        if (strcmp(header.verification, VERIFICATION_STR) != 0)
        {
            fprintf(stderr, "[🚨 ALERT] Verification mismatch! Malicious scan or unauthorized packet dropped.\n");
            close(client_fd);
            close(server_fd);
            return;
        }

        // Phase 2: Convert Network Byte Order back to Host Byte Order
        uint32_t file_size = ntohl(header.payload_size);
        printf("[🔓] Header Verified! Incoming File: '%s' | Size: %u bytes\n", header.filename, file_size);

        // Phase 3: Receive File Payload in Chunks
        FILE *out_file = fopen(header.filename, "wb");
        if (out_file == NULL)
        {
            perror("[!] Failed to create local file replica");
            close(client_fd);
            close(server_fd);
            return;
        }

        char buffer[1024];
        uint32_t total_payload_read = 0;

        printf("[📥] Downloading payload streams...\n");
        while (total_payload_read < file_size)
        {
            uint32_t bytes_to_read = file_size - total_payload_read;
            if (bytes_to_read > sizeof(buffer))
            {
                bytes_to_read = sizeof(buffer);
            }

            int n = recv(client_fd, buffer, bytes_to_read, 0);
            if (n <= 0)
            {
                if (n < 0)
                    perror("[!] Payload transmission collapsed midway");
                else
                    printf("[!] Connection lost before full file transmission completed.\n");
                break;
            }

            fwrite(buffer, 1, n, out_file);
            total_payload_read += n;
        }

        printf(" Success! File transmission complete. Saved as '%s' (%u/%u bytes received)\n",
               header.filename, total_payload_read, file_size);

        fclose(out_file);
        close(client_fd);
        close(server_fd);
    }
    else if (strcmp(args[1], "-s") == 0)
    {
        // Validation: Ensure we have both target IP and the filename
        if (args[2] == NULL || args[3] == NULL)
        {
            fprintf(stderr, "Usage: nittalk -s <Target_IP> <Filename>\n");
            return;
        }

        char *target_ip = args[2];
        char *filename = args[3];

        printf("[*] Preparing stealth transmission payload to %s...\n", target_ip);

        // Open and inspect the file to calculate its raw payload size
        FILE *file = fopen(filename, "rb");
        if (file == NULL)
        {
            perror("[!] Error opening local file");
            return;
        }

        // Seek to the end of the file to determine its size, then return to the beginning
        fseek(file, 0, SEEK_END);
        long raw_file_size = ftell(file);
        fseek(file, 0, SEEK_SET);

        if (raw_file_size < 0)
        {
            perror("[!] Error reading file dimensions");
            fclose(file);
            return;
        }

        // Assemble the strict 72-Byte Radio Header
        struct RadioHeader header;
        memset(&header, 0, sizeof(header));

        // Copy the friendly verification signature ("NIT\x00")
        memcpy(header.verification, VERIFICATION_STR, 3);
        header.verification[3] = '\x00';

        // Safely extract and copy just the base filename (limit to 63 chars + null terminator)
        char *base_name = strrchr(filename, '/');
        if (base_name == NULL)
        {
            base_name = filename;
        }
        else
        {
            base_name++; // Skip past the '/' characters
        }
        strncpy(header.filename, base_name, 63);
        header.filename[63] = '\x00'; // Hard boundary enforcement

        // CRITICAL: Flip host size to Network Byte Order (Big-Endian) before transmission
        header.payload_size = htonl((uint32_t)raw_file_size);

        // Network Target Routing: Initialize socket
        int sock_fd;
        struct sockaddr_in serv_addr;

        if ((sock_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        {
            perror("[!] Socket assembly failed");
            fclose(file);
            return;
        }

        serv_addr.sin_family = AF_INET;
        serv_addr.sin_port = htons(PORT);

        // Convert IP text string ("192.168.x.x") into raw binary representation
        if (inet_pton(AF_INET, target_ip, &serv_addr.sin_addr) <= 0)
        {
            fprintf(stderr, "[!] Invalid target network address architecture.\n");
            close(sock_fd);
            fclose(file);
            return;
        }

        // Establish handshaking link with the Listener
        printf("[*] Pinging target frequency... Handshaking...\n");
        if (connect(sock_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
        {
            perror("[!] Connection handshake refused");
            close(sock_fd);
            fclose(file);
            return;
        }

        printf("[+] Link established! Transmitting 72-byte radio header...\n");

        // Blast the header onto the wire
        if (send(sock_fd, &header, sizeof(struct RadioHeader), 0) < 0)
        {
            perror("[!] Header transmission collapsed");
            close(sock_fd);
            fclose(file);
            return;
        }

        printf("[+] Metadata broadcast complete.\n");
        printf("[🚀] Initiating file transmission stream...\n");
        char send_buffer[1024];
        size_t bytes_read;

        // Read from the local file in 1024-byte chunks and push them over the socket
        while ((bytes_read = fread(send_buffer, 1, sizeof(send_buffer), file)) > 0)
        {
            int total_sent = 0;
            // Handle cases where send() does not transmit the whole chunk in one go
            while (total_sent < (int)bytes_read)
            {
                int sent = send(sock_fd, send_buffer + total_sent, bytes_read - total_sent, 0);
                if (sent < 0)
                {
                    perror("[!] Data stream collapsed mid-transit");
                    close(sock_fd);
                    fclose(file);
                    return;
                }
                total_sent += sent;
            }
        }
        printf("All bytes deployed over the wire successfully.\n");
    }
    else
    {
        printf("Unknown option: %s\n", args[1]);
    }
}
