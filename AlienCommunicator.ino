//////////////////////////////////
// Alien Communication EG110 Project
// Date: November 1, 2024
// Author by: Kenyang Lual
//////////////////////////////////

#include <LedControl.h>
#include <Servo.h>

Servo myservo;

// Pin settings
const int buttonPin1 = 37; // Button for Happy
const int buttonPin2 = 36; // Button for Sad
const int buttonPin3 = 35; // Button for Arrow

const int TRIG_PIN = 42; // Arduino pin connected to Ultrasonic Sensor's TRIG pin
const int ECHO_PIN = 41; // Arduino pin connected to Ultrasonic Sensor's ECHO pin

const int LED_PIN1 = 45; // Arduino pin connected to LED RED
const int LED_PIN2 = 44; // Arduino pin connected to another LED YELLOW
const int LED_PIN3 = 43; // Arduino pin connected to another LED GREEN

int pos = 0;

// Variables will change:
float duration_us, distance_cm;

#define DIN 40 // Data In pin for the DotMatrix
#define CS 39   // Changed to avoid conflict with LED_PIN
#define CLK 38 // Clock pin for the DotMatrix


// Define the emoji patterns
byte smile[8] = {0x3C, 0x42, 0xA5, 0x81, 0xA5, 0x99, 0x42, 0x3C}; // Smiley face
byte sad[8] = {0x3C, 0x42, 0xA5, 0x81, 0x99, 0xA5, 0x42, 0x3C};   // Sad face
byte arrow[8] = {0x08, 0x1C, 0xFE, 0xFF, 0xFF, 0xFE, 0x1C, 0x08}; // Arrow

// Initialize the LED matrix from the library
LedControl lc = LedControl(DIN, CLK, CS, 1);

void setup() {
  // Set button pins as input with resistors
  pinMode(buttonPin1, INPUT_PULLUP);
  pinMode(buttonPin2, INPUT_PULLUP);
  pinMode(buttonPin3, INPUT_PULLUP);

  myservo.attach(47 );  // attaches the servo on pin 34 to the Servo object

  lc.shutdown(0, false); // Wake up the DotMatrix
  lc.setIntensity(0, 8); // Set brightness level (0 is min, 15 is max)
  lc.clearDisplay(0);    // Clear display

  Serial.begin(9600);         // Initialize serial port
  pinMode(TRIG_PIN, OUTPUT);  // Set Arduino pin to output mode
  pinMode(ECHO_PIN, INPUT);   // Set Arduino pin to input mode
  pinMode(LED_PIN1, OUTPUT);  // Set Arduino pin to output mode

   

}

void motionSensor() {
  // Generate 10-microsecond pulse to TRIG pin
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // Measure duration of pulse from ECHO pin
  duration_us = pulseIn(ECHO_PIN, HIGH);
  // Calculate the distance
  distance_cm = 0.017 * duration_us;
  
  if (distance_cm < 100) {
    digitalWrite(LED_PIN1, HIGH); // Turn on LED
    digitalWrite(LED_PIN2, LOW);  // Turn off LED
    digitalWrite(LED_PIN3, LOW);  // Turn off LED
  }
   if (distance_cm > 100 && distance_cm < 200) {
    digitalWrite(LED_PIN1, LOW);  // Turn off LED
    digitalWrite(LED_PIN2, HIGH); // Turn on LED
    digitalWrite(LED_PIN3, LOW);  // Turn off LED
   }
  if (distance_cm > 200) {
    digitalWrite(LED_PIN1, LOW);  // Turn off LED
    digitalWrite(LED_PIN2, LOW);  // Turn off LED
    digitalWrite(LED_PIN3, HIGH); // Turn on LED
  }
  
  

  // Print the value to Serial Monitor
  Serial.print("distance: ");
  Serial.print(distance_cm);
  Serial.println(" cm");

  delay(300);
}

void displayEmoji(byte emoji[]) {
  // Send the emoji pattern to the matrix
  for (int row = 0; row < 8; row++) {
    lc.setRow(0, row, emoji[row]); // Display each row of the emoji
  }
}
void loop() {
  
  if  (digitalRead(buttonPin1) == HIGH) {
  for (pos = 100; pos <= 140; pos += 1); { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(300);                       // waits 15 ms for the servo to reach the position
  }
  for (pos = 140; pos >= 100; pos -= 1); { // goes from 180 degrees to 0 degrees
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(300);                       // waits 15 ms for the servo to reach the position
  }
  }

  motionSensor();

  // Check if the first button is pressed
  if (digitalRead(buttonPin1) == HIGH) { // If Button 1 is pressed
    displayEmoji(smile); // Display the smiley face
  }
  // Check if the second button is pressed
  if (digitalRead(buttonPin2) == HIGH) { // If Button 2 is pressed
    displayEmoji(sad); // Display the sad face
  
  }
  // Check if the third button is pressed
  if (digitalRead(buttonPin3) == HIGH) { // If Button 3 is pressed
    displayEmoji(arrow); // Display the arrow
  }
}
 

