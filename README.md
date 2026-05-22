```
██╗  ██╗ █████╗ ███████╗██╗  ██╗██╗   ██╗ █████╗ ██╗   ██╗██╗  ████████╗
██║  ██║██╔══██╗██╔════╝██║  ██║██║   ██║██╔══██╗██║   ██║██║  ╚══██╔══╝
███████║███████║███████╗███████║╚██╗ ██╔╝███████║██║   ██║██║     ██║
██╔══██║██╔══██║╚════██║██╔══██║ ╚████╔╝ ██╔══██║██║   ██║██║     ██║
██║  ██║██║  ██║███████║██║  ██║  ╚██╔╝  ██║  ██║╚██████╔╝███████╗██║
╚═╝  ╚═╝╚═╝  ╚═╝╚══════╝╚═╝  ╚═╝   ╚═╝   ╚═╝  ╚═╝ ╚═════╝ ╚══════╝╚═╝
```
> ASCII art generated using [patorjk.com/software/taag](https://patorjk.com/software/taag/#p=display&f=Graffiti&t=hashvault&x=none&v=4&h=4&w=80&we=false) — font: **Graffiti**

# HashVault

A beginner-friendly password hashing project written in C.

This project helps you understand how password hashing works **internally** — from raw bits to authentication — without any external libraries.

## What You'll Learn

- ASCII conversion (char → integer)
- Bitwise XOR operations (`^`)
- Bit shifting (`<<`) as cheap multiplication
- Prime number seeding
- Avalanche effect (1 char change → completely different hash)
- How password authentication works without storing plaintext

## Project Structure

```
hashvault/
├── src/
│   ├── main.c      # Interactive CLI with 4 modes
│   ├── hash.c      # Core hashing logic (4 × 64-bit states)
│   └── auth.c      # Register + Login using stored hashes
├── include/
│   ├── hash.h      # Hash types and function declarations
│   └── auth.h      # Auth function declarations
├── data/
│   └── users.txt   # username:hash storage (auto-created)
└── Makefile
```

## Build & Run

```bash
make run
```

Or manually:
```bash
gcc src/*.c -Iinclude -o build/hashvault
./build/hashvault
```

## Interactive Modes

| Mode | What it does |
|------|-------------|
| Hash Explorer | Shows ASCII breakdown + hash state evolving per character |
| Avalanche Tester | Compare two strings, count how many hash bits differ |
| Register | Hash a password and store `username:hash` in users.txt |
| Login | Hash input, compare to stored hash (constant-time) |

## The Algorithm

Uses 4 independent 64-bit states seeded with primes:

```c
h1 = ((h1 << 5)  + h1) ^ ch;   // djb2-style, ×33
h2 = ((h2 << 7)  + h2) ^ ch;   // ×129
h3 = ((h3 << 3)  + h3) ^ ch;   // ×9
h4 = ((h4 << 11) + h4) ^ ch;   // ×2049
```

Final output: `%016llx` × 4 = **256-bit hex digest**

## Security Note

This is an **educational** hash — not for production.  
Real systems use bcrypt, argon2, or scrypt for passwords.
