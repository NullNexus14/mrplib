```text
███╗   ███╗██████╗ ██████╗ ██╗     ██╗██████╗
████╗ ████║██╔══██╗██╔══██╗██║     ██║██╔══██╗
██╔████╔██║██████╔╝██████╔╝██║     ██║██████╔╝
██║╚██╔╝██║██╔══██╗██╔═══╝ ██║     ██║██╔══██╗
██║ ╚═╝ ██║██║  ██║██║     ███████╗██║██████╔╝
╚═╝     ╚═╝╚═╝  ╚═╝╚═╝     ╚══════╝╚═╝╚═════╝
```

**MRP Encrypted Message Filetype System — System-level registration, encoding library, and cross-platform installer for the `.mrp` format.**

---

## Overview

mrplib provides the foundational layer for the MRP encrypted message file format. It handles system filetype registration (MIME types, icons, magic detection), provides the core encoding/decoding library, and delivers a self-contained installer per platform. The actual encryption layer is handled by `mrp-tool`.

---

## Features

### Filetype Registration
- `.mrp` extension registration on Windows, Linux, macOS, and FreeBSD
- MIME type association (`application/x-mrp`)
- Custom icon set (SVG + PNG: 48, 96, 128, 256px)
- Magic byte detection (`MRP1`)
- Thumbnailer integration

### Encoding Library (libmrplib)
- Caesar shift (+8) cipher
- Custom Morse code encoding
- Bit conversion and inversion
- Length-prefixed byte packing (3+5 bits per character)
- Full character set: A–Z, 0–9, space
- Machine-readable format specification (`mrp-spec.json`)

### CLI Tools
- `mrp-info` — File metadata and validation info
- `mrp-check` — File integrity and format validation
- `mrp-register` — System filetype registration utility

### Cross-Platform Installer
- Self-contained installer binary (static-linked)
- Embeds all resource files (MIME, icons, man pages, rules)
- One command setup

---

## Installation

### Prerequisites

- **C compiler (GCC or Clang)**
- **Python 3** (for installer resource embedding)
- **Make**

### Recommended Installation (Stable 

Download the Setup form the Releases section

### If manual installation

After downloading, run the installer:

```bash
# Linux / FreeBSD
./mrp-installer.bin --prefix=/usr/local

# macOS
./mrp-installer.bin --prefix=/usr/local

# Windows
mrp-installer.exe
```

### Repository Edition Installation

Clone the repository and build the platform of your choice:

```bash
git clone https://github.com/NullNexus14/mrplib
cd mrplib
```

Each platform directory is self-contained:

```bash
cd Linux
make
make test
```

The resulting directory structure:

```text
mrplib/
├── Windows/     # Builds for Windows (.exe, .dll)
├── Linux/       # Builds for Linux (.bin, .so)
├── macOS/       # Builds for macOS (.bin, .dylib)
└── FreeBSD/     # Builds for FreeBSD (.efl, .so)
```

---

## Building

### Linux

```bash
cd Linux
make          # builds library, CLI tools, and installer
make test     # runs unit + integration tests
make clean
```

### Windows (cross-compile or native)

```bash
cd Windows
make
```

### macOS

```bash
cd macOS
make
```

### FreeBSD

```bash
cd FreeBSD
make
```

---

## Format Pipeline

The `.mrp` format uses a five-step encoding pipeline:

| Step | Operation | Description |
|------|-----------|-------------|
| 1 | Caesar +8 | Shift letters A–Z by 8 positions |
| 2 | Morse encode | Convert to custom Morse code |
| 3 | Bit convert | Dot=0, Dash=1 |
| 4 | Bit invert | Invert all bits |
| 5 | Pack | 3-bit length prefix + 5-bit data → 1 byte |

Full specification: `files/rules/mrp-spec.json`

---

## Project Structure

| Component | Purpose |
|-----------|---------|
| `src/lib/` | Core encoding library (`libmrplib`) |
| `src/cli/` | Command-line tools |
| `src/installer/` | Self-contained installer binary and embed script |
| `files/` | Resource files (MIME, icons, man pages, rules) |
| `tests/` | Unit and integration test suite |

---

## Support

For issues, questions, or configuration assistance:

**Contact:** NullNexus14@proton.me

**Please provide:**
- Description of the issue or question
- Operating system and version
- Error message or screenshot (recommended)
- Steps to reproduce (if applicable)

---

## License

All Rights Reserved. No modifications permitted.

Copyright (c) 2026 NullNexus14.

---

## Author

**NullNexus14**  
SREA — Security Research & Engineering Architecture

---

## Disclaimer

mrplib is a filetype registration and encoding library for the MRP encrypted message format. Users are responsible for ensuring compliance with applicable laws and organizational policies when using this software.
