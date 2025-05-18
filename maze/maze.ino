#include <Servo.h>

// Ultrasonic sensor pins
#define trigPin 9
#define echoPin 8

// Motor Driver pins
#define IN1 2
#define IN2 3
#define IN3 4
#define IN4 5

// Servo pin
#define servoPin 6

Servo scanServo;

long getDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH, 20000);  // 20ms timeout
  long distance = duration * 0.034 / 2;

  return distance;  // in cm
}

void forward() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void stopMoving() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

void turnLeft() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  delay(400);  // adjust based on your robot’s turn radius
}

void turnRight() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  delay(400);  // adjust based on your robot’s turn radius
}

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  scanServo.attach(servoPin);
  scanServo.write(90);  // center position

  Serial.begin(9600);
}

void loop() {
  long front = getDistance();
  Serial.print("Front: ");
  Serial.println(front);

  if (front > 20) {
    forward();
  } else {
    stopMoving();
    delay(300);

    // Look left
    scanServo.write(150);
    delay(500);
    long left = getDistance();
    Serial.print("Left: ");
    Serial.println(left);

    // Look right
    scanServo.write(30);
    delay(500);
    long right = getDistance();
    Serial.print("Right: ");
    Serial.println(right);

    scanServo.write(90);  // center

    // Decide direction
    if (left > right && left > 15) {
      turnLeft();
    } else if (right > 15) {
      turnRight();
    } else {
      // Dead-end: turn around
      turnLeft();
      delay(400);
    }
  }

  delay(100);
}