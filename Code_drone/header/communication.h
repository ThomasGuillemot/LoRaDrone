#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include <pthread.h>
/* Librairie de communication série sur Raspberry */
#include <wiringPi.h>
#include <wiringSerial.h>

/* Flux de données permettant, avec les modules Lora, la communication série */
#define FLUX "/dev/ttyAMA0"

/* file descriptor permettant de stocker le flux de communication */
static volatile int fd;
/* Variable booléenne servant d'indice d'intégrité */
static volatile unsigned short int validation = 0;
/* variable globale contenant le message envoyer par la télécommande */
unsigned char *msg_recu = "";
unsigned short int coordonnee[6] = {0}; // XA, YA, BA, XB, YB, BB

extern void tache(void);

#endif
