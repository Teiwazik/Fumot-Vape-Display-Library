#include <Fumot_Display.h>

Fumot_Display display;

unsigned int percent = 0;

void setup() {
  display.begin();
}

void loop() {
  display.printNumber(percent / 10, percent % 10);

  unsigned long now = millis();

  display.tick();

  // Scaling precent with pause
  static unsigned long lastPercentUpdate = 0;
  if (now - lastPercentUpdate > 1000) {
    percent = (percent + 1) % 100;
    lastPercentUpdate = now;
  }
}
