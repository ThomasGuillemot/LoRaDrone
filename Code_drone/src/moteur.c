#include "../header/moteur.h"

/* Pin sur lequel on branche le moteur */
#define PIN 4

extern void main(void){
  int position = 0, angle = 0;
  pinMode(PIN, PWM_OUTPUT);
  pwmSetMode(PWM_MOD_MS);
  pwmSetClock(192); /* Soit 10.24 ms */
  
  printf("Entrer une valeur entre 10 et 360 : ");
  scanf("%d", angle);
  
  position = angle + 60;
  pwmWrite(PIN, position);
  exit(0);
}