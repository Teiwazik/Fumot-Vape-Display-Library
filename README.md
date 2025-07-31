# Fumot-Vape-Display-Library
Fumot_Display is an Arduino library for controlling a custom 6-wire Charlieplexed LED display. It can show two digits, symbols, text (English &amp; Russian), and cool animations like falling drops. Russian tho in not very good invented because of 7 segment display

---

## 🔌 Pin Configuration

This display uses **6 digital pins** and 30 Charlieplexed LEDs:

```
Arduino pins: 2, 3, 4, 5, 6, 7
```

Each LED is controlled using unique pin pairs.

---

## ⚡ Example Usage

```cpp
#include <Fumot_Display.h>

Fumot_Display display;

void setup() {
  display.begin();
  display.printNumber(4, 2);        // Display "42"
  display.setPercent(true);         // Show percent symbol
  display.setLightning(true);       // Show lightning symbol
  display.scrollText("HELLO");      // Start scrolling text
}

void loop() {
  display.updateScroll();           // Update scrolling frame
  display.animateDrop();            // Animate puddle drop
  display.animateDrop2();           // Animate second drop (LEDs 20–23)
  display.tick();                   // Keep display running (required)
}
```

---

## 🔧 Available Functions

| Function                     | Description                             |
|------------------------------|-----------------------------------------|
| `begin()`                    | Initialize display pins                 |
| `printNumber(left, right)`   | Show two digits (0–9)                   |
| `printChar('A')`             | Show single character (right digit only)|
| `printText("HI")`            | Show two characters                     |
| `scrollText("HELLO")`        | Start scrolling text                    |
| `updateScroll()`             | Update scroll frame                     |
| `animateDrop()`              | Animate puddle (5 LEDs)                 |
| `animateDrop2()`             | Animate second drop (LEDs 20–23)        |
| `setPercent(true/false)`     | Show/hide percent symbol                |
| `setLightning(true/false)`   | Show/hide lightning symbol              |
| `tick()`                     | Main refresh function (call every loop) |

---

## 💡 Notes

- `tick()` **must be called frequently** in the main `loop()` to keep the display running.
- The `scrollText()` function supports **Latin and Cyrillic letters** (`А-Я`).
- You can skip `animateDrop()` or `animateDrop2()` if not needed.
- Digits and symbols are mapped to a 30-LED Charlieplex layout.
- Supports percent symbol (LED 16) and lightning bolt (LED 17).


---

# 🧰 7-Segment Symbol Editor (Python Tool)

A lightweight graphical tool built with **Tkinter** to visually design and export 7-segment characters.

## 🔧 Features

- Click to toggle segments (A–G)
- Automatically generates the corresponding **bit pattern** (`0bXXXXXXX`)
- Displays both **binary** and **hexadecimal** codes
- Automatically **copies the result to clipboard**
- Useful for creating custom characters for `Fumot_Display`

---

## 📦 Installation

1. Download or clone this repository.
2. Copy the `Fumot_Display` folder into your Arduino `libraries/` directory.
3. Restart the Arduino IDE.
4. Include the library in your sketch:

```cpp
#include <Fumot_Display.h>
```

---

## 🧪 Requirements

- Arduino Uno or compatible board
- 6 digital I/O pins
- Custom Charlieplexed display with 30 LEDs

---

## 📜 License

This project is licensed under the MIT License.  
Feel free to modify or adapt it to your own Charlieplex display!

---

## 🛠 Author

Made by [Teiwazik] – feel free to contribute or open issues.
