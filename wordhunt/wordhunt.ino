#include <Stepper.h>
#include <Servo.h>

const int stepsPerRevolution = 2048;
const int revPerMin = 13;

Stepper capac(stepsPerRevolution, 8, 10, 9, 11);
int sPos, jPos;
bool cOn = false;

Servo joint;
int jointPin = 4; 
Servo should;
int shouldPin = 5;

int resetPin = 2;
bool coords = true;
bool python = true;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  while (Serial.available() > 0) {
    Serial.print(Serial.read());
  }
  Serial.println("");
  joint.attach(jointPin);
  should.attach(shouldPin);

  // Reset positions
  joint.write(100);
  jPos = 100;
  should.write(90);
  sPos = 90;
  
  capac.setSpeed(revPerMin);
  pinMode(resetPin, INPUT);
  while (digitalRead(resetPin) == 0) {
    capac.step(1);
  }
  cOn = false;
  /*
  Serial.println("Coords?");
  while (Serial.available() == 0);
  int coordsInt = Serial.parseInt();
  Serial.read();
  if (coordsInt == 0) {
    coords = false;
  }
  */
}

int sLookup[][4] = 
{
  {105, 90, 80, 68},
  {92, 80, 72, 62},
  {77, 70, 63, 54},
  {62, 56, 53, 47}
};

int jLookup[][4] = 
{
  {160, 145, 135, 120},
  {163, 149, 137, 122},
  {161, 147, 136, 120},
  {156, 144, 133, 117}
};

void smooth(int newS, int newJ) {
  int n = 20;
  int sDiff = newS - sPos;
  int jDiff = newJ - jPos;
  if (sDiff < 0) {
    sDiff -= 3;
  }
  else if (sDiff > 0){
    sDiff += 3;
  }
  if (jDiff < 0) {
    jDiff -= 3;
  }
  else if (jDiff > 0){
    jDiff += 3;
  }
  for (int i = 1; i <= n; i++) {
    should.write(sPos + (i*sDiff)/n);
    delay(10);
    joint.write(jPos+(i*jDiff)/n);
    delay(10);
  }
  should.write(newS);
  joint.write(newJ);
  sPos = newS;
  jPos = newJ;
}

void tap(int cap) {
  if (cap == 0 && cOn == true) {
    while (digitalRead(resetPin) == 0) {
      capac.step(1);
    }
    cOn = false;
  }
  else if (cap == 1 && cOn == false) {
    capac.step(-200);
    cOn = true;
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  
  while(true) {
   

    char buf[50];
    int cap;

    if (python) {
      while (Serial.available() == 0);
      char c = Serial.read();
      String w = "";
      while (c != '\n') {
        if (c >= '0' && c <= '3') {
          w.concat(c);
        }
        c = Serial.read();
      }
      Serial.println(w);
      for (int i = 0; i < w.length(); i += 2) {
        int r = (int)(w.charAt(i)-'0');
        int c = (int)(w.charAt(i+1)-'0');
        if (r >= 0 && r < 4 && c >= 0 && c < 4) {
          smooth(sLookup[r][c], jLookup[r][c]);
          delay(200);
        }
        if (i == 0) {
          tap(1);
        }
      }
      tap(0);
      delay(500);
    }
    else{
      if (coords) {
        Serial.println("Enter row, col, capac");
        while (Serial.available() == 0);
        int r = Serial.parseInt();
        int c = Serial.parseInt();
        cap = Serial.parseInt();
        Serial.read();
        sprintf(buf, "%d %d %d", r, c, cap); 
        Serial.println(buf);
        if (r >= 0 && r < 4 && c >= 0 && c < 4) {
          smooth(sLookup[r][c], jLookup[r][c]);
        }
      }
      else {
        Serial.println("Enter shoulder, joint, capac");
        while (Serial.available() == 0);
        int s = Serial.parseInt();
        int j = Serial.parseInt();
        cap = Serial.parseInt();
        Serial.read();
        sprintf(buf, "%d %d %d", s, j, cap);
        Serial.println(buf); 
        if (j >= 15 && j <= 170 && s >= 15 && s <= 170) {
          smooth(s, j);
        }
      }
      tap(cap);
    }
  }
}
