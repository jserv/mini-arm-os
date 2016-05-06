Preemptive round-robin scheduling with user-level threads on STM32F429i-Discovery(physical device) and STM32P103(qemu).

Prerequisites:
=========================
- [GNU Toolchain for ARM](https://launchpad.net/gcc-arm-embedded)
  - arm-none-eabi-gcc/gdb,etc
- If you would like to run the os on STM32F29i-Discovery
  - [st-link](https://github.com/texane/stlink)
    - You could also use OpenOCD,but the Makefile has the rules with using st-link.
  - [NeoCon](http://wiki.openmoko.org/wiki/NeoCon) or [screen](http://www.commandlinefu.com/commands/view/6130/use-screen-as-a-terminal-emulator-to-connect-to-serial-consoles)
- Not necessary tools
  - [Cscope](http://cscope.sourceforge.net/)


Support Devices:
=========================
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
=========================

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
======================
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


Porting Guide:
======================
You should know what is [CMSIS](http://www.arm.com/products/processors/cortex-m/cortex-microcontroller-software-interface-standard.php),and why it save us a lot of efforts.

The full project can be divide into two layer:
- hardware-dependent layer(Hardware abstruction Layer,HAL)
  - "platform" directory
- hardware-indepentent layer(based on HAL)
  - "core" directory

Following the steps below,then you can run this os on your own devices!
----------------------------------------------------------------------------

**STEP 1.**
Select a target name for your device,such as STM32F429
In this guide,I assume its name is "LPC_EXAMPLE".
Create "LPC_EXAMPLE" directory in "platform" and "cmsis" directory.
Create "inc" and "src" directory in "platform/LPC_EXAMPLE/"


**STEP 2.**
Introducing your CMSIS for your target,where it should be in the [mbed repo](https://github.com/mbedmicro/mbed/tree/master/libraries/mbed/targets/cmsis).
For example,the CMSIS for STM32F429 could be found [here](https://github.com/mbedmicro/mbed/tree/master/libraries/mbed/targets/cmsis/TARGET_STM/TARGET_STM32F4/TARGET_DISCO_F429ZI).
We only need ".h" files,do not copy any ".c" files.
Put the header files into "cmsis/LPC_EXAMPLE"

`NOTE:` 
You may encounter some error message during building binary for your target.
You need to sovle it mannually.
Usually,it may be some file missing caused by some specific "define".You could just comment out that definition to solve the question.


**STEP 3.**
This is the most difficult part.
You have to implement the files in "platform/NXP_EXAMPLE/inc/" and "platform/NXP_EXAMPLE/src/"
According to different device vendor(such as STM,NXP,etc),the implementation is very different.
Please look into the current example:STM32F429,and you will figure it out!
The function interface must be as same as the function interface in "platform/STM32F429/inc/" due to this is HAL for the entire project.


**STEP 4.**
Add your target rules into Makefile.
Please look the example "STM32F429" in Makefile.
Most of the rules are reusable,so all you need is copy-n-paste , modifying the variable/target name and knowing what gcc arguments suit your target!



**STEP 5.**
Congratulations!
Now,you can try the "Available commands" in this README.




