# AtlasBoot

> **A Portable Bootloader Framework for Embedded Systems**
>
> **Write the bootloader logic once. Implement only the hardware layer.**

---

# Overview

AtlasBoot is a **hardware-independent bootloader framework** designed for embedded systems.

Unlike traditional bootloaders that are tightly coupled to a specific MCU, AtlasBoot provides only the **bootloader architecture** and **update logic**.

Platform-specific implementations such as Flash programming, communication drivers, and cryptographic engines are provided by the user through abstraction interfaces.

AtlasBoot is **not another STM32 bootloader or TC387 bootloader**.

It is a reusable framework that allows developers to build their own bootloader by implementing a small set of hardware interfaces.

---

# Philosophy

Traditional Bootloader

```text
Bootloader
    │
Flash Driver
    │
 MCU
```

Every MCU requires a different bootloader implementation.

AtlasBoot

```text
        AtlasBoot Framework
                │
      Hardware Abstraction Layer
                │
    ┌───────────┼────────────┐
    │           │            │
 STM32      TC387        RH850
```

Only the hardware layer changes.

The bootloader logic remains identical.

---

# What AtlasBoot Provides

AtlasBoot is responsible for:

* Bootloader State Machine
* Firmware Update Flow
* Image Parsing
* Image Validation
* CRC / Hash Verification
* Boot Metadata Management
* Application Jump Logic
* Update Manager

AtlasBoot intentionally **does not implement MCU-specific code.**

---

# What Users Implement

Users only need to provide hardware-specific implementations.

Examples include:

* Flash Driver
* Communication Driver
* Cryptographic Backend
* Memory Map
* Application Entry

This keeps the framework portable across different MCU families.

---

# Architecture

```text
                +---------------------------+
                |      AtlasBoot Core       |
                |---------------------------|
                | Boot Manager              |
                | Update Manager            |
                | Image Parser              |
                | Validator                 |
                | State Machine             |
                +-------------+-------------+
                              │
                    Hardware Abstraction
                              │
      +-----------+-----------+-----------+-----------+
      |           |           |           |           |
    IFlash    ITransport   ICrypto    ILogger   IStorage
      |           |           |           |           |
      +-----------+-----------+-----------+-----------+
                              │
                      User Implementation
                              │
        +-----------+---------+---------+-----------+
        |           |                   |           |
      STM32      TC387               S32K      Custom MCU
```

---

# Directory Structure

```text
atlasboot/

├── core/
│
├── interfaces/
│
├── simulator/
│
├── examples/
│
├── tests/
│
├── docs/
│
└── README.md
```

Notice that AtlasBoot **does not contain target-specific implementations.**

Target implementations belong to the application or user project.

---

# Framework Workflow

```text
Application
      │
      ▼
AtlasBoot Core
      │
      ▼
Interface
      │
      ▼
User Implementation
      │
      ▼
Hardware
```

---

# Core Interfaces

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

---

## Transport

```cpp
class ITransport
{
public:
    virtual bool Send(...) = 0;
    virtual bool Receive(...) = 0;
};
```

---

## Crypto

```cpp
class ICrypto
{
public:
    virtual bool Verify(...) = 0;
};
```

---

# Porting Guide

Supporting a new MCU only requires implementing the interfaces.

Example:

```cpp
class MyFlash : public IFlash
{
    ...
};
```

```cpp
class MyCan : public ITransport
{
    ...
};
```

No modifications are required inside AtlasBoot Core.

---

# Desktop Simulator

AtlasBoot includes a desktop simulator for framework verification.

Instead of writing to real Flash,

```cpp
uint8_t flash_memory[];
```

is used as a virtual Flash device.

This enables:

* Unit Testing
* Continuous Integration
* Algorithm Verification
* Framework Development
* No Hardware Required

---

# Design Goals

* Hardware Independent
* Interface-Oriented Design
* Testable
* Portable
* Extensible
* Lightweight
* Modern C++

---

# Non-Goals

AtlasBoot intentionally does **not** provide:

* MCU Startup Code
* Linker Scripts
* Flash Register Definitions
* CAN Drivers
* UART Drivers
* BSP
* HAL

Those components belong to the target platform.

---

# Future Roadmap

## Framework

* Boot State Machine
* Image Manager
* Update Manager
* Boot Metadata
* Rollback Manager

## Interfaces

* Flash
* Transport
* Crypto
* Logger
* Storage

## Simulator

* Virtual Flash
* Virtual Memory Map
* Update Simulation
* Image Validation

---

# Why AtlasBoot?

Instead of maintaining dozens of MCU-specific bootloaders,

```text
STM32 Bootloader
TC387 Bootloader
RH850 Bootloader
S32K Bootloader
```

AtlasBoot enables a single architecture:

```text
Bootloader Logic
        │
AtlasBoot Framework
        │
Hardware Interface
        │
Target MCU
```

The framework never changes.

Only the hardware implementation does.

---

# License

MIT License
