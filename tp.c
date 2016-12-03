#include <stdio.h>
#include <stdlib.h>
#include "tp.h"
#include <sys/stat.h>
#define _LARGEFILE_SOURCE
#define _LARGEFILE64_SOURCE
#define _FILE_OFFSET_BITS 64
#define SYMBOLISM "/dev/sda"

void Lire_Secteur(unsigned char secteur[512],long long numero_secteur)
{
    FILE *f = NULL;
    char symbolism[25];
    long long position ;
    taille_secteur = 512LL;
    position=(long long)(taille_secteur*numero_secteur);

    //printf("Enter Partition to read , ex: /dev/sda if building from linux or \\\\.\\PhysicalDrive0 if from Windows\n");
    //scanf("%s",symbolism);

    f=fopen(SYMBOLISM,"rb");
    fflush(f);
    fseeko64(f,(long long)position,SEEK_SET);//se positionner dans le secteur à lire
    fread(secteur,512,1,f); //lire le secteur dans le buffer
    fclose(f);

}

void Afficher_secteur(long long numero_secteur)
{
    long long i=0,d=0,k=0;
    long long j = 0;
    unsigned char secteur[512];

    taille_secteur=512;

    Lire_Secteur(secteur,numero_secteur);
    printf("\n");
    while(i<taille_secteur)//pour afficher 32 ligne de 16 octets chacune
        {
            printf("%2.4d ",j*2);
            for(d = 0 ; d < 8 ; d++)
            {
            printf("%c ",secteur[i]);//affichage en HEXA des informations contenu dans le secteur
            j++;
            i++;
            }
            printf("\n");

        }

}



long long recherche_secteur1(unsigned char secteur[512],char chaine_info[2048],long long secteur_debut,long long secteur_fin){

    long long i ,j ;
    long long da;
    char secteur2[2048]; // Va conenir le secteur dans le parcours .
    unsigned char secteur1[512];


    da=0;

    for(i=secteur_debut;i<secteur_fin;i++){ // Parcour du secteur debut jusqu'a secteur fin

        Lire_Secteur(secteur1,i);    // Lecteur du i eme secteur , l'info est stocké dans secteur[]

        j=0;
         while(j<taille_secteur)
        {
            if(choix2==1){ // Si la recherche en Hexa
            sprintf(&secteur2[j*2],"%2.2X",secteur1[j]);  // Remplir le tableau avec des Charachtéres Hexa
            printf("%2.2X m ",secteur1[j]);
            }
            else{    // Si la recherche en ASCII
            sprintf(&secteur2[j],"%c",secteur1[j]);  // Remplir le tableau avec des Charachtéres Ascii
            printf("%2.2X m ",secteur1[j]);
            }
            j++;
        }
        if(strstr(secteur2,chaine_info)!=NULL){ // Si la chaine est dans le tableau

        da=1;
        }
    }

    return da;
}



long long recherche_secteur2(char chaine_info[2048],long long secteur_debut,long long width){

    long long i ,j ;
    long long trouv;

    char secteur2[2048]; // Va conenir le secteur dans le parcours .
    unsigned char secteur[512];


    trouv=0;

    for(i=secteur_debut;i<(secteur_debut+width);i++){ // Parcour du secteur debut jusqu'a secteur fin

        Lire_Secteur(secteur,i);    // Lecteur du i eme secteur , l'info est stocké dans secteur[]

        j=0;
         while(j<taille_secteur)
        {
            if(choix2==1){ // Si la recherche en Hexa
            sprintf(&secteur2[j*2],"%2.2X",secteur[j]);  // Remplir le tableau avec des Charachtéres Hexa
            }
            else{    // Si la recherche en ASCII
            sprintf(&secteur2[j],"%c",secteur[j]);  // Remplir le tableau avec des Charachtéres Ascii
            }
            j++;
        }
        if(strstr(secteur2,chaine_info)!=NULL){ // Si la chaine est dans le tableau
        trouv=1;  // Ajouter le numero de secteur a la file d'entiers

        }
    }

    return trouv;
}

void extraire_table_partition(Partition table[4] , long long s){ // Mettre les 5 Descripteursdu MBR dans le tableau de structure .

    long long j;

    unsigned char secteur[512];

    Lire_Secteur(secteur,s); // Lire le boot secteur s .

    for(j=0;j<4;j++){  // Remplir les champs de la structure .


        memcpy(&table[j].status,&secteur[446+j*16],1);
        memcpy(&table[j].debut_chs,&secteur[447+j*16],3);
        memcpy(&table[j].type,&secteur[450+j*16],1);
        memcpy(&table[j].fin_chs,&secteur[451+j*16],3);
        memcpy(&table[j].premier_secteur_lba,&secteur[454+j*16],4);
        memcpy(&table[j].nbr_secteurs,&secteur[458+j*16],4);

    }




}




long long ascii_vers_entier1(char *ascii){ // Transfert une chaine ascii en Hexa vers un entier .

    char ascii2[20];
    char ascii3[9]="";
    sprintf(ascii2,"%02x%02x%02x%02x",ascii[3],ascii[2],ascii[1],ascii[0]);
    memcpy(&ascii3[0],&ascii2[0],8);

    return strtoll(ascii3,NULL,16);

}

long long est_principal(Partition table[4],long long i){ // Renvoie 1 si la partition est principal .

    char type[5];


    sprintf(type,"%2.2X",table[i].type);


    if(strcmp(type,"05")!=0 && strcmp(type,"0F")!=0){ // Compare avec 07h
        return 1;
    }else{
        return 0;
    }

}

long long est_etendu(Partition table[4],long long i){ // Retourne vrai si la partition est etendue

    char type[5]="";

    sprintf(type,"%2.2X",table[i].type);



    if(strcmp(type,"05")==0 || strcmp(type,"0F")==0){
        return 1;
    }else{
        return 0;
    }


}

long long est_ntfs(Partition table[4],long long j){ // Retourne vrai si la partition est NTFS

    long long num_secteur;
    long long i ;

    char recherche[2048]="NTFS";
    char recherchehex[2048];
    long long w;

    choix2=1; // Pour la recherche en hexa


    num_secteur=ascii_vers_entier1(table[j].premier_secteur_lba);


    for(w=0;w<4;w++){  // Conversion de FAT32 en hexa .

    sprintf(&recherchehex[w*2],"%2.2X",recherche[w]); // Utilise la fonction de recherche en HEXA

    }

    if(recherche_secteur2(recherchehex,num_secteur,1)==1){
    return 1;
    }
    else{
        return 0;
    }



}

long long est_fat32(Partition table[4],long long j){ // retourne vrai si la partition est FAT32

    long long num_secteur;
    long long i ;

    char recherche[2048]="FAT32";
    char recherchehex[2048];
    long long w;

    choix2=1; // Pour la recherche en hexa


    num_secteur=ascii_vers_entier1(table[j].premier_secteur_lba);


    for(w=0;w<5;w++){  // Conversion de FAT32 en hexa .

    sprintf(&recherchehex[w*2],"%2.2X",recherche[w]);

    }

    if(recherche_secteur2(recherchehex,num_secteur,1)==1){
    return 1;
    }
    else{
        return 0;
    }


}


long long est_boot_fat32(){ // pour traiter les disque Flash formater FAT32


    char recherche[2048]="FAT32";
    char recherchehex[2048];
    long long w;

    choix2=1; // Pour la recherche en hexa


    for(w=0;w<5;w++){  // Conversion de FAT32 en hexa .

    sprintf(&recherchehex[w*2],"%2.2X",recherche[w]);

    }

    if(recherche_secteur2(recherchehex,0,1)==1){
    return 1;
    }
    else{
        return 0;
    }

}




void lire_type_partition(char type[30],Partition table[4],long long j){ // Afficher le type de partition correspendant .

        char sgfpart[2];

        sprintf(&sgfpart[0],"%2.2X",table[j].type);



            if(strcmp(sgfpart,"82")==0){

                strcpy(type,"Espace Swap Linux / Solaris / Prime");

            }
            else if(strcmp(sgfpart,"83")==0){

                strcpy(type,"System de fichier linux native ");

            }
            else if(strcmp(sgfpart,"45")==0){

                strcpy(type,"Priam / Boot-US /EUMEL/ELAN ");

            }
            else if(strcmp(sgfpart,"65")==0){

                strcpy(type,"System de fichier NetWare");

            }
            else if(strcmp(sgfpart,"20")==0){

                strcpy(type,"XIP/OSF");

            }
            else if(strcmp(sgfpart,"0D")==0){

                strcpy(type,"Partition reserver");

            }
            else{

                strcpy(type,sgfpart);
            }


}

float octet_vers_mo_go(long long octet,char uniter[3]){ // Convertir un Miga en Octet

        float mo_go;

        if((octet)>=(1024*1024*2)){

            mo_go=(float)(octet*2.0/(1024*1024*4.0));
            strcpy(uniter,"Go");

        }
        else {


            mo_go=(float)(octet*512.0/(1024*1024.0));
            strcpy(uniter,"Mo");

        }

        return mo_go;
}

long long extraire_secteur_par_cluster(long long j){ // Extraire le nombre de secteur par cluster

    long long taille,num_secteur,i;
    char type[30];
    char taille_cluster[2];
    unsigned char secteur[512];

    Lire_Secteur(secteur,j);


    taille=secteur[13];

    return taille;

}

long long extraire_mft(long long j){ // Extraire le cluster de la MFT

    long long taille,num_secteur;
    char taille_cluster[2];
    unsigned char secteur[512];
    char adresse[9];

    long long i;

    Lire_Secteur(secteur,j);

    taille=0;

    sprintf(adresse,"%02x%02x%02x%02x%02x%02x%02x%02x",secteur[55],secteur[54],secteur[53],secteur[52],secteur[51],secteur[50],secteur[49],secteur[48]);

    return  strtoll(adresse,NULL,16);

}

void lire_taille_disque(long long s){ // Permet de cumuler les tailles des partitions pour savoir la taille totale

    long long total;
    long long i;
    Partition tableau_partitions[4];
    char uniter[3];

    total=0;
    if(est_boot_fat32(s)==0){

    extraire_table_partition(tableau_partitions,s);

    for(i=0;i<4;i++){
        total+=ascii_vers_entier1(tableau_partitions[i].nbr_secteurs);
    }


    printf("--------------------------------------------------------------------------------");
    printf("\n La taille du disque est de:");


    printf(" %.2f  %s   \n",octet_vers_mo_go(total,uniter),uniter);

    }

}


void extraire_infos_fat32(long long i){ // Affiche les infos du FAT32 pour le flash et la partition

    unsigned char secteur[512];
    char taille[4]="";
    unsigned char secteur2[1024];
    char nom[11];
    char adrFAT[2];
    char cluster;
    long long j ;

    Lire_Secteur(secteur,i);


    for(j=0;j<512;j++){

    sprintf(&secteur2[j],"%c",secteur[j]); // Remplir le secteur 2 par le Secteur 0 .

    }


    memcpy(&adrFAT[0],&secteur2[14],2);

    printf("Adresse de la FAT est : %2.2x%2.2x \n",adrFAT[1],adrFAT[0]);


    memcpy(&nom[0],&secteur2[71],11);

    printf("Le nom logique est :%s \n",nom);

}
void afficher_partition(char separateur[100] ,long long  s){ // Afficher les partitions

    long long i,j,a,b;
    Partition tableau_partitions3[4];

    unsigned char secteur1[512]="";
    unsigned char secteur2[512]="";
    char uniter[3];
    char adresse[9];
    char type[30];
    taille_secteur=512;

    extraire_table_partition(tableau_partitions3,s);

    Lire_Secteur(secteur1,0);


    printf("\n\n");


    if(est_boot_fat32()==1){ // Si c'est un flash disque alors imprimer les infos

            printf("Formatage disque : FAT32 \n");
            printf("Type             : Principal \n");
            printf("Formatage disque : FAT32 \n");
            printf("Secteur par Cluster : %d \n",extraire_secteur_par_cluster(0));
            extraire_infos_fat32(0);


    }
    else{ // Sinon c'est un MBR

     for(i=0;i<4;i++){ // Pour chaque partition on dois afficher ces infos

        a = ascii_vers_entier1(tableau_partitions3[i].premier_secteur_lba);
        Lire_Secteur(secteur2,a);




        printf("%s Partition %d ",separateur,i+1);printf("-----------------\n");

        if(est_principal(tableau_partitions3,i)==1){ // Si c'est pas etenu alors on l'affiche directement

            printf("Type: Primaire \n");

                    if(est_ntfs(tableau_partitions3,i)==1){

                        printf("SGF: NTFS \n");
                        printf("Taille: %.2f  %s",octet_vers_mo_go(ascii_vers_entier1(tableau_partitions3[i].nbr_secteurs),uniter),uniter);
                        printf("\n");
                        printf("Adresse debut partition: %I64u \n",ascii_vers_entier1(tableau_partitions3[i].premier_secteur_lba));
                        printf("Secteurs par Clusters: %d \n",extraire_secteur_par_cluster(a));
                        printf("Taille du cluster: %d \n",extraire_secteur_par_cluster(a)*taille_secteur);

                        printf("Premier cluster de la MFT: %d \n",extraire_mft(a));
                        printf("Premier secteur de la MFT: %d \n",extraire_mft(a)*extraire_secteur_par_cluster(a));



                    }
                    else if(est_fat32(tableau_partitions3,i)==1){

                        printf("SGF:  FAT32 \n");
                        printf("Taille: %.2f  %s",octet_vers_mo_go(ascii_vers_entier1(tableau_partitions3[i].nbr_secteurs),uniter),uniter);
                        printf("\n");
                        printf("Adresse debut partition: %I64u \n",ascii_vers_entier1(tableau_partitions3[i].premier_secteur_lba));
                        printf("Secteurs par Clusters: %d \n",extraire_secteur_par_cluster(a));
                        printf("Taille du cluster: %d \n",extraire_secteur_par_cluster(a)*taille_secteur);
                        printf("Premier cluster de la MFT: %d",extraire_mft(a)*extraire_secteur_par_cluster(a));
                        extraire_infos_fat32(a);

                    }
                    else{
                        lire_type_partition(type,tableau_partitions3,i);
                        printf("SGF: %s \n",type);
                        printf("Taille: %.2f  %s",octet_vers_mo_go(ascii_vers_entier1(tableau_partitions3[i].nbr_secteurs),uniter),uniter);
                        printf("\n");
                        printf("Adresse debut partition: %I64u \n",ascii_vers_entier1(tableau_partitions3[i].premier_secteur_lba));
                        printf("Secteurs par Clusters : / \n");
                        printf("Taille du cluster : /  \n");
                        printf("Adresse de la MFT: %d",extraire_mft(a)*extraire_secteur_par_cluster(a));
                        }

        }
        else if(est_etendu(tableau_partitions3,i)==1 && strcmp(secteur1,secteur2)!=0 ){ // Sinon on entre de maniére recurcive

            printf("Type: Etendue\n");
            printf("Taille: %.2f  %s",octet_vers_mo_go(ascii_vers_entier1(tableau_partitions3[i].nbr_secteurs),uniter),uniter);
            printf("\n");
            printf("Adresse debut partition: %I64u \n",ascii_vers_entier1(tableau_partitions3[i].premier_secteur_lba));
            printf("Secteurs par Clusters: %d \n",extraire_secteur_par_cluster(a));
            printf("Taille du cluster: %d \n",extraire_secteur_par_cluster(a)*taille_secteur);
            printf("Premier cluster de la MFT: %d \n",extraire_mft(a));
            printf("Premier secteur de la MFT: %d \n",extraire_mft(a)*extraire_secteur_par_cluster(a));
            afficher_partition("-----------------------",a); // On entre recursivement et on traite 'a' comme un MBR (EBR)

        }
        else if(est_etendu(tableau_partitions3,i)==1 && strcmp(secteur1,secteur2)==0){ // Pour gerer le cas ou on lis un disque tres large

            printf("Type: Inconnue \n");
             printf("Taille: %.2f  %s \n",octet_vers_mo_go(ascii_vers_entier1(tableau_partitions3[i].nbr_secteurs),uniter),uniter);
            printf("Aucune autre informations ne peut etre afficher\n");

        }
        else{ // Si c'est un autre type de partition

                    printf("Taille: %.2f  %s \n",octet_vers_mo_go(ascii_vers_entier1(tableau_partitions3[i].nbr_secteurs),uniter),uniter);
                     lire_type_partition(type,tableau_partitions3,i);
                    printf("Type: %s \n",type);

        }
        printf("\n\n");

        }//End for
    }

    printf("\n\n");
}


