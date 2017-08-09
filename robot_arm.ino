

//Import Wire and The Adafruit PWM servodriver class
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>


// Make a new PWMServodrive class called pwm
// If no adress is given, it uses the default address 0x40
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(/*Add adress here if wanted*/);


// Depending on your servo make, the pulse width min and max may vary, you 
// want these to be as small/large as possible without hitting the hard stop
// for max range. You'll have to tweak them as necessary to match the servos you
// have!
#define SERVOMIN  150 // this is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX  600 // this is the 'maximum' pulse length count (out of 4096)
#define HZ 50         // this is the PWM period for the microservo

// our servo # counter
int servonum = 0;
bool done = false;
String monitorLine = "null";

void setup() {
  Serial.begin(9600);
  Serial.println("16 channel Servo test!");

  pwm.begin();
  
  pwm.setPWMFreq(HZ);  // Analog servos run at ~50 Hz updates

  yield();
}

// you can use this function if you'd like to set the pulse length in seconds
// e.g. setServoPulse(0, 0.001) is a ~1 millisecond pulse width. its not precise!
void setServoPulse(uint8_t n, double pulse) {
  double pulselength;
  
  pulselength = 1000000;   // 1,000,000 us per second
  pulselength /= HZ;   // 50 Hz
  Serial.print(pulselength); Serial.println(" us per period"); 
  pulselength /= 4096;  // 12 bits of resolution
  Serial.print(pulselength); Serial.println(" us per bit"); 
  pulse *= 1000;
  pulse /= pulselength;
  Serial.println(pulse);
  pwm.setPWM(n, 0, pulse);
}

void loop() {
 // Read until the program is done
 while(!done){

  // Wait until you recieve something from the Serial monitor.
  if (Serial.available() > 0) {
    //read the monitor like a string
    monitorLine = myRead();

    // Compute the command written
    stringSwitch(monitorLine);
    
  // say what you got:
  //Serial.print("I received: ");
  //Serial.println(monitorLine);
  }
  
  delay(500);
  monitorLine = "null";
 }
}

void stringSwitch(String str){
  String sNum;
  int num =0;
  
  if(str != "null"){
    if(str == "min"){
      Serial.println("Enter which servo please. (1-16)");
      while(Serial.available() == 0){}
      sNum = myRead();
      num = sNum.toInt();
      if(num > 0 || num <= 16){
        servo_to_min(num-1);
      }
      else{Serial.println("Something went wrong, are you sure you type in an integer between 1-16?");}
    }
    else if(str == "max"){
      Serial.println("Enter which servo please. (1-16)");
      while(Serial.available() == 0){}
      sNum = myRead();
      num = sNum.toInt();
      if(num > 0 || num <= 16){
        servo_to_max(num-1);
      }
      else{Serial.println("Something went wrong, are you sure you type in an integer between 1-16?");}
    }
    else if(str == "quit"){
      Serial.println("Exting the main loop. Dave what will happen to me?");
      done = !done;
    }
    else{
      Serial.println("That command does not compute Idiot...");
      Serial.println("Try min, max or quit.");
    }
  }
}

// Reads everythin from the Serial monitor,
// Returns as a lowercase string.
String myRead(){
  // read the incoming byte:
  String str;
  char cha = Serial.read();

  // Construct a string of the input.
  while(Serial.peek() > 0){
    str += cha;
    cha = Serial.read();
    delay(50);
  }
  // Add the last character to the string
  str += cha;
  str.toLowerCase();
  return str;
}

void servo_to_min(int num){
  Serial.print("Setting servo: "); Serial.print(num); Serial.println(" to minimum.");
  for (int pulselen = SERVOMIN; pulselen < SERVOMAX; pulselen++) {
    pwm.setPWM(servonum, 0, pulselen);
  }
}

void servo_to_max(int num){
  Serial.print("Setting servo: "); Serial.print(num); Serial.println(" to maximum.");
  for (int pulselen = SERVOMAX; pulselen > SERVOMIN; pulselen--) {
    pwm.setPWM(servonum, 0, pulselen);
  }
}

