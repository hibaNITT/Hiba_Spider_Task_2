Spider_Task_2/
├── include/ # Header files (.h)
├── src/ # Source code files (.c)
├── tests/ # Dummy files for testing transmission
├── Makefile # Automation build script
└── README.md # Setup and compilation instructions

# REFER TO THIS WEBSITE FOR EASY UNDERSTANDING

[Guide](https://hibanitt.github.io/spider-task-2-website/)

# Demo videos

[Level 1](https://drive.google.com/file/d/1ECC0w0aHqCJPXbryqg4ybzVGYhxjXL-D/view?usp=sharing)

[Level 2 & 3](https://drive.google.com/file/d/1XaxarL0JlMlD31t3QKRorK5TYJlz15VD/view?usp=sharing)

[Level 4](https://drive.google.com/file/d/1b3Pidx3tbQF4EtJnq291Zx7GBuxAgtn0/view?usp=sharing)

# HOW TO RUN

cd /src

wsl -d ubuntu
gcc -Iinclude src/main.c src/execute.c -o octo-shell
./octo-shell

## CODECRAFTERS

### Proof

![Codecrafters Shell Challenge](./proof.png)

# How to Build and Run

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

### Notes

- **Do not compile/run in Git Bash or MinGW** — they don’t support POSIX headers. Always use WSL Ubuntu.
- If you see `Exec format error`, it means you tried to run a Linux binary from Windows. Switch back to WSL.
- Codecrafters tests will run your program in a Linux environment, so WSL matches the expected setup.

Compile on Linux (Ubuntu, WSL, or a VM).

Install build tools:

bash
sudo apt update
sudo apt install build-essential
Then compile with gcc filename.c -o output.

TO TEST EVEL 2
Phase 1: Recompilation
Run your compilation routine to pull in the newly expanded src/nittalk.c logic:

Bash
gcc -Wall -Wextra -Iinclude src/main.c src/execute.c src/nittalk.c -o octo-shell
Phase 2: Simulating the Exchange
Open two separate terminal windows side by side to simulate Terminal A (Listener) and Terminal B (Sender).

In Terminal A (The Listener):
Launch your shell and prime the listening socket:

Bash
./octo-shell
octo-shell$ nittalk -listen
You should see your beacon switch online, showing it is bound to port 8080 and blocking inside the accept() system call, waiting for traffic.

In Terminal B (The Sender):
Create a test file containing some dummy data, launch your shell, and blast it over to localhost (127.0.0.1):

Bash
echo "SPIDER network data stream test transmission." > secrets.txt
./octo-shell
octo-shell$ nittalk -s 127.0.0.1 secrets.txt

## How to test level 2

Here’s your workflow neatly formatted into **Markdown (MD)** so you can drop it straight into a README or documentation file:

````markdown
# Octo-Shell Transmission Workflow

## Step 1: Compile the Project

Make sure your changes are properly compiled. Run your project's build command or invoke `gcc` from your root directory:

```bash
make clean && make
```
````

Or, if you don't have a Makefile configured yet, compile the source files manually:

```bash
gcc -g -Iinclude src/main.c src/execute.c src/nittalk.c -o octo-shell
```

---

## Step 2: Set Up the Listening Post (Terminal A)

Launch your shell and start it up in listener mode. This will open up port 8080 and sit in a blocking state waiting for bytes.

Fire up the shell:

```bash
./octo-shell
```

Execute the listener command at your custom prompt:

```bash
octo-shell$ nittalk -listen
```

Expected output:

```plaintext
 Initializing covert listening post on port 8080...
 Beacon online. Awaiting inbound transmission...
```

---

## Step 3: Trigger the Stealth Transmission (Terminal B)

Open a separate terminal window or tab, navigate to your project directory, and create a dummy text file to send.

Generate a test file containing some recognizable text:

```bash
echo "This is a high-security transmission passing through our continuous stream cipher." > test_payload.txt
```

Launch your second shell instance:

```bash
./octo-shell
```

Run the sender command targeting your local loopback IP address:

```bash
octo-shell$ nittalk -s 127.0.0.1 test_payload.txt
```

Sender output:

```plaintext
[*] Preparing stealth transmission payload to 127.0.0.1...
[+] Link established! Transmitting 72-byte radio header...
[+] Metadata broadcast complete.
 Initiating file transmission stream...
All bytes deployed over the wire successfully.
```

Listener output:

```plaintext
 Target connected! Secure link established.
[*] Synchronizing stream windows... Awaiting 72-byte header...
 Header Verified! Incoming File: 'test_payload.txt' | Size: 84 bytes
 Downloading payload streams...
 Success! File transmission complete. Saved as 'test_payload.txt' (84/84 bytes received)
```

---

## Step 4: Verify the Results

Check if the file was written out successfully without corruption by viewing it in your terminal:

```bash
cat test_payload.txt
```

```

Would you like me to also add **syntax highlighting for commands vs. outputs** (like `bash` for commands and `plaintext` for logs) throughout the MD so it looks polished in GitHub?
```

# Level 3

## Step 1 Testing Guide (Diffie–Hellman Key Exchange)

Terminal 1 (Receiver)
./octo-shell
nittalk -listen 4443

Expected:

[📡] Beacon online. Awaiting inbound transmission...
Terminal 2 (Sender)

Create a small test file:

echo "Hi" > test.txt

Run:

./octo-shell
nittalk -s 127.0.0.1 test.txt

(or use your updated syntax if you've modified the parser).

Expected sender output:

Preparing transmission...
Connection established...
Shared Secret: ####
Header transmitted...
File transmitted successfully.

Expected receiver output:

Target connected...
Shared Secret: ####
Header verified...
Receiving file...
Transmission complete.
✅ Verify
Both terminals print a Shared Secret.
The numbers must be identical.

Example:

Sender

Shared Secret: 1571

Receiver

Shared Secret: 1571

If they're the same, the Diffie–Hellman exchange is working.

## Step 2- testing

Test

Compile:

make

Run sender and receiver again.

Expected

Sender:

Shared Secret: 1571
Keystream: 186 209 252 43 142 149 240 ...

Receiver:

Shared Secret: 1571
Keystream: 186 209 252 43 142 149 240 ...

The entire sequence should match.

## step 3 testing

Create a test file
echo "Hi!" > test.txt

make

Terminal 1 (Receiver)
./octo-shell
nittalk -listen 4443

Expected:

[📡] Beacon online. Awaiting inbound transmission...
Terminal 2 (Sender)
./octo-shell
nittalk -s 127.0.0.1 test.txt 4443

Expected:

Keystream: 206 213 48 207 226 217 100 115 54 29 (something)
Shared Secret: ####
File transmitted successfully.

Receiver:

Keystream: 206 213 48 207 226 217 100 115 54 29 (something)
Shared Secret: ####
File transmission complete.
Verify

Open the received file:

cat test.txt

It should print

Hi!

# Level 4

Terminal 1: The Guard (Tripwire)
This is where you run your detection system.

Compile the code:

Bash
gcc src/tripwire.c -o bin/tripwire
Run the Tripwire:

Bash
sudo ./bin/tripwire
(The terminal should say "Tripwire Active. Monitoring traffic...")

Terminal 2: The "Friendly" Listener (Receiver)
This simulates your teammate's machine waiting for a connection.

Open a port to act as the "Receiver":

Bash
nc -l 8080
(This terminal will now just blink, waiting for data. Keep it open.)

Terminal 3: The "Teammate" (The Sender)
This acts as the authorized connection.

Connect to your own machine (Terminal 2) using your IP:
(Replace [YOUR_IP_ADDRESS] with your actual machine's IP, e.g., 172.17.66.xx. You can find it by typing hostname -I in any terminal).

Bash
nc [YOUR_IP_ADDRESS] 8080
Type anything (e.g., "Hello teammate") and hit Enter.

The Verification Checklist
Check Terminal 1: Do you see Green text saying [*] Authorized traffic from: [YOUR_IP]?

If yes: Your whitelist filter is working perfectly.

Check Terminal 1 (The Intruder Test): Now, keep everything running and go back to Terminal 3. Instead of typing, just press Ctrl+C to close the connection. Then, go to a fourth terminal (or just use one of your existing ones) and run:

Bash
curl -I https://www.google.com
Final Check: Did Terminal 1 immediately switch to Red text showing [!] ALERT: Intruder detected!?

If you see Green for the nc connection and Red for the curl command

## Testing with another laptop instead of anothre terminal on the same laptop

Phase 1: Prepare Laptop A (The Sentinel)
This laptop runs your C code.

Find your IP: Open a terminal and run hostname -I. Note this down (e.g., 172.17.66.10).

Ensure Tripwire is ready:

Bash

# Navigate to your project folder

cd path/to/your/project

# Compile

gcc src/tripwire.c -o bin/tripwire

# Run with sudo

sudo ./bin/tripwire
Keep this terminal open. It will show the Green/Red alerts.

Phase 2: Setup Laptop B (The Intruder/Teammate)
we will use a direct method that works even if you don't have WSL installed yet. You can use PowerShell directly, which has built-in networking tools.

Open PowerShell: Search for "PowerShell" in the Start menu, right-click, and Run as Administrator.

Verify connection: Ping Laptop A to ensure they see each other on the Wi-Fi:

PowerShell
ping [IP_OF_LAPTOP_A]
(If this fails, ensure both laptops are on the same Wi-Fi and that the "Network Profile" in Windows is set to "Private" rather than "Public".)

Phase 3: The Testing Steps
Now that you have your two laptops ready, follow these exact steps to see the result.

Test 1: The "Teammate" (Green Alert)
On Laptop A: You need a process listening for a connection. Run this in a new terminal window:

Bash
nc -l 8080
On Laptop B: Run this to connect to Laptop A:

PowerShell

# Use the Netcat equivalent in PowerShell

Test-NetConnection -ComputerName [IP_OF_LAPTOP_A] -Port 8080
(Or, if you have netcat installed on Laptop B, just use nc [IP_OF_LAPTOP_A] 8080).

Watch Laptop A: The tripwire terminal should print a Green message: [*] Authorized traffic from: [IP_OF_LAPTOP_B].

Test 2: The "Intruder" (Red Alert)
On Laptop A: Keep the tripwire running.

On Laptop B: Generate unauthorized traffic by trying to connect to a port that you aren't using:

Bash

# Try connecting to port 22 (SSH) or any random port

Test-NetConnection -ComputerName [IP_OF_LAPTOP_A] -Port 22
Watch Laptop A: The tripwire terminal should immediately print a Red message: [!] ALERT: Intruder detected! Source: [IP_OF_LAPTOP_B].

Critical Checklist for Success:
Same Network: Both laptops must be on the same Wi-Fi. If you are using a mobile hotspot, make sure both are connected to the same phone.

Firewall: If you get "Request Timed Out" or "Connection Refused," Laptop A's Windows Firewall is likely blocking the incoming traffic.

Quickest test: Temporarily turn off the firewall on Laptop A (Control Panel > System and Security > Windows Defender Firewall > Turn Windows Defender Firewall on or off). Don't forget to turn it back on after your test!

IP Addressing: Always use hostname -I (on Linux) or ipconfig (on Windows) to get the correct current IP, as they can change if you reconnect to Wi-Fi.

Does your ping command succeed between the two laptops? If ping works, the Tripwire test will definitely work.

ipconfig on laptop a to get windows wifi ip

# WSL Network Connectivity Troubleshooting Guide

## Problem Encountered

Laptop B was unable to ping Laptop A or communicate with the WSL instance.

### Symptoms

- `ping 192.168.1.39` → Request timed out.
- `Test-NetConnection` → Ping failed.
- WSL IP (`172.17.66.30`) was visible, but communication from another laptop was not possible.

---

# Step 1: Verify WSL IP

Inside WSL, check the IP address:

```bash
ip a
```

or

```bash
hostname -I
```

Result:

```
WSL IP = 172.17.66.30
```

---

# Step 2: Verify Windows Wi-Fi IP

In Windows PowerShell:

```powershell
ipconfig
```

Result:

```
Wi-Fi IP = 192.168.1.39
```

---

# Step 3: Test Network Connectivity

From Laptop B:

```powershell
ping 192.168.1.39
```

Result:

```
Request timed out
```

This indicated that the problem was with Windows networking, not WSL.

---

# Step 4: Check Network Profile

Run:

```powershell
Get-NetConnectionProfile
```

Result:

```
NetworkCategory : Public
```

A Public network blocks many incoming connections by default.

---

# Step 5: Change Network to Private

Run as Administrator:

```powershell
Set-NetConnectionProfile -InterfaceAlias "Wi-Fi" -NetworkCategory Private
```

Verify:

```powershell
Get-NetConnectionProfile
```

Result:

```
NetworkCategory : Private
```

---

# Step 6: Enable Ping Through Firewall

Run:

```powershell
Enable-NetFirewallRule -Name FPS-ICMP4-ERQ-In
```

This allows incoming ICMP (ping) requests.

---

# Step 7: Test Again

From Laptop B:

```powershell
ping 192.168.1.39
```

Result:

```
Reply from 192.168.1.39
```

Communication between the two laptops was successfully established.

---

# Root Cause

The Windows Wi-Fi network was configured as **Public**, causing Windows Firewall to block incoming ICMP and network traffic between devices. Since Laptop B could not even reach Windows on Laptop A, the issue was unrelated to WSL.

---

# Solution Summary

1. Verified WSL IP using `ip a`.
2. Verified Windows Wi-Fi IP using `ipconfig`.
3. Tested connectivity using `ping`.
4. Checked network profile using `Get-NetConnectionProfile`.
5. Changed the network from **Public** to **Private**.
6. Enabled the Windows Firewall rule for ICMP (`Enable-NetFirewallRule -Name FPS-ICMP4-ERQ-In`).
7. Confirmed successful communication by pinging Laptop A from Laptop B.

This resolved the connectivity issue and allowed communication with the WSL environment.
