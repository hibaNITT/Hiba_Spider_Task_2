# Octo-Shell

## The Story

Imagine!!! Our campus had actually been breached. Real terrorists, but not the ordinary kind — these knew exactly who they were dealing with. Fearing that students might communicate and coordinate a counter-attack, they seized control of the main server rooms and threw a digital net over the entire campus LAN. Wireshark and Tshark running in promiscuous mode across every Wi-Fi access point, sniffing every single packet. Send anything unencrypted — even a plain "hello" — and they'd intercept it, pinpoint the exact location, and that would be it. Game over. Possibly a lifetime sentence to Windows 11.

No internet. No cellular data. Just one local Wi-Fi router in the corridor, fully monitored by the enemy, and a teammate hiding somewhere else on campus with no safe way to reach them.

So ya to communicate with my teammate, We had to find a solution.

## What I Built

Standard shells leave history files and logs the adversary could trace, so even the shell itself had to be custom-built. This is `octo-shell` — built up in four levels.

### Level 1 — The Core Shell

A bare-bones REPL written in pure C. External commands (`ls`, `cat`, etc.) run via `fork()` + `execvp()`. Built-ins like `cd` are handled internally with `chdir()`. Clean memory teardown on `exit`.

### Level 2 — The Radio (`nittalk`)

A custom point-to-point file transfer tool over TCP. Every file is preceded by a strict **72-byte header** (4-byte verification tag, 64-byte filename, 4-byte payload size) so the receiver knows what's coming and when it ends. Sizes are converted with `htonl()`/`ntohl()` to survive cross-architecture byte-order differences.

### Level 3 — Encryption

Hand-rolled encryption, no libraries:

- **Key exchange:** Diffie-Hellman using modular exponentiation (square-and-multiply) to avoid overflow — both sides derive the same secret `K` without ever transmitting it.
- **Keystream:** `K` seeds a custom pseudorandom byte generator.
- **Cipher:** every byte is XOR'd against the keystream before it hits the socket, and XOR'd again on the other end to recover it. To a sniffer, it's just noise.

### Level 4 — Tripwire

A passive intrusion detector using a raw socket (`SOCK_RAW`) to inspect every incoming packet at the IP layer before the OS touches it. Compares each source IP against my teammate's known address — anything else triggers an instant, color-coded alert. My cue to move.

## Why can't we just use the usual shell and go through all this trouble???!!!

On a network the enemy fully controls, trusting an unaudited library wasn't an option. Every layer — sockets, header parsing, modular exponentiation, the cipher, the packet sniffer — was written and understood by hand.

# Sounds Interesting ryt?? Wanna try it out...Its simple just follow this steps i have mentioned in this website. Trust me its gonna be fun!!

[Guide](https://hibanitt.github.io/spider-task-2-website/)

#  Demo videos

[Level 1](https://drive.google.com/file/d/1ECC0w0aHqCJPXbryqg4ybzVGYhxjXL-D/view?usp=sharing)

[Level 2 & 3](https://drive.google.com/file/d/1XaxarL0JlMlD31t3QKRorK5TYJlz15VD/view?usp=sharing)

[Level 4](https://drive.google.com/file/d/1b3Pidx3tbQF4EtJnq291Zx7GBuxAgtn0/view?usp=sharing)


# CODECRAFTERS

### Proof

![Codecrafters Shell Challenge](./proof.png)

# How to Build and Run the code of the codecrafters challenge

## Testing level 1

### 1. Open WSL (Ubuntu)

Since this project uses POSIX headers (`unistd.h`, `sys/wait.h`), it must be compiled inside Linux.  
On Windows, launch Ubuntu via WSL:

```bash
wsl -d ubuntu
```

### 2. Navigate to the project folder

Move into the `src` directory where `main.c` is located:

```bash
cd /mnt/c/Upskilling/Hiba_spider_task2/codecrafters-shell-c/src
```

### 3. Compile the program

Use `gcc` to compile `main.c` into an executable named `shell`:

```bash
gcc main.c -o shell
```

If compilation succeeds, you’ll see a new file called `shell` in the directory.

### 4. Run the shell

Execute the program:

```bash
./shell
```

You should now see your custom shell prompt:

```
$
```

### 5. Test builtins and external commands

Try out the builtins:

```bash
echo hello world
cd src


```

And external programs:

```bash
ls -l
cat main.c
grep int main.c
```

### 6. Exit the shell

Type:

```bash
exit
```

to quit.

---



# Important

- **Do not compile/run in Git Bash or MinGW**  
  These environments don’t support POSIX headers. Always use **WSL Ubuntu**.

- **Exec format error**  
  If you see this error, it means you tried to run a Linux binary from Windows. Switch back to **WSL**.

- **Codecrafters tests**  
  They will run your program in a Linux environment, so WSL matches the expected setup.

- **Compile on Linux**  
  Use Ubuntu (WSL or a VM).

---

## Install Build Tools

```bash
sudo apt update
sudo apt install build-essential

