#include <TFT_eSPI.h>                           // Graphics and font library for ST7735 driver chip
#include <SPI.h>                                // Communication to the screen happens over SPI
#include "SOMETHING.h"                          // Our Image code

#ifndef ST7735_SLPIN
#define ST7735_SLPIN 0x10                       // Setup some commands for our specific screen
#endif

#define LED_PIN 4                               // LED is on Pin 4

TFT_eSPI tft = TFT_eSPI();                      // Invoke library, pins defined in User_Setup.h

void setup(void)                                // SETUP
{
  tft.init();                                   // Start the display
  tft.setRotation(1);                           // This sets the screen orientation so that the touch button is to the right of the display
  tft.setSwapBytes(true);                       // Needed for reading the .h file correctly
  tft.pushImage(0, 0,  160, 80, SOMETHING);     // This is the line that writes the image-- make sure you change SOMETHING to the name of your file
  pinMode(LED_PIN, OUTPUT);                     // Setup LED pin
}

void loop()                                     // LOOP
{
  digitalWrite(LED_PIN, HIGH);                  // Turn on LED...
  delay(100);                                   // ...for .1 sec
  digitalWrite(LED_PIN, LOW);                   // Turn it off...
  delay(1500);                                  // ...and wait 1.5 secs
}
