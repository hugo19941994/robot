#include <Servo.h>

int speed1 = 0;
int speed2 = 0;
char dir1 = 'F';
char dir2 = 'F';
String s1 = "";
String s2 = "";
String st = "";
boolean stringComplete = false;
int pwmM1 = 12;
int M11 = 22;
int M12 = 23;
int pwmM2 = 13;
int M21 = 24;
int M22 = 25;

void setup() {
  // put your setup code here, to run once:
  pinMode(pwmM1, OUTPUT);
  pinMode(M11, OUTPUT);
  pinMode(M12, OUTPUT);
  pinMode(pwmM2, OUTPUT);
  pinMode(M21, OUTPUT);
  pinMode(M22, OUTPUT);
  Serial.begin(19200);
  st.reserve(20);
}

void loop() {
  if (dir1 == 'F') {  // Motor 1 Forwards
    digitalWrite(M11, HIGH);
    digitalWrite(M12, LOW);
  }
  else {  // Motor 1 Backwards
    digitalWrite(M11, LOW);
    digitalWrite(M12, HIGH);
  }
  if (dir2 == 'F') {  // Motor 2 Forwards
    digitalWrite(M21, HIGH);
    digitalWrite(M22, LOW);
  }
  else {  // Motor 2 Backwards
    digitalWrite(M21, LOW);
    digitalWrite(M22, HIGH);
  }

  /*  Formato: char-int-int-char-int-int-int-int
   *  Los char indican direccion
   *  Los int indican velocidad
   *  Los 2 ultimos int controlan los servos
   */
  if(stringComplete) {  // Mensaje desde serial
    dir1 = st[0];
    s1 = "";
    s1 += st[1];
    s1 += st[2];
    s1 += st[3];
    dir2 = st[4];
    s2 = "";
    s2 += st[5];
    s2 += st[6];
    s2 += st[7];
    speed1 = atoi(s1.c_str());
    analogWrite(pwmM1, speed1);
    speed2 = atoi(s2.c_str());
    analogWrite(pwmM2, speed2);
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
