#include "Fumot_Display.h"

Fumot_Display display;

void setup() {
  display.begin();
}

void loop() {
  // Example of chaanging 2 letters every 500 seconds
  const char* words[] = { "HI", "ON", "UP", "AF", "GO", "NO" };
  const int wordCount = sizeof(words) / sizeof(words[0]);

  for (int i = 0; i < wordCount; i++) {
    display.printText(words[i]);

    unsigned long start = millis();
    while (millis() - start < 500) {
      display.tick();  // Update multiplex
    }
  }
}
