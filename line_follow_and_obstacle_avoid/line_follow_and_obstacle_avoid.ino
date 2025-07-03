#include <Servo.h>

// Left Motor
#define LM_DIR_10 10  // Direction pin
#define LM_PWM_11 11  // Speed (PWM) pin

// Right Motor
#define RM_DIR_12 12  // Direction pin
#define RM_PWM_13 13  // Speed (PWM) pin

#define LS_SENSOR A0
#define RS_SENSOR A1

#define US_ECHO A2
#define US_TRIGGER A3

#define SERVO_PIN A5

Servo myServo;

// Thresholds
#define THRESHOLD 500        // IR sensor threshold
int distanceThreshold = 20;  // in cm

void setup() {
  Serial.begin(9600);

  pinMode(LM_DIR_10, OUTPUT);
  pinMode(LM_PWM_11, OUTPUT);
  pinMode(RM_DIR_12, OUTPUT);
  pinMode(RM_PWM_13, OUTPUT);

  pinMode(US_ECHO, INPUT);
  pinMode(US_TRIGGER, OUTPUT);

  myServo.attach(SERVO_PIN);
  myServo.write(90);  // Center position

  pinMode(LS_SENSOR, INPUT);
  pinMode(RS_SENSOR, INPUT);
}

void loop() {
  long distance = readDistanceCM();

  if (distance < distanceThreshold) {
    avoidObstacle();
  } else {
    lineFollow();
  }

  delay(50);
}

// Movement functions
void moveForward() {
  digitalWrite(LM_DIR_10, HIGH);
  digitalWrite(LM_PWM_11, LOW);
  digitalWrite(RM_DIR_12, HIGH);
  digitalWrite(RM_PWM_13, LOW);
}

void turnLeft() {
  digitalWrite(LM_DIR_10, HIGH);
  digitalWrite(LM_PWM_11, LOW);
  digitalWrite(RM_DIR_12, LOW);
  digitalWrite(RM_PWM_13, HIGH);
}

void turnRight() {
  digitalWrite(LM_DIR_10, LOW);
  digitalWrite(LM_PWM_11, HIGH);
  digitalWrite(RM_DIR_12, HIGH);
  digitalWrite(RM_PWM_13, LOW);
}

void stopMotors() {

  digitalWrite(LM_DIR_10, LOW);
  digitalWrite(LM_PWM_11, LOW);
  digitalWrite(RM_DIR_12, LOW);
  digitalWrite(RM_PWM_13, LOW);
}

// This function is designed to measure distance in centimeters using an ultrasonic sensor.
// Function that returns a long integer representing distance in centimeters.
long readDistanceCM() {
  // Sets the trigger pin LOW for 2 microseconds to ensure a clean pulse start.
  digitalWrite(US_TRIGGER, LOW);
  delayMicroseconds(2);
  /*
  	•	Sends a HIGH pulse for 10 microseconds to the trigger pin.
	  •	This causes the ultrasonic sensor to emit 8 ultrasonic sound waves (at 40kHz).
  */
  digitalWrite(US_TRIGGER, HIGH);
  delayMicroseconds(10);
  //Ends the trigger signal
  digitalWrite(US_TRIGGER, LOW);

  /*
  	•	Waits for the echo pin to go HIGH.
    •	Measures the time (in microseconds) that the echo pin stays HIGH.
    •	This time is equal to the round-trip time of the sound wave: from the sensor → to the object → back to the sensor.
  */
  long duration = pulseIn(US_ECHO, HIGH);

  /*
  	•	Speed of sound: ~343 meters per second, which is 0.034 cm per microsecond.
    •	Multiplies the time (duration) by 0.034 cm to get the total distance traveled (there and back).
    •	Divides by 2 to get the one-way distance from sensor to object.
  */
  return duration * 0.034 / 2;
}

// Line following logic
void lineFollow() {
  int leftValue = analogRead(LS_SENSOR);
  int rightValue = analogRead(RS_SENSOR);


  // IR sensor logic: true = sensor sees black line
  bool leftOnLine = leftValue < THRESHOLD;
  bool rightOnLine = rightValue < THRESHOLD;

  if (leftOnLine && rightOnLine) {
    moveForward();
  } else if (!leftOnLine && rightOnLine) {
    turnRight();
  } else if (leftOnLine && !rightOnLine) {
    turnLeft();
  } else {
    stopMotors();
  }
  delay(50);
}

// Object avoidance
// The function helps the robot detect obstacles ahead and navigate around them by choosing the best path — either left or right — based on which side has more free space.
void avoidObstacle() {

  /*
  	•	The robot stops to stabilize.
	  •	It takes a distance reading directly in front (center) to confirm the obstacle.
  */
  stopMotors();
  delay(300);
  long center = readDistanceCM();
  /////////////////////

  myServo.write(20);
  delay(500);
  long rightDist = readDistanceCM();

  myServo.write(160);
  delay(500);
  long leftDist = readDistanceCM();

  myServo.write(90);  // center
  delay(300);
  /////////////////////
  /*
  Decision Making — Left vs. Right:
  	•	If left has more space, choose left path.
	  •	Otherwise, choose the right path.
  */
  if (leftDist > rightDist) {
    // Go turnLeft
    /*
    If Left is Clear:
    	•	Pattern: Left → Forward → Right → Forward → Right → Forward → Left and joins the line
	    •	This creates a rectangular path to bypass the obstacle from the left.
    */
    turnLeft();
    delay(1000);
    moveForward();
    delay(1800);
    turnRight();
    delay(1000);
    moveForward();
    delay(1800);
    turnRight();
    delay(1000);
    moveForward();
    delay(2000);
    turnLeft();
    delay(1000);
  } else {
    // Go turnRight
    /*
     If Right is Clear:
     	•	Pattern: Right → Forward → Left → Forward → Left → Forward → Right and joins the line
	    •	Creates a rectangular path to bypass the obstacle from the right.
    */
    turnRight();
    delay(1000);
    moveForward();
    delay(1800);
    turnLeft();
    delay(1000);
    moveForward();
    delay(2000);
    turnLeft();
    delay(1000);
    moveForward();
    delay(2000);
    turnRight();
    delay(1000);
  }

  // Move moveForward a bit and try to realign with line
  moveForward();
  delay(600);
}