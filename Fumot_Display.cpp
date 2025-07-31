#include "Fumot_Display.h"
#include <ctype.h>
#include <string.h>

// --- Static constants ---
const uint8_t Fumot_Display::pins[6] = {2, 3, 4, 5, 6, 7};

const uint8_t Fumot_Display::led_pairs[30][2] = {
  {2,3},{2,4},{2,5},{2,6},{2,7},
  {3,2},{3,4},{3,5},{3,6},{3,7},
  {4,2},{4,3},{4,5},{4,6},{4,7},
  {5,2},{5,3},{5,4},{5,6},{5,7},
  {6,2},{6,3},{6,4},{6,5},{6,7},
  {7,2},{7,3},{7,4},{7,5},{7,6}
};

const uint8_t Fumot_Display::left_digit_segments[7]  = {11, 6, 12, 7, 8, 13, 18};
const uint8_t Fumot_Display::right_digit_segments[7] = {5, 0, 10, 1, 15, 2, 3};
const uint8_t Fumot_Display::puddle[5] = {26, 27, 28, 29, 25};

const uint8_t Fumot_Display::puddle_pairs[5][2] = {
  {26, 27}, {27, 28}, {28, 29}, {29, 25}, {25, 26}
};

const uint8_t Fumot_Display::segmap[10] = {
  0b1111110, 0b0110000, 0b1101101, 0b1111001, 0b0110011,
  0b1011011, 0b1011111, 0b1110000, 0b1111111, 0b1111011
};

const uint8_t Fumot_Display::char_map[26] = {
  0b1110111, // A
  0b0011111, // B
  0b1001110, // C
  0b0111101, // D
  0b1001111, // E
  0b1000111, // F
  0b1011110, // G
  0b0110111, // H
  0b0110000, // I
  0b0111100, // J
  0b1010111, // K
  0b0001110, // L
  0b1110110, // M
  0b1110110, // N
  0b1111110, // O
  0b1100111, // P
  0b1110011, // Q
  0b1101111, // R
  0b1011011, // S
  0b0000111, // T
  0b0111110, // U
  0b0111110, // V
  0b0111110, // W
  0b0110111, // X
  0b0111011, // Y
  0b1101101  // Z
};

const uint8_t Fumot_Display::rus_map[32] = {
  0b1110111, // А (Аналог A)
  0b1111101,  // Б
  0b1111100,  // В
  0b0110001,  // Г
  0b1011110,  // Д
  0b1111001,  // Е
  0b0110111,  // Ж
  0b1001111,  // З
  0b0000110,  // И
  0b0010101,  // Й
  0b1010111,  // К
  0b0110111,  // Л
  0b1010100,  // М
  0b1110110,  // Н
  0b0111111,  // О
  0b0110111,  // П
  0b1110011,  // Р
  0b0111001,  // С
  0b1111000,  // Т
  0b1101110,  // У
  0b1101011,  // Ф
  0b1110110,  // Х
  0b1100110,  // Ц
  0b1100110,  // Ч
  0b1100010,  // Ш
  0b1100110,  // Щ
  0b1111100,  // Ъ
  0b0010101,  // Ы
  0b1111100,  // Ь
  0b1001111,  // Э
  0b1110000,  // Ю
  0b1010111   // Я
};

// --- Constructor ---
Fumot_Display::Fumot_Display() :
  currentLeftDigit(0), currentRightDigit(0),
  puddleFrame(0), lastTickTime(0), multiplexStep(0),
  scrollIndex(0), lastScrollTime(0), scrollSpeed(350),
  showPercent(false), showLightning(false),
  separateDropFrame(0), drop2Frame(0), lastDropFrameTime(0)
{
  scrollBuffer[0] = '\0';
}

// --- First drop anim method ---
void Fumot_Display::animateDrop() {
  puddleFrame = (puddleFrame + 1) % 5;
}

// --- Second drop anim method ---
void Fumot_Display::animateDrop2() {
  static const uint8_t dropFrames[][2] = {
    {20, 255}, {20, 21}, {21, 22}, {22, 23},
    {23, 255}, {22, 23}, {21, 22}, {20, 21}
  };

  unsigned long now = millis();
  if (now - lastDropFrameTime < 100) return;
  lastDropFrameTime = now;

  drop2Frame = (drop2Frame + 1) % (sizeof(dropFrames) / sizeof(dropFrames[0]));

  uint8_t a = dropFrames[drop2Frame][0];
  uint8_t b = dropFrames[drop2Frame][1];

  currentSeparateDropPattern = 0;
  currentSeparateDropPattern |= (1 << (a - 20));
  if (b != 255) {
    currentSeparateDropPattern |= (1 << (b - 20));
  }
}

// --- Initialization ---
void Fumot_Display::begin() {
  setAllHiZ();
}

// --- Getting symbhol pattern ---
uint8_t Fumot_Display::getCharPattern(const char* &p) {
  if (!p || *p == '\0') return 0;

  if (((uint8_t)*p & 0x80) == 0) {
    char c = *p;
    p++;
    if (c == ' ') return 0;
    if (c >= '0' && c <= '9') return segmap[c - '0'];
    if (c >= 'A' && c <= 'Z') return char_map[c - 'A'];
    if (c >= 'a' && c <= 'z') return char_map[c - 'a'];
    return 0;
  }

  if (((uint8_t)*p & 0xE0) == 0xC0) {
    uint8_t first = (uint8_t)(*p++);
    if (*p == '\0') return 0;
    uint8_t second = (uint8_t)(*p++);
    uint16_t code = ((first & 0x1F) << 6) | (second & 0x3F);

    if (code >= 0x0410 && code <= 0x042F) {
      return rus_map[code - 0x0410];
    }

    if (code >= 0x0430 && code <= 0x044F) {
      return rus_map[code - 0x0430];
    }

    return 0;
  }

  p++;
  return 0;
}

// --- Out number ---
void Fumot_Display::printNumber(uint8_t leftDigit, uint8_t rightDigit) {
  if (leftDigit > 9) leftDigit = 0;
  if (rightDigit > 9) rightDigit = 0;
  currentLeftDigit = segmap[leftDigit];
  currentRightDigit = segmap[rightDigit];
}

// --- One number out ---
void Fumot_Display::printChar(char c) {
  const char* p = &c;
  currentLeftDigit = 0;
  currentRightDigit = getCharPattern(p);
}

// --- Out text ---
void Fumot_Display::printText(const char* text) {
  if (!text) return;
  const char* p = text;
  uint8_t left = getCharPattern(p);
  uint8_t right = getCharPattern(p);
  currentLeftDigit = left;
  currentRightDigit = right;
}

// --- Scroll start ---
void Fumot_Display::scrollText(const char* text, uint16_t speedMs) {
  if (!text) return;
  strncpy(scrollBuffer, text, sizeof(scrollBuffer) - 1);
  scrollBuffer[sizeof(scrollBuffer) - 1] = '\0';
  scrollIndex = 0;
  scrollSpeed = speedMs;
  lastScrollTime = millis();
}

// --- Scroll update ---
void Fumot_Display::updateScroll() {
  unsigned long now = millis();
  if (now - lastScrollTime < scrollSpeed) return;
  lastScrollTime = now;

  char left = scrollBuffer[scrollIndex];
  char right = scrollBuffer[scrollIndex + 1];

  if (!left) left = ' ';
  if (!right) right = ' ';

  char pair[3] = { left, right, '\0' };
  printText(pair);

  size_t len = strlen(scrollBuffer);

  if (scrollIndex < len + 1) {
    scrollIndex++;
  } else {
    scrollIndex = 0;
  }
}

// --- Flags with precent and lightning ---
void Fumot_Display::setPercent(bool enabled) {
  showPercent = enabled;
}

void Fumot_Display::setLightning(bool enabled) {
  showLightning = enabled;
}

// --- Main update cycle (multiplexing) ---
void Fumot_Display::tick() {
  unsigned long now = millis();
  if (now - lastTickTime < 1) return;
  lastTickTime = now;

  if (multiplexStep < 7) {
    if (currentLeftDigit & (1 << (6 - multiplexStep))) {
      lightLed(left_digit_segments[multiplexStep]);
    } else setAllHiZ();
  } else if (multiplexStep < 14) {
    uint8_t i = multiplexStep - 7;
    if (currentRightDigit & (1 << (6 - i))) {
      lightLed(right_digit_segments[i]);
    } else setAllHiZ();
  } else if (multiplexStep == 14) {
    lightLed(puddle_pairs[puddleFrame][0]);
  } else if (multiplexStep == 15) {
    lightLed(puddle_pairs[puddleFrame][1]);
  } else if (multiplexStep == 16 && showPercent) {
    lightLed(16);
  } else if (multiplexStep == 17 && showLightning) {
    lightLed(17);
  } 
  // New 4 steps for second drop
  else if (multiplexStep >= 18 && multiplexStep < 22) {
    uint8_t ledIndex = 20 + (multiplexStep - 18);
    if (currentSeparateDropPattern & (1 << (multiplexStep - 18))) {
      lightLed(ledIndex);
    } else {
      setAllHiZ();
    }
  } else {
    setAllHiZ();
  }

  if (++multiplexStep >= 22) multiplexStep = 0;
}

// --- Turn on led ---
void Fumot_Display::lightLed(uint8_t ledNum) {
  if (ledNum >= 30) return;
  setAllHiZ();
  uint8_t highPin = led_pairs[ledNum][0];
  uint8_t lowPin = led_pairs[ledNum][1];
  setPinState(highPin, true);
  setPinState(lowPin, false);
  delayMicroseconds(500);
  setAllHiZ();
}

// --- Disable all of pins (HiZ) ---
void Fumot_Display::setAllHiZ() {
  for (uint8_t i = 0; i < 6; i++) {
    pinMode(pins[i], INPUT);
    digitalWrite(pins[i], LOW);
  }
}

// --- Set pin state ---
void Fumot_Display::setPinState(uint8_t pin, bool high) {
  pinMode(pin, OUTPUT);
  digitalWrite(pin, high ? HIGH : LOW);
}
