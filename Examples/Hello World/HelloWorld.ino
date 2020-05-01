#include <TFT_eSPI.h>                           // Graphics and font library for ST7735 driver chip
#include <SPI.h>                                // Communication to the screen happens over SPI

#ifndef ST7735_SLPIN
#define ST7735_SLPIN 0x10                       // Setup some commands for our specific screen
#endif

#define LED_PIN 4                               // LED is on Pin 4

TFT_eSPI tft = TFT_eSPI();                      // Invoke library, pins defined in User_Setup.h

void setup(void)                                // SETUP
{
  tft.init();                                   // Start the display
  tft.setSwapBytes(true);                       // Needed for reading .h files correctly
  tft.setTextColor(TFT_BLUE, TFT_BLACK);        // Sets Font-- this does not draw the background color for all of the screen
  pinMode(LED_PIN, OUTPUT);                     // Setup LED pin
}

void loop()                                     // LOOP
{
  for(int i = 0; i > -1; i++)                   // Lets count i up
  {
    tft.setRotation(i);                         // This sets the screen orientation to i
    tft.fillScreen(TFT_BLACK);                  // Gotta clear the screen or else
    tft.setCursor (80, 40);                       // Move our "pen" to the center of the screen
    tft.printf("HelloWorld");                   // This uses the standard ADAFruit small font-- very basic API, recommend using drawString()
    digitalWrite(LED_PIN, HIGH);                // Turn on LED...
    delay(500);                                 // ...for .5 sec
    digitalWrite(LED_PIN, LOW);                 // Turn it off
    delay(500);
  }
}
