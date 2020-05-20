#include <Adafruit_GFX.h>

#include <gfxfont.h>

#include "Adafruit_ILI9341_8bit.h"

Adafruit_ILI9341_8bit_STM tft = Adafruit_ILI9341_8bit_STM();
#define USE_BUFFER   // buffer all 155 KB of data for bliting - uses passive ram but looks nicer?

const int16_t
  bits        = 20,   // Fractional resolution
  pixelWidth  = 480,  // TFT dimensions
  pixelHeight = 320,
  iterations  = 128;  // Fractal iteration limit or 'dwell'
float
  centerReal  = -0.6, // Image center point in complex plane
  centerImag  =  0.0,
  rangeReal   =  3.0, // Image coverage in complex plane
  rangeImag   =  3.0;

#if defined(USE_BUFFER)
  uint16_t buffer[pixelWidth * pixelHeight];
#endif

void setup(void) {
  Serial.begin(115200);
  Serial.print("Mandelbrot drawer!");

  tft.begin();
  tft.setRotation(1);
  tft.fillScreen(ILI9341_BLACK);

  // Turn on backlight (required on PyPortal)
#if defined(TFT_BACKLIGHT)
  pinMode(TFT_BACKLIGHT, OUTPUT);
  digitalWrite(TFT_BACKLIGHT, HIGH);
#endif
}

void loop() {
  int64_t       n, a, b, a2, b2, posReal, posImag;
  uint32_t      startTime,elapsedTime;


  int32_t
    startReal   = (int64_t)((centerReal - rangeReal * 0.5)   * (float)(1 << bits)),
    startImag   = (int64_t)((centerImag + rangeImag * 0.5)   * (float)(1 << bits)),
    incReal     = (int64_t)((rangeReal / (float)pixelWidth)  * (float)(1 << bits)),
    incImag     = (int64_t)((rangeImag / (float)pixelHeight) * (float)(1 << bits));
  
  startTime = millis();
  posImag = startImag;
  for (int y = 0; y < pixelHeight; y++) {
    posReal = startReal;
    for (int x = 0; x < pixelWidth; x++) {
      a = posReal;
      b = posImag;
      for (n = iterations; n > 0 ; n--) {
        a2 = (a * a) >> bits;
        b2 = (b * b) >> bits;
        if ((a2 + b2) >= (4 << bits)) 
          break;
        b  = posImag + ((a * b) >> (bits - 1));
        a  = posReal + a2 - b2;
      }
      #if defined(USE_BUFFER)
        buffer[y * pixelWidth + x] = (n * 29)<<8 | (n * 67);
      #else
        tft.drawPixel(x, y, (n * 29)<<8 | (n * 67)); // takes 500ms with individual pixel writes
      #endif
      posReal += incReal;
    }
    posImag -= incImag;
  }
  #if defined(USE_BUFFER)
    tft.drawRGBBitmap(0, 0, buffer, pixelWidth, pixelHeight); // takes 169 ms
  #endif
  elapsedTime = millis()-startTime;
  Serial.print("Took "); Serial.print(elapsedTime); Serial.println(" ms");

  rangeReal *= 0.95;
  rangeImag *= 0.95;
}
