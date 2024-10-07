
//This will run on both Xiao and addafruit esp32-s3 
//By: Javi October 2024
//This is the same as the regular USBHID with the added feature of using F1-F4 for macros/duckysripts 
//F2 is for mac 
//F3 is for windows 
//F4 is for Linux

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
    // FN key combinations for F1 to F10
    else if (key >= 0x81 && key <= 0x8A) {
      handleFnKey(key);
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

void handleFnKey(char key) {
  switch (key) {
    case 0x81:  // FN + 1 -> Open Terminal and run "ls"
      // Open Spotlight (Cmd + Space)
      Keyboard.press(KEY_LEFT_GUI);  // Cmd key on Mac
      Keyboard.press(' ');
      Keyboard.releaseAll();
      delay(500);  // Wait for Spotlight to open

      // Type "Terminal"
      Keyboard.print("Terminal");
      delay(500);  // Wait for typing

      // Press Enter to open Terminal
      Keyboard.write(KEY_RETURN);
      delay(1000);  // Wait for Terminal to open

      // Type the command "ls"
      Keyboard.print("ls");
      delay(500);  // Small delay before pressing Enter

      // Press Enter to execute the command
      Keyboard.write(KEY_RETURN);
      Serial.println("Terminal Command 'ls' Executed");
      break;


case 0x82:  // FN + 2 -> Open TextEdit, create a new file, and type text
  // Open Spotlight (Cmd + Space)
  Keyboard.press(KEY_LEFT_GUI);  // Cmd key on Mac
  Keyboard.press(' ');
  Keyboard.releaseAll();
  delay(500);  // Wait for Spotlight to open

  // Type "TextEdit"
  Keyboard.print("TextEdit");
  delay(500);

  // Press Enter to open TextEdit
  Keyboard.write(KEY_RETURN);
  delay(1000);  // Wait for TextEdit to open

  // Press Cmd + N to create a new document
  Keyboard.press(KEY_LEFT_GUI);  // Cmd key
  Keyboard.press('n');
  Keyboard.releaseAll();
  delay(500);  // Wait for new document window

  // Type the initial message
  Keyboard.print("Hello, you have been hacked by Javi!\n\n");

  // Type the recipe for Pasta Carbonara
  Keyboard.print("Recipe for Pasta Carbonara for Two:\n");
  Keyboard.print("Ingredients:\n");
  Keyboard.print("- 200g spaghetti\n");
  Keyboard.print("- 100g pancetta or guanciale\n");
  Keyboard.print("- 2 large eggs\n");
  Keyboard.print("- 50g grated Pecorino Romano cheese\n");
  Keyboard.print("- Freshly ground black pepper\n");
  Keyboard.print("- Salt\n\n");

  Keyboard.print("Instructions:\n");
  Keyboard.print("1. Boil water in a large pot, add salt, and cook the spaghetti until al dente.\n");
  Keyboard.print("2. In a separate pan, cook the pancetta until crispy.\n");
  Keyboard.print("3. Beat the eggs in a bowl and mix in the grated cheese and pepper.\n");
  Keyboard.print("4. Drain the pasta, reserving some water, and quickly add it to the pancetta.\n");
  Keyboard.print("5. Remove from heat and stir in the egg mixture, adding pasta water to create a creamy sauce.\n");
  Keyboard.print("6. Serve immediately with extra cheese and pepper on top.\n");

  // Save the file using Cmd + S
  Keyboard.press(KEY_LEFT_GUI);  // Cmd key
  Keyboard.press('s');
  Keyboard.releaseAll();
  delay(500);  // Wait for Save dialog

  // Type the file name "Hacked.txt"
  Keyboard.print("Hacked");
  delay(500);

  // Press Enter to save
  Keyboard.write(KEY_RETURN);
  delay(500);  // Wait for save to complete

  // Close TextEdit using Cmd + W
  Keyboard.press(KEY_LEFT_GUI);  // Cmd key
  Keyboard.press('w');
  Keyboard.releaseAll();
  Serial.println("TextEdit Payload Executed");
  break;

    case 0x83:  // FN + 3 -> Open Notepad, create a new file, and type text
  // Open Run dialog (Win + R)
  Keyboard.press(KEY_LEFT_GUI);  // Win key on Windows
  Keyboard.press('r');
  Keyboard.releaseAll();
  delay(500);  // Wait for Run dialog to open

  // Type "notepad" and press Enter
  Keyboard.print("notepad");
  delay(500);
  Keyboard.write(KEY_RETURN);
  delay(1000);  // Wait for Notepad to open

  // Type the initial message
  Keyboard.print("Hello, you have been hacked by Javi!\n\n");

  // Type the recipe for Pasta Carbonara
  Keyboard.print("Recipe for Pasta Carbonara for Two:\n");
  Keyboard.print("Ingredients:\n");
  Keyboard.print("- 200g spaghetti\n");
  Keyboard.print("- 100g pancetta or guanciale\n");
  Keyboard.print("- 2 large eggs\n");
  Keyboard.print("- 50g grated Pecorino Romano cheese\n");
  Keyboard.print("- Freshly ground black pepper\n");
  Keyboard.print("- Salt\n\n");

  Keyboard.print("Instructions:\n");
  Keyboard.print("1. Boil water in a large pot, add salt, and cook the spaghetti until al dente.\n");
  Keyboard.print("2. In a separate pan, cook the pancetta until crispy.\n");
  Keyboard.print("3. Beat the eggs in a bowl and mix in the grated cheese and pepper.\n");
  Keyboard.print("4. Drain the pasta, reserving some water, and quickly add it to the pancetta.\n");
  Keyboard.print("5. Remove from heat and stir in the egg mixture, adding pasta water to create a creamy sauce.\n");
  Keyboard.print("6. Serve immediately with extra cheese and pepper on top.\n");

  // Save the file using Ctrl + S
  Keyboard.press(KEY_LEFT_CTRL);  // Ctrl key on Windows
  Keyboard.press('s');
  Keyboard.releaseAll();
  delay(500);  // Wait for Save dialog

  // Type the file name "Hacked.txt"
  Keyboard.print("Hacked.txt");
  delay(500);

  // Press Enter to save
  Keyboard.write(KEY_RETURN);
  delay(500);  // Wait for the file to save

  // Close Notepad window using Alt + F4
  Keyboard.press(KEY_LEFT_ALT);  // Alt key
  Keyboard.press(KEY_F4);
  Keyboard.releaseAll();
  Serial.println("Notepad Payload Executed");
  break;

    case 0x84:  // FN + 4 -> Open terminal, create a new file with nano, and type text
  // Open terminal (Ctrl + Alt + T)
  Keyboard.press(KEY_LEFT_CTRL);  // Ctrl key
  Keyboard.press(KEY_LEFT_ALT);    // Alt key
  Keyboard.press('t');
  Keyboard.releaseAll();
  delay(1000);  // Wait for the terminal to open

  // Type the command to create a new file using nano
  Keyboard.print("nano Hacked.txt\n");
  delay(1000);  // Wait for nano to open

  // Type the initial message
  Keyboard.print("Hello, you have been hacked by Javi!\n\n");

  // Type the recipe for Pasta Carbonara
  Keyboard.print("Recipe for Pasta Carbonara for Two:\n");
  Keyboard.print("Ingredients:\n");
  Keyboard.print("- 200g spaghetti\n");
  Keyboard.print("- 100g pancetta or guanciale\n");
  Keyboard.print("- 2 large eggs\n");
  Keyboard.print("- 50g grated Pecorino Romano cheese\n");
  Keyboard.print("- Freshly ground black pepper\n");
  Keyboard.print("- Salt\n\n");

  Keyboard.print("Instructions:\n");
  Keyboard.print("1. Boil water in a large pot, add salt, and cook the spaghetti until al dente.\n");
  Keyboard.print("2. In a separate pan, cook the pancetta until crispy.\n");
  Keyboard.print("3. Beat the eggs in a bowl and mix in the grated cheese and pepper.\n");
  Keyboard.print("4. Drain the pasta, reserving some water, and quickly add it to the pancetta.\n");
  Keyboard.print("5. Remove from heat and stir in the egg mixture, adding pasta water to create a creamy sauce.\n");
  Keyboard.print("6. Serve immediately with extra cheese and pepper on top.\n");

  // Save the file in nano using Ctrl + O
  Keyboard.press(KEY_LEFT_CTRL);  // Ctrl key
  Keyboard.press('o');
  Keyboard.releaseAll();
  delay(500);  // Wait for the save prompt

  // Press Enter to confirm saving the file
  Keyboard.write(KEY_RETURN);
  delay(500);  // Wait for the save confirmation

  // Exit nano using Ctrl + X
  Keyboard.press(KEY_LEFT_CTRL);  // Ctrl key
  Keyboard.press('x');
  Keyboard.releaseAll();
  Serial.println("Nano Payload Executed");
  break;

    case 0x85:  // FN + 5 -> F5
      Keyboard.write(KEY_F5);
      break;
    case 0x86:  // FN + 6 -> F6
      Keyboard.write(KEY_F6);
      break;
    case 0x87:  // FN + 7 -> F7
      Keyboard.write(KEY_F7);
      break;
    case 0x88:  // FN + 8 -> F8
      Keyboard.write(KEY_F8);
      break;
    case 0x89:  // FN + 9 -> F9
      Keyboard.write(KEY_F9);
      break;
    case 0x8A:  // FN + 0 -> F10
      Keyboard.write(KEY_F10);
      break;
  }
}


// Function to handle Control key combinations
void handleCtrlCombination(char key) {
  Keyboard.press(KEY_LEFT_CTRL); 

  switch (key) {
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
