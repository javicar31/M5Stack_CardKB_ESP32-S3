//this wont run unless you have edited your library files, extra brownie points if you make it work.

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
    else if (key == KEY_ESC+KEY_RIGHT_ARROW) {  // Control key (or adjust as needed)
      bleKeyboard.press(KEY_LEFT_CTRL);
      delay(100);  // Short delay
      bleKeyboard.releaseAll();
      Serial.println("Control Key Pressed");
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

void fadeBlue() {
  unsigned long currentMillis = millis();
  if (currentMillis - lastFadeTime >= 15) {  // Adjust delay to change fading speed
    lastFadeTime = currentMillis;
    if (fadingUp) {
      brightness += fadeAmount;
      if (brightness >= 50) {  // Max brightness
        brightness = 50;
        fadingUp = false;
      }
    } else {
      brightness -= fadeAmount;
      if (brightness <= 0) {  // Min brightness
        brightness = 0;
        fadingUp = true;
      }
    }

    pixels.setBrightness(brightness);
    pixels.fill(0x0000FF);  // Solid Blue
    pixels.show();
  }
}

void fadeToBlue() {
  unsigned long currentMillis = millis();
  if (currentMillis - lastFadeTime >= 10) {  // Adjust delay to change fading speed
    lastFadeTime = currentMillis;
    if (fadingUp) {
      brightness += fadeAmount;
      if (brightness >= 50) {  // Max brightness
        brightness = 50;
        fadingUp = false;
      }
    } else {
      brightness -= fadeAmount;
      if (brightness <= 0) {  // Min brightness
        brightness = 0;
        fadingUp = true;
      }
    }

    pixels.setBrightness(brightness);
    pixels.fill(0x0000FF);  // Solid Blue
    pixels.show();
  }
}
