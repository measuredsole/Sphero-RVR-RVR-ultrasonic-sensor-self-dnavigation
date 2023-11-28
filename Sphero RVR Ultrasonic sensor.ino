#include <SpheroRVR.h> //Always needed if using a SpheroRVR 
 
 
static uint32_t ledGroup; //Specifically for the lights 
const int pingPin = 7; // Trigger Pin of Ultrasonic Sensor (on the board) 
const int echoPin = 6; // Echo Pin of Ultrasonic Sensor (on the board) 
 
 
void setup() { 
  Serial.begin(9600); // Starting Serial Terminal 
 
  // set up communication with the RVR. 
  rvr.configUART(&Serial); //Do not remove or change this line. Always needed if using a SpheroRVR 
   
  // setting up the led group for both headlights.  
  ledGroup = 0; //Do not remove or change this line 
  ledGroup |= (1 << static_cast<uint8_t>(LEDs::rightHeadlightRed)); //Do not remove or change this line 
  ledGroup |= (1 << static_cast<uint8_t>(LEDs::rightHeadlightGreen)); //Do not remove or change this line 
  ledGroup |= (1 << static_cast<uint8_t>(LEDs::rightHeadlightBlue)); //Do not remove or change this line 
  ledGroup |= (1 << static_cast<uint8_t>(LEDs::leftHeadlightRed)); //Do not remove or change this line 
  ledGroup |= (1 << static_cast<uint8_t>(LEDs::leftHeadlightGreen)); //Do not remove or change this line 
  ledGroup |= (1 << static_cast<uint8_t>(LEDs::leftHeadlightBlue)); //Do not remove or change this line 
} 
 
void loop() { 
  //Code largely borrowed from https://create.arduino.cc/projecthub/Isaac100/getting-started-with-the-hc-sr04-ultrasonic-sensor-036380 
  long duration, inches, cm; //Random variables that are integers. 
   
  /*  When the sound waves hit the receiver, it turns the Echo pin high for however long the  
   *  waves were traveling for. To get that, we can use a handy Arduino function called pulseIn().  
   *  It takes 2 arguments, the pin you are listening to(In our case, the Echo pin), and a state(HIGH or LOW).  
   *  What the function does is waits for the pin to go whichever state you put in, starts timing, and then  
   *  stops timing when it switches to the other state. In our case we would put HIGH since we want to start  
   *  timing when the Echo pin goes high. We will store the time in the duration variable.  
   *  (It returns the time in microseconds) 
   */ 
  pinMode(pingPin, OUTPUT); //tells the pingPin that is output 
  digitalWrite(pingPin, LOW); 
  delayMicroseconds(2); 
  digitalWrite(pingPin, HIGH); 
  delayMicroseconds(10); 
  digitalWrite(pingPin, LOW); 
  pinMode(echoPin, INPUT); 
 
  /* 
   * Now that we have the time, we can use the equation speed = distance/time, but we will make it time x  
   * speed = distance because we have the speed. What speed do we have? The speed of sound, of course! 
   */ 
  duration = pulseIn(echoPin, HIGH); 
 
  inches = microsecondsToInches(duration); //Sets inches to how far in inches the closest thing is in front of it is 
  if(inches < 9  ){  
    //If an object is less than 9 inches then stop both motors 
uint8_t redArray[] = {0xFF, 0x00, 0x00, 0xFF, 0x00, 0x00};
    rvr.setAllLeds(ledGroup, redArray, sizeof(redArray) / sizeof(redArray[0]));
rvr.driveWithHeading(0, 0, 0); // stop any previous movement
  rvr.resetYaw(); // reset heading to zero
  
  rvr.driveWithHeading(0, 93, 0); // turn right 90 degrees
  delay(600); // wait for turn to complete
 
  }else{ 
    //if an object is more than 9 inches then drive forward iwth 64 speed
    rvr.rawMotors(RawMotorModes::forward, 100, RawMotorModes::forward, 100) ;
 uint8_t greenArray[] = {0x00, 0xFF, 0x00, 0x00, 0xFF, 0x00}; 
    rvr.setAllLeds(ledGroup, greenArray, sizeof(greenArray) / sizeof(greenArray[0]));
  
  } 
  cm = microsecondsToCentimeters(duration); //In case you want it in centimeters. 
  delay(100); 
} 
 
//A function that converts microseconds to inches. 
long microsecondsToInches(long microseconds) { 
   return microseconds / 74 / 2; 
} 
 
//A function that converts microseconds to centimeters. 
long microsecondsToCentimeters(long microseconds) { 
   return microseconds / 29 / 2; 
} 
