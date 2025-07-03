// === IR Sensor Pins ===
#define LS A0  // Left IR Sensor
#define RS A1  // Right IR Sensor

// === Motor Pins ===
// Left Motor
#define LM_DIR_10 10  // Direction pin
#define LM_PWM_11 11  // Speed (PWM) pin

// Right Motor
#define RM_DIR_12 12  // Direction pin
#define RM_PWM_13 13  // Speed (PWM) pin

// === Constants ===
#define THRESHOLD 500  // IR sensor threshold

void setup() {
  // Motor pin modes
  pinMode(LM_DIR_10, OUTPUT);
  pinMode(LM_PWM_11, OUTPUT);
  pinMode(RM_DIR_12, OUTPUT);
  pinMode(RM_PWM_13, OUTPUT);

  pinMode(LS, INPUT);
  pinMode(RS, INPUT);

  // Serial monitor for debugging
  Serial.begin(9600);
}

void loop() {
  int leftValue = analogRead(LS);
  int rightValue = analogRead(RS);


  // IR sensor logic: true = sensor sees black line
  bool leftOnLine = leftValue < THRESHOLD;
  bool rightOnLine = rightValue < THRESHOLD;

  if (leftOnLine && rightOnLine) {
    Serial.println("Move Forward");
    moveForward();
  } else if (!leftOnLine && rightOnLine) {
    Serial.println("Turn Right");
    turnRight();
  } else if (leftOnLine && !rightOnLine) {
    Serial.println("Turn Left");
    turnLeft();
  } else {
    Serial.println("STOP");
    stopMotors();
  }

  delay(50);
}

void moveForward() {
  digitalWrite(LM_DIR_10, HIGH);
  digitalWrite(LM_PWM_11, LOW);
  digitalWrite(RM_DIR_12, LOW);
  digitalWrite(RM_PWM_13, HIGH);
}

void turnLeft() {
  digitalWrite(LM_DIR_10, HIGH);
  digitalWrite(LM_PWM_11, LOW);
  digitalWrite(RM_DIR_12, HIGH);
  digitalWrite(RM_PWM_13, LOW);
}

void turnRight() {
  digitalWrite(LM_DIR_10, LOW);
  digitalWrite(LM_PWM_11, HIGH);
  digitalWrite(RM_DIR_12, LOW);
  digitalWrite(RM_PWM_13, HIGH);
}

void stopMotors() {
  digitalWrite(LM_DIR_10, LOW);
  digitalWrite(LM_PWM_11, LOW);
  digitalWrite(RM_DIR_12, LOW);
  digitalWrite(RM_PWM_13, LOW);
}