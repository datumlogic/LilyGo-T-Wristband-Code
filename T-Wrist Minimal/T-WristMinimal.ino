#include <pcf8563.h>
#include <TFT_eSPI.h>            // Graphics and font library for ST7735 driver chip
#include <SPI.h>
#include <WiFi.h>
#include <Wire.h>
#include "sensor.h"
#include "esp_adc_cal.h"
#include "charge.h"

#ifndef ST7735_SLPIN
#define ST7735_SLPIN    0x10
#define ST7735_DISPOFF  0x28
#endif

#define TP_PIN_PIN          33
#define I2C_SDA_PIN         21
#define I2C_SCL_PIN         22
#define RTC_INT_PIN         34
#define BATT_ADC_PIN        35
#define TP_PWR_PIN          25
#define LED_PIN             4
#define CHARGE_PIN          32

TFT_eSPI tft = TFT_eSPI();       // Invoke library, pins defined in User_Setup.h
PCF8563_Class rtc;

char buff[256];
bool rtcIrq = false;
bool initial = 1;
RTC_DATA_ATTR int boots = 0;
uint8_t func_select = 0;
uint8_t omm = 99;
uint8_t xcolon = 0;
uint32_t targetTime = 0;       // for next 1 second timeout
uint32_t colour = 0;
int vref = 1100;
bool pressed = false;
uint32_t pressedTime = 0;
bool charge_indication = false;
bool charge_show = false;
uint8_t hh, mm, ss ;

void setupWire() {
  Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN);
  Wire.setClock(400000);
}

void setupPins() {
  pinMode(TP_PIN_PIN, INPUT);
  pinMode(TP_PWR_PIN, PULLUP);
  //! Must be set to pull-up output mode in order to wake up in deep sleep mode
  digitalWrite(TP_PWR_PIN, HIGH);
  pinMode(LED_PIN, OUTPUT);
  pinMode(CHARGE_PIN, INPUT_PULLUP);
  attachInterrupt(CHARGE_PIN, [] {
      charge_indication = true;
  }, CHANGE);
  if (digitalRead(CHARGE_PIN) == LOW) {
      charge_indication = true;
  }

}

void setupTFT() {
  tft.init();
  tft.setRotation(1);
  tft.setSwapBytes(true);
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_YELLOW, TFT_BLACK); // Note: the new fonts do not draw the background color
}

void setupVolt() {
    esp_adc_cal_characteristics_t adc_chars;
    esp_adc_cal_value_t val_type = esp_adc_cal_characterize((adc_unit_t)ADC_UNIT_1, (adc_atten_t)ADC1_CHANNEL_6, (adc_bits_width_t)ADC_WIDTH_BIT_12, 1100, &adc_chars);
    //Check type of calibration value used to characterize ADC
    if (val_type == ESP_ADC_CAL_VAL_EFUSE_VREF) {
//        Serial.printf("eFuse Vref:%u mV", adc_chars.vref);
        vref = adc_chars.vref;
    } else if (val_type == ESP_ADC_CAL_VAL_EFUSE_TP) {
//        Serial.printf("Two Point --> coeff_a:%umV coeff_b:%umV\n", adc_chars.coeff_a, adc_chars.coeff_b);
    } else {
//        Serial.println("Default Vref: 1100mV");
    }
}

void setupRTC() {
    rtc.begin(Wire);
    //Check if the RTC clock matches, if not, use compile time
    rtc.check();

    RTC_Date datetime = rtc.getDateTime();
    hh = datetime.hour;
    mm = datetime.minute;
    ss = datetime.second;
}

void setup(void) {
  WiFi.mode(WIFI_OFF);
  setCpuFrequencyMhz(80);         // scaling it back saves so much of the precious battery life
  btStop();
  setupWire();
  setupTFT();
  setupRTC();
  setupVolt();
  if(boots == 0) //Run this only the first time
  {
    targetTime = millis() + 1000;  //adds 1 min to compiler time
    boots ++;
  }
  setupPins();
}

String Volt() {
    uint16_t v = analogRead(BATT_ADC_PIN);
    float battery_voltage = ((float)v / 4095.0) * 2.0 * 3.3 * (vref / 1000.0);
    return String(battery_voltage) + "V";
}

void RTC() {
    if (targetTime < millis()) {
        RTC_Date datetime = rtc.getDateTime();
        hh = datetime.hour;
        mm = datetime.minute;
        ss = datetime.second;
        // Serial.printf("hh:%d mm:%d ss:%d\n", hh, mm, ss);
        targetTime = millis() + 1000;
        if (ss == 0 || initial) {
            initial = 0;
            tft.setTextColor(TFT_GREEN, TFT_BLACK);
            tft.setCursor (8, 60);
            tft.print(__DATE__); // This uses the standard ADAFruit small font
        }
        tft.setTextColor(TFT_BLUE, TFT_BLACK);
        tft.drawCentreString(Volt(), 120, 60, 1); // Next size up font 2
        // Update digital time
        uint8_t xpos = 6;
        uint8_t ypos = 0;
        if (omm != mm) { // Only redraw every minute to minimise flicker
            // Uncomment ONE of the next 2 lines, using the ghost image demonstrates text overlay as time is drawn over it
            tft.setTextColor(0x39C4, TFT_BLACK);  // Leave a 7 segment ghost image, comment out next line!
            //tft.setTextColor(TFT_BLACK, TFT_BLACK); // Set font colour to black to wipe image
            // Font 7 is to show a pseudo 7 segment display.
            // Font 7 only contains characters [space] 0 1 2 3 4 5 6 7 8 9 0 : .
            tft.drawString("88:88", xpos, ypos, 7); // Overwrite the text to clear it
            tft.setTextColor(0xFBE0, TFT_BLACK); // Orange
            omm = mm;

            if (hh < 10) xpos += tft.drawChar('0', xpos, ypos, 7);
            xpos += tft.drawNumber(hh, xpos, ypos, 7);
            xcolon = xpos;
            xpos += tft.drawChar(':', xpos, ypos, 7);
            if (mm < 10) xpos += tft.drawChar('0', xpos, ypos, 7);
            tft.drawNumber(mm, xpos, ypos, 7);
        }

        if (ss % 2) { // Flash the colon
            tft.setTextColor(0x39C4, TFT_BLACK);
            xpos += tft.drawChar(':', xcolon, ypos, 7);
            tft.setTextColor(0xFBE0, TFT_BLACK);
        } else {
            tft.drawChar(':', xcolon, ypos, 7);
        }
    }
}

void Sleep() {
  tft.fillScreen(TFT_BLACK);
  tft.writecommand(ST7735_SLPIN);
  tft.writecommand(ST7735_DISPOFF);
  delay(200);
  esp_sleep_enable_ext1_wakeup(GPIO_SEL_33, ESP_EXT1_WAKEUP_ANY_HIGH);
  esp_deep_sleep_start();
}

void loop() {
  if (charge_indication) {
      charge_indication = false;
      if (digitalRead(CHARGE_PIN) == LOW) {
          tft.pushImage(140, 55, 16, 16, charge);
      } else {
          tft.fillRect(140, 55, 16, 16, TFT_BLACK);
      }
  }
  if (digitalRead(TP_PIN_PIN) == HIGH) {
      if (!pressed) {
          initial = 1;
          targetTime = millis() + 1000;
          tft.fillScreen(TFT_BLACK);
          omm = 99;
          func_select = func_select + 1 > 2 ? 0 : func_select + 1;
          pressed = true;
          pressedTime = millis();
      } else {
          if (millis() - pressedTime > 3000) {
              esp_restart();
          }
      }
  } else {
      pressed = false;
  }
  switch (func_select) {
    case 0:
      for(int i = 0; i < 5; i ++)
      {
        RTC();
      }
      Sleep();
      break;
    case 1:
      Sleep();
      break;
    default:
      break;
  }
}
