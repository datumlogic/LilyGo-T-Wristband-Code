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
  tft.setRotation(3);                           // This sets the screen orientation so that the touch button is to the left of the display
  
  //  0 = touch button is under the display     ***IMAGE APPEARS GARBLED THOUGH***
  
  //  1 = touch button on left side for right-handed people (assuming you wear your watch on the left side)
  
  //  2 = touch button is over the display      ***IMAGE APPEARS GARBLED THOUGH***
  
  //  3 = touch button on left side for left-handed people (assuming you wear your watch on the right side)
  
  tft.setSwapBytes(true);                       // Needed for reading the .h file correctly
  tft.fillScreen(TFT_BLACK);                    // Gotta clear the screen or else
  tft.pushImage(0, 0, 160, 80, SOMETHING);      // This is the line that writes the image-- make sure you change SOMETHING to the name of your file
  pinMode(LED_PIN, OUTPUT);                     // Setup LED pin
}

void loop()                                     // LOOP
{
  digitalWrite(LED_PIN, HIGH);                  // Turn on LED...
  delay(100);                                   // ...for .1 sec
  digitalWrite(LED_PIN, LOW);                   // Turn it off...
  delay(1500);                                  // ...and wait 1.5 secs
}
