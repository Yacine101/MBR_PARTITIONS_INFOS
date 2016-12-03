#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tp.h"
#include "tp.c"

int main()
{
    printf("---------------------------------------------------------------------------\n");
    printf("                  Printing hard disk Characteristics\r\n");
    printf("---------------------------------------------------------------------------\n");
    printf("                  By : OUAREZKI Yacine and ZENIOU SOUFIANE\n");
    printf("---------------------------------------------------------------------------\n");
    printf("****************************************************************************\n\n");
    printf("\nAppuyer sur [ENTRER] pour commencer : \n");
    getchar();
    // Lecteur du premier secteur
    // Verification que c'est un secteur MBR .

    int k,i ;

    char chaine[2048]="AA55";
    Partition tableau_partitions1[4];
    Partition tableau_partitions2[4];

     // On extrait la table de partition du premier secteur du MBR .

    lire_taille_disque(0);
    afficher_partition("---",0);printf("------------------------------------------");


    printf("\n appuyer sur 0 pour quitter.");
    scanf("%d",&k);

    return 0;
}






