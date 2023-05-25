// Senso - The Laser Ruler - HCI - By: Theodore Ross C. Bermejo

// Note: This is the whole code process of the device 
// and is based on the algorithm process
// Essential Libraries
#include <Wire.h> // - The wires
#include <Adafruit_GFX.h> // - The Graphic
#include <Adafruit_SSD1306.h> // - The LCD Display (128x64 pixels)
#include <VL53L0X.h> // - The Laser Module (Adafruit_VL53L0X)

#define SCREEN_WIDTH 128 // Defining the width by 128 pixels
#define SCREEN_HEIGHT 64 // Defining the height by 64 pixels
#define OLED_RESET -1 // OLED Reset set to negative 1
// Class that interacts with the libraries (SSD1306)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define BUTTON_MODE 2 // Defining the Mode Button to Pin 2
#define BUTTON_ONSTART 3 // Defining the On/Start Button to Pin 3
#define BUTTON_OFFPRINT 4 // Defining the Off/Print Button to Pin 4
#define BUTTON_CONVERT 5 // Defining the Convert Button to Pin 5

// Class = VL53L0X and Variable = sensor
VL53L0X sensor;
// Setting the Default Conversion and Data Type
int mode = 1; // 1 = distance, 2 = dimensions
int conversion = 1; // 1 = mm, 2 = cm, 3 = m
bool measuring = false;
int distance = 0;

void setup() {
  pinMode(BUTTON_MODE, INPUT_PULLUP); // Mode Button
  pinMode(BUTTON_ONSTART, INPUT_PULLUP); // On/Start Button
  pinMode(BUTTON_OFFPRINT, INPUT_PULLUP); // Off/Print Button
  pinMode(BUTTON_CONVERT, INPUT_PULLUP); // Convert Button

  // initialize sensor (VL53L0X)
  sensor.init();
  // set measurement timing budget to 200ms
  sensor.setMeasurementTimingBudget(200000);
  // start continous readings
  sensor.startContinuous();

  // Starting Point of LCD Display
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay(); // Clearing Text
  display.setTextSize(1); // Declaring Text Size to 1
  display.setTextColor(WHITE); // Declaring Text color to White
  display.setCursor(0, 0); // Positions
  display.println("Senso"); // Message
  display.display(); // Displaying Text
  delay(2000); // This is 2 seconds before clearing text
  display.clearDisplay(); // Clearing Text
  display.setCursor(0, 0); // Positions
  display.println("Welcome"); // Message
  display.display(); // Displaying Text
  delay(1000); // This is 1 second before clearing text
  display.clearDisplay(); // Clearing Text
}

void loop() {
  // Process of Checking Whether User Switch Mode of Laser Measuring
  if (digitalRead(BUTTON_ONSTART) == LOW) {
    longPress(BUTTON_ONSTART, 2000);
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println("Press Mode Button");
    display.println("to Switch Mode");
    display.display();
    while (digitalRead(BUTTON_MODE) == HIGH) {
      // wait for mode button press
    }
    // Checking if User Choose Mode 1 or Mode 2
    if (digitalRead(BUTTON_MODE) == LOW) {
      mode = (mode == 1) ? 2 : 1;
      display.clearDisplay();
      display.setCursor(0, 0);
      // This explains if mode is equal 1 then 1 and if not then it proceed to mode 2
      if (mode == 1) { 
        display.println("Mode 1. Distance"); // Mode 1. Distance
      } else {
        display.println("Mode 2. Dimensions"); // Mode 2. Dimensions
      }
      display.display();
      delay(1000);
      display.clearDisplay();
      display.setCursor(0, 0);
      display.println("Press Start Button");
      display.println("to Begin Measuring");
      display.display();
      // The process of waiting for the user to press the start button
      while (digitalRead(BUTTON_ONSTART) == HIGH) {
        // wait for start button press
      }
      if (digitalRead(BUTTON_ONSTART) == LOW) {
        measuring = true;
        if (mode == 1) {
          measureDistance();
        } else {
          measureDimensions();
        }
      }
    }
  }
  // Beginning Process of Laser Measuring
  if (digitalRead(BUTTON_OFFPRINT) == LOW && measuring) {
    measuring = false;
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println("Done");
    display.display();
    delay(1000);
    display.clearDisplay();
    display.setCursor(0, 0);
    // The following codes displays the format of processing results 
    if (mode == 1) {
      // If the user choose mode 1 then this will be the format
      display.println("Mode 1. Distance"); // Title
      display.print("Distance: "); // Measure Result for Distance
      display.print(sensor.readRangeContinuousMillimeters()); // Distance Measure Process
      display.println(" mm"); // Default Conversion
    } else {
      // if the user choose mode 2 then this will be the format
      display.println("Mode 2. Dimensions"); // Title
      display.print("Height: "); // Measure Result for Height
      display.print(sensor.readRangeContinuousMillimeters()); // Height Measure Process
      display.println(" mm"); // Default Conversion
      delay(1000);
      display.clearDisplay();
      display.setCursor(0, 0);
      display.println("Width: "); // Title
      display.print(sensor.readRangeContinuousMillimeters()); // Width Measure Process
      display.println(" mm"); // Default Conversion
      delay(1000);
      display.clearDisplay();
      display.setCursor(0, 0);
      display.println("Length: "); // Title
      display.print(sensor.readRangeContinuousMillimeters()); // Length Measure Process
      display.println(" mm"); // Default Conversion
    }
    display.println("Press C Button to Convert"); // Allowing the user to convert conversion results
    display.display();
  }
  // Beginning Process of Converting Conversion Current Results
  if (digitalRead(BUTTON_CONVERT) == LOW && !measuring) {
    display.clearDisplay();
    display.setCursor(0, 0);
    if (mode == 1) {
      display.println("Mode 1. Distance");
      display.print("Distance: ");
      display.print(sensor.readRangeContinuousMillimeters());
      display.print(" mm");
      if (conversion == 2) {
        display.print(" (");
        // Converting Current Result to Centimeter
        display.print(sensor.readRangeContinuousMillimeters() / 10);
        display.print(" cm)");
      } else if (conversion == 3) {
        display.print(" (");
        // Converting Current Result to Meter
        display.print(sensor.readRangeContinuousMillimeters() / 1000);
        display.print(" m)");
      }
    } else {
      // 1. Height
      display.println("Mode 2. Dimensions");
      display.print("Height: ");
      display.print(sensor.readRangeContinuousMillimeters());
      if (conversion == 2) {
        display.print(" (");
        // Converting Current Result to Centimeter
        display.print(sensor.readRangeContinuousMillimeters() / 10);
        display.print(" cm)");
      } else if (conversion == 3) {
        display.print(" (");
        // Converting Current Result to Meter
        display.print(sensor.readRangeContinuousMillimeters() / 1000);
        display.print(" m)");
      }
      // 2. Width
      display.println();
      display.print("Width: ");
      display.print(sensor.readRangeContinuousMillimeters());
      if (conversion == 2) {
        display.print(" (");
        // Converting Current Result to Centimeter
        display.print(sensor.readRangeContinuousMillimeters() / 10);
        display.print(" cm)");
      } else if (conversion == 3) {
        display.print(" (");
        // Converting Current Result to Meter
        display.print(sensor.readRangeContinuousMillimeters() / 1000);
        display.print(" m)");
      }
      // 3. Length
      display.println();
      display.print("Length: ");
      display.print(sensor.readRangeContinuousMillimeters());
      if (conversion == 2) {
        display.print(" (");
        // Converting Current Result to Centimeter
        display.print(sensor.readRangeContinuousMillimeters() / 10);
        display.print(" cm)");
      } else if (conversion == 3) {
        display.print(" (");
        // Converting Current Result to Meter
        display.print(sensor.readRangeContinuousMillimeters() / 1000);
        display.print(" m)");
      }
    }
    display.println();
    // Allowing the user to repeat the process of measuring or to end
    display.println("Long Press Start Button (Repeat)");
    display.println("Long Press E Button (Shut Down)");
    display.display();
  }
  // If ever user choose E button = end process
  if (digitalRead(BUTTON_OFFPRINT) == LOW && !measuring) {
    longPress(BUTTON_OFFPRINT, 2000);
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
// Starting Point of Repeating the Process of Laser Measuring
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

// The beginning of Powering On and Off the Device
void longPress(int button, int duration) {
  unsigned long startTime = millis();
  while (digitalRead(button) == LOW && millis() - startTime < duration) {
    // wait for button release or timeout
  }
  if (millis() - startTime >= duration) {
    if (button == BUTTON_ONSTART) {
      powerOn();
    } else if (button == BUTTON_OFFPRINT) {
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
