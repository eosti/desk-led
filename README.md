# desk-led
This is a IoT solution for a single string of RGB LEDs.

Originally for an English final, actually going to be used for a lighting solution around my house.

Run and debugged on an ESP8266-01 and Adafruit FTDI Friend

Pin connections are as follows:

  | Pin     | Component                           |
  |:-------:|:-----------------------------------:|
  | GPIO1   | Red LED transistor via SPST switch  |
  | GPIO2   | Green LED transistor via SPST switch|
  | GPIO3   | Blue LED transistor                 |
  | GND     | Common Ground                       | 
  | Vcc     | 3.3VDC, **NOT 5V**                  |
  | CHIP_EN | 3.3VDC                              |
  
  About the switches: when the ESP8266 boots up, it looks at GPIO 0 and 2 for what mode to boot up in.
  Unfortunately, in order to use them they must be grounded/low during boot
  so the switch isolates them because I guess they were high otherwise.
  As for GPIO 1, I do not know why a switch is needed. Testing will be done later.
  It works, and that's all I care about for now. 
