#ifndef HEADERS_H
#define HEADERS_H
#include <gtk/gtk.h>
#define MAX_LINE_LENGTH 256
#define TEMP_FILE "temp_signup.txt"
typedef struct {
    char nom[50];
    char prenom[50];
    int cin;
    char ville[50];
    char modele[50];
    char matricule[50];
    int age;
    char sexe[10];
} citoyen;
typedef struct {
    int idCitoyen ;
  char nomParking[100] ;
  char typeService[100] ;
  char weekendPrix[100] ;
  int mois ;
}gestion ;


typedef struct {
    int idParking;
    int priceParking;
    char nameParking[50];
} Parking;

typedef struct {
    int idService;
    int priceService;
    char nameService[50];
} Service;

typedef struct {
    int idCitoyen;
    char date[11];
    char hour[6];
    int idParking;
    int idService;
} Reservation; 

typedef struct{
   char nom[100];
   char prenom[100]; 
   char email[300]; 
   char mdp[50];
   char type[100];
} Users;


int ajouter( char *citoyenFichier, citoyen c,char *errorMessage);
int modifier(char *citoyenFichier, int cin, citoyen nouv,char *errorMessage);
int chercherMois(char *servicesFichier, char *findFichierMois ,int mois);
int chercherNomParcking(char *servicesFichier, char *findFichierNom ,char nomParcking[100]);
int supprimer(char *citoyenFichier, int cin);
int affichier(GtkWidget *liste);

int loadParkingData(char *filename, Parking parkings[], int maxCount);
int loadServiceData(char *filename, Service services[], int maxCount);
void generateFacture(const char *resFile, const char *srvFile, const char *prkFile, const char *factureFile, int targetIdCitoyen,char *totaleMessage);
void searchByMonth(const char *factureFile, int month,char *totaleMessage);
int is_valid_email(char *email);
int password_strength(char *password);
int signup( char *email,char *password,char *nom,char *prenom);
int signin( char *email,char *password,char *userType);
int emailExist(char *email,char *errorMessage);
void viderTreeView(GtkWidget *treeview);
void populate_treeview_with_file2(GtkWidget *treeview, const char *filename);
size_t payload_source(void *ptr, size_t size, size_t nmemb, void *userp);
void send_email(const char *recipient, const char *subject, const char *body);
void changePassword(const char *email, const char *newPassword);

/////////////////////////////////////////////////////////////////////////////itaf
// Structure pour une date (année, mois, jour)
typedef struct {
    int annee;
    int mois;
    int jour;
} date;

// Structure pour une réservation
typedef struct {
    int cin;        // Numéro CIN de la personne
    int place;      // Numéro de la place de parking réservée
    date res_date;  // Date de la réservation (format YYYY-MM-DD)
    int duree;      // Durée de la réservation en heures
} reservation;

// Fonction pour trouver une place disponible
int trouver_place_libre(int *places, int taille);

// Fonction pour ajouter une nouvelle réservation
int ajouter(const char *filename, reservation *res, int *places, int taille);

// Fonction pour afficher les réservations d'une personne par CIN
int afficher(const char *filename, int cin, char *output);

// Fonction pour rechercher une réservation par numéro de place
void rechercher_par_place(const char *filename, int place, char *output);

// Fonction pour modifier une réservation existante
int modifier(const char *filename, int cin, int place, reservation *res);

// Fonction pour supprimer une réservation
int supprimer(const char *filename, int cin, int place);
///////////////////////
void add_row_reservation(GtkListStore *list_store, int cin, int place, date res_date, int duree) ;
void populate_treeview_with_reservations(GtkWidget *treeview, const char *filename);
//////////////// 
int compare_reservations(const void *a, const void *b);
void sort_and_write_reservations(const char *input_filename, const char *output_filename); 
//////////////////////////////////////////////////////////////////////////wael
typedef struct {
    int id;
    char nom[50];
    char description[100];
    float tarif;
} Service;

// Définir la structure Reservation
typedef struct {
    int id_reservation;    // ID de la réservation
    int id_service;        // ID du service réservé
    char date[20];         // Date de la réservation (par exemple "YYYY-MM-DD")
    char client[100];      // Nom du client
} Reservation;

// Prototypes des fonctions de gestion des services
void ajouterService(Service service);
void modifierService(int id, Service service);
void supprimerService(int id);
void afficherServices();
void AffecterUnService(int id_reservation, int id_service, const char *date, const char *client);
////////////////////////////////////////////////////////////////////ayoub
typedef struct
{
    int jour;
    int mois;
    int annee;
} date;

typedef struct
{
    char nom[20];
    int id;
    int place;
    int durree;
    char statut[10];
    date date_reservation;
}reservation;

typedef struct
{
    char nom [20];
    char prenom [20];
    int CIN ;
    date date_naissance;
    int sexe;
    char ville[20];
    int numTEL;
    char email[20];
    date date_recrutement;
    int horraires_de_travail;
    int poste_de_travail;
}agent;

enum {
    NOM,
    PRENOM,
    CIN,
    DN,
    VILLE,
    NUMTEL,
    EMAIL,
    DR,
    SEXE,
    WH,
    WP,
    COLUMNS
};

enum {
    NNOM,
    ID,
    PLACE,
    DUREE,
    STATUE,
    DRES,
    CCOLUMNS
};

void verif_CIN(agent *G);
void verif_numTEL(agent *G);
int verif_sexe(agent *G);
int verif_horraires(agent *G);
int verif_poste(agent *G);
void saisir_agent(agent *G);
int ajouter_agent(char * file_agent , agent G);
int modifier(char *file_agent, int CIN ,agent nouvG);
int supprimer(char *file_agent, int CIN);
int chercher(char *file_agent , int CIN, agent *pG);
int afficher_cherche_agent(char *file_agent, int CIN);
int chercher_reservation(char *file_reservation, int jour,int mois,int annee, reservation *pR);
int afficher_reservation(char *file_reservation,int jour,int mois,int annee);
void afficher_agent(GtkWidget *liste);
void afficher_liste_reservation(GtkWidget *liste);



#endif



