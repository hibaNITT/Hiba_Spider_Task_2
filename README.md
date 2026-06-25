Spider_Task_2/
├── include/ # Header files (.h)
├── src/ # Source code files (.c)
├── tests/ # Dummy files for testing transmission
├── Makefile # Automation build script
└── README.md # Setup and compilation instructions

# HOW TO RUN

cd /src

wsl -d ubuntu
gcc -Iinclude src/main.c src/execute.c -o octo-shell
./octo-shell

# CODECRAFTERS

## How to Build and Run

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
type ls
exit
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
