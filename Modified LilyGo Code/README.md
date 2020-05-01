This is still the original code template, but modified so that it will at least compile. Offending lines are commented out and marked `// COMPILE ERROR`. This is very similar to what comes preprogrammed except that it lacks OTA WiFi features (the main compile problem). It depends on [WiFiManager library](https://github.com/tzapu/WiFiManager) but oddly that library doesn't support ESP32...

Two compile options are available
+ `#define FACTORY_HW_TEST` Runs through all system checks upon Deep Sleep wakeup
+ `#define ARDUINO_OTA_UPDATE` Supposedly allows you to wirelessly update from Arduino IDE
