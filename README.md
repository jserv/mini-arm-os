Build a minimal multi-tasking OS kernel for ARM from scratch


From 00 to 07,read the following contents
===========================================
**IMPORTANT:**
If you are interested in 08-CMSIS,please jump to the corresponding part of the README:`Guide to Hacking 08-CMSIS`.
This is because `08-CMSIS` has a plenty of differences with `07-Threads`.


Prerequisites
-------------
- [QEMU with an STM32 microcontroller implementation](http://beckus.github.io/qemu_stm32/)
  - Build instructions
```
./configure --disable-werror --enable-debug \
    --target-list="arm-softmmu" \
    --extra-cflags=-DSTM32_UART_NO_BAUD_DELAY \
    --extra-cflags=-DSTM32_UART_ENABLE_OVERRUN \
    --disable-gtk
make
```
- [GNU Toolchain for ARM](https://launchpad.net/gcc-arm-embedded)
- Set `$PATH` accordingly

Steps
-----
* `00-Semihosting`
  - Minimal semihosted ARM Cortex-M "Hello World"
* `00-HelloWorld`
  - Enable STM32 USART to print trivial greetings
* `01-HelloWorld`
  - Almost identical to the previous one but improved startup routines
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
* `07-Threads`
  - Implement user-level threads
* `08-CMSIS`
  - Portable version by introducing CMSIS.
  - The corresponding README part explains elaborately.

Building and Verification
-------------------------
* Changes the current working directory to the specified one and then
```
make
make qemu
```

Guide to Hacking 08-CMSIS
=========================================
Preemptive round-robin scheduling with user-level threads on STM32F429i-Discovery(physical device) and STM32P103(qemu).


How to get the full functional source code:
-----------------------------------------------
```
git clone --recursive https://github.com/jserv/mini-arm-os.git
cd mini-arm-os/08-CMSIS
```

Prerequisites:
------------------------------------
- [GNU Toolchain for ARM](https://launchpad.net/gcc-arm-embedded)
  - arm-none-eabi-gcc/gdb,etc
- If you would like to run the os on STM32F29i-Discovery
  - [st-link](https://github.com/texane/stlink)
    - You could also use OpenOCD,but the Makefile has the rules with using st-link.
  - [NeoCon](http://wiki.openmoko.org/wiki/NeoCon) or [screen](http://www.commandlinefu.com/commands/view/6130/use-screen-as-a-terminal-emulator-to-connect-to-serial-consoles)
- Not necessary tools
  - [Cscope](http://cscope.sourceforge.net/)
  - [Astyle(Artistic Style)](http://astyle.sourceforge.net/)


Support Devices:
-------------------------------------
- [STM32F429i-Discovery(physical devices)](http://www2.st.com/content/st_com/en/products/evaluation-tools/product-evaluation-tools/mcu-eval-tools/stm32-mcu-eval-tools/stm32-mcu-discovery-kits/32f429idiscovery.html)
  - [Details in Chinese by NCKU](http://wiki.csie.ncku.edu.tw/embedded/STM32F429)

- [STM32-P103(emulator)](http://beckus.github.io/qemu_stm32/)
  - [How to build QEMU environment by NCKU](http://wiki.csie.ncku.edu.tw/embedded/Lab39)
  - If QEMU is ready
    - Assuming "qemu_stm32" directory in at "$HOME/workspace"
```
export PATH=~/workspace/qemu_stm32/arm-softmmu:$PATH
git clone --recursive git@github.com:jserv/mini-arm-os.git
cd ~/workspace/mini-arm-os-1/07-Threads
make qemu
```

Available commands:
-----------------------------------
**IMPORTANT:**
We assuming that your current directory is at `mini-arm-os/08-CMSIS`

**Overall**
  - `make all`
    - Build all target's bin,elf,objdump files in "release"
    - NOTE: `make` doe NOT equal to `make all` here. This is because this Makefile use `eval` for `targets`.
  - `make clean`
    - Remove the entire "release" directory
  - `make cscope`
    - The best friend with your powerful VIM!
    - Producing cscope file in this project.
  - `make astyle`
    - Formatting all the ".c" and ".h" files in [Linux style](https://github.com/jserv/mini-arm-os/blob/master/coding-style.txt),excluding the cmsis sub-module.

**STM32-P103(QEMU)**
  - `make p103` or `make target PLAT=p103`
    - Build "p103.bin"
  - `make qemu`
    - Build "p103.bin" and run QEMU automatically.
  - `make qemu_GDBstub`
    - Build "p103.bin" and run QEMU with GDB stub and wait for remote GDB automatically.
  - `make qemu_GDBconnect`
    - Open remote GDB to connect to the QEMU GDB stub with port:1234(the default port).

**STM32F429i-Discovery(physical device)**
  - `make f429disco` or `make target PLAT=f429disco`
    - Build "f429disco.bin"
  - `make flash`
    - Build "f429disco.bin" and flash the binary into STM32F429 with st-link toolchain.
  - `make erase`
    - Sometimes,the STM32F429i-Discovery will not be able to flash new binary file,then you will need this with st-link toolchain.
    - Erase the entire flash on STM32F429.
  - `make gdb_ST-UTIL`
    - Using GDB with ST-LINK on STM32F429.
    - Remember to open another terminal,and type "st-util" to open "STLINK GDB Server"

Directory Tree:
-------------------------------
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
------------------------------------
You should know what is [CMSIS](http://www.arm.com/products/processors/cortex-m/cortex-microcontroller-software-interface-standard.php),and why it save us a lot of efforts.

`cmsis` is a submodule from [JaredCJR/cmsis](https://github.com/JaredCJR/cmsis).

The full project can be divide into two layer:
- hardware-dependent layer(Hardware abstruction Layer,HAL)
  - "platform" directory
- hardware-indepentent layer(based on HAL)
  - "core" directory

**The following chapter will help you a lot!**

Following the steps below,then you can run this os on your own devices!
----------------------------------------------------------------------------

**STEP 1.**
Select a target name for your device,such as STM32F429
In this guide,we assume its name is "LPC_EXAMPLE".
Create "LPC_EXAMPLE" directory in "platform" and "cmsis" directory.
Create "inc" and "src" directory in "platform/LPC_EXAMPLE/"


**STEP 2.**
Introducing your CMSIS for your target,where it should be in the [mbed repo](https://github.com/mbedmicro/mbed/tree/master/libraries/mbed/targets/cmsis).
For example,the CMSIS for STM32F429 could be found [here](https://github.com/mbedmicro/mbed/tree/master/libraries/mbed/targets/cmsis/TARGET_STM/TARGET_STM32F4/TARGET_DISCO_F429ZI).
We only need ".h" files,do not copy any ".c" files.
Put the header files into "cmsis/LPC_EXAMPLE"
[cmsis](https://github.com/JaredCJR/cmsis) is a submodule in this project,maintianed by [Jia-Rung Chang](https://github.com/JaredCJR).
Pull request is welcome!


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

- `rules.mk` 
  - You should not modify it!All of the rules are encapsulation into macro,which is used in `Makefile`.
- `Makefile`:
  - If your device vendor name does not exist,create new variable and assign name to it!
    - Ex:`STM32 := STM32`
  - Add your device name
    - Ex:`STM32F429_DEVICE := STM32F429`
  - Check your device CPU type(Cortex-M3/4)
    - In `target_using_CM4_list`
  - Will you use CMSIS?
    - If NOT,add to `target_NOT_using_CMSIS_list`
  - Use the predefined macro to produce the corresponding directory and device specific variable(device name,vendor name)
    - Ex:`$(eval $(call eval_all_variable,$(STM32F429_DEVICE),$(STM32)))`
    - The `vendor name` is used in the `cmsis` directory name.They must corresponding to each other.
  - Use the predefined macro to produce the corresponding GCC toolchain commands
    - Ex: `$(eval $(call eval_build_command,$(STM32F429_DEVICE)))`
  - Add your device name to `all`
    - In the specific form:`$($(YOUR_DEVICE_NAME)_TARGET)` ,this variable will be automatic derived by `rules.mk`
    - Ex: `$($(STM32F429_DEVICE)_TARGET)`


**STEP 5.**
Congratulations!
Now,you can try the "Available commands" in this README.



Licensing
---------
`mini-arm-os` is freely redistributable under the two-clause BSD License.
Use of this source code is governed by a BSD-style license that can be found
in the `LICENSE` file.

Reference
---------
* [Baking Pi – Operating Systems Development](http://www.cl.cam.ac.uk/projects/raspberrypi/tutorials/os/)
