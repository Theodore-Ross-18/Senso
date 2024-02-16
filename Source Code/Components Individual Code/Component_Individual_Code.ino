// 1. Arduino Uno Rev 3

void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

}

// 2. Adafruit VL53L0X

#include <Wire.h>
#include <Adafruit_VL53L0X.h>

Adafruit_VL53L0X lox = Adafruit_VL53L0X();

void setup() {
  Serial.begin(9600);
  Wire.begin();
  lox.begin();
}

void loop() {
  VL53L0X_RangingMeasurementData_t measure;

  lox.rangingTest(&measure, false);

  if (measure.RangeStatus != 4) {  // phase failures have incorrect data
    Serial.print("Distance (mm): ");
    Serial.println(measure.RangeMilliMeter);
  } else {
    Serial.println("Out of range");
  }
  delay(100);

// 3. LCD Screen 16x2

#include <LiquidCrystal.h>

// Initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

void setup() {
  // Set up any initial configuration for the LCD screen
  lcd.begin(16, 2);
}

void loop() {
  // Perform any necessary operations on the LCD screen
}

// 4. Potentiometer 10k Ohm

int potPin = A0;  // Analog input pin for the potentiometer

void setup() {
  // Set up any initial configuration for the board
}

void loop() {
  int potValue = analogRead(potPin);  // Read the value of the potentiometer
  // Perform any necessary operations on the potentiometer value
}

// 5. Mini Pushbuttons

int buttonPin = 2;  // Digital input pin for the button

void setup() {
  // Set up any initial configuration for the board
  pinMode(buttonPin, INPUT_PULLUP);
}

void loop() {
  if (digitalRead(buttonPin) == LOW) {
    // The button is pressed, perform any necessary operations
  }
}

// 6. 9V Alkaline Battery

// There is no code needed for the 9V alkaline battery. 
// Simply connect the battery to the board's power input.


