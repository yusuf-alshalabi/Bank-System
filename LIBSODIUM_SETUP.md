# 🛡 Libsodium Setup Guide

Complete installation guide for the *Libsodium* cryptographic library required by the *CRUD Bank System* (v2.0.0, OOP).

---

## 📖 What is Libsodium?

*Libsodium* is a modern, easy-to-use software library for encryption, decryption, signatures, password hashing, and more. It is a portable, cross-compilable, installable, and packageable fork of *NaCl*, with a compatible API.

The Bank System uses Libsodium for:

| Feature | Libsodium API |
|:--------|:--------------|
| **Password hashing** | `crypto_pwhash` / `crypto_pwhash_str_verify` (Argon2id) |
| **Session encryption** | `crypto_secretbox_easy` / `crypto_secretbox_open_easy` (XChaCha20-Poly1305) |
| **Secure randomness** | `randombytes_buf` / `randombytes_random` |
| **Secure memory wipe** | `sodium_memzero` |
| **Initialization** | `sodium_init` (via the `SodiumRaiiGuard` RAII helper) |

---

## 📋 System Requirements

* *Linux*: Ubuntu 16.04+, CentOS 7+, or equivalent
* *macOS*: 10.12 Sierra or newer
* *Windows*: Windows 7 or newer
* *Compiler*: **C++17** or later
  * GCC **7.0+** (or newer)
  * Clang **5.0+** (or newer)
  * MSVC **2017+** / Visual Studio **2019 / 2022** (v142 / v143)
* *Memory (RAM)*: 100MB free for compilation
* *Storage*: 50MB free space

> 📌 **Project standard:** The Bank System v2.0.0 is built with the C++17 standard and a header-only OOP architecture. `Main.cpp` is the single translation unit (`.cpp`) that pulls in the entire layered class hierarchy (`Core/` and `Screens/`).

---

## 💻 Linux Installation

### Ubuntu/Debian

```bash
sudo apt update
sudo apt install libsodium-dev
```
### Fedora/CentOS/RHEL
```bash
sudo dnf install libsodium-devel
```
### CentOS 7/RHEL 7
```bash
sudo yum install epel-release
sudo yum install libsodium-devel
```

### Arch Linux
```bash
sudo pacman -S libsodium
```

## 🍎 macOS Installation
### Using Homebrew
```bash
brew update
brew install libsodium
```
### Using MacPorts
```bash
sudo port install libsodium
```

## 🪟 Windows Installation
### Method 1: MSYS2 (Recommended for MinGW)
```bash
# Update package database
pacman -Syu

# Install libsodium for 64-bit MinGW
pacman -S mingw-w64-x86_64-libsodium
```
### Method 2: vcpkg (Recommended for Visual Studio)
```bash
vcpkg install libsodium
```
> The project solution (`BankSystem.sln`) is preconfigured for vcpkg at `C:\vcpkg\installed\x64-windows\include` and links `libsodium.lib`.

### Method 3: Pre-built Binaries
 * Download from the Libsodium Releases page.
 * Extract the archive to a location like C:\libsodium.
 * Add the necessary paths to your compiler's include and library search paths.

## 🔧 Manual Compilation
### Download Source
```bash
wget https://download.libsodium.org/libsodium/releases/LATEST.tar.gz
tar -xzf LATEST.tar.gz
cd libsodium-stable
```
### Compile & Install
```bash
./configure
make
sudo make install
```
### Update Library Cache (Linux)
```bash
sudo ldconfig
```

## 🔗 Linking with C++

> ⚠️ **v2.0.0 Note:** The entry point is `Main.cpp`. The codebase is now a **header-only OOP architecture** — `Main.cpp` includes the layered `Core/` and `Screens/` headers directly.  
> Use `Main.cpp` in all compilation commands below and compile with **C++17**.

| Purpose | Command |
|---|---|
| Basic Compilation | `g++ -o BankSystem Main.cpp -std=c++17 -lsodium` |
| With Optimizations | `g++ -o BankSystem Main.cpp -std=c++17 -O2 -lsodium` |
| With Debug Information | `g++ -o BankSystem Main.cpp -std=c++17 -g -lsodium` |
| With All Warnings | `g++ -o BankSystem Main.cpp -std=c++17 -Wall -Wextra -lsodium` |

> 🪟 **Visual Studio (Windows):** open `BankSystem.sln`, choose **x64** and **Release/Debug**, then build. The project sets `<LanguageStandard>stdcpp17</LanguageStandard>` and adds `libsodium.lib` to `AdditionalDependencies`.

## ✅ Verification Test
### Create a file named test_sodium.cpp:
```cpp
#include <sodium.h>
#include <iostream>

int main() {
    if (sodium_init() < 0) {
        std::cout << "Libsodium initialization failed!" << std::endl;
        return 1;
    }
    std::cout << "✔ Libsodium installed successfully!" << std::endl;
    std::cout << "✔ Version: " << sodium_version_string() << std::endl;
    return 0;
}
```

### Compile and run the test:
```bash
g++ -o test_sodium test_sodium.cpp -std=c++17 -lsodium
./test_sodium
```
### Expected Output:
```
✔ Libsodium installed successfully!
✔ Version: 1.0.18
```
(Note: The version number may vary)

⚠️ Note: The Bank System calls `sodium_init()` once via the `SodiumRaiiGuard` RAII class in `CryptoEngine.h`, so no manual initialization is required in application code.

## 🐛 Troubleshooting
**Error:** `sodium.h: No such file or directory`  
**Solution:** Install the development packages (-dev or -devel).
```bash
# Ubuntu/Debian
sudo apt install libsodium-dev

# Fedora/CentOS
sudo dnf install libsodium-devel
```

**Error:** `undefined reference to 'sodium_init'`  
**Solution:** The `-lsodium` flag must be placed at the **end** of the compilation command.
```bash
# Wrong
g++ -lsodium -o program Main.cpp

# Correct
g++ -o program Main.cpp -std=c++17 -lsodium
```

**Error:** `libsodium not found on macOS`  
**Solution:** Update Homebrew and reinstall the library.
```bash
brew update
brew reinstall libsodium
```

**Error:** `Library not found at runtime`  
**Solution:** Update the dynamic linker run-time bindings cache.
```bash
sudo ldconfig
```

**Error:** `C++17 features not recognized` (e.g. `std::optional`, `std::filesystem`)  
**Solution:** Ensure you pass `-std=c++17` (or `-std=c++20`). The v2.0.0 OOP build requires C++17 or later.
```bash
g++ -o BankSystem Main.cpp -std=c++17 -lsodium
```

### Windows: MinGW Path Issues
**Solution:** Add the MinGW binary path to the system's PATH environment variable.
```bash
export PATH=/mingw64/bin:$PATH
```

## 🔍 Advanced Configuration
### Custom Installation Prefix
```bash
./configure --prefix=/usr/local/custom
make
sudo make install
```
### Static Linking
```bash
g++ -o BankSystem Main.cpp -std=c++17 -static -lsodium
```
### Specific Library Path
```bash
g++ -o BankSystem Main.cpp -std=c++17 -L/usr/local/lib -I/usr/local/include -lsodium
```

## 📚 Library Information
| Action | Command |
|---|---|
| Check Linking Flags | `pkg-config --libs libsodium` |
| Check Include Flags | `pkg-config --cflags libsodium` |
| Check Version | `sodium-config --version` |
| Check Features | `sodium-config --help` |

## 🔒 Security Notes
 * Libsodium is extensively audited and is considered secure for production use.
 * Regular updates are recommended to obtain security patches.
 * Compiled binaries should be verified against known checksums.
 * Use official repositories whenever possible.
 * The Bank System stores encryption keys in the per-user profile directory (`%LOCALAPPDATA%\BankSystem` on Windows) and wipes session files with a 3-pass overwrite on logout.

## 📞 Support Resources
 * Official Documentation
 * GitHub Repository
 * Mailing List

## ✅ Final Checklist
 * [x] Libsodium installed successfully
 * [x] Development headers available
 * [x] Library linked correctly in compilation (`-lsodium` at the end)
 * [x] Compiler supports **C++17** (`-std=c++17`)
 * [x] Test program runs without errors
 * [x] Bank System compiles with `Main.cpp -std=c++17 -lsodium`

> Note: After successful installation, you can compile the Bank System using the commands detailed in the main project README file.