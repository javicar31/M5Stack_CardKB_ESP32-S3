
//By: Javi 2024
#include <BleKeyboard.h>
#include <Wire.h>
#include <Adafruit_NeoPixel.h>

#define CARDKB_I2C_ADDR 0x5F  // I2C address for the M5 CardKB

// NeoPixel setup for QTPY ESP32-S3
#define NUMPIXELS        1
#define NEOPIXEL_PIN     39
#define NEOPIXEL_POWER   38
Adafruit_NeoPixel pixels(NUMPIXELS, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);

// Create the BleKeyboard object with a custom name
BleKeyboard bleKeyboard("KeyboardCard");

bool isConnected = false;  // To track connection state
bool firstStart = true;    // To track if it's the first start before any connection
unsigned long redStartTime = 0;  // Track the time when disconnected
int fadeAmount = 1;  // Amount to increase/decrease brightness by
int brightness = 0; // Initial brightness for fading
bool fadingUp = true;  // Direction of fading
unsigned long lastFadeTime = 0;  // Track time for fading

void setup() {
  // Start Serial for debugging
  Serial.begin(115200);

  // Initialize NeoPixel
  pinMode(NEOPIXEL_POWER, OUTPUT);
  digitalWrite(NEOPIXEL_POWER, HIGH);  // Power up NeoPixel
  pixels.begin();

  // Start I2C communication
  Wire.begin();  // Use Wire1 if you are using a different I2C bus

  // Initialize BLE Keyboard
  bleKeyboard.begin();
  Serial.println("BLE Keyboard Initialized");

  // Set NeoPixel to initial blue state
  pixels.setBrightness(50);  // Set initial brightness
  pixels.fill(0x0000FF);  // Solid Blue
  pixels.show();
}

void loop() {
  // Check BLE connection status
  if (bleKeyboard.isConnected()) {
    if (!isConnected) {  // Only change color when connection state changes
      isConnected = true;
      firstStart = false;  // Bluetooth connection established, no longer first start
      // Set NeoPixel to solid green when connected
      pixels.setBrightness(5);  // Green standard brightness
      pixels.fill(0x00FF00);  // Solid Green
      pixels.show();
      Serial.println("BLE Connected");
    }
  } else {
    if (isConnected) {  // If it was connected and now disconnected
      isConnected = false;
      redStartTime = millis();  // Record the time it turns red
      // Set NeoPixel to solid red when disconnected
      pixels.setBrightness(5);  // Red standard brightness
      pixels.fill(0xFF0000);  // Solid Red
      pixels.show();
      Serial.println("BLE Disconnected");
    } else if (firstStart) {
      // Fade in and out blue until first connection is made
      fadeBlue();
    } else if (!isConnected && (millis() - redStartTime >= 10000)) {
      // If disconnected for 10 seconds, return to fading blue (Bluetooth mode)
      fadeToBlue();
      Serial.println("Returned to Bluetooth mode after 10 seconds of disconnection");
    }
  }

  // Request 1 byte of data from CardKB
  Wire.requestFrom(CARDKB_I2C_ADDR, 1);

  // Check if data is available
  if (Wire.available()) {
    char key = Wire.read();

    // Handle specific keys based on your keymap
    if (key == 0x0D) {  // Enter key
      bleKeyboard.write(KEY_RETURN);
      Serial.println("Enter Key Pressed");
    }
    else if (key == 0x1B) {  // Escape key
      bleKeyboard.write(KEY_ESC);
      Serial.println("Escape Key Pressed");
    }
    else if (key == 0x08) {  // Backspace key
      bleKeyboard.write(KEY_BACKSPACE);
      Serial.println("Backspace Key Pressed");
    }
    else if (key == 0xB5) {  // Up Arrow
      bleKeyboard.write(KEY_UP_ARROW);
      Serial.println("Up Arrow Pressed");
    }
    else if (key == 0xB4) {  // Left Arrow
      bleKeyboard.write(KEY_LEFT_ARROW);
      Serial.println("Left Arrow Pressed");
    }
    else if (key == 0xB6) {  // Down Arrow
      bleKeyboard.write(KEY_DOWN_ARROW);
      Serial.println("Down Arrow Pressed");
    }
    else if (key == 0xB7) {  // Right Arrow
      bleKeyboard.write(KEY_RIGHT_ARROW);
      Serial.println("Right Arrow Pressed");
    }
    // Handle Control key combinations
    else if (key >= 0x80 && key <= 0xAF) {
      handleCtrlCombination(key);
    }
    else {
      // Send other keys as normal characters
      bleKeyboard.write(key);
      Serial.print("Key Pressed: ");
      Serial.println(key);
    }
  }

  delay(50);  // Small delay to prevent flooding
}

// Function to handle Control key combinations
void handleCtrlCombination(char key) {
  bleKeyboard.press(KEY_LEFT_CTRL);

  switch (key) {
    case 0x81: bleKeyboard.write('1'); break;  // Ctrl + 1
    case 0x82: bleKeyboard.write('2'); break;  // Ctrl + 2
    case 0x83: bleKeyboard.write('3'); break;  // Ctrl + 3
    case 0x84: bleKeyboard.write('4'); break;  // Ctrl + 4
    case 0x85: bleKeyboard.write('5'); break;  // Ctrl + 5
    case 0x86: bleKeyboard.write('6'); break;  // Ctrl + 6
    case 0x87: bleKeyboard.write('7'); break;  // Ctrl + 7
    case 0x88: bleKeyboard.write('8'); break;  // Ctrl + 8
    case 0x89: bleKeyboard.write('9'); break;  // Ctrl + 9
    case 0x8A: bleKeyboard.write('0'); break;  // Ctrl + 0
    case 0x8B: bleKeyboard.write(KEY_BACKSPACE); break;  // Ctrl + Backspace
    case 0x8C: bleKeyboard.write(KEY_TAB); break;  // Ctrl + Tab
    case 0x8D: bleKeyboard.write('q'); break;  // Ctrl + Q
    case 0x8E: bleKeyboard.write('w'); break;  // Ctrl + W
    case 0x8F: bleKeyboard.write('e'); break;  // Ctrl + E
    case 0x90: bleKeyboard.write('r'); break;  // Ctrl + R
    case 0x91: bleKeyboard.write('t'); break;  // Ctrl + T
    case 0x92: bleKeyboard.write('y'); break;  // Ctrl + Y
    case 0x93: bleKeyboard.write('u'); break;  // Ctrl + U
    case 0x94: bleKeyboard.write('i'); break;  // Ctrl + I
    case 0x95: bleKeyboard.write('o'); break;  // Ctrl + O
    case 0x96: bleKeyboard.write('p'); break;  // Ctrl + P
    case 0x9A: bleKeyboard.write('a'); break;  // Ctrl + A
    case 0x9B: bleKeyboard.write('s'); break;  // Ctrl + S
    case 0x9C: bleKeyboard.write('d'); break;  // Ctrl + D
    case 0x9D: bleKeyboard.write('f'); break;  // Ctrl + F
    case 0x9E: bleKeyboard.write('g'); break;  // Ctrl + G
    case 0x9F: bleKeyboard.write('h'); break;  // Ctrl + H
    case 0xA0: bleKeyboard.write('j'); break;  // Ctrl + J
    case 0xA1: bleKeyboard.write('k'); break;  // Ctrl + K
    case 0xA2: bleKeyboard.write('l'); break;  // Ctrl + L
    case 0xA3: bleKeyboard.write(KEY_RETURN); break;  // Ctrl + Enter
    case 0xA6: bleKeyboard.write('z'); break;  // Ctrl + Z
    case 0xA7: bleKeyboard.write('x'); break;  // Ctrl + X
    case 0xA8: bleKeyboard.write('c'); break;  // Ctrl + C
    case 0xA9: bleKeyboard.write('v'); break;  // Ctrl + V
    case 0xAA: bleKeyboard.write('b'); break;  // Ctrl + B
    case 0xAB: bleKeyboard.write('n'); break;  // Ctrl + N
    case 0xAC: bleKeyboard.write('m'); break;  // Ctrl + M
    case 0xAD: bleKeyboard.write(','); break;  // Ctrl + ,
    case 0xAE: bleKeyboard.write('.'); break;  // Ctrl + .
    case 0xAF: bleKeyboard.write(KEY_RIGHT_SHIFT); break;  // Ctrl + Shift

    default:
      Serial.print("Unknown Ctrl combination: ");
      Serial.println(key);
      break;
  }

  bleKeyboard.release(KEY_LEFT_CTRL);
}

// Function to fade in and out blue color before connection
void fadeBlue() {
  unsigned long currentTime = millis();
  if (currentTime - lastFadeTime >= 30) {  // Adjust the time between fades for smoothness
    if (fadingUp) {
      brightness += fadeAmount;
      if (brightness >= 50) {  // Max brightness
        fadingUp = false;  // Switch direction
      }
    } else {
      brightness -= fadeAmount;
      if (brightness <= 0) {  // Min brightness
        fadingUp = true;  // Switch direction
      }
    }

    // Set new brightness
    pixels.setBrightness(brightness);
    pixels.fill(0x0000FF);  // Blue color
    pixels.show();

    // Update lastFadeTime
    lastFadeTime = currentTime;
  }
}

// Function to revert to blue fading after disconnect
void fadeToBlue() {
  firstStart = true;  // Set the first start flag so that fadeBlue function can work again
}
