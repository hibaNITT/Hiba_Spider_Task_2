# Scenario 1: Secure File Transfer Between Two Trusted Systems

## Overview

This scenario demonstrates a secure file transfer between two trusted systems using the `nittalk` utility. Before any file is transmitted, both the sender and receiver establish a secure communication channel by performing a Diffie–Hellman key exchange

This allows both systems to independently generate the same secret key without transmitting it over the network. The shared secret is then used to create a pseudorandom keystream, which encrypts the file before transmission and decrypts it upon reception.

---

## Step-by-Step Working

### 1. Receiver Starts Listening

The receiver first starts the listener using the `nittalk -listen` command. It creates a TCP socket, binds it to a specific port, and waits for an incoming connection from a sender.

At this stage, the receiver is idle but ready to accept secure file transfers.

---

### 2. Sender Initiates the Connection

The sender executes the `nittalk` command with the destination IP address and the file to be transferred.

A TCP connection is established between the sender and receiver. Once the connection is successful, both systems begin the secure key exchange process.

---

### 3. Diffie–Hellman Key Exchange

Each system generates its own private key randomly.

Using the common public parameters (generator and prime number), both systems calculate their corresponding public keys.

The public keys are then exchanged through the TCP connection.

Although the public keys travel across the network, the private keys never leave their respective systems.

After receiving the other system's public key, both the sender and receiver independently compute the shared secret.

Even though each system performs the calculation separately, both obtain the exact same shared secret.

This shared secret is never transmitted over the network, making it impossible for an attacker to directly obtain it simply by observing the communication.

---

### 4. Keystream Generation

The shared secret acts as the seed for a Linear Congruential Generator (LCG).

The LCG generates a sequence of pseudorandom bytes known as the **keystream**.

Since both systems start with the same shared secret, they generate identical keystreams independently.

This synchronized keystream enables secure encryption and decryption without exchanging any additional encryption key.

---

### 5. File Encryption

The sender reads the file in small chunks.

Each byte of the file is encrypted using the XOR operation with the corresponding byte from the generated keystream.

Mathematically,

```
Ciphertext = Plaintext XOR Keystream
```

The encrypted data appears completely different from the original file and is therefore unintelligible to anyone intercepting the transmission.

---

### 6. Secure Transmission

The encrypted data is transmitted through the established TCP connection.

Along with the encrypted file contents, a header containing metadata such as the filename and file size is also transmitted.

Since only encrypted bytes are sent across the network, the original file contents remain protected during transmission.

---

### 7. File Decryption

The receiver reads the encrypted bytes from the network.

Using the same shared secret obtained during the Diffie–Hellman exchange, it generates the identical keystream.

The receiver then applies the XOR operation again:

```
Plaintext = Ciphertext XOR Keystream
```

Because XOR is reversible, applying the same keystream restores the original file exactly.

---

### 8. File Reconstruction

After decrypting every byte, the receiver writes the recovered data into a new file using the filename received in the header.

The reconstructed file is identical to the original file sent by the sender, demonstrating that the encryption and decryption processes were successful.

---

# Why This Approach Is Secure

The security of this protocol comes from the fact that the encryption key is **never directly transmitted**.

Instead:

- Each system generates its own private key.
- Only public keys are exchanged.
- Both systems independently calculate the same shared secret.
- The shared secret is used only as a seed to generate the encryption keystream.
- The transmitted data consists solely of encrypted bytes.

Even if an attacker captures every packet exchanged over the network, they can only observe the public keys and the encrypted file. Without access to either private key, generating the shared secret is computationally difficult, making it impractical to recover the original file.

---

# Flow Diagram

```text
                Sender                              Receiver
                   │                                    │
                   │ Generate Private Key               │
                   │                                    │ Generate Private Key
                   ▼                                    ▼
             Generate Public Key                 Generate Public Key
                   │                                    │
                   ├──────── Exchange Public Keys ──────┤
                   │                                    │
                   ▼                                    ▼
           Generate Shared Secret              Generate Shared Secret
                   │                                    │
                   ▼                                    ▼
          Generate Keystream                 Generate Keystream
                   │                                    │
                   ▼                                    ▼
        Encrypt File Using XOR          Decrypt File Using XOR
                   │                                    │
                   ├────── Encrypted Data ─────────────►│
                   │                                    │
                   ▼                                    ▼
           File Successfully                  Original File
             Transmitted                        Reconstructed
```

---

## Conclusion

This scenario demonstrates how secure communication can be achieved without ever transmitting the encryption key. By combining the Diffie–Hellman key exchange with a keystream-based XOR encryption mechanism, the system ensures confidentiality while maintaining efficient file transfer. The receiver successfully reconstructs the original file because both parties independently generate the same shared secret and identical keystream, allowing the encrypted data to be decrypted correctly. This approach illustrates the fundamental principles used in many modern secure communication protocols.
