//This will run on both Xiao and addafruit esp32-s3 
//By: Javi August/september 2024
#include <Wire.h>
#include "USB.h"
#include "USBHIDKeyboard.h"

USBHIDKeyboard Keyboard;

#define CARDKB_I2C_ADDR 0x5F  // I2C address for the M5 CardKB

void setup() {
  // Start I2C communication on Wire1
  Wire.begin();
  
  // Start Serial for debugging
  Serial.begin(115200);
  
  // Initialize USB and keyboard
  USB.begin();
  Keyboard.begin();
}

void loop() {
  // Request 1 byte of data from CardKB
  Wire.requestFrom(CARDKB_I2C_ADDR, 1);
  
  // If data is available from CardKB
  if (Wire.available()) {
    char key = Wire.read();
    
    // Enter key (0x0D)
    if (key == 0x0D) {  
      Keyboard.write(KEY_RETURN);
      Serial.println("Enter Key Pressed");
    }
    // Escape key (0x1B)
    else if (key == 0x1B) {
      Keyboard.write(KEY_ESC);
      Serial.println("Escape Key Pressed");
    }
    // Backspace key (0x08)
    else if (key == 0x08) {
      Keyboard.write(KEY_BACKSPACE);
      Serial.println("Backspace Key Pressed");
    }
    // Arrow keys
    else if (key == 0xB5) {  // Up Arrow
      Keyboard.write(KEY_UP_ARROW);
      Serial.println("Up Arrow Pressed");
    }
    else if (key == 0xB4) {  // Left Arrow
      Keyboard.write(KEY_LEFT_ARROW);
      Serial.println("Left Arrow Pressed");
    }
    else if (key == 0xB6) {  // Down Arrow
      Keyboard.write(KEY_DOWN_ARROW);
      Serial.println("Down Arrow Pressed");
    }
    else if (key == 0xB7) {  // Right Arrow
      Keyboard.write(KEY_RIGHT_ARROW);
      Serial.println("Right Arrow Pressed");
    }
    // Control key combinations
    else if (key >= 0x80 && key <= 0xAF) {
      handleCtrlCombination(key);
    }
    else {
      // Send other keys as normal characters
      Keyboard.write(key);
      Serial.print("Key Pressed: ");
      Serial.println(key);
    }
  }

  delay(100);  // Adjust the delay to optimize performance
}

// Function to handle Control key combinations using fn key
void handleCtrlCombination(char key) {
  Keyboard.press(KEY_LEFT_CTRL);

  switch (key) {
    case 0x81: Keyboard.write('1'); break;  // Ctrl + 1
    case 0x82: Keyboard.write('2'); break;  // Ctrl + 2
    case 0x83: Keyboard.write('3'); break;  // Ctrl + 3
    case 0x84: Keyboard.write('4'); break;  // Ctrl + 4
    case 0x85: Keyboard.write('5'); break;  // Ctrl + 5
    case 0x86: Keyboard.write('6'); break;  // Ctrl + 6
    case 0x87: Keyboard.write('7'); break;  // Ctrl + 7
    case 0x88: Keyboard.write('8'); break;  // Ctrl + 8
    case 0x89: Keyboard.write('9'); break;  // Ctrl + 9
    case 0x8A: Keyboard.write('0'); break;  // Ctrl + 0
    case 0x8B: Keyboard.write(KEY_BACKSPACE); break;  // Ctrl + Backspace
    case 0x8C: Keyboard.write(KEY_TAB); break;  // Ctrl + Tab
    case 0x8D: Keyboard.write('q'); break;  // Ctrl + Q
    case 0x8E: Keyboard.write('w'); break;  // Ctrl + W
    case 0x8F: Keyboard.write('e'); break;  // Ctrl + E
    case 0x90: Keyboard.write('r'); break;  // Ctrl + R
    case 0x91: Keyboard.write('t'); break;  // Ctrl + T
    case 0x92: Keyboard.write('y'); break;  // Ctrl + Y
    case 0x93: Keyboard.write('u'); break;  // Ctrl + U
    case 0x94: Keyboard.write('i'); break;  // Ctrl + I
    case 0x95: Keyboard.write('o'); break;  // Ctrl + O
    case 0x96: Keyboard.write('p'); break;  // Ctrl + P
    case 0x9A: Keyboard.write('a'); break;  // Ctrl + A
    case 0x9B: Keyboard.write('s'); break;  // Ctrl + S
    case 0x9C: Keyboard.write('d'); break;  // Ctrl + D
    case 0x9D: Keyboard.write('f'); break;  // Ctrl + F
    case 0x9E: Keyboard.write('g'); break;  // Ctrl + G
    case 0x9F: Keyboard.write('h'); break;  // Ctrl + H
    case 0xA0: Keyboard.write('j'); break;  // Ctrl + J
    case 0xA1: Keyboard.write('k'); break;  // Ctrl + K
    case 0xA2: Keyboard.write('l'); break;  // Ctrl + L
    case 0xA3: Keyboard.write(KEY_RETURN); break;  // Ctrl + Enter
    case 0xA6: Keyboard.write('z'); break;  // Ctrl + Z
    case 0xA7: Keyboard.write('x'); break;  // Ctrl + X
    case 0xA8: Keyboard.write('c'); break;  // Ctrl + C
    case 0xA9: Keyboard.write('v'); break;  // Ctrl + V
    case 0xAA: Keyboard.write('b'); break;  // Ctrl + B
    case 0xAB: Keyboard.write('n'); break;  // Ctrl + N
    case 0xAC: Keyboard.write('m'); break;  // Ctrl + M
    case 0xAD: Keyboard.write(','); break;  // Ctrl + Comma
    case 0xAE: Keyboard.write('.'); break;  // Ctrl + Dot
    case 0xAF: Keyboard.write(' '); break;  // Ctrl + Space
    case 0x98: Keyboard.write(KEY_LEFT_ARROW); break;  // Ctrl + Left Arrow
    case 0x99: Keyboard.write(KEY_UP_ARROW); break;    // Ctrl + Up Arrow
    case 0xA4: Keyboard.write(KEY_DOWN_ARROW); break;  // Ctrl + Down Arrow
    case 0xA5: Keyboard.write(KEY_RIGHT_ARROW); break; // Ctrl + Right Arrow
  }

  Keyboard.releaseAll();
  Serial.println("Control Key Combination Pressed");
}
