#include <Servo.h>
#define servoPin 6 // Pin à relier à la sortie d'arduino

Servo myservo;  // create servo object to control a servo
// twelve servo objects can be created on most boards

int angle = 0;    // variable to store the servo position

void setup() {
  myservo.attach(servoPin);  // attaches the servo on pin 9 to the servo object
  //myservo.write(angle);
  for (angle = 0; angle <= 90; angle += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myservo.write(angle);              // tell servo to go to position in variable 'pos'
    delay(15);
  }
}

void loop() {
  //myservo.write(angle);
  bouge();
  delay(20000); 
}

void bouge()
{
  for (angle = 90; angle >= 0; angle -= 1) { // goes from 180 degrees to 0 degrees
    myservo.write(angle);              // tell servo to go to position in variable 'pos'
    delay(20);                       // waits 15ms for the servo to reach the position
  }
  for (angle = 0; angle <= 90; angle += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myservo.write(angle);              // tell servo to go to position in variable 'pos'
    delay(20);                       // waits 15ms for the servo to reach the position
  }
}
