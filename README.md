# AtlasBoot

> **A Simulator-First Bootloader Validation Framework for Embedded Systems**
>
> **Verify bootloader update logic without target hardware. Port only after the behavior is proven.**

---

# Overview

AtlasBoot is a **hardware-independent bootloader simulator and validation framework** for embedded systems.

The main goal is not to provide another MCU-specific bootloader. The goal is to make bootloader behavior testable on a desktop before it is integrated into real hardware.

Bootloaders are usually difficult to verify because many critical behaviors depend on flash state, metadata, interrupted updates, invalid images, rollback rules, and boot decisions. Those behaviors are often tested late, manually, and only on a physical board.

AtlasBoot takes a different approach:

```text
Design the boot flow
        |
Run it in the native simulation port
        |
Test update, failure, rollback, and metadata cases
        |
Port the proven behavior to the target MCU
```

The framework provides reusable bootloader logic, virtual hardware models, testable image handling, and hardware abstraction interfaces for later MCU integration.

---

# Core Idea

AtlasBoot is built around one principle:

> **A bootloader update policy should be executable and testable without hardware.**

Traditional bootloader development usually looks like this:

```text
Bootloader logic
        |
MCU flash driver
        |
Target board
        |
Manual validation
```

AtlasBoot is intended to move most validation earlier:

```text
Bootloader logic
        |
Virtual flash / simulated app jump / test memory layout
        |
Native simulation port
        |
Automated tests
```

After the behavior is verified, the same core logic can be connected to real MCU implementations through a small set of hardware primitives such as `IFlash` and `IApplicationLauncher`.

---

# What AtlasBoot Is

AtlasBoot is intended to be:

* A desktop-executable bootloader simulator
* A validation environment for firmware update flows
* A reusable bootloader state machine
* A virtual flash and metadata test platform
* A reference architecture for portable bootloader design
* A hardware abstraction layer for later MCU porting

AtlasBoot is useful when you want to answer questions such as:

* What happens if power is lost during an update?
* What happens if the image CRC is invalid?
* What happens if boot metadata is corrupted?
* Which slot should boot after a failed confirmation?
* Should rollback occur after an application boot failure?
* Is downgrade prevention working?
* Can the update flow be verified in CI without a board?

---

# What AtlasBoot Is Not

AtlasBoot is not:

* A ready-to-flash STM32 bootloader
* A TC387, RH850, S32K, or vendor-specific bootloader
* A BSP or HAL
* A replacement for startup code or linker scripts
* A collection of MCU register drivers
* Only a set of abstract interfaces

The interfaces exist so the validated behavior can be connected to real hardware later. The simulator and testable boot flow are the primary value.

---

# Main Features

AtlasBoot aims to provide:

* Boot decision state machine
* Firmware update flow simulation
* Image parsing and validation
* CRC / hash verification hooks
* Boot metadata management
* A/B slot update model
* Rollback policy simulation
* Application confirmation flow
* Virtual flash device
* Virtual memory map
* Failure injection
* Unit and scenario tests
* Minimal hardware abstraction interfaces for real target ports

---

# Simulator-First Architecture

```text
                  +--------------------------------+
                  |        AtlasBoot Core          |
                  |--------------------------------|
                  | Boot Manager                   |
                  | Update Manager                 |
                  | Image Parser                   |
                  | Validator                      |
                  | Metadata Manager               |
                  | Rollback Manager               |
                  | State Machine                  |
                  +---------------+----------------+
                                  |
                  +---------------+----------------+
                  |       Hardware Primitives       |
                  |--------------------------------|
                  | IFlash | IApplicationLauncher  |
                  | MemoryLayout                   |
                  +---------------+----------------+
                                  |
        +-------------------------+-------------------------+
        |                                                   |
+-------+----------------+                    +-------------+--------+
| Native Sim Port        |                    | Target MCU Port      |
|------------------------|                    |----------------------|
| VirtualFlash           |                    | STM32Flash           |
| Simulated App Jump     |                    | Real App Jump        |
| Memory Layout          |                    | Memory Layout        |
| Failure Injection      |                    | Flash Error Handling |
+------------------------+                    +----------------------+
```

The same core behavior should run against the native simulation port and against real hardware-specific implementations.

---

# Expected Workflow

```text
1. Define image format and memory layout
2. Implement or configure the boot/update policy
3. Run update scenarios in the native simulation port
4. Add failure cases and metadata corruption tests
5. Verify boot decisions and rollback behavior
6. Port the interfaces to the target MCU
7. Reuse the same scenario tests where possible
```

This workflow makes the simulator the first-class development target, not an optional demo.

---

# Example Scenarios

AtlasBoot should be able to simulate scenarios like:

```text
Scenario: Valid update
  - Receive image
  - Write image to inactive slot
  - Validate CRC/hash
  - Mark image as pending
  - Boot new image
  - Confirm image
  - Keep new image active
```

```text
Scenario: Interrupted update
  - Start writing image
  - Stop before completion
  - Reboot simulator
  - Detect incomplete update
  - Keep previous application active
```

```text
Scenario: Failed new application
  - Install valid image
  - Boot image as pending
  - Application does not confirm
  - Reboot
  - Roll back to previous confirmed image
```

```text
Scenario: Invalid image
  - Receive image
  - CRC/hash check fails
  - Reject image
  - Do not update boot metadata
  - Continue booting the current valid application
```

---

# Native Simulation Port

The native simulation port replaces target hardware with deterministic virtual components.

## Virtual Flash

Instead of writing to real MCU flash, the native simulation port uses a memory-backed flash model:

```cpp
uint8_t flash_memory[FLASH_SIZE];
```

The virtual flash should model real flash constraints:

* Erased value
* Sector erase
* Page or word write granularity
* Address range validation
* Write-before-erase errors
* Optional injected write or erase failures

# Core Interfaces

The hardware interfaces are intentionally small. AtlasBoot should own the boot policy, image parsing, checksum validation, metadata transitions, and rollback decisions. A target port should only provide the low-level hardware actions that cannot be implemented portably.

## Flash

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

## Application Launcher

```cpp
class IApplicationLauncher
{
public:
    virtual bool JumpTo(uint32_t address) = 0;
};
```

Future transport, crypto, and persistent metadata backends should be added only when the core provides enough built-in behavior to keep the port layer thin.

---

# Directory Structure

Planned structure:

```text
atlasboot/
|-- core/
|   |-- boot/
|   |-- update/
|   |-- image/
|   |-- metadata/
|   `-- rollback/
|
|-- interfaces/
|   |-- IFlash.hpp
|   `-- IApplicationLauncher.hpp
|
|-- ports/
|   |-- native_sim/
|   |   |-- NativeSimPort.hpp
|   |   |-- NativeSimPort.cpp
|   |   |-- VirtualFlash.hpp
|   |   `-- VirtualFlash.cpp
|   `-- template/
|       |-- TargetPort.hpp
|       |-- TargetFlash.hpp
|       |-- TargetApplicationLauncher.hpp
|       `-- TargetMemoryLayout.hpp
|
|-- examples/
|   |-- desktop_update_sim/
|   |-- porting_minimal/
|   `-- target_port_template/
|
|-- tests/
|   |-- update_flow_tests.cpp
|   |-- rollback_tests.cpp
|   |-- metadata_tests.cpp
|   `-- image_validation_tests.cpp
|
|-- docs/
|   |-- image_format.md
|   |-- memory_layout.md
|   `-- porting_guide.md
|
`-- README.md
```

Target-specific implementations should live in the user project or in separate example ports, not inside the core framework. Use `ports/template` as the copy-and-rename starting point for a new MCU port.

---

# Porting Model

Once the simulator behavior is proven, a target MCU port only needs to implement the interfaces.

Example:

```cpp
class Stm32Flash : public IFlash
{
public:
    bool Init() override;
    bool Erase(uint32_t address, uint32_t size) override;
    bool Write(uint32_t address, const uint8_t* data, uint32_t size) override;
    bool Read(uint32_t address, uint8_t* data, uint32_t size) override;
};
```

```cpp
class Stm32ApplicationLauncher : public IApplicationLauncher
{
public:
    bool JumpTo(uint32_t address) override;
};
```

No boot policy changes should be required inside AtlasBoot Core when moving from simulator to hardware.

---

# Design Goals

* Simulator-first development
* Hardware-independent boot policy
* Deterministic scenario testing
* Failure injection
* CI-friendly verification
* Thin target ports
* Lightweight embedded integration
* Modern C++
* Clear separation between policy and hardware access

---

# Non-Goals

AtlasBoot intentionally does not provide:

* MCU startup code
* Linker scripts
* Vector table setup
* Flash register definitions
* CAN, UART, USB, or Ethernet drivers
* BSP
* Vendor HAL
* RTOS integration layer
* Production key management

Those components belong to the target platform or product-specific integration.

---

# Roadmap

## Phase 1: Executable Simulator

* Virtual flash
* Virtual metadata storage
* File or memory-buffer image input
* Basic image header
* CRC validation
* Boot decision state machine
* Desktop example executable

## Phase 2: Update and Rollback Scenarios

* A/B slot model
* Pending and confirmed image states
* Interrupted update recovery
* Rollback after failed confirmation
* Metadata corruption tests
* Failure injection for flash operations

## Phase 3: Porting Layer

* Stable hardware interfaces
* Target port template
* Example MCU integration guide
* Memory layout documentation
* Application jump contract

## Phase 4: Security Extensions

* Hash backend abstraction
* Signature verification hook
* Anti-rollback version policy
* Secure metadata policy

---

# Why AtlasBoot?

The value of AtlasBoot is not that it hides every MCU detail. Real products still need real flash drivers, real communication drivers, startup code, linker scripts, memory maps, and security integration.

The value is that the dangerous part of bootloader behavior can be tested before hardware integration:

```text
Update policy
Rollback policy
Image validation
Boot metadata transitions
Interrupted update recovery
Failure handling
```

AtlasBoot should let developers prove those behaviors on a desktop, automate them in CI, and then connect the proven logic to target hardware through small interfaces.

---

# License

MIT License
