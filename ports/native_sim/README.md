# Native Simulation Port

This folder is a concrete port example.

It uses native simulation components as the hardware implementation:

* `VirtualFlash` implements `IFlash`
* `ApplicationLauncher` implements `IApplicationLauncher`

For a real MCU port, keep the same shape and replace these members with target-specific implementations:

* `Stm32Flash`
* `Stm32ApplicationLauncher`
* Target memory layout values

The core boot/update logic should not need to change.
