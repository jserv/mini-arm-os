Preemptive round-robin scheduling with user-level threads on STM32F429i-Discovery(physical device) and STM32P103(qemu).

Prerequisites:
--------------------
- [GNU Toolchain for ARM](https://launchpad.net/gcc-arm-embedded)
  - arm-none-eabi-gcc/gdb,etc
- If you would like to run the os on STM32F29i-Discovery
  - [st-link](https://github.com/texane/stlink)
    - You could also use OpenOCD,but the Makefile has the rules with using st-link.
  - [NeoCon](http://wiki.openmoko.org/wiki/NeoCon) or [screen](http://www.commandlinefu.com/commands/view/6130/use-screen-as-a-terminal-emulator-to-connect-to-serial-consoles)
- Not necessary tools
  - [Cscope](http://cscope.sourceforge.net/)


Support Devices:
--------------------
- [STM32F429i-Discovery(physical devices)](http://www2.st.com/content/st_com/en/products/evaluation-tools/product-evaluation-tools/mcu-eval-tools/stm32-mcu-eval-tools/stm32-mcu-discovery-kits/32f429idiscovery.html)
  - [Details in Chinese by NCKU](http://wiki.csie.ncku.edu.tw/embedded/STM32F429)

- [STM32-P103(emulator)](http://beckus.github.io/qemu_stm32/)
  - [How to build QEMU environment by NCKU](http://wiki.csie.ncku.edu.tw/embedded/Lab39)
  - If QEMU is ready
    - Assuming "qemu_stm32" directory in at "$HOME/workspace"
```
export PATH=~/workspace/qemu_stm32/arm-softmmu:$PATH
git clone git@github.com:JaredCJR/mini-arm-os-1.git
cd ~/workspace/mini-arm-os-1/07-Threads
make qemu
```

Available commands:
----------------------

**Overall**
  - `make`
    - Build all target's bin,elf,objdump files in "release"
  - `make clean`
    - Remove the entire "release" directory
  - `make cscope`
    - The best friend with your powerful VIM!
    - Producing cscope file in this project.

**STM32P103(QEMU)**
  - `make STM32P103_os.bin`
    - Build "STM32P103_os.bin"
  - `make qemu`
    - Build "STM32P103_os.bin" and run QEMU automatically.
  - `make qemu_GDBstub`
    - Build "STM32P103_os.bin" and run QEMU with GDB stub and wait for remote GDB automatically.
  - `make qemu_GDBconnect`
    - Open remote GDB to connect to the QEMU GDB stub with port:1234(the default port).

**STM32F429(physical device)**
  - `make STM32F429_os.bin`
    - Build "STM32F429_os.bin"
  - `make flash`
    - Build "STM32F429_os.bin" and flash the binary into STM32F429 with st-link toolchain.
  - `make erase`
    - Sometimes,the STM32F429 will not be able to flash new binary file,then you will need this with st-link toolchain.
    - Erase the entire flash on STM32F429.
  - `make gdb_ST-UTIL`
    - Using GDB with ST-LINK on STM32F429.
    - Remember to open another terminal,and type "st-util" to open "STLINK GDB Server"



Directory Tree:
-----------------
- core
  - Hardware independent source and header files
- platform
  - Hardware dependent source and header files
- cmsis
  - With cmsis,porting would be much easier!
- release
  - This directory will be create after "Target" in Makefile is called.
  - Containing the elf,bin,objdump files in corresponding directory.
  - "make clean" will remove the entire directory,do not put personal files inside it!


