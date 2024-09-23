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
    
    // Handle specific keys based on the provided image configuration

    // Enter key (0x0D in your image)
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
    // Arrow keys (Up, Down, Left, Right)
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
    // Control key (0x80 - as a placeholder, adjust based on keymap)
    else if (key == 0x80) {  
      Keyboard.press(KEY_LEFT_CTRL);
      delay(100);  // Adjust the delay as needed
      Keyboard.releaseAll();
      Serial.println("Control Key Pressed");
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
