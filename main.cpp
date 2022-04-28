#include <stdint.h>
#include <stdio.h>

void delay(){
    for(uint32_t i = 0; i < 500000/2; i++);
}

int counter = 1;
int counter2 = 2;

int main(){
    printf("%d", counter);
    printf("%d", counter2);
    while(1){
    }
}
/*
 * This could be caused by a botched image running on the on-board target triggers a watchdog and asserts the reset line. In my K22 board this is visible also because the reset line is connected to an LED.

In order to fix, you need to halt the target before entering the reset state and flash it with a good image.

You can do this as follows:

If you openocd is launched on usb hotplub by systemd, disable it.

systemctl –user disable openocd-cmsis-dap.service

Attach the USB cable to the OpenSDA USB port.

1. Open a telnet session to openocd and issue the following commands

2. reset_config srst_only separate srst_nogate srst_open_drain connect_deassert_srst

3. reset halt
***** ATTENTION! ATTENTION! ATTENTION! ATTENTION! **** ** ** ** Your Kinetis MCU is in secured state, which means that, ** ** with exception for very basic communication, JTAG/SWD ** ** interface will NOT work. In order to restore its ** ** functionality please issue ‘kinetis mdm mass_erase’ ** ** command, power cycle the MCU and restart OpenOCD. ** ** ** ***** ATTENTION! ATTENTION! ATTENTION! ATTENTION! **** target state: halted target halted due to debug-request, current mode: Thread xPSR: 0x01000000 pc: 0xfffffffe msp: 0xfffffffc

4. kinetis mdm mass_erase

Now you should be able to flash a new image on your target arm-none-eabi-gdb –batch –ex ‘target remote localhost:3333’ -ex ‘load freertos.elf’
 */