# T-Wristband Code

## Why

Tired of searching the whole internet for code examples for your T-Wristband? Look no further. These examples and battery optimized Arduino sketches will help you get your feet planted in this pretty cool ESP32 microdevice.


## Background

I got my hands on one of these and couldn't find any useful piece of code anywhere. The test example that comes pre-programmed yields a battery life of 21.5 minutes. "Why did I buy this again?" The code from repo made by LilyGo won't even compile. So I surgically cut off the code until I had compiled a skeleton to run on it. Then Power Save features, better GUI features, oh, and a custom boot logo if you want.


## Research

There are a few devices by this company and by others that utilize this very small low-power ESP32 variant. While it is perfect for all your DIY mobile applications, it isnt directly BFFs with Arduino IDE. The internet didn't help (yet again) and after too much trial and error, I figured out that you have to:
+ select ESP32 Dev Module in Board Manager
+ set Flash Frequency to 80MHz
+ set PSRAM to Disabled
+ Upload Speed lower than 921600 is more reliable
> Note: The Expansion/Programming Board that comes with the Wristband has a standard ESP "boot button" but interestingly, pressing that button only screws up the Wristband during programming.

This little package actually packs a punch, housing
+ ESP32-PICO-D4 (WiFi/BT and clocks @ 240MHz) 
+ ST7735 160x80 Color Display
+ LSM9DS1 9DOF Sensor
+ PCF8563 RTC (battery-backed, too)
+ 3.7v 80mAh battery (yeah, its pretty small)
+ Capacitive-touch button
+ SMD Red LED (right next to the touch-button)
+ Ribbon-cable expansion port for expander/programmer board

## Original LilyGo Code

[Find it here,](https://github.com/Xinyuan-LilyGO/LilyGo-T-Wristband) but be prepared to see red at the bottom of your IDE

## Datasheet

- [ESP32-PICO-D4:](https://www.espressif.com/sites/default/files/documentation/esp32-pico-d4_datasheet_en.pdf) Main ESP Chip
- [PCF8563:](https://www.nxp.com/docs/en/data-sheet/PCF8563.pdf) RTC
- [ST7735:](http://www.displayfuture.com/Display/datasheet/controller/ST7735.pdf) Screen
- [LSM9DS1:](https://www.st.com/resource/en/datasheet/lsm9ds1.pdf) 9DOF Sensor


## Pin definition

| Name              | Pin    |
| ----------------- | ------ |
| TFT Driver        | ST7735 |
| TFT_MISO          | N/A    |
| TFT_MOSI          | 19     |
| TFT_SCLK          | 18     |
| TFT_CS            |  5     |
| TFT_DC            | 23     |
| TFT_RST           | 26     |
| TFT_BL            | 27     |
| Built-in Red LED  |  4     |
| Touchpad          | 33     |
| Touchpad Power    | 25     |
| RTC Interrupt     | 34     |
| Battery ADC       | 35     |
| I2C_SDA           | 21     |
| I2C_SCL           | 22     |
| CHARGE Indication | 32     |
| IMU INT1          | 38     |
| IMU INT2          | 39     |
| IMU INTM          | 37     |
| IMU RDY           | 36     |
