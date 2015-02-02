Build a minimal multi-tasking OS kernel for ARM from scratch

Prerequisites
-------------
- [QEMU with an STM32 microcontroller implementation](http://beckus.github.io/qemu_stm32/)
- [GNU Toolchain for ARM](https://launchpad.net/gcc-arm-embedded)

Steps
-----
* `01-HelloWorld`
  - Enable STM32 USART to print trivial greetings
* `02-ContextSwitch-1`
  - Basic switch between user and kernel mode
* `03-ContextSwitch-2`
  - system call is introduced as an effective method to return to kernel
* `04-Multitasking`
  - Two user tasks are interatively switching
* `05-TimerInterrupt`
  - Enable SysTick for future scheduler implementation
* `06-Preemptive`
  - Basic preemptive scheduling

Building and Verification
-------------------------
$ make
$ make qemu

Reference
---------
* [Baking Pi – Operating Systems Development](http://www.cl.cam.ac.uk/projects/raspberrypi/tutorials/os/)
