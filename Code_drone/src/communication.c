#include "../header/communication.h"
#include "../header/controle.h"

#define TAILLE 32

/* Vérifie l'ouverture du flux de communication série ttyAMA0 */
static void connexion(void){
    /* Dispositif d'entrée et nombre de caractères par seconde */
    fd = serialOpen(FLUX, 9600);
    /* Problème d'ouverture série du flux de connexion */
    if(fd < 0){
        puts("Erreur communication");
        exit(1);
    /* Erreur de déploiement de certaines fonctionnalité de la librairie wiringPi */
    }else if(wiringPiSetup() == -1){
        puts("Erreur de librairie");
        exit(2);
    }
    return fd;
}

/* Fonction permettant de lire le flux de données envoyé par la télécommande */
static void *lecture(void * flux){
    /* Variable de récupération des caractères servant de tampon */
    unsigned char buffer[TAILLE];
    /* Message reçu par le drone */
    msg_recu = malloc(sizeof(buffer));
    static volatile unsigned short int i = 0;
    while(1){
        /* Si le flux de données est lisible */
        if(serialDataAvail(fd)){
            /* Renvoie un caractère correspondant au code ascii entier */
            buffer[i] = serialGetchar(fd);
            /* S'il y a fin de transmission ou dépassement de la taille du message */
            if((buffer[i] == '\4') || (i > TAILLE+1)){
                /* Réupèration du message en copiant le buffer dans la variable du message recu */
                memcpy(msg_recu, buffer, sizeof(buffer));
                printf("%s\n", msg_recu);
                /* Fin de la chaine de caractères */
                for(i = 0; i < 31; i++){ buffer[i] = '\0'; }
                i = 0; /* Réinitialisation du buffer */
            /* Stockage des caractères dans le buffer */
            }else{ i++; }
        }
    }
}

/* Fonction permettant d'écrire dans le flux de données à la télécommande */
static void *ecriture(void * flux) {
  while(!validation){
      serialPrintf(fd, "LINK\4");
      /* Si la télécommande est appairée au drone */
      if(!strcmp(msg_recu, "PAIR\4")){ validation = 1; }
      sleep(5);
   }
}

/* Permet de déterminer toutes les actions à effectuer,
permettant de terminer la communciations drone-télécommande */
static void sortie(void){
    free(msg_recu);
    serialClose(fd);
    pthread_exit(NULL);
    exit(0);
}

/* Listing de tous les processus à créer et lancer en multitâche */
extern void tache(void){
    connexion();
    static pthread_t th_com[2];
    /* Ecriture et lecture synchronisés */
    pthread_create(&th_com[0], NULL, lecture, (void *)&fd);
    pthread_create(&th_com[1], NULL, ecriture, (void *)&fd);
    /* Lancement de toutes les tâches */
    for(volatile unsigned short int i = 0; i < 2; i++)
        pthread_join(th_com[i], NULL);
    sortie();
}
