/**
 * This example turns the ESP32 into a Bluetooth LE mouse that continuously moves the mouse.
 */
#include <Wire.h>
#include <MPU6050.h>
#include <BleMouse.h>

MPU6050 mpu;
BleMouse BLEMOUSE;

const int buttonLeftClick = 12;   // GPIO pin for left click button
const int buttonRightClick = 13;  // GPIO pin for right click button
const int buttonScroll = 14;      // GPIO pin for scroll button
const int buttonMove = 27;

int16_t ax, ay, az;
int16_t gx, gy, gz;

// Sensitivity scaling factors for cursor movement and scrolling
const float cursorSensitivity = 0.002; // Faster cursor movement
const float scrollSensitivity = 0.0001; // Smoother and slower scrolling


void setup() {
  Serial.begin(115200);
  Wire.begin();
  mpu.initialize();

  if (!mpu.testConnection()) {
    Serial.println("MPU6050 connection failed");
    while (1);
  }

  Serial.println("MPU6050 connection successful");

  BLEMOUSE.begin();
  while (!BLEMOUSE.isConnected()) {
    delay(500);
    Serial.println("Waiting for BLE connection...");
  }
  Serial.println("BLE Mouse Connected!");

  // Initialize buttons
  pinMode(buttonLeftClick, INPUT_PULLUP);
  pinMode(buttonRightClick, INPUT_PULLUP);
  pinMode(buttonScroll, INPUT_PULLUP);
  pinMode(buttonMove, INPUT_PULLUP);
}

void loop() {
  if (BLEMOUSE.isConnected()) {
    mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

    // Calculate movement
    int xMove = ax * cursorSensitivity;
    int yMove = ay * cursorSensitivity;

    // Move the cursor or scroll
    if (digitalRead(buttonScroll) == LOW) {
      // Scroll mode
      BLEMOUSE.move(0, 0, -yMove * scrollSensitivity / (cursorSensitivity/2) , xMove * scrollSensitivity / (cursorSensitivity/2));
      delay(200);
    } 
    
    if(digitalRead(buttonMove) == LOW)
    {
      // Normal cursor movement
      BLEMOUSE.move(xMove, yMove);
    }

    // Print accelerometer values to Serial Monitor
    Serial.print("ax: "); Serial.print(ax);
    Serial.print(", ay: "); Serial.print(ay);
    Serial.print(", az: "); Serial.print(az);
    Serial.print(", gx: "); Serial.print(gx);
    Serial.print(", gy: "); Serial.print(gy);
    Serial.print(", gz: "); Serial.println(gz);

    // Check buttons for clicks
    if (digitalRead(buttonLeftClick) == LOW) {
      BLEMOUSE.click(MOUSE_LEFT);
      delay(200);  // Debounce delay
    }
    if (digitalRead(buttonRightClick) == LOW) {
      BLEMOUSE.click(MOUSE_RIGHT);
      delay(200);  // Debounce delay
    }

    delay(10);  // Adjust delay for smoother cursor movement
  }
}
