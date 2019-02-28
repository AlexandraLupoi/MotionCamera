#include <Stepper.h>

#define STEPS 32

Stepper stepper(STEPS, 24, 26, 25, 27);

#define BUTTON 22

struct Pozitie{
  int Senzor;
  char Directie[10];
} Poz;

struct Senzor{
  int Trig;
  int Echo;
  int Durata;
  int Distanta;
  int DistantaAnt;
} St[4], Dr[4];

void setup(){
  int t1 = 2, t2 = 5;
  int e1 = 28, e2 = 31;
  for (int i = 1; i <= 3; i++)
  {
    St[i].Trig = t1;
    t1++;
    pinMode(St[i].Trig, OUTPUT);
    St[i].Echo = e1;
    e1++;
    pinMode(St[i].Echo, INPUT);
    Dr[i].Trig = t2;
    t2++;
    pinMode(Dr[i].Trig, OUTPUT);
    Dr[i].Echo = e2;
    e2++;
    pinMode(Dr[i].Echo, INPUT);
  }
  for (int i = 1; i <= 3; i++)
  {
    digitalWrite(St[i].Trig, LOW);
    delayMicroseconds(2);
    digitalWrite(St[i].Trig, HIGH);
    delayMicroseconds(10);
    digitalWrite(St[i].Trig, LOW);
    St[i].Durata = pulseIn(St[i].Echo, HIGH);
    St[i].DistantaAnt = St[i].Durata * 0.034/2;
    digitalWrite(Dr[i].Trig, LOW);
    delayMicroseconds(2);
    digitalWrite(Dr[i].Trig, HIGH);
    delayMicroseconds(10);
    digitalWrite(Dr[i].Trig, LOW);
    Dr[i].Durata = pulseIn(Dr[i].Echo, HIGH);
    Dr[i].DistantaAnt = Dr[i].Durata * 0.034/2;
  }
  pinMode(BUTTON, INPUT_PULLUP);
  stepper.setSpeed(500);
  Poz.Senzor = 0;
  strcpy(Poz.Directie, "St");
}

void loop(){
  if (digitalRead(BUTTON) == HIGH && Poz.Senzor == 0)
    stepper.step(50);
  if (digitalRead(BUTTON) == LOW)
  {
    Poz.Senzor = 1;
    delay(5000);
  }
  if (Poz.Senzor > 0)
  {
    for (int i = 1; i <= 3; i++)
    {
      digitalWrite(St[i].Trig, LOW);
      delayMicroseconds(2);
      digitalWrite(St[i].Trig, HIGH);
      delayMicroseconds(10);
      digitalWrite(St[i].Trig, LOW);
      St[i].Durata = pulseIn(St[i].Echo, HIGH);
      St[i].Distanta = St[i].Durata * 0.034/2;
      digitalWrite(Dr[i].Trig, LOW);
      delayMicroseconds(2);
      digitalWrite(Dr[i].Trig, HIGH);
      delayMicroseconds(10);
      digitalWrite(Dr[i].Trig, LOW);
      Dr[i].Durata = pulseIn(Dr[i].Echo, HIGH);
      Dr[i].Distanta = Dr[i].Durata * 0.034/2;
      if (strcmp(Poz.Directie, "Dr") == 0)
      {
        if (St[i].Distanta > 2 && St[i].Distanta < 400 && abs(St[i].Distanta - St[i].DistantaAnt) > 50)
        {
          stepper.step((-342) * i + Poz.Senzor - 1);
          St[i].DistantaAnt = St[i].Distanta;
          strcpy(Poz.Directie, "St");
          Poz.Senzor = i;
        }
        else
        if (Dr[i].Distanta > 2 && Dr[i].Distanta < 400 && abs(Dr[i].Distanta - Dr[i].DistantaAnt) > 50)
        {
          stepper.step(342 * i - Poz.Senzor);
          Dr[i].DistantaAnt = Dr[i].Distanta;
          Poz.Senzor = i;
        }
      }
      if (strcmp(Poz.Directie, "St") == 0)
      {
        if (St[i].Distanta > 2 && St[i].Distanta < 400 && abs(St[i].Distanta - St[i].DistantaAnt) > 50)
        {
          stepper.step((-342) * i - Poz.Senzor);
          St[i].DistantaAnt = St[i].Distanta;
          Poz.Senzor = i;
        }
        else
        if (Dr[i].Distanta > 2 && Dr[i].Distanta < 400 && abs(Dr[i].Distanta - Dr[i].DistantaAnt) > 50)
        {
          stepper.step(342 * i + Poz.Senzor - 1);
          Dr[i].DistantaAnt = Dr[i].Distanta;
          strcpy(Poz.Directie, "Dr");
          Poz.Senzor = i;
        }
      }
    }
  }
}
