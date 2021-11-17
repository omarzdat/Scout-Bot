// =======================================================
// ENGG1000 - Final Software Design
// NavBot Controller
// Written by Sasha Barisic and Rory Maclean, 2021
// Controls navigational rescue robot
// ======================================================= 

#include <Servo.h>

// -------------------------------------------------------
// Global Variables
// -------------------------------------------------------

// Front left DC motor
int frontLeftSpeedPin = 10;
int frontLeftMotorLogicPin1 = 0;
int frontLeftMotorLogicPin2 = 1;

// Front right DC motor
int frontRightSpeedPin = 9;
int frontRightMotorLogicPin1 = A4;
int frontRightMotorLogicPin2 = A5;

// Back left DC motor
int backLeftSpeedPin = 11;
int backLeftMotorLogicPin1 = A3;
int backLeftMotorLogicPin2 = A2;

// Back right DC motor
int backRightSpeedPin = 6;
int backRightMotorLogicPin1 = A1;
int backRightMotorLogicPin2 = A0;

// Set up variables for DC motor control
int pwmDutyCycle = 255;
int currentSpeed = 127;
char currentDirection = 's';

// Initialise Servomotors
Servo frontLeftServo;
Servo frontRightServo;
Servo backLeftServo;
Servo backRightServo;
Servo mountServo;

// Setup up pins needed to communicate with Ultrasonic Sensor
long duration; // variable for the duration of produced sound wave
int distance; // variable for distance traversed by sound wave
const int trigPin = 13; // Trigger Pin of Ultrasonic Sensor
const int echoPin = 12; // Echo Pin of Ultrasonic Sensor

// -------------------------------------------------------
// Subroutine: Initialise Modules
// -------------------------------------------------------
void initialiseDC() {
  pinMode(frontLeftMotorLogicPin1, OUTPUT);
  pinMode(frontLeftMotorLogicPin2, OUTPUT);
  pinMode(frontLeftSpeedPin, OUTPUT);
  pinMode(frontRightMotorLogicPin1, OUTPUT);
  pinMode(frontRightMotorLogicPin2, OUTPUT);
  pinMode(frontRightSpeedPin, OUTPUT);
  pinMode(backLeftMotorLogicPin1, OUTPUT);
  pinMode(backLeftMotorLogicPin2, OUTPUT);
  pinMode(backLeftSpeedPin, OUTPUT);
  pinMode(backRightMotorLogicPin1, OUTPUT);
  pinMode(backRightMotorLogicPin2, OUTPUT);
  pinMode(backRightSpeedPin, OUTPUT);
}

void initialiseServo() {
  // Configuration of the Servomotors 
  frontLeftServo.attach(7);
  frontRightServo.attach(5); 
  backLeftServo.attach(3);
  backRightServo.attach(4);
  mountServo.attach(2);
}

void initialiseSensor() {
  // Configuration of the Ultrasonic Sensor
  pinMode(trigPin, OUTPUT); 
  pinMode(echoPin, INPUT);
}

// -------------------------------------------------------
// Subroutine: Process Input
// -------------------------------------------------------
void process_input() {
  // Check if user has input command:
  char incomingByte = 0;
  if (Serial.available() > 0)  {

    // Read the oldest byte in the serial buffer:
    incomingByte = Serial.read();
    
    // Is this a direction; 'f' 'b' 'l' 'r' 's'
    if (incomingByte == 'f') {
      currentDirection = 'f';
      Serial.println("Forwards");
    }
    else if (incomingByte == 'b') {
      currentDirection = 'b';
      Serial.println("Backwards");
    }
    else if (incomingByte == 'l') {
      currentDirection = 'l';
      Serial.println("Turn Left");
    }
    else if (incomingByte == 'r') {
      currentDirection = 'r';
      Serial.println("Turn Right");
    }
    else if (incomingByte == 's') {
      currentDirection = 's';
      Serial.println("Stop");
    }
    else if (incomingByte == '+' && currentSpeed < 252) {
      currentSpeed += 25;
      Serial.println("Increasing speed by 10%");
      analogWrite(frontLeftSpeedPin, currentSpeed);
      analogWrite(frontRightSpeedPin, currentSpeed);
      analogWrite(backLeftSpeedPin, currentSpeed);
      analogWrite(backRightSpeedPin, currentSpeed);
    }
    else if (incomingByte == '-' && currentSpeed > 2) {
      currentSpeed -= 25;
      Serial.println("Decreasing speed by 10%");
      analogWrite(frontLeftSpeedPin, currentSpeed);
      analogWrite(frontRightSpeedPin, currentSpeed);
      analogWrite(backLeftSpeedPin, currentSpeed);
      analogWrite(backRightSpeedPin, currentSpeed);
    }
  }
}

// -------------------------------------------------------
// Subroutine: Reset Pins
// -------------------------------------------------------
void reset_pins() {
    // Reset all of the pins
    analogWrite(frontLeftMotorLogicPin1, 0);
    analogWrite(frontLeftMotorLogicPin2, 0);
    analogWrite(frontRightMotorLogicPin1, 0);
    analogWrite(frontRightMotorLogicPin2, 0);
    analogWrite(backLeftMotorLogicPin1, 0);
    analogWrite(backLeftMotorLogicPin2, 0);
    analogWrite(backRightMotorLogicPin1, 0);
    analogWrite(backRightMotorLogicPin2, 0);
}

void update_sensor(){
    // Resets trigPin sound emission
    digitalWrite(trigPin, LOW); 
    delayMicroseconds(2);
    // Sets the trigPin to HIGH for 10 microseconds
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    // Reads echoPin
    duration = pulseIn(echoPin, HIGH);
    // Calculating distance traversed
    distance = duration * 0.034 / 2; // Sound Wave speed over 2 for 'there and back'
    // Read result to Serial Monitor
    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.println(" cm");
}

// -------------------------------------------------------
// Subroutines: Move Robot
// -------------------------------------------------------
void move_forward() {
    frontLeftServo.write(90);
    frontRightServo.write(90); 
    backLeftServo.write(90);
    backRightServo.write(90);
    mountServo.write(0);
            
    while (distance > 10) {
        analogWrite(frontLeftMotorLogicPin1, 0);
        analogWrite(frontLeftMotorLogicPin2, pwmDutyCycle);
        analogWrite(frontRightMotorLogicPin1, 0);
        analogWrite(frontRightMotorLogicPin2, pwmDutyCycle);
        analogWrite(backLeftMotorLogicPin1, 0);
        analogWrite(backLeftMotorLogicPin2, pwmDutyCycle);
        analogWrite(backRightMotorLogicPin1, 0);
        analogWrite(backRightMotorLogicPin2, pwmDutyCycle);
        update_sensor();
    }
}

void move_backward() {
    frontLeftServo.write(90);
    frontRightServo.write(90); 
    backLeftServo.write(90);
    backRightServo.write(90);
    mountServo.write(180);
            
    while (distance > 10) {
        analogWrite(frontLeftMotorLogicPin1, pwmDutyCycle);
        analogWrite(frontLeftMotorLogicPin2, 0);
        analogWrite(frontRightMotorLogicPin1, pwmDutyCycle);
        analogWrite(frontRightMotorLogicPin2, 0);
        analogWrite(backLeftMotorLogicPin1, pwmDutyCycle);
        analogWrite(backLeftMotorLogicPin2, 0);
        analogWrite(backRightMotorLogicPin1, pwmDutyCycle);
        analogWrite(backRightMotorLogicPin2, 0);
        update_sensor();
    }
}

void move_left() {
    frontLeftServo.write(0);
    frontRightServo.write(0); 
    backLeftServo.write(0);
    backRightServo.write(0);
    mountServo.write(270);
            
    while (distance > 10) {
        analogWrite(frontLeftMotorLogicPin1, 0);
        analogWrite(frontLeftMotorLogicPin2, pwmDutyCycle);
        analogWrite(frontRightMotorLogicPin1, 0);
        analogWrite(frontRightMotorLogicPin2, pwmDutyCycle);
        analogWrite(backLeftMotorLogicPin1, 0);
        analogWrite(backLeftMotorLogicPin2, pwmDutyCycle);
        analogWrite(backRightMotorLogicPin1, 0);
        analogWrite(backRightMotorLogicPin2, pwmDutyCycle);
        update_sensor();
    }
}

void move_right() {
    frontLeftServo.write(0);
    frontRightServo.write(0); 
    backLeftServo.write(0);
    backRightServo.write(0);
    mountServo.write(90);
            
    while (distance > 10) {
        analogWrite(frontLeftMotorLogicPin1, pwmDutyCycle);
        analogWrite(frontLeftMotorLogicPin2, 0);
        analogWrite(frontRightMotorLogicPin1, pwmDutyCycle);
        analogWrite(frontRightMotorLogicPin2, 0);
        analogWrite(backLeftMotorLogicPin1, pwmDutyCycle);
        analogWrite(backLeftMotorLogicPin2, 0);
        analogWrite(backRightMotorLogicPin1, pwmDutyCycle);
        analogWrite(backRightMotorLogicPin2, 0);
        update_sensor();
    }
}

void stop_movement() {
    analogWrite(frontLeftMotorLogicPin1, 0);
    analogWrite(frontLeftMotorLogicPin2, 0);
    analogWrite(frontRightMotorLogicPin1, 0);
    analogWrite(frontRightMotorLogicPin2, 0);
    analogWrite(backLeftMotorLogicPin1, 0);
    analogWrite(backLeftMotorLogicPin2, 0);
    analogWrite(backRightMotorLogicPin1, 0);
    analogWrite(backRightMotorLogicPin2, 0);
}

// -------------------------------------------------------
// The setup() method runs once, when the sketch starts
// -------------------------------------------------------
void setup(){ 
  // Initialize the serial communications
  Serial.begin(9600);

  Serial.println("Valid commands:");
  Serial.println("'f' = forward");
  Serial.println("'b' = backwards");
  Serial.println("'l' = left");
  Serial.println("'r' = right");
  Serial.println("'s' = stop");
  Serial.println("'+' = increase speed by 10%");
  Serial.println("'-' = decrease speed by 10% ");
  // Configuration of the DC motor pins
  initialiseDC();
  initialiseServo();
  initialiseSensor();
}

// -------------------------------------------------------
// The loop() method runs over and over again
// -------------------------------------------------------
void loop(){
  
  //configure robot prior to motion
  process_input();
  reset_pins();

  //configure motion
  if (currentDirection == 'f') {
      move_forward();
  }
  else if (currentDirection == 'b') {
      move_backward();
  }
  else if (currentDirection == 'l') {
      move_left();
  } 
  else if (currentDirection == 'r') {
      move_right();
  } 
  else if (currentDirection == 's') {
      stop_movement();
  }
  
}
