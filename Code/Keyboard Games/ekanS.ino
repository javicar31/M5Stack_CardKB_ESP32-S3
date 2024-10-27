//snake game using arrow key to move the snake. Esc to exit the game. 
//replace any Fn key with: "case 0x86: snakeGame(); break;"
//Javier C. October 2024
void snakeGame() {
  // Dimensions of the game grid
  #define GRID_WIDTH 16
  #define GRID_HEIGHT 8

  int snakeX[128], snakeY[128];  // Arrays to store the positions of the snake's body
  int snakeLength = 3;  // Initial length of the snake
  int foodX = random(0, GRID_WIDTH), foodY = random(0, GRID_HEIGHT);  // Initial position of food
  int direction = 0;  // 0 = right, 1 = down, 2 = left, 3 = up
  bool gameOver = false;

  // Initialize the snake's starting position
  snakeX[0] = 2; snakeY[0] = 2;
  snakeX[1] = 1; snakeY[1] = 2;
  snakeX[2] = 0; snakeY[2] = 2;

  while (!gameOver) {
    // Move the body of the snake
    for (int i = snakeLength - 1; i > 0; i--) {
      snakeX[i] = snakeX[i - 1];
      snakeY[i] = snakeY[i - 1];
    }

    // Move the head of the snake based on the current direction
    if (direction == 0) snakeX[0]++;  // Right
    else if (direction == 1) snakeY[0]++;  // Down
    else if (direction == 2) snakeX[0]--;  // Left
    else if (direction == 3) snakeY[0]--;  // Up
   // Wrap around the screen
    if (snakeX[0] >= GRID_WIDTH) snakeX[0] = 0;
    else if (snakeX[0] < 0) snakeX[0] = GRID_WIDTH - 1;
    if (snakeY[0] >= GRID_HEIGHT) snakeY[0] = 0;
    else if (snakeY[0] < 0) snakeY[0] = GRID_HEIGHT - 1;

    // Check if the snake eats the food
    if (snakeX[0] == foodX && snakeY[0] == foodY) {
      snakeLength++;  // Increase the length of the snake
      foodX = random(0, GRID_WIDTH);  // Spawn new food
      foodY = random(0, GRID_HEIGHT);
    }

    // Check if the snake runs into itself
    for (int i = 1; i < snakeLength; i++) {
      if (snakeX[0] == snakeX[i] && snakeY[0] == snakeY[i]) {
        gameOver = true;  // End the game if collision occurs
        break;
      }
    }

    // Render the snake and food on the display, i still need to stop the food from appearing inside the body of the snake.
    display.clearDisplay();
    for (int i = 0; i < snakeLength; i++) {
      display.drawRect(snakeX[i] * 8, snakeY[i] * 4, 8, 4, SSD1306_WHITE);
    }
    display.fillRect(foodX * 8, foodY * 4, 8, 4, SSD1306_WHITE);
    display.display();

    // Delay to control the speed of the game
    delay(250);

    // Handle arrow key inputs for controlling the snake
    Wire.requestFrom(CARDKB_I2C_ADDR, 1);
    if (Wire.available()) {
      char key = Wire.read();
      if (key == 0xB4) direction = 2;  // Left arrow
      else if (key == 0xB7) direction = 0;  // Right arrow
      else if (key == 0xB5) direction = 3;  // Up arrow
      else if (key == 0xB6) direction = 1;  // Down arrow
      if (key == 0x1B) {  // ESC key to exit the game
        return;
      }
    }
  }

  // Game Over message
  display.clearDisplay();
  display.setCursor(10, 10);
  display.print("Game Over!");
  display.display();
  delay(2000);
}
