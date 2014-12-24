Build a minimal multi-tasking OS kernel for ARM from scratch

Prerequisites
-------------
- [QEMU ARM system emulation](http://qemu.org/)
- [GNU Toolchain for ARM](https://launchpad.net/gcc-arm-embedded)

Steps
-----
* `01-HelloWorld`
  - Use ARM PrimeCell PL011 UART0 to print trivial greetings
* `02-ContextSwitch-1`
  - Basic switch between user and kernel mode
* `03-ContextSwitch-2`
  - system call is introduced as an effective method to return to kernel
* `04-Multitasking`
  - Two user tasks are interatively switching
* `05-TimerInterrupt`
  - Use ARM timer peripheral for future scheduler implementation
* `06-Preemptive`
  - Basic preemptive scheduling

Building and Verification
-------------------------
$ make
$ make qemu
