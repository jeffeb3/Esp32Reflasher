## Flashing instructions

Use platform io to build and then upload the code.

You need to connect the esp32cam to a uart/usb (or ftdi adapter, or whatever). Connect these pins

| UART/USB Pin | ESP32 PIN | Note |
|--------------|-----------|------|
|5V            |5V         |Power |
|GND           |GND        |Ground|
|Tx            |VOR        |Data to ESP|
|Rx            |VOT        |Data from ESP|

You also need to put the ESP32 CAM into the bootloader. Connect pins `IO0` (next to IO16 and one of the GND pins) to ground.
I just used a female/female dupont connector to connect those neighboring pins.

Once you have that jumper installed, anytime the ESP32 CAM boots, it will not run your program, but be in the bootloader, ready to accept 
firmware from platform io. So these are the steps I take when uploading:

1. Connect IO0 to ground on the ESP
2. Press the reset button on the ESP
3. Hit the upload button in platformio. Make sure it finishes with success.
4. Disconnect IO0 from ground on the ESP
5. Press the reset button on the ESP

