# Overview
TinySat collects the energy from up to two solar panels (max 6V) and stores it in a 1 Farad super capacitor. The ATtiny13A uses this stored energy to make two LEDs blink - even when the sun is not shining.

- Design Files (EasyEDA): https://easyeda.com/wagiminator/attiny13-tinysat

<iframe width="680" height="382" src="//www.youtube.com/embed/PQvtNVte2JE" frameborder="0"> </iframe>

![TinySat_pic1.jpg](https://raw.githubusercontent.com/wagiminator/ATtiny13-TinySat/main/documentation/TinySat_pic1.jpg)

# Hardware
![TinySat_wiring.png](https://raw.githubusercontent.com/wagiminator/ATtiny13-TinySat/main/documentation/TinySat_wiring.png)

# Compiling and Uploading the Firmware
Since there is no ICSP header on the board, you have to program the ATtiny before putting it into the IC socket. The [AVR Programmer Adapter](https://github.com/wagiminator/AVR-Programmer/tree/master/AVR_Programmer_Adapter) can help with this.

## If using the Arduino IDE
- Make sure you have installed [MicroCore](https://github.com/MCUdude/MicroCore).
- Go to **Tools -> Board -> MicroCore** and select **ATtiny13**.
- Go to **Tools** and choose the following board options:
  - **Clock:**  128 kHz internal osc.
  - **BOD:**    disabled
  - **Timing:** Micros disabled
- Connect your programmer to your PC and to the ATtiny.
- Go to **Tools -> Programmer** and select your ISP programmer (e.g. [USBasp](https://aliexpress.com/wholesale?SearchText=usbasp)).
- Go to **Tools -> Burn Bootloader** to burn the fuses.
- Open the TinySat sketch and click **Upload**.

## If using the precompiled hex-file
- Make sure you have installed [avrdude](https://learn.adafruit.com/usbtinyisp/avrdude).
- Connect your programmer to your PC and to the ATtiny.
- Open a terminal.
- Navigate to the folder with the hex-file.
- Execute the following command (if necessary replace "usbasp" with the programmer you use):
  ```
  avrdude -c usbasp -p t13 -U lfuse:w:0x3b:m -U hfuse:w:0xff:m -U flash:w:tinysat.hex
  ```

## If using the makefile (Linux/Mac)
- Make sure you have installed [avr-gcc toolchain and avrdude](http://maxembedded.com/2015/06/setting-up-avr-gcc-toolchain-on-linux-and-mac-os-x/).
- Connect your programmer to your PC and to the ATtiny.
- Open the makefile and change the programmer if you are not using usbasp.
- Open a terminal.
- Navigate to the folder with the makefile and the sketch.
- Run "make install" to compile, burn the fuses and upload the firmware.

# License
![license.png](https://i.creativecommons.org/l/by-sa/3.0/88x31.png)

This work is licensed under Creative Commons Attribution-ShareAlike 3.0 Unported License. 
(http://creativecommons.org/licenses/by-sa/3.0/)
