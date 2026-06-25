# Target Port Template

Copy this folder when starting a new MCU port.

Example:

```text
ports/template  ->  ports/stm32
```

Then rename the classes:

```text
TargetPort                  -> Stm32Port
TargetFlash                 -> Stm32Flash
TargetApplicationLauncher   -> Stm32ApplicationLauncher
```

The target port should stay thin. It should only provide hardware actions that AtlasBoot cannot implement portably:

* Flash init / erase / write / read
* Application jump
* Memory layout values
* Initial boot metadata defaults

AtlasBoot Core owns the update policy, image parsing, checksum validation, pending/confirmed state, and rollback decisions.

## Files

```text
TargetPort.hpp/.cpp
  Wires the target-specific primitives into the AtlasBoot Core shape.

TargetFlash.hpp/.cpp
  Implements IFlash using the target MCU flash driver or registers.

TargetApplicationLauncher.hpp/.cpp
  Implements IApplicationLauncher using the target startup/vector-table rules.

TargetMemoryLayout.hpp
  Defines bootloader/app slot addresses and slot size.
```

## Porting Checklist

1. Set the flash memory layout in `TargetMemoryLayout.hpp`.
2. Implement `TargetFlash::Init`.
3. Implement `TargetFlash::Erase`.
4. Implement `TargetFlash::Write`.
5. Implement `TargetFlash::Read`.
6. Implement `TargetApplicationLauncher::JumpTo`.
7. Rename `TargetPort` to your MCU port name.
8. Use the port from your bootloader `main.cpp`.

The simulator port in `ports/native_sim` is the reference for the expected shape.
