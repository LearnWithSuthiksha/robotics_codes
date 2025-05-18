#include <Servo.h>

#define trigPin 9
#define echoPin 8
#define IN1 2
#define IN2 3
#define IN3 4
#define IN4 5
#define servoPin 6

Servo scanServo;

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  scanServo.attach(servoPin);
  scanServo.write(90);  // Center position

  Serial.begin(9600);
}

long getDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH);
  long distance = duration * 0.034 / 2;
  return distance;
}

void moveForward() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void moveBackward() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void turnLeft() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void turnRight() {
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

void loop() {
  long frontDistance = getDistance();
  Serial.print("Front: ");
  Serial.println(frontDistance);

  if (frontDistance < 15) {
    // stopMoving();
    delay(300);

    // Look left
    scanServo.write(150);
    delay(500);
    long leftDistance = getDistance();

    // Look right
    scanServo.write(30);
    delay(500);
    long rightDistance = getDistance();

    // Center again
    scanServo.write(90);
    delay(300);

    Serial.print("Left: ");
    Serial.println(leftDistance);
    Serial.print("Right: ");
    Serial.println(rightDistance);

    if (leftDistance > rightDistance) {
      turnLeft();
    } else {
      turnRight();
    }
    delay(500);
  } else {
    moveForward();
  }
}
