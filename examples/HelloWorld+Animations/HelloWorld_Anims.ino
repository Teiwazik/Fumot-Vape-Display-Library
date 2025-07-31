#include <Fumot_Display.h>

Fumot_Display display;

unsigned long lastDropAnim = 0;
unsigned long lastDrop2Anim = 0;

unsigned long animTimer = 0;
bool animState = false;

void setup() {
  display.begin();
  display.scrollText(" HELLO WORLD", 300);
}

void loop() {
  display.tick();

  unsigned long now = millis();

  // Анимация капли
  if (now - lastDropAnim > 100) {
    display.animateDrop();
    lastDropAnim = now;
  }

  if (now - lastDrop2Anim > 100) {
    lastDrop2Anim = now;
    display.animateDrop2();  // запускаем анимацию второй капли
  }

  // Чередование молнии и процента
  if (now - animTimer > 500) {
    animTimer = now;
    animState = !animState;

    display.setLightning(animState);
    display.setPercent(!animState);
  }
  
  display.updateScroll();
}
