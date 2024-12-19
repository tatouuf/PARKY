typedef struct {
    char id[50];
    char localisation[50];
    char horaire[50];
    int places;
    int prix;
    char type1[20];
    char type2[20];
    char type3[20];
    char couverture[20];
    char id_agent[50]; // Nouvelle colonne pour l'ID de l'agent
} Parking;



typedef struct {
    char cin[20];
    char first_name[50];
    char last_name[50];
    int age;
    char sexe[10];
    int affecte;  // 0 = not assigned, 1 = assigned
} Agent;
int est_id_valide(const char *str);
int verifier_id_existant(const char *id);
void afficher_parkings(GtkTreeView *treeview, const gchar *parking_id);
int supprimer_parking_par_id(const char *fichier, const char *id);
int rechercher_parking_par_id(const gchar *id, char *localisation, char *horaire, int *places, int *prix, char *type_parking, char *couverture);
int verifier_parking(const char *id_parking);
int verifier_agent(const char *cin_agent);
void affecter_agent(const char *id_parking, const char *cin_agent);
