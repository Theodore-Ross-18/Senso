// Senso - The Laser Ruler - Theodoer Ross C. Bermejo 

// This will include the components used through their library
#include <Wire.h> // Library for Wires
#include <Adafruit_VL53L0X.h> // Library for Laser Module
#include <LiquidCrystal_I2C.h> // Library for LCD screen 

// Pin Definitions - This is where the wires are placed through 
// pins in the main board for which is the Arduino Uno
const int START_PIN = 2; // Connected to Start Button
const int MODE_PIN = 3; // Connected to Mode Button
const int CONVERT_PIN = 4; // Connected to C or Convert Button
const int PRINT_PIN = 5; // Will print out the outputs or results
const int END_PIN = 6; // Will end the operation if satisfied

// Variables - For Laser Measuring
bool isDeviceOn = false;
int mode = 1; // 1 for Mode 1 (Measure Certain Distance), 2 for Mode 2 (Measure Length and Width)
float distance = 0.0;
float length = 0.0;
float width = 0.0;

Adafruit_VL53L0X vl53l0x = Adafruit_VL53L0X(); // The Laser Module
LiquidCrystal_I2C lcd(0x27, 16, 2); // The LCD Screen

// Function declarations
void measureCertainDistance();
void measureLengthAndWidth();
float measureDistance();
void printMeasurement();
void convertMeasurement();
void endMeasurement();
void turnOnDevice();

bool startButtonPressed() {
  return digitalRead(2 == LOW);
}

bool modeButtonPressed() {
  return digitalRead(3 == LOW);
}

bool convertButtonPressed() {
  return digitalRead(4 == LOW);
}

bool printButtonPressed() {
  return digitalRead(5 == LOW);
}

bool endButtonPressed() {
  return digitalRead(6) == LOW;
}

bool endButtonLongPressed() {
  // Set the duration threshold for a long press (in milliseconds)
  const unsigned long longPressDuration = 2000; //

  // Get the current time
  unsigned long currentMillis = millis();

  // Wait for the end button to be released
  while (digitalRead(6) == LOW)
      delay(10);

  // Calculate the duration of the button press
  unsigned long buttonPressDuration = millis() - currentMillis;

  // Check if the button press duration exceeds the long press threshold
  return buttonPressDuration >= longPressDuration;

}

int waitForButtonPress();

void setup() {
  // Initialize the LCD
  lcd.begin(16, 2);
  lcd.print("Senso Device");
  
  // Initialize the VL53L0X sensor
  Wire.begin();
  vl53l0x.begin();
  
  // Set up button pins
  pinMode(START_PIN, INPUT_PULLUP);
  pinMode(MODE_PIN, INPUT_PULLUP);
  pinMode(CONVERT_PIN, INPUT_PULLUP);
  pinMode(PRINT_PIN, INPUT_PULLUP);
  pinMode(END_PIN, INPUT_PULLUP);
}

void loop() {
  // Check if the device is on
  if (!isDeviceOn && startButtonPressed()) {
    turnOnDevice();
  }
   
   while (isDeviceOn) {
     if (modeButtonPressed()) {
       mode = (mode + 1) % 2; // Toggle between Mode 1 and Mode 2
       lcd.clear();
       lcd.print("Mode ");
       lcd.print(mode + 1);
       delay(200); // Button debounce delay
     }

      if (mode == 1) {
        if (startButtonPressed()) {
          measureLengthAndWidth();
        }
      } else if (mode == 2) {
        if (startButtonPressed()) {
          measureLengthAndWidth();
        }
      }

    if (printButtonPressed()) {
      printMeasurement();
    }

    if (convertButtonPressed()) {
      convertMeasurement();
    }

    if (endButtonPressed()) {
      if (endButtonLongPressed()) {
        endMeasurement();
      }
    }

   }
}

void measureCertainDistance() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Mode 1: Measure");
  lcd.setCursor(0, 1);
  lcd.print("Certain Distance");
  
  // Measure the distance
  lcd.clear();
  lcd.setCursor(0, 0);

  lcd.print("Measuring...");
  float distance = measureDistance();
  lcd.setCursor(0, 1);
  lcd.print("Distance: ");
  lcd.print(distance);
  lcd.print(" cm");
  
  delay(1000); // Delay for measurement display
}

void measureLengthAndWidth() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Mode 2: Measure");
  lcd.setCursor(0, 1);
  lcd.print("Length and Width");
  
  // Measure the length
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Measuring...");
  lcd.setCursor(0, 1);
  lcd.print("Length: ");
  length = measureDistance();
  lcd.print(length);
  lcd.print(" cm");
  
  // Wait for the user to reposition the sensor
  while (startButtonPressed()) {
    lcd.setCursor(0, 1);
    lcd.print("Reposition...");
    delay(100);
  }
  
  // Measure the width
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Measuring...");
  lcd.setCursor(0, 1);
  lcd.print("Width: ");
  width = measureDistance();
  lcd.print(width);
  lcd.print(" cm");
  
  delay(1000); // Delay for measurement display
}

float measureDistance() {
  VL53L0X_RangingMeasurementData_t measure;
  
  vl53l0x.rangingTest(&measure, false); // Set second parameter to 'true' for more accurate measurements
  
  if (measure.RangeStatus != 4) {
    return measure.RangeMilliMeter / 10.0; // Convert millimeters to centimeters
  } else {
    return -1.0; // Invalid measurement
  }
}

void printMeasurement() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Printing...");
  
  // Print the measurement
  lcd.setCursor(0, 1);
  if (mode == 0) {
    lcd.print("Distance: ");
    lcd.print(distance);
    lcd.print(" cm");
  } else if (mode == 1) {
    lcd.print("Length: ");
    lcd.print(length);
    lcd.print(" cm");
    lcd.setCursor(0, 2);
    lcd.print("Width: ");
    lcd.print(width);
    lcd.print(" cm");
  }
  
  delay(2000); // Delay for printing display
}

  // Perform the conversion logic
  void convertMeasurement() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Converting...");
  
  // Convert the measurement
  lcd.setCursor(0, 1);
  if (mode == 0) {
    lcd.print("Convert: Distance");
    lcd.setCursor(0, 2);
    lcd.print("1. Meter   2. Millimeter");
    int conversionOption = waitForButtonPress();
    
    float convertedDistance;
    
    switch (conversionOption) {
      case 1: // Convert to meter
        convertedDistance = distance / 100.0;
        lcd.clear();
        lcd.print("Converted: ");
        lcd.print(convertedDistance);
        lcd.print(" m");
        break;
      case 2: // Convert to millimeter
        convertedDistance = distance * 10.0;
        lcd.clear();
        lcd.print("Converted: ");
        lcd.print(convertedDistance);
        lcd.print(" mm");
        break;
      default:
        lcd.clear();
        lcd.print("Invalid Option");
        break;
    }
  } else if (mode == 1) {
    lcd.print("Convert: Length");
    lcd.setCursor(0, 2);
    lcd.print("1. Meter   2. Millimeter");
    int conversionOption = waitForButtonPress();
    
    float convertedLength;
    float convertedWidth;
    
    switch (conversionOption) {
      case 1: // Convert to meter
        convertedLength = length / 100.0;
        convertedWidth = width / 100.0;
        lcd.clear();
        lcd.print("Converted Length: ");
        lcd.print(convertedLength);
        lcd.print(" m");
        lcd.setCursor(0, 3);
        lcd.print("Converted Width: ");
        lcd.print(convertedWidth);
        lcd.print(" m");
        break;
      case 2: // Convert to millimeter
        convertedLength = length * 10.0;
        convertedWidth = width * 10.0;
        lcd.clear();
        lcd.print("Converted Length: ");
        lcd.print(convertedLength);
        lcd.print(" mm");
        lcd.setCursor(0, 3);
        lcd.print("Converted Width: ");
        lcd.print(convertedWidth);
        lcd.print(" mm");
        break;
      default:
        lcd.clear();
        lcd.print("Invalid Option");
        break;
    }
  }
  
  delay(2000); // Delay for conversion display
}

int waitForButtonPress() {
  while (true) {
    if (digitalRead(CONVERT_PIN) == LOW) {
      delay(10); // Button debounce delay
      if (digitalRead(CONVERT_PIN) == LOW) {
        // Wait for button release
        while (digitalRead(CONVERT_PIN) == LOW) {
          delay(10);
        }
        // Return the selected option
        return 1;
      }
    }
    if (digitalRead(PRINT_PIN) == LOW) {
      delay(10); // Button debounce delay
      if (digitalRead(PRINT_PIN) == LOW) {
        // Wait for button release
        while (digitalRead(PRINT_PIN) == LOW) {
          delay(10);
        }
        // Return the selected option
        return 2;
      }
    }
  }
  delay(2000); // Delay for conversion display
}
  
void endMeasurement() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Turning off...");
  delay(1000);
  isDeviceOn = false;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Device Off");
}

void turnOnDevice() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Turning on...");
  delay(1000);
  isDeviceOn = true;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Device On");
}
