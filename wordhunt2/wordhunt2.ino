#include <Stepper.h>
#include <Servo.h>

int sPos, jPos;
bool fingOn;

Servo fing;
int fingPin = 9;
int offDeg = 100;
int onDeg = 134;

Servo joint;
int jointPin = 10; 
int startJoint = 100;

Servo should;
int shouldPin = 11;
int startShould = 30;

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
  fing.attach(fingPin);
  // Reset positions
  joint.write(startJoint);
  jPos = startJoint;
  should.write(startShould);
  sPos = startShould;
  fing.write(offDeg);
  fingOn = false;
}

int sLookup[][4] = 
{
  {106, 92, 82, 74},
  {92, 78, 63, 46},
  {85, 66, 48, 32},
  {82, 62, 41, 22}
};

int jLookup[][4] = 
{
  {105, 79, 58, 40},
  {103, 78, 56, 36},
  {106, 79, 58, 42},
  {118, 87, 67, 52}
};

void smooth(int newS, int newJ) {
  int n = 7;
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
    delay(50);
    joint.write(jPos+(i*jDiff)/n);
    delay(50);
  }
  should.write(newS);
  joint.write(newJ);
  sPos = newS;
  jPos = newJ;
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
        }
        if (i == 0) {
          fing.write(onDeg);
        }
      }
      fing.write(offDeg);
      delay(120);
    }
    else{
      int f;
      if (coords) {
        Serial.println("Enter row, col, tap");
        while (Serial.available() == 0);
        int r = Serial.parseInt();
        int c = Serial.parseInt();
        f = Serial.parseInt();
        Serial.read();
        sprintf(buf, "%d %d %d", r, c, f); 
        Serial.println(buf);
        if (r >= 0 && r < 4 && c >= 0 && c < 4) {
          smooth(sLookup[r][c], jLookup[r][c]);
        }
      }
      else {
        Serial.println("Enter shoulder, joint, tap");
        while (Serial.available() == 0);
        int s = Serial.parseInt();
        int j = Serial.parseInt();
        f = Serial.parseInt();
        Serial.read();
        sprintf(buf, "%d %d %d", s, j, f);
        Serial.println(buf); 
        if (j >= 15 && j <= 170 && s >= 15 && s <= 170) {
          smooth(s, j);
        }
      }
      
      if (f == 0 && fingOn) {
        fing.write(offDeg);
        fingOn = false;
      }
      else if (f != 0 && !fingOn) {
        fing.write(onDeg);
        fingOn = true;
      }
    }
  }
}
