Build a multitask OS kernel for ARM from scratch

Prerequisites
-------------
- QEMU ARM system emulation
- GNU Toolchain for ARM

Steps
-----
* `01-HelloWorld`
  - Use ARM PrimeCell PL011 UART0 to print trivial greetings
* `02-ContextSwitch-1`
  - Basic switch between user and kernel mode

Building and Verification
-------------------------
$ make
$ make qemu
