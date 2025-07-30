#include <Servo.h>//Servo.h: It Allows control of the servo motor.

#include <U8g2lib.h>// U8g2lib.h: This library is Used to control the OLED display with graphical functions.

// Let's define the PINS
#define TRIG_PIN 10
#define ECHO_PIN 11
#define SERVO_PIN 12
#define BUZZER_PIN 7
#define LED_PIN 13
/*
	•	TRIG_PIN & ECHO_PIN: Used for Ultrasonic sensor to measure distance.
	•	SERVO_PIN: Controls the servo motor’s rotation.
	•	BUZZER_PIN: Sounds an alarm when object is detected.
	•	LED_PIN: Lights up when object is detected.
*/

// Now create the Radar and Detection Parameters
const int sweepMin = 25;
const int sweepMax = 180;
const int radarCenterX = 64;
const int radarCenterY = 63;
const int radarRadius = 60;
const int distanceThreshold = 40;

/*
	•	sweepMin/sweepMax: Limits for radar sweep angle.
	•	radarCenterX/Y: Center point of radar display.
	•	radarRadius: How far the radar sweeps visually.
	•	distanceThreshold: Distance under which the object is considered detected (in cm).
*/

// Lets create the Variables and Object Creation

long duration;
int distance;
Servo myServo;  // Creates a servo object for controlling the servo motor
U8G2_SH1106_128X64_NONAME_F_HW_I2C display(U8G2_R0, U8X8_PIN_NONE);
/*
	•	duration: Stores time taken by ultrasonic pulse.
	•	distance: Calculated distance to an object.
	•	myServo: Servo motor object.
	•	display: OLED display object using I2C.
*/

void setup() { // I am initializing the components
  pinMode(TRIG_PIN, OUTPUT);  // Sets the TRIG_PIN as an Output
  pinMode(ECHO_PIN, INPUT);   // Sets the ECHO_PIN as an Input
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  Serial.begin(9600);
  myServo.attach(SERVO_PIN);  // Defines on which pin is the servo motor attached
  myServo.write(90);
  display.begin();
  /*
	•	First, Initializes pin modes.
	•	then, Starts serial communication to 9600 port
	•	then, Attaches the servo to its pin.
	•	and finally, Starts the OLED display.
*/
}

// Draw Radar Grid on the OLED display
// Create a drawRadarGrid() function
void drawRadarGrid() {
  // let's Draw a 3 arcs representing distance levels.
  display.drawCircle(radarCenterX, radarCenterY, 15);
  display.drawCircle(radarCenterX, radarCenterY, 30);
  display.drawCircle(radarCenterX, radarCenterY, 45);

  // 	let's Draw a lines at every 30° to look like a radar screen. 
  for (int angle = 0; angle <= 180; angle += 30) {
    /*
    •	We cannot use the exact angle in degrees to draw a line
    •	So here we use Trigonometric functions, this functions like cos() and sin() use radians, not degrees.
    In Simple words, Trigonometry is a branch of math that deals with the angles and sides of triangles — especially right-angled triangles 
    (triangles that have a 90° angle). Here we use trigonometric functions to know exactly where to draw on the screen 
    when the radar sweeps at different angles.
	  •	So this converts the angle from degrees (like 30°, 60°) into radians.
	  •	Formula:
      radians = angle in degrees * (pi/180)
    */
    float rad = angle * (3.14159265 / 180.0);
    /*
    	•	This finds the X position of the line’s end on the screen.
      •	Uses cos() to find how far along the horizontal (x-axis) the end point is.
      •	radarCenterX: the center of the radar screen.
      •	radarRadius * cos(rad): how far to move from center based on angle.
    */
    int xEnd = radarCenterX + radarRadius * cos(rad);
    int yEnd = radarCenterY - radarRadius * sin(rad);
    display.drawLine(radarCenterX, radarCenterY, xEnd, yEnd);
  }
}

//Now Draw Radar Sweep Line & Detected Object
void drawRadarSweep(int angle, long distance) {
  float radAngle = angle * (3.14159265 / 180.0);

  // Draws sweeping radar line.
  int xEnd = radarCenterX + radarRadius * cos(radAngle);
  int yEnd = radarCenterY - radarRadius * sin(radAngle);
  display.drawLine(radarCenterX, radarCenterY, xEnd, yEnd);

  // 	If an object is detected within the threshold, draws a small dot.
  /*
  When the ultrasonic sensor detects an object within range, we want to show that object as a small dot on our radar. 
  We take the real distance, shrink it to fit our radar screen using the map() function, and then use a bit of math 
  to figure out exactly where that dot should go based on the radar’s sweep angle. Finally, we draw the dot on the 
  OLED screen. Let's start to draw a disc.
  */
  if (distance < distanceThreshold) {
    // Now lets convert the real-world measured distance into a visual distance on the radar display.
    /*
    We are using the map() function: map(value, fromLow, fromHigh, toLow, toHigh)
      •	If distance is 0 cm, r becomes 0 (dot at radar center).
      •	If distance is distanceThreshold (like 40 cm), r becomes radarRadius (edge of radar).
      •	If distance is in between, r will be proportionally in between.
      Example:
      distance = 20 cm, distanceThreshold = 40, and radarRadius = 60
      r = map(20, 0, 40, 0, 60) = 30
      So the dot will be halfway from the radar center to the edge.
    */
    int r = map(distance, 0, distanceThreshold, 0, radarRadius);
    /*
    Calculate X Position of the Object:
      	•	Converts the distance (r) and the sweep angle (radAngle) into an X coordinate.
	      •	Adds this to radarCenterX to find the object’s position relative to the center.
    */
    int xObj = radarCenterX + r * cos(radAngle);
    /*
    Calculate Y Position of the Object:
      	•	Calculates Y coordinate of the object dot.
	      •	Subtracts because display Y-axis grows downwards, so going “up” means subtracting.
    */
    int yObj = radarCenterY - r * sin(radAngle);
    /*
    	•	Draws a small filled circle (radius 2 pixels) at the (xObj, yObj) position.
	    •	This dot shows where the object is detected on the radar screen.
    */
    display.drawDisc(xObj, yObj, 2);
  }
}

// Distance Calculation Function
int calculateDistance() {
  // •	Sends ultrasonic pulse.
	// •	Waits for echo.

  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  duration = pulseIn(ECHO_PIN, HIGH);
  // •	Calculates distance using the formula:
  distance = duration * 0.034 / 2;
  return distance;
}

void loop() {
  /*
  	•	Moves the servo from 15° to 165°.
    •	For each angle:
      •	Rotate servo
      •	Calculate distance
      •	Print to Serial
      •	Update OLED with radar visuals and values
      •	Turn on buzzer/LED if object is close
      •	Small delay
  */
  for (int i = sweepMin; i <= sweepMax; i++) {
    myServo.write(i);

    distance = calculateDistance();  

    Serial.print(i);       
    Serial.print(",");       
    Serial.print(distance);  
    Serial.print(".");       
    display.clearBuffer();
    drawRadarGrid();
    drawRadarSweep(i, distance);

    display.setFont(u8g2_font_5x8_tr);
    display.setCursor(0, 10);
    display.print("Dist:");
    display.print(distance);
    display.print("cm");

    display.setCursor(80, 10);
    display.print("Ang:");
    display.print(i);

    display.sendBuffer();
    if (distance < distanceThreshold) {
      digitalWrite(BUZZER_PIN, HIGH);
      digitalWrite(LED_PIN, HIGH);
    } else {
      digitalWrite(BUZZER_PIN, LOW);
      digitalWrite(LED_PIN, LOW);
    }
    delay(30);
  }
  // Same logic but servo moves in reverse (165° back to 15°).
  for (int i = sweepMax; i > sweepMin; i--) {
    myServo.write(i);

    distance = calculateDistance();
    Serial.print(i);
    Serial.print(",");
    Serial.print(distance);
    Serial.print(".");
    display.clearBuffer();
    drawRadarGrid();
    drawRadarSweep(i, distance);

    display.setFont(u8g2_font_5x8_tr);
    display.setCursor(0, 10);
    display.print("Dist:");
    display.print(distance);
    display.print("cm");

    display.setCursor(80, 10);
    display.print("Ang:");
    display.print(i);

    display.sendBuffer();
    if (distance < distanceThreshold) {
      digitalWrite(BUZZER_PIN, HIGH);
      digitalWrite(LED_PIN, HIGH);
    } else {
      digitalWrite(BUZZER_PIN, LOW);
      digitalWrite(LED_PIN, LOW);
    }
    delay(30);
  }
}

