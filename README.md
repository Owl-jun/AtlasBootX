# AtlasBoot

> **A Simulator-First Bootloader Validation Framework for Embedded Systems**
>
> Verify bootloader update behavior on a desktop before connecting it to target hardware.

---

## Overview

AtlasBoot is a hardware-independent bootloader simulator and validation framework for embedded systems.

The project focuses on making bootloader behavior executable and testable on a desktop. The current implementation provides a small core boot/update flow, a native simulation port, example executables, and CTest-based tests.

AtlasBoot is not a ready-to-flash MCU bootloader. It does not provide startup code, linker scripts, target flash register drivers, BSP code, transport drivers, or production security features. Those pieces belong in the final target integration.

---

## Current Implementation

The current codebase implements:

- CMake-based C++17 build
- Core boot decision logic
- Core firmware image build/parse helpers
- Simple payload checksum validation
- A/B slot metadata model
- Pending image boot flow
- Application confirmation flow
- Rollback to confirmed slot after an unconfirmed pending boot
- `IFlash` and `IApplicationLauncher` interfaces
- Native simulation port
- Memory-backed `VirtualFlash`
- Virtual flash erase/write/read range checks
- Write-before-erase protection
- Single-step erase/write failure injection hooks
- Desktop example executables
- CTest-registered core flow test executable

Implemented build targets:

```text
atlasboot
atlasboot_port_native_sim
desktop_update_sim
porting_minimal
atlasboot_core_flow_tests
```

---

## Current Repository Layout

```text
.
|-- CMakeLists.txt
|-- README.md
|-- LICENSE
|
|-- src/
|   `-- atlasboot/
|       |-- core/
|       |   |-- BootManager.hpp
|       |   |-- BootManager.cpp
|       |   |-- BootTypes.hpp
|       |   |-- Image.hpp
|       |   |-- Image.cpp
|       |   |-- UpdateManager.hpp
|       |   `-- UpdateManager.cpp
|       |
|       |-- interfaces/
|       |   |-- IFlash.hpp
|       |   `-- IApplicationLauncher.hpp
|       |
|       `-- ports/
|           |-- native_sim/
|           |   |-- NativeSimApplicationLauncher.hpp
|           |   |-- NativeSimApplicationLauncher.cpp
|           |   |-- NativeSimMemoryLayout.hpp
|           |   |-- NativeSimPort.hpp
|           |   |-- NativeSimPort.cpp
|           |   |-- VirtualFlash.hpp
|           |   |-- VirtualFlash.cpp
|           |   `-- README.md
|           |
|           `-- template/
|               |-- TargetApplicationLauncher.hpp
|               |-- TargetApplicationLauncher.cpp
|               |-- TargetFlash.hpp
|               |-- TargetFlash.cpp
|               |-- TargetMemoryLayout.hpp
|               |-- TargetPort.hpp
|               |-- TargetPort.cpp
|               `-- README.md
|
|-- examples/
|   |-- desktop_update_sim/
|   |   `-- main.cpp
|   `-- porting_minimal/
|       `-- main.cpp
|
`-- tests/
    `-- core_flow_tests.cpp
```

---

## Core Concepts

AtlasBoot currently models a two-slot update flow.

```text
confirmed slot
      |
install update into the other slot
      |
mark updated slot as pending
      |
boot pending slot
      |
application confirms the image
      |
pending slot becomes confirmed
```

If a pending image is booted but not confirmed within the allowed number of attempts, `BootManager` rolls back to the confirmed slot.

---

## Core Interfaces

The hardware-facing interfaces are intentionally small.

### Flash

```cpp
class IFlash
{
public:
    virtual bool Init() = 0;

    virtual bool Erase(uint32_t address,
                       uint32_t size) = 0;

    virtual bool Write(uint32_t address,
                       const uint8_t* data,
                       uint32_t size) = 0;

    virtual bool Read(uint32_t address,
                      uint8_t* data,
                      uint32_t size) = 0;
};
```

### Application Launcher

```cpp
class IApplicationLauncher
{
public:
    virtual bool JumpTo(uint32_t address) = 0;
};
```

The native simulator implements these target-facing concepts with desktop-safe components.

---

## Native Simulation Port

The native simulation port replaces target hardware with deterministic desktop components.

Currently implemented:

- `VirtualFlash`
- `NativeSimApplicationLauncher`
- `NativeSimMemoryLayout`
- `NativeSimPort`

`VirtualFlash` models these flash constraints:

- erased value is `0xFF`
- erase must be aligned to sector size
- erase/write/read ranges are validated
- writes can only clear bits, not set erased bits back to `1`
- next erase or write operation can be forced to fail

---

## Image Format

The current image format is intentionally small:

```cpp
struct ImageHeader {
    std::uint32_t magic;
    std::uint32_t version;
    std::uint32_t payload_size;
    std::uint32_t checksum;
};
```

The current checksum is produced by `CalculateChecksum()`. It is not a production CRC, hash, or signature scheme.

---

## Build

Requirements:

- CMake 3.16 or newer
- C++17 compiler
- A CMake generator such as Ninja or Visual Studio

Configure and build with Ninja:

```powershell
cmake -S . -B build -G Ninja
cmake --build build
```

If the build directory is already configured:

```powershell
cmake --build build
```

---

## Run Examples

Desktop update simulator:

```powershell
.\build\desktop_update_sim.exe
```

Minimal porting example:

```powershell
.\build\porting_minimal.exe
```

---

## Run Tests

Tests are registered through CTest in `CMakeLists.txt`.

```powershell
ctest --test-dir build --output-on-failure
```

The current test executable is:

```text
atlasboot_core_flow_tests
```

It currently covers:

- valid update boots as pending and can be confirmed
- invalid image is rejected
- unconfirmed pending image rolls back to confirmed slot
- virtual flash rejects writes that would set bits without erase

---

## CI Example

The registered CTest tests can be run in GitHub Actions with a workflow like this:

```yaml
name: CI

on:
  push:
  pull_request:

jobs:
  build-test:
    runs-on: windows-latest

    steps:
      - uses: actions/checkout@v4

      - name: Configure
        run: cmake -S . -B build -G Ninja

      - name: Build
        run: cmake --build build

      - name: Test
        run: ctest --test-dir build --output-on-failure
```

---

## Target Porting Model

Target-specific implementations should live outside the core framework or under a dedicated target port.

Use `src/atlasboot/ports/template` as the starting point for a new MCU port. A target port is expected to provide concrete implementations for:

- flash initialization
- flash erase/write/read
- application jump
- target memory layout

The boot/update policy should remain in the core layer where it can continue to be tested on the native simulator.

---

## Planned

These items are not fully implemented yet:

- persistent metadata backend
- dedicated metadata manager module
- dedicated rollback manager module
- dedicated image validator abstraction
- interrupted update recovery
- metadata corruption detection/recovery
- broader failure injection tests
- downgrade prevention
- real CRC backend
- hash verification hook
- signature verification hook
- anti-rollback version policy
- secure metadata policy
- documented image format file
- documented memory layout file
- target porting guide
- additional scenario tests split by behavior

Planned test files:

```text
tests/update_flow_tests.cpp
tests/rollback_tests.cpp
tests/metadata_tests.cpp
tests/image_validation_tests.cpp
```

Planned docs:

```text
docs/image_format.md
docs/memory_layout.md
docs/porting_guide.md
```

---

## Roadmap

### Phase 1: Executable Simulator

Mostly implemented:

- virtual flash
- basic image header
- checksum validation
- boot decision logic
- desktop example executable
- CTest integration

Still pending:

- persistent metadata storage
- more complete scenario coverage

### Phase 2: Update and Rollback Scenarios

Partially implemented:

- A/B slot model
- pending and confirmed image states
- rollback after failed confirmation

Still pending:

- interrupted update recovery
- metadata corruption tests
- erase/write failure scenario tests

### Phase 3: Porting Layer

Partially implemented:

- stable hardware interfaces
- target port template
- native simulator reference port

Still pending:

- full porting guide
- memory layout documentation
- application jump contract documentation

### Phase 4: Security Extensions

Not implemented:

- hash backend abstraction
- signature verification hook
- anti-rollback version policy
- secure metadata policy

---

## License

MIT License
