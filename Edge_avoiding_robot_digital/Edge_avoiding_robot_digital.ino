// DEFINING INPUT SENSOR
#define LS 10     // left sensor
#define RS 9      // right sensor

// DEFINING OUPUT MOTORS
#define LM1 7       // left motor one
#define LM2 6     // left motor two
#define RM1 5       // right motor one
#define RM2 4       // right motor two

void setup()
{
  pinMode(LS, INPUT);
  pinMode(RS, INPUT);
  pinMode(LM1, OUTPUT);
  pinMode(LM2, OUTPUT);
  pinMode(RM1, OUTPUT);
  pinMode(RM2, OUTPUT);
}
void loop()
{
  
  if(!(digitalRead(LS)) && !(digitalRead(RS)))     // Move Forward
  {
    digitalWrite(LM1, HIGH);
    digitalWrite(LM2, LOW);
    digitalWrite(RM1, HIGH);
    digitalWrite(RM2, LOW);
  }
  
  if(!(digitalRead(LS)) && digitalRead(RS))     // Turn Left
  {
    digitalWrite(LM1, HIGH);
    digitalWrite(LM2, LOW);
    digitalWrite(RM1, LOW);
    digitalWrite(RM2, HIGH);
    delay(1000);
  }
  
  if(digitalRead(LS) && !(digitalRead(RS)))     // Turn Right
  {
    digitalWrite(LM1, LOW);
    digitalWrite(LM2, HIGH);
    digitalWrite(RM1, HIGH);
    digitalWrite(RM2, LOW);
    delay(1000);
  }
  
  if((digitalRead(LS)) && (digitalRead(RS)))     // turn (Right)
  {
    digitalWrite(LM1, LOW);
    digitalWrite(LM2, HIGH);
    digitalWrite(RM1, HIGH);
    digitalWrite(RM2, LOW);
    delay(1000);
  }
}
