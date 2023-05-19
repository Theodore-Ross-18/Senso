// Senso - The Laser Ruler - Theodoer Ross C. Bermejo 

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <VL53L0X.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define BUTTON_MODE 2
#define BUTTON_START 3
#define BUTTON_E 4
#define BUTTON_C 5

VL53L0X sensor;
int mode = 1; // 1 = distance, 2 = dimensions
int conversion = 1; // 1 = mm, 2 = cm, 3 = m
bool measuring = false;

void setup() {
  pinMode(BUTTON_MODE, INPUT_PULLUP);
  pinMode(BUTTON_START, INPUT_PULLUP);
  pinMode(BUTTON_E, INPUT_PULLUP);
  pinMode(BUTTON_C, INPUT_PULLUP);

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println("Senso");
  display.display();
  delay(2000);
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("Welcome");
  display.display();
  delay(1000);
  display.clearDisplay();
}

void loop() {
  if (digitalRead(BUTTON_START) == LOW) {
    longPress(BUTTON_START, 2000);
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println("Press Mode Button");
    display.println("to Switch Mode");
    display.display();
    while (digitalRead(BUTTON_MODE) == HIGH) {
      // wait for mode button press
    }
    if (digitalRead(BUTTON_MODE) == LOW) {
      mode = (mode == 1) ? 2 : 1;
      display.clearDisplay();
      display.setCursor(0, 0);
      if (mode == 1) {
        display.println("Mode 1. Distance");
      } else {
        display.println("Mode 2. Dimensions");
      }
      display.display();
      delay(1000);
      display.clearDisplay();
      display.setCursor(0, 0);
      display.println("Press Start Button");
      display.println("to Begin Measuring");
      display.display();
      while (digitalRead(BUTTON_START) == HIGH) {
        // wait for start button press
      }
      if (digitalRead(BUTTON_START) == LOW) {
        measuring = true;
        if (mode == 1) {
          measureDistance();
        } else {
          measureDimensions();
        }
      }
    }
  }

  if (digitalRead(BUTTON_E) == LOW && measuring) {
    measuring = false;
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println("Done");
    display.display();
    delay(1000);
    display.clearDisplay();
    display.setCursor(0, 0);
    if (mode == 1) {
      display.println("Mode 1. Distance");
      display.print("Distance: ");
      display.print(sensor.readRangeContinuousMillimeters());
      display.println(" mm");
    } else {
      display.println("Mode 2. Dimensions");
      display.print("Height: ");
      display.print(sensor.readRangeContinuousMillimeters());
      display.println(" mm");
      delay(1000);
      display.clearDisplay();
      display.setCursor(0, 0);
      display.println("Width: ");
      display.print(sensor.readRangeContinuousMillimeters());
      display.println(" mm");
      delay(1000);
      display.clearDisplay();
      display.setCursor(0, 0);
      display.println("Length: ");
      display.print(sensor.readRangeContinuousMillimeters());
      display.println(" mm");
    }
    display.println("Press C Button to Convert");
    display.display();
  }

  if (digitalRead(BUTTON_C) == LOW && !measuring) {
    display.clearDisplay();
    display.setCursor(0, 0);
    if (mode == 1) {
      display.println("Mode 1. Distance");
      display.print("Distance: ");
      display.print(sensor.readRangeContinuousMillimeters());
      display.print(" mm");
      if (conversion == 2) {
        display.print(" (");
        display.print(sensor.readRangeContinuousMillimeters() / 10);
        display.print(" cm)");
      } else if (conversion == 3) {
        display.print(" (");
        display.print(sensor.readRangeContinuousMillimeters() / 1000);
        display.print(" m)");
      }
    } else {
      display.println("Mode 2. Dimensions");
      display.print("Height: ");
      display.print(sensor.readRangeContinuousMillimeters());
      if (conversion == 2) {
        display.print(" (");
        display.print(sensor.readRangeContinuousMillimeters() / 10);
        display.print(" cm)");
      } else if (conversion == 3) {
        display.print(" (");
        display.print(sensor.readRangeContinuousMillimeters() / 1000);
        display.print(" m)");
      }
      display.println();
      display.print("Width: ");
      display.print(sensor.readRangeContinuousMillimeters());
      if (conversion == 2) {
        display.print(" (");
        display.print(sensor.readRangeContinuousMillimeters() / 10);
        display.print(" cm)");
      } else if (conversion == 3) {
        display.print(" (");
        display.print(sensor.readRangeContinuousMillimeters() / 1000);
        display.print(" m)");
      }
      display.println();
      display.print("Length: ");
      display.print(sensor.readRangeContinuousMillimeters());
      if (conversion == 2) {
        display.print(" (");
        display.print(sensor.readRangeContinuousMillimeters() / 10);
        display.print(" cm)");
      } else if (conversion == 3) {
        display.print(" (");
        display.print(sensor.readRangeContinuousMillimeters() / 1000);
        display.print(" m)");
      }
    }
    display.println();
    display.println("Start Button (Repeat)");
    display.println("E Button (Shut Down)");
    display.display();
  }

  if (digitalRead(BUTTON_E) == LOW && !measuring) {
    longPress(BUTTON_E, 2000);
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println("Shutting Down");
    display.display();
    delay(2000);
    display.clearDisplay();
    display.display();
    delay(1000);
    powerOff();
  }
}

void measureDistance() {
  sensor.setTimeout(500);
  sensor.startContinuous();
}

void measureDimensions() {
  sensor.setTimeout(500);
  sensor.startContinuous();
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("Height: ");
  display.display();
  delay(1000);
  sensor.readRangeContinuousMillimeters();
  display.print(sensor.readRangeContinuousMillimeters());
  display.println(" mm");
  delay(1000);
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("Width: ");
  display.display();
  delay(1000);
  sensor.readRangeContinuousMillimeters();
  display.print(sensor.readRangeContinuousMillimeters());
  display.println(" mm");
  delay(1000);
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("Length: ");
  display.display();
  delay(1000);
  sensor.readRangeContinuousMillimeters();
  display.print(sensor.readRangeContinuousMillimeters());
  display.println(" mm");
  delay(1000);
  display.clearDisplay();
}

void longPress(int button, int duration) {
  unsigned long startTime = millis();
  while (digitalRead(button) == LOW && millis() - startTime < duration) {
    // wait for button release or timeout
  }
  if (millis() - startTime >= duration) {
    if (button == BUTTON_START) {
      powerOn();
    } else if (button == BUTTON_E) {
      powerOff();
    }
  }
}

void powerOn() {
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("Powering On");
  display.display();
  delay(2000);
  display.clearDisplay();
}

void powerOff() {
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("Powering Off");
  display.display();
  delay(2000);
  display.clearDisplay();
  display.display();
  delay(1000);
  while (true) {
    // wait for power to turn off
  }
}

