#include <Servo.h>
String ang1 = "";
String ang2 = "";
String ang3 = "";
String ang4 = "";
String ang5 = "";
String st = "";
boolean stringComplete = false;

Servo servo1;
Servo servo2;
Servo servo3;
Servo servo4;
Servo servo5;

void setup() {
  // put your setup code here, to run once:
  servo1.attach(3);
  servo2.attach(5);
  servo3.attach(6);
  servo4.attach(9);
  servo5.attach(10);

  Serial.begin(19200);
  st.reserve(30);
}

void loop() {
  /*  Formato: 5X 3X int
   */
  if(stringComplete) {  // Mensaje desde serial
    // Servo 1
    ang1 = "";
    ang1 += st[0];
    ang1 += st[1];
    ang1 += st[2];
    int ang = atoi(ang1.c_str());
    servo1.write(ang);

    // Servo 2
    ang2 = "";
    ang2 += st[3];
    ang2 += st[4];
    ang2 += st[5];
    ang = atoi(ang2.c_str());
    servo2.write(ang);
    
    // Servo 3
    ang3 = "";
    ang3 += st[6];
    ang3 += st[7];
    ang3 += st[8];
    ang = atoi(ang3.c_str());
    servo3.write(ang);

    // Servo 4
    ang4 = "";
    ang4 += st[9];
    ang4 += st[10];
    ang4 += st[11];
    ang = atoi(ang4.c_str());
    servo4.write(ang);

    // Servo 5
    ang5 = "";
    ang5 += st[12];
    ang5 += st[13];
    ang5 += st[14];
    ang = atoi(ang5.c_str());
    servo5.write(ang);
    
    st = "";
    stringComplete = false;
  }
}

void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    st += inChar;
    // if the incoming character is a newline, set a flag
    // so the main loop can do something about it:
    if (inChar == '\n') {
      stringComplete = true;
    }
  }
}
