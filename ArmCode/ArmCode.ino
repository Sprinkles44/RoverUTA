// Pot & gripper pot pins
int J1 = 0;
int J2 = 1;
int J3 = 2;
int J5 = 4;
int G = 5;
// Pot & gripper analog values
int Base= 0;
int Shoulder= 0;
int Elbow= 0;
int Wrist= 0;
int Gripper= 0;
char input;
// Base Pins
int InA1 = 22;
int InB1 = 23;
int PWM1 = 12;
// Shoulder Pins
int InA2 = 24;
int InB2 = 25;
int PWM2 = 11;
// Elbow Pins
int InB3 = 26;
int InA3 = 27;
int PWM3 = 10;
// Wrist Rotation Pins
int InB4 = 28;
int InA4 = 29;
int PWM4 = 9;
// Wrist Pitch Pins
int InB5 = 30;
int InA5 = 31;
int PWM5 = 8;
// Gripper Pins
int InB6 = 32;
int InA6 = 33;
int PWM6 = 7;
// Solenoid Pin
int Sole = 53;
bool SoleState;
// New Angle Values
int newBase;
int newSh;
int newEl;
int newWr;
int xi = 4; // precision/sloppyness compensation of angle reading
// Should or should not be rotating
bool rot; 
bool rota;
bool rotb;
bool rotc;
bool rotd;
// Manual Controls
bool mcont;
// Arm Limb lengths in inches
int L1 = 27.5;
int L2 = 7;
int L3 = 4;
int L5 = 12;


void setup() {
  Serial.begin(9600); 
  pinMode(InA1, OUTPUT);
  pinMode(InB1, OUTPUT);
  pinMode(PWM1, OUTPUT); 
  pinMode(InA2, OUTPUT);
  pinMode(InB2, OUTPUT);
  pinMode(PWM2, OUTPUT);
  pinMode(InA3, OUTPUT);
  pinMode(InB3, OUTPUT);
  pinMode(PWM3, OUTPUT);
  pinMode(InA5, OUTPUT);
  pinMode(InB5, OUTPUT);
  pinMode(PWM5, OUTPUT);
  pinMode(Sole, OUTPUT);
  // Turning all motors off by default
  restartAngles();
  mcont = false;
}


// Main loop
void loop() {
  readAngleValues();
  printAngleValues();
  if (Serial.available() > 0){
    input = Serial.read();
    if(input == 'r'){ restartAngles(); }
    else if(input == 'c'){
      rot = false;
      mcont = true;
      while(mcont){
        keyboardArmControls();
        readNewAngles();
        printAngleValues();
      }
      
    }
    else if(input == 'n'){ 
      readNewAngles();
      rot = true;
      rota = true;
      rotb = true;
      rotc = true;
      rotd = true;
      }
  }
  if(rot){
    adjustArmAngles();
  }
}


void readAngleValues(){
  Base = analogRead(J1);
  Shoulder = analogRead(J2);
  Elbow = analogRead(J3);
  Wrist = analogRead(J5);
  Gripper = analogRead(G);
  // Mapped values of pot to degrees
  Base = map(Base,0,1024,0,330);
  Shoulder = map(Shoulder,0,1024,0,330);
  Elbow = map(Elbow,0,1024,0,330);
  Wrist = map(Wrist,0,1024,0,330);
  Gripper = map(Gripper,0,1024,0,330);
}


void printAngleValues(){
  Serial.print("Controls: ");
  if(mcont){ Serial.print("Manual") };
  else{ Serial.print("Spacial"); }
  Serial.print(" | J1: ");
  Serial.print(Base);
  Serial.print(" | J2: ");
  Serial.print(Shoulder);
  Serial.print(" | J3: ");
  Serial.print(Elbow);
  Serial.print(" | J5: ");
  Serial.print(Wrist);
  Serial.print(" | G: ");
  Serial.print(Gripper);
  Serial.print(" | Sole: ");
  Serial.println(SoleState);
  delay(50);
}


void readNewAngles(){  
  newBase = Serial.parseInt();
  newSh = Serial.parseInt();
  newEl = Serial.parseInt();
  newWr = Serial.parseInt();
}


void restartAngles(){
  rot = false;
  digitalWrite(InA1, LOW);
  digitalWrite(InB1, LOW);
  analogWrite(PWM1, 0);
  digitalWrite(InA2, LOW);
  digitalWrite(InB2, LOW);
  analogWrite(PWM2, 0);
  digitalWrite(InA3, LOW);
  digitalWrite(InB3, LOW);
  analogWrite(PWM3, 0);
  digitalWrite(InA4, LOW);
  digitalWrite(InB4, LOW);
  analogWrite(PWM4, 0);
  digitalWrite(InA5, LOW);
  digitalWrite(InB5, LOW);
  analogWrite(PWM5, 0);
  digitalWrite(Sole, LOW);
  SoleState=false;
  readAngleValues();
  newBase = Base;
  newSh = Shoulder;
  newEl = Elbow;
  newWr = Wrist;
}


void adjustArmAngles(){
  if(newBase > Base + xi){ //if statements for J1/Base
    digitalWrite(InA1, HIGH);
    digitalWrite(InB1, LOW);
    analogWrite(PWM1, 127);
  }else if(newBase < Base - xi){
    digitalWrite(InA1, LOW);
    digitalWrite(InB1, HIGH);
    analogWrite(PWM1, 127);
  }else if(newBase >= Base - xi && newBase <= Base + xi){
    digitalWrite(InA1, LOW);
    digitalWrite(InB1, LOW);
    analogWrite(PWM1, 0);
    rota = false;
  }
  if(newSh < Shoulder - xi){  //if statements for J2/shoulder
    digitalWrite(InA2, HIGH);
    digitalWrite(InB2, LOW);
    analogWrite(PWM2, 255);
  }else if(newSh > Shoulder + xi){
    digitalWrite(InA2, LOW);
    digitalWrite(InB2, HIGH);
    analogWrite(PWM2, 255);
  }else if(newSh >= Shoulder - xi && newSh <= Shoulder + xi){
    digitalWrite(InA2, LOW);
    digitalWrite(InB2, LOW);
    analogWrite(PWM2, 0);
    rotb = false;
  }
  if(newEl < Elbow - xi){ //if statements for J3/Elbow
    digitalWrite(InA3, HIGH);
    digitalWrite(InB3, LOW);
    analogWrite(PWM3, 64);
  }else if(newEl > Elbow + xi){
    digitalWrite(InA3, LOW);
    digitalWrite(InB3, HIGH);
    analogWrite(PWM3, 64);
  }else if(newEl >= Elbow - xi && newEl <= Elbow + xi){
    digitalWrite(InA3, LOW);
    digitalWrite(InB3, LOW);
    analogWrite(PWM3, 0);
    rotc = false;
  }
  if(newWr > Wrist + xi){ //if statements for J5/Wrist Pitch
    digitalWrite(InA5, HIGH);
    digitalWrite(InB5, LOW);
    analogWrite(PWM5, 255);
  }else if(newWr < Wrist - xi){
    digitalWrite(InA5, LOW);
    digitalWrite(InB5, HIGH);
    analogWrite(PWM5, 255);
  }else if(newWr >= Wrist - xi && newWr <= Wrist + xi){
    digitalWrite(InA5, LOW);
    digitalWrite(InB5, LOW);
    analogWrite(PWM5, 0);
    rotd = false;
  }
  if (!rota && !rotb && !rotc && !rotd){ rot = false; }
}


//void anglesTo3D() {
//}


// Regular Keyboard controls. Add to loop and comment Angle methods
void keyboardArmControls(){
  if (Serial.available() > 0) {
    input = Serial.read();
    if(input == 'c'){ mcont = false; }
    else if(input == 'a'){ //if statements for J1/Base
      Serial.println("J1 DOWN");
      digitalWrite(InA1, HIGH);
      digitalWrite(InB1, LOW);
      analogWrite(PWM1, 127);
    }else if(input == '1'){
      Serial.println("J1 UP");
      digitalWrite(InA1, LOW);
      digitalWrite(InB1, HIGH);
      analogWrite(PWM1, 127);
    }else if(input == 'q'){
      Serial.println("J1 STOP");
      digitalWrite(InA1, LOW);
      digitalWrite(InB1, LOW);
      analogWrite(PWM1, 0);
   
   }else if(input == 's'){  //if statements for J2/shoulder
      Serial.println("J2 DOWN");
      digitalWrite(InA2, HIGH);
      digitalWrite(InB2, LOW);
      analogWrite(PWM2, 255);
    }else if(input == '2'){
      Serial.println("J2 UP");
      digitalWrite(InA2, LOW);
      digitalWrite(InB2, HIGH);
      analogWrite(PWM2, 255);
    }else if(input == 'w'){
      Serial.println("J2 STOP");
      digitalWrite(InA2, LOW);
      digitalWrite(InB2, LOW);
      analogWrite(PWM2, 0);
    
    }else if(input == 'd'){ //if statements for J3/Elbow
      Serial.println("J3 DOWN");
      digitalWrite(InA3, HIGH);
      digitalWrite(InB3, LOW);
      analogWrite(PWM3, 64);
    }else if(input == '3'){
      Serial.println("J3 UP");
      digitalWrite(InA3, LOW);
      digitalWrite(InB3, HIGH);
      analogWrite(PWM3, 64);
    }else if(input == 'e'){
      Serial.println("J3 STOP");
      digitalWrite(InA3, LOW);
      digitalWrite(InB3, LOW);
      analogWrite(PWM3, 0);
      
    } else if (input == 'f') { //if statements for J4/Wrist Rotation
      Serial.println("J4 DOWN");
      digitalWrite(InA4, HIGH);
      digitalWrite(InB4, LOW);
      analogWrite(PWM4, 64);
    } else if (input == '4') {
      Serial.println("J4 UP");
      digitalWrite(InA4, LOW);
      digitalWrite(InB4, HIGH);
      analogWrite(PWM4, 64);
    } else if (input == 'r') {
      Serial.println("J4 STOP");
      digitalWrite(InA4, LOW);
      digitalWrite(InB4, LOW);
      analogWrite(PWM4, 0);
      
    }else if(input == 'g'){ //if statements for J5/Wrist Pitch
      Serial.println("J5 DOWN");
      digitalWrite(InA5, HIGH);
      digitalWrite(InB5, LOW);
      analogWrite(PWM5, 255);
    }else if(input == '5'){
      Serial.println("J5 UP");
      digitalWrite(InA5, LOW);
      digitalWrite(InB5, HIGH);
      analogWrite(PWM5, 255);
    }else if(input == 't'){
      Serial.println("J5 STOP");
      digitalWrite(InA5, LOW);
      digitalWrite(InB5, LOW);
      analogWrite(PWM5, 0);
    
    }else if (input == '7') { //if statements for Solenoid
      Serial.println("Sole HIGH");
      digitalWrite(Sole, HIGH);
      SoleState=true;
    } else if (input == 'u') {
      Serial.println("Sole LOW");
      digitalWrite(Sole, LOW);
      SoleState=false;
    }
  }
}
