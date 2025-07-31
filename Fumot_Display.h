#pragma once
#include <Arduino.h>

class Fumot_Display {
public:
  Fumot_Display();

  void begin();

  void printNumber(uint8_t leftDigit, uint8_t rightDigit);
  void printChar(char c);
  void printText(const char* text);

  void scrollText(const char* text, uint16_t speedMs = 350);
  void updateScroll();

  void animateDrop();
  void animateDrop2();

  void tick();

  void setPercent(bool enabled);
  void setLightning(bool enabled);

private:
  static const uint8_t pins[6];
  static const uint8_t led_pairs[30][2];

  static const uint8_t left_digit_segments[7];
  static const uint8_t right_digit_segments[7];
  static const uint8_t puddle[5];
  static const uint8_t puddle_pairs[5][2];

  static const uint8_t segmap[10];
  static const uint8_t char_map[26]; // A–Z
  static const uint8_t rus_map[32];  // Added Russian characters

  uint8_t currentLeftDigit;
  uint8_t currentRightDigit;

  uint8_t drop2Frame;
  uint8_t puddleFrame;
  uint8_t multiplexStep;
  unsigned long lastTickTime;



  // Floating drop animation
  void lightLed(uint8_t ledNum);
  void setAllHiZ();
  void setPinState(uint8_t pin, bool high);

  uint8_t separateDropFrame;
  unsigned long lastDropFrameTime;

  uint8_t currentSeparateDropPattern;

  // Scroll
  char scrollBuffer[32];
  uint8_t scrollIndex;
  unsigned long lastScrollTime;
  uint16_t scrollSpeed;

  // Flags
  bool showPercent;
  bool showLightning;

  // Function to get character pattern
  uint8_t getCharPattern(const char*& p);
};
