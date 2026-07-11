#include <stdio.h>      // Standard Input/Output (for printf)
#include <stdlib.h>     // Standard Library (for malloc)
#include <string.h>     // String functions (for memset)
#include <sys/socket.h> // The core socket functions
#include <netinet/ip.h> // Contains the 'struct iphdr' blueprint
#include <arpa/inet.h>  // Contains 'inet_ntoa' to make IPs readable

#define BUFFER_SIZE 65536 // The maximum size of an IP packet (64KB)

int main()
{

    //  setup
    int sock;
    unsigned char *buffer = malloc(BUFFER_SIZE); // Reserve memory to hold raw data
    struct sockaddr saddr;
    socklen_t saddr_len = sizeof(saddr);

    // Create a raw socket to capture TCP packets
    sock = socket(AF_INET, SOCK_RAW, IPPROTO_TCP);
    if (sock < 0)
    {
        perror("Socket Error (Did you run as sudo?)");
        return 1;
    }

    printf("Tripwire Active. Monitoring traffic...\n");

    while (1)
    {
        // Wait for incoming packet
        int data_size = recvfrom(sock, buffer, BUFFER_SIZE, 0, &saddr, &saddr_len);

        // Cast the buffer to an IP header struct
        struct iphdr *iph = (struct iphdr *)buffer;

        // Print the Source IP address
        struct sockaddr_in source;
        memset(&source, 0, sizeof(source));
        source.sin_addr.s_addr = iph->saddr;

        printf("Incoming packet from: %s\n", inet_ntoa(source.sin_addr));
    }

    return 0;
}