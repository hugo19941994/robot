#include <Servo.h>
#define debug

// Global variables for motors
int speed1 = 0;
int speed2 = 0;
int speed3 = 0;
char dir1 = 'F';
char dir2 = 'F';
char dir3 = 'F';
String s1 = "";
String s2 = "";
String s3 = "";
String st = "";
boolean stringComplete = false;
int pwmM1 = 2;
int M11 = 52;
int M12 = 53;
int pwmM2 = 3;
int M21 = 50;
int M22 = 51;
int pwmM3 = 4;
int M31 = 49;
int M32 = 48;

// Global variables for servos
const int NumServos = 4;
const int NumPos = 6;
const int PosDef[NumPos][NumServos] =
{ {41,  141, 90, 101},  //Resposo:  1
  {128,  20, 90,  61},  //Vertical: 2
  {151,  55, 90, 120},  //Alante:   3
  {90,   90, 90, 100},  //firme:    4
  {90,   90, 90, 90},  //por asignar
  {90,   90, 90, 90}  //por asignar
};
int ultimapos;
Servo servo0;
Servo servo1;
Servo servo2;
Servo servo3;
Servo servo4;

void setup() {
  initMotors();
  initServos();
}

void initMotors(){
  pinMode(pwmM1, OUTPUT);
  pinMode(M11, OUTPUT);
  pinMode(M12, OUTPUT);
  pinMode(pwmM2, OUTPUT);
  pinMode(M21, OUTPUT);
  pinMode(M22, OUTPUT);
  pinMode(pwmM3, OUTPUT);
  pinMode(M31, OUTPUT);
  pinMode(M32, OUTPUT);
  Serial.begin(19200);
  st.reserve(20);
}

void loop() {
  // Motors
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
  if (dir3 == 'F') {  // Motor 3 Forwards (Flaps)
    digitalWrite(M31, HIGH);
    digitalWrite(M32, LOW);
  }
  else {  // Motor 3 Backwards (Flaps)
    digitalWrite(M31, LOW);
    digitalWrite(M32, HIGH);
  }

  if(stringComplete) {  // From serial
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
    dir3 = st[8];
    s3 = "";
    s3 += st[9];
    s3 += st[10];
    s3 += st[11];
    speed1 = atoi(s1.c_str());
    analogWrite(pwmM1, speed1);
    speed2 = atoi(s2.c_str());
    analogWrite(pwmM2, speed2);
    speed3 = atoi(s3.c_str());
    analogWrite(pwmM3, speed3);
    st = "";
    stringComplete = false;
  }

  // Arm
  static int val = 0;
  val =  st[12];
#ifdef debug
  Serial.println("===============================================================");
#endif
  muestraPos(ultimapos);
  switch (val) {
    case '1':  muestraPos(0); moverservos(0);  muestraPos(0); break;
    case '2':  muestraPos(1); moverservos(1);  muestraPos(1); break;
    case '3':  muestraPos(2); moverservos(2);  muestraPos(2); break;
    case '4':  muestraPos(3); moverservos(3);  muestraPos(3); break;
    case '5':  muestraPos(4); moverservos(4);  muestraPos(4); break;
    case '6':  muestraPos(5); moverservos(5);  muestraPos(5); break;
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

void initServos() {
  ultimapos = 0;
  servo0.attach(3);
  servo1.attach(5);
  servo2.attach(6);
  servo3.attach(10);
  servo4.attach(11);
  servo0.write(PosDef[ultimapos][0]);
  servo1.write(180 - PosDef[ultimapos][0]);
  servo2.write(PosDef[ultimapos][1]);
  servo3.write(PosDef[ultimapos][2]);
  servo4.write(PosDef[ultimapos][3]);
}

void moverservos(int nuevapos) {
  int i, j, dif;
  int paso[NumServos];
  int maxDif = 0;
  for (i = 0; i < NumServos; i++) {
    dif = abs(PosDef[ultimapos][i] - PosDef[nuevapos][i]);
    if(maxDif < dif) maxDif = dif;                        
  }
#ifdef debug
  Serial.print("Ult: ");
  Serial.print(ultimapos);
  Serial.print(", Nuev: ");
  Serial.print(nuevapos);
  Serial.print(", Dif: ");
  Serial.println(maxDif);
#endif
  for (j = 0; j < maxDif; j++) {
    for (i = 0; i < NumServos; i++) {
      paso[i] = map(j, 0, maxDif - 1, PosDef[ultimapos][i], PosDef[nuevapos][i]);
    }
#ifdef debug
    Serial.print(j);
    Serial.print(":\t");
    Serial.print(paso[0]);
    Serial.print(",");
    Serial.print(paso[1]);
    Serial.print(",");  
    Serial.print(paso[2]);
    Serial.print(",");
    Serial.println(paso[3]);
#endif
    servo0.write(paso[0]);
    servo1.write(180 - paso[0]);
    servo2.write(paso[1]);
    servo3.write(paso[2]);
    servo4.write(paso[3]);
#ifdef debug
    delay(3);
#else
    delay(20);
#endif
 }
  ultimapos = nuevapos;
}

void muestraPos(int p) {
#ifdef debug
  Serial.print(p);
  Serial.print("*:\t");
  Serial.print(PosDef[p][0]);
  Serial.print(",");
  Serial.print(PosDef[p][1]);
  Serial.print(",");
  Serial.print(PosDef[p][2]);
  Serial.print(",");
  Serial.println(PosDef[p][3]);
#else
  Serial.print(".");
#endif
}

