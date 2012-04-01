********************************************************************************
* Files Organization                                                           *
********************************************************************************

--{root}                - eSolid directory.
  +--readme.txt         - This file.
  +--documentation.html - Shortcut to the web documentation page.
  +--todo.txt           - Current plan (development/unstable versions only).
  +--license.txt        - GPL license text.
  +--doc/               - Documentation.
  |  +--html/           - Local HTML documentation (after rebuild).
  |  +--inc/            - Documentation source include files (required for rebuild).
  |  +--img/            - Documentation image files (required for rebuild).
  |  +--example/        - Documentation example files (required for rebuild).
  |  +--index.html      - Local documentation access (after rebuild).
  +--inc/               - eSolid include files.
  |  +--core/           - eSolid core include files.
  |  +--module/         - eSolid modules include files.
  +--src/               - eSolid source files.
  |  +--core/           - eSolid core source files.
  |  |  +--eot/         - Core - Event Object Transportation module.
  |  |  +--epe/         - Core - Event Processing Engine module.
  |  |  +--epn/         - Core - Event Processing Network module.
  |  |  +--smm/         - Core - Simple Memory Management module.
  |  |  +--smp/         - Core - State Machine Processor module.
  |  |  +--str/         - Core - Software Trace Recording module.
  |  |  +--tem/         - Core - Time Event Management module.
  |  +--module/         - eSolid modules source files.
  |  +--platform/       - Hardware Abstraction Layer.
  |  |  +--inc/         - HAL high level headers.
  |  |  +--src/         - HAL high level source.
  |  |  +--platforms/   - HAL low level drivers implementations.
  |  |     +--LPC13xx/  - Drivers for LPC13xx platform.
  |  |     +--STM32/    - Drivers for STM32 platform.
  |  |     +--posix/    - Drivers for x86 Linux/OSX simulator platform.
  |  |     +--win32/    - Drivers for x86 Win32 simulator platform.
  |  +--ports/          - Port files for the various architectures.
  |     +--GCC/         - Ports for the GCC compiler.
  |     |  +--ARM-CMx/  - Port files for ARM-CMx architectures (ARMv6/7-M).
  |     +--IAR/         - Ports for the IAR compiler.
  |        +--ARM-CMx/  - Port files for ARMCMx architectures (ARMv6/7-M).
  +--test/              - eSolid test suite source code.

*****************************************************************************
* Releases                                                                  *
*****************************************************************************

*** 1.0.0 ***
- INFO: First release
