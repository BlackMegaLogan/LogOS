# LogOS

**LogOS** is a work-in-progress OS framework written in Go. It aims to create a
simplistic computing experience both with its internal workings and usability.

**LogOS** is built upon the Triple L Framework, including:

- **Lush** – The command-line interface
- **Long** – The planned backend language (coming soon)
- **Light** – GUI and graphical shell (coming soon)

## Getting Started

```bash
git clone https://github.com/BlackMegaLogan/LogOS
cd LogOS
go run cmd/logos/main.go
```

## Lush Commands

| Command | Description |
|---------|-------------|
| `s`     | Spawn a program (e.g. `s ls -la`) |
| `e`     | Echo text to the screen |
| `sys`   | Display system information under a **SystemStat** header |

Use `exit` or `quit` to leave the shell.
