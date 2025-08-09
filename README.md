# MultiPatcher

MultiPatcher is a simple command-line tool for applying patch files (such as `.ips` and `.bps`) to files.
## Features
- IPS patching: Apply `.ips` patches to ROM files.  
- (Planned) BPS patching: Support for `.bps` patches (coming soon).  
- Simple command-line interface.  
- Cross-platform C++ codebase.

## Usage
```bash
MultiPatcher <input_file> <patch_file>
```
- <input_file>: Path to the file you want to patch.
- <patch_file>: Path to the patch file (.ips supported).

### Example:

```bash
MultiPatcher game.nes patch.ips
```
This will create a patched file in the same directory, named output.xxx.

## Building
### Prerequisites
- C++17 compatible compiler (e.g., MSVC, GCC, Clang)
- CMake (recommended)

### Build steps
```bash
git clone https://github.com/eduz1/MultiPatcher
cd MultiPatcher
cmake -S . -B build
cmake --build build --config Release
```

## Testing
Done using GoogleTest.
