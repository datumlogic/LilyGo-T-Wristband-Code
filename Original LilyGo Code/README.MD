This is the code folder for the original code taken directly from the LilyGo T-Wrist repo. The original code will not compile due to multiple dependecy errors dealing with the WiFi management library. It depends on [WiFiManager library](https://github.com/tzapu/WiFiManager) but oddly that library doesn't support ESP32...

The **MODIFIED** code is still the original code template, but modified so that it will at least compile. Offending lines are commented out and marked `// COMPILE ERROR`. This is very similar to what comes preprogrammed except that it lacks OTA WiFi features (the main compile problem).

Two compile options are available
+ `#define FACTORY_HW_TEST` Runs through all system checks upon Deep Sleep wakeup
+ `#define ARDUINO_OTA_UPDATE` Supposedly allows you to wirelessly update from Arduino IDE

Read the Manufacturer's repo [here](https://github.com/Xinyuan-LilyGO/LilyGo-T-Wristband/blob/master/examples/T-Wristband-LSM9DS1/README.MD)
> Beware: it may confuse you more after reading their pages

> Also Beware: this program/example is very power hungry- battery life is like 20 mins tops
