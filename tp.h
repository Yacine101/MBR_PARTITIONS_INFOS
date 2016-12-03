#ifndef TP_H_INCLUDED
#define TP_H_INCLUDED



typedef struct part {

     unsigned char status;
     unsigned char debut_chs[3];
     unsigned char type;
     unsigned char fin_chs[3];
     unsigned char premier_secteur_lba[4];
     unsigned char nbr_secteurs[4];

} Partition ;


typedef struct file
{
    long long  donnee;
    struct pile *suivant;
} File;

long long  nombre_bytes;
long long taille_secteur;
long long  choix2;


File * f;


void Lire_Secteur(unsigned char secteur[512],long long numero_secteur);
void Afficher_secteur(long long  numero_secteur);
long long  recherche_secteur1(unsigned char secteur[512],char chaine_info[2048],long long  secteur_debut,long long  secteur_fin);
long long  recherche_secteur2(char chaine_info[2048],long long  secteur_debut,long long  width);


void extraire_table_partition(Partition table[4] , long long s);
long long ascii_vers_entier(char *ascii);
long long est_principal(Partition table[4],long long i);
long long est_etendu(Partition table[4],long long i);
long long est_ntfs(Partition table[4],long long j);
long long est_fat32(Partition table[4],long long j);
long long est_boot_fat32();
long long est_boot_ntfs();
void lire_type_partition(char type[30],Partition table[4],long long j);
float octet_vers_mo_go(long long octet,char uniter[3]);
long long extraire_secteur_par_cluster(long long j);
long long extraire_mft(long long j);
void lire_taille_disque(long long s);
void extraire_infos_fat32(long long i);
void afficher_partition(char separateur[100] ,long long  s);



#endif // TP_H_INCLUDED
