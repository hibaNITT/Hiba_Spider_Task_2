# Technical Write-Up – NITTalk Secure File Transfer Project

## Introduction

## In this project, I implemented **NITTalk**, a custom file transfer feature integrated into a shell environment. The goal was to build a system that can send files between two machines over a network while gradually adding security features such as a custom protocol header, key exchange, and stream encryption. The implementation follows a client-server architecture where one terminal listens for incoming connections and another terminal sends files.

# 1. Overall Architecture

The project operates in two modes:

### Listener Mode

```bash
nittalk -listen
```

The listener acts as the receiver. It:

1. Creates a socket.
2. Binds it to port 8080.
3. Waits for incoming connections.
4. Receives metadata and file contents.
5. Reconstructs the file on disk.

### Sender Mode

```bash
nittalk -s <IP> <filename>
```

The sender:

1. Opens the target file.
2. Builds a custom protocol header.
3. Connects to the receiver.
4. Performs a key exchange.
5. Encrypts and transmits the file.

---

# 2. Mathematical Approach Used for Key Exchange

To prevent transmitting encryption keys directly over the network, I implemented a **Diffie–Hellman style key exchange**. The main idea is that both systems independently calculate the same secret key without ever sending that secret itself across the network.

## Public Parameters

Both machines know:

- Prime number (`p`) = 9973
- Generator (`g`) = 5

These values are public and are hardcoded into the project.

## Step 1: Generate Private Keys

Each machine generates its own random private key:

```text
Sender:   a
Receiver: b
```

These values are never transmitted.

## Step 2: Generate Public Keys

Using modular exponentiation:

```text
A = g^a mod p
B = g^b mod p
```

Only A and B are exchanged over the network.

## Step 3: Compute Shared Secret

Sender computes:

```text
K = B^a mod p
```

Receiver computes:

```text
K = A^b mod p
```

Both sides arrive at the exact same value:

```text
Shared Secret = K
```

## without ever sending K directly.

## Why Modular Exponentiation Was Necessary

A normal power function such as:

```c
pow(g, a)
```

would quickly overflow because the intermediate values become extremely large.

To avoid this, I used the **Square-and-Multiply Modular Exponentiation algorithm**, which applies the modulo operation during every step of the calculation. This keeps numbers within safe limits and allows efficient computation of:

```text
g^a mod p
```

even for large exponents.

---

# 3. Keystream Generation and Encryption

After establishing the shared secret, the project converts that secret into a stream of pseudorandom bytes.

I used a **Linear Congruential Generator (LCG)**.

Formula:

```text
next = (A × current + C) mod M
```

The shared secret becomes the initial seed.

Example:

```text
Shared Secret = 1571

1571
↓
92
↓
13
↓
244
↓
87
↓
166
```

Both sides generate the exact same byte sequence because they start with the same seed.

## Encryption

Every outgoing file byte is encrypted using XOR:

```text
Cipher = Plaintext XOR KeystreamByte
```

## Decryption

The receiver performs:

```text
Plaintext = Cipher XOR KeystreamByte
```

Because:

```text
X XOR K XOR K = X
```

## the original file is restored perfectly.

# 4. Custom 72-Byte Protocol Header

Since TCP is just a continuous stream of bytes, the receiver needs a way to identify where the file metadata ends and the actual file begins.

To solve this, I designed a fixed-size 72-byte header.
Structure:

```c
struct RadioHeader
{
    char verification[4];
    char filename[64];
    uint32_t payload_size;
};
```

Layout:

| Field               | Size     |     |
| ------------------- | -------- | --- |
| Verification String | 4 Bytes  |     |
| Filename            | 64 Bytes |     |
| Payload Size        | 4 Bytes  |     |
| Total               | 72 Bytes |

---

# 5. Edge Cases Handled

## A. Struct Padding

One major issue in C is **struct padding**.

Compilers often insert hidden bytes between fields to improve memory alignment.

Without protection:

```c
struct RadioHeader
```

might become larger than expected, causing protocol mismatches between different systems.

To prevent this, I used:

```c
__attribute__((packed))
```

This forces all fields to be stored directly next to each other.

As a result:

```text
Header Size = Exactly 72 Bytes
```

## on every machine.

## B. TCP Stream Fragmentation

TCP does not guarantee that a single `recv()` call returns all requested bytes.

For example:

```text
Expected Header = 72 Bytes

First recv()  -> 40 Bytes
Second recv() -> 32 Bytes
```

If the program assumed the first read contained the complete header, the metadata would be corrupted.

To fix this, I implemented a strict accumulation loop that continues reading until exactly 72 bytes have been received:

```text
while(bytes_received < 72)
```

## This guarantees complete header reconstruction.

## C. Buffer Constraints

The file is never loaded fully into memory.

Instead, transmission occurs in:

```c
char buffer[1024];
```

Advantages:

- Lower memory usage
- Supports larger files
- Prevents excessive RAM consumption

The sender and receiver repeatedly process 1024-byte blocks until the entire payload has been transferred.

---

## D. Partial send()

A single call to:

```c
send()
```

is not guaranteed to transmit all data.

To handle this, the sender tracks:

```c
total_sent
```

and repeatedly calls `send()` until the full chunk has been transmitted. This prevents incomplete file transfers.

---

## E. Filename Safety

To prevent buffer overflow issues, filenames are copied using:

```c
strncpy()
```

and explicitly null-terminated:

```c
header.filename[63] = '\0';
```

This guarantees the filename never exceeds its allocated space.

## F. Endianness Handling

Different CPU architectures store numbers differently.

To ensure all systems interpret file sizes and exchanged values correctly, the project converts values between:

- Host Byte Order
- Network Byte Order

using:

```c
htonl()
ntohl()
```

---

# 6. Exact Socket System Calls Used

The following Linux networking system calls were used.

## Listener Side

### socket()

Creates a TCP socket:

````c
socket(AF_INET, SOCK_STREAM, 0)
```

### setsockopt()

Enables address reuse:

```c
setsockopt(
    server_fd,
    SOL_SOCKET,
    SO_REUSEADDR,
    &opt,
    sizeof(opt)
);
````

### bind()

Associates the socket with port 8080:

````c
bind()
```

### listen()

Places the socket into passive listening mode:

```c
listen()
```

### accept()

Accepts an incoming connection:

```c
accept()
```

### recv()

Used to receive:

- Public keys
- Header
- Encrypted file data



---

## Sender Side

### socket()

Creates the client socket.

### inet_pton()

Converts a printable IP address into binary format:

```c
inet_pton()
```

### connect()

Establishes the TCP connection:

```c
connect()
````

### send()

Used for:

- Key exchange
- Header transmission
- File transmission

### close()

## Closes sockets after communication is finished.

# 7. Raw Socket Component (Tripwire IDS Design)

For the final IDS stage, the planned approach uses a **raw socket**:

```c
socket(AF_INET, SOCK_RAW, ...)
```

## Raw sockets allow direct access to packets before they reach higher protocol layers. The IDS can inspect IP headers, extract source addresses using structures such as `struct iphdr`, and compare them against trusted addresses. Suspicious packets trigger warnings on the terminal.

# Conclusion

Through this project, I implemented a complete secure file-transfer workflow that combines socket programming, protocol design, cryptography, and defensive programming. The system uses a custom 72-byte packed header, handles TCP stream edge cases safely, performs a Diffie–Hellman-style key exchange to establish a shared secret, generates a synchronized keystream using an LCG, and encrypts transferred files using XOR-based stream encryption. Careful handling of struct padding, buffer limits, partial transmissions, and byte-order conversion helped make the solution reliable and portable across different systems. t_edu/Documents/Microsoft%20Copilot%20Chat%20Files/nittalk.c
