#include <stdio.h>
#include <gtk/gtk.h>
#include <string.h>
#include <fct.h>

// Fonction pour vérifier si une chaîne contient uniquement des chiffres
int est_id_valide(const char *str) {
    if (str == NULL || *str == '\0') {
        return 0; // Faux si la chaîne est vide
    }
    while (*str) {
        if (*str < '0' || *str > '9') { // Vérifie que chaque caractère est un chiffre
            return 0;
        }
        str++;
    }
    return 1; // Vrai si tous les caractères sont des chiffres
}

// Fonction pour vérifier si un ID existe déjà dans le fichier "parc.txt"
int verifier_id_existant(const char *id) {
    FILE *file = fopen("parc.txt", "r");
    if (file == NULL) {
        return 0; // Considérer comme inexistant si le fichier n'existe pas
    }

    char ligne[256];
    while (fgets(ligne, sizeof(ligne), file)) {
        char id_existant[50];
        sscanf(ligne, "%[^;]", id_existant); // Lire l'ID jusqu'au premier point-virgule
        if (strcmp(id, id_existant) == 0) {
            fclose(file);
            return 1; // L'ID existe déjà
        }
    }

    fclose(file);
    return 0; // L'ID n'existe pas
}


// Fonction d'affichage des parkings dans le TreeView
void afficher_parkings(GtkTreeView *treeview, const gchar *parking_id) {
    GtkListStore *store;
    GtkTreeIter iter;

    // Définir les colonnes du TreeView si ce n'est pas déjà fait
    if (gtk_tree_view_get_model(treeview) == NULL) {
        GtkCellRenderer *renderer;

        renderer = gtk_cell_renderer_text_new();
        gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(treeview), -1, "ID", renderer, "text", 0, NULL);

        renderer = gtk_cell_renderer_text_new();
        gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(treeview), -1, "Localisation", renderer, "text", 1, NULL);

        renderer = gtk_cell_renderer_text_new();
        gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(treeview), -1, "Horaire", renderer, "text", 2, NULL);

        renderer = gtk_cell_renderer_text_new();
        gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(treeview), -1, "Places", renderer, "text", 3, NULL);

        renderer = gtk_cell_renderer_text_new();
        gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(treeview), -1, "Prix", renderer, "text", 4, NULL);

        renderer = gtk_cell_renderer_text_new();
        gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(treeview), -1, "Type", renderer, "text", 5, NULL);

        renderer = gtk_cell_renderer_text_new();
        gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(treeview), -1, "Couverture", renderer, "text", 6, NULL);

        renderer = gtk_cell_renderer_text_new();
        gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(treeview), -1, "ID Agent", renderer, "text", 7, NULL);
    }

    // Créer un nouveau modèle de données
    store = gtk_list_store_new(8, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_INT, G_TYPE_INT, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING);

    // Charger les données depuis le fichier parc.txt
    FILE *file = fopen("parc.txt", "r");
    if (file == NULL) {
        g_print("Erreur : Impossible d'ouvrir le fichier parc.txt.\n");
        return;
    }

    char ligne[256];
    Parking p;
    while (fgets(ligne, sizeof(ligne), file)) {
        sscanf(ligne, "%49[^;];%49[^;];%49[^;];%d;%d;%19[^;];%19[^;];%49[^\n]",
               p.id, p.localisation, p.horaire, &p.places, &p.prix, p.type1, p.couverture, p.id_agent);

        // Appliquer le filtre sur l'ID si nécessaire
        if (parking_id == NULL || g_strcmp0(parking_id, p.id) == 0) {
            gtk_list_store_append(store, &iter);
            gtk_list_store_set(store, &iter,
                               0, p.id,
                               1, p.localisation,
                               2, p.horaire,
                               3, p.places,
                               4, p.prix,
                               5, p.type1,
                               6, p.couverture,
                               7, p.id_agent, // Nouvelle colonne
                               -1);
        }
    }

    fclose(file);

    // Associer le modèle au TreeView
    gtk_tree_view_set_model(GTK_TREE_VIEW(treeview), GTK_TREE_MODEL(store));

    // Libérer la référence au modèle
    g_object_unref(store);
}


int supprimer_parking_par_id(const char *fichier, const char *id) {
    FILE *file = fopen(fichier, "r");
    FILE *temp = fopen("temp_parc.txt", "w");
    if (!file || !temp) {
        g_print("Erreur : Impossible d'ouvrir les fichiers pour suppression.\n");
        return 0;
    }

    char ligne[256];
    int id_found = 0;

    // Lire le fichier ligne par ligne
    while (fgets(ligne, sizeof(ligne), file)) {
        char id_existant[50];
        sscanf(ligne, "%[^;]", id_existant); // Extraire l'ID de chaque ligne

        // Si l'ID ne correspond pas, écrire la ligne dans le fichier temporaire
        if (strcmp(id, id_existant) != 0) {
            fputs(ligne, temp);
        } else {
            id_found = 1; // ID trouvé
        }
    }

    fclose(file);
    fclose(temp);

    // Remplacer l'ancien fichier par le fichier temporaire si l'ID a été trouvé
    if (id_found) {
        remove(fichier);
        rename("temp_parc.txt", fichier);
        return 1; // Suppression réussie
    } else {
        remove("temp_parc.txt");
        return 0; // ID non trouvé
    }
}

// Fonction pour récupérer les informations d'un parking par ID
int rechercher_parking_par_id(const gchar *id, char *localisation, char *horaire, int *places, int *prix, char *type_parking, char *couverture) {
    FILE *file = fopen("parc.txt", "r");
    if (file == NULL) {
        g_print("Erreur : Impossible d'ouvrir le fichier parc.txt.\n");
        return 0;
    }

    char ligne[256];
    while (fgets(ligne, sizeof(ligne), file)) {
        char id_lu[50];

        // Extraire les champs de la ligne
        sscanf(ligne, "%49[^;]; %49[^;]; %49[^;]; %d; %d; %49[^;]; %49[^\n]",
               id_lu, localisation, horaire, places, prix, type_parking, couverture);

        // Comparer l'ID lu avec l'ID recherché
        if (strcmp(id_lu, id) == 0) {
            fclose(file);
            return 1; // ID trouvé
        }
    }

    fclose(file);
    return 0; // ID non trouvé
}


// Fonction pour trouver l'index correspondant à une valeur dans une GtkComboBox
gint trouver_index_combobox(GtkComboBox *combo, const gchar *texte) {
    GtkTreeModel *model = gtk_combo_box_get_model(combo);
    GtkTreeIter iter;
    gint index = 0;
    gchar *current_text;

    if (gtk_tree_model_get_iter_first(model, &iter)) {
        do {
            gtk_tree_model_get(model, &iter, 0, &current_text, -1);
            if (g_strcmp0(current_text, texte) == 0) {
                g_free(current_text);
                return index;
            }
            g_free(current_text);
            index++;
        } while (gtk_tree_model_iter_next(model, &iter));
    }
    return -1; // Non trouvé
}



//////////////
// Function to verify if parking exists and is not assigned
int verifier_parking(const char *id_parking) {
    FILE *file = fopen("parc.txt", "r");
    if (!file) return 0;

    char ligne[256];
    Parking p;
    while (fgets(ligne, sizeof(ligne), file)) {
        sscanf(ligne, "%49[^;];%49[^;];%49[^;];%d;%d;%19[^;];%19[^;];%49[^\n]",
               p.id, p.localisation, p.horaire, &p.places, &p.prix, p.type1, p.couverture, p.id_agent);


        if (strcmp(p.id, id_parking) == 0 && strcmp(p.id_agent, "non_affecter") == 0) {
            fclose(file);
            return 1; // Parking found and not assigned
        }
    }
    fclose(file);
    return 0;
}

// Function to verify if an agent exists and is not assigned
int verifier_agent(const char *cin_agent) {
    FILE *file = fopen("agents.txt", "r");
    if (!file) return 0;

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        Agent agent;
        sscanf(line, "%19[^;];%49[^;];%49[^;];%d;%9[^;];%d",
               agent.cin, agent.first_name, agent.last_name, &agent.age, agent.sexe, &agent.affecte);

        if (strcmp(agent.cin, cin_agent) == 0 && agent.affecte == 0) {
            fclose(file);
            return 1; // Agent found and not assigned
        }
    }
    fclose(file);
    return 0;
}

// Function to assign an agent to a parking
void affecter_agent(const char *id_parking, const char *cin_agent) {
    // Update in parc.txt
    FILE *file = fopen("parc.txt", "r+");
    FILE *temp = fopen("temp_parc.txt", "w");
    char line[256];
    Parking p;
     while (fgets(line, sizeof(line), file)) {
        sscanf(line, "%49[^;];%49[^;];%49[^;];%d;%d;%19[^;];%19[^;];%49[^\n]",
               p.id, p.localisation, p.horaire, &p.places, &p.prix, p.type1, p.couverture, p.id_agent);
        if (strcmp(p.id, id_parking) == 0) {
            fprintf(temp, "%s;%s;%s;%d;%d;%s;%s;%s\n",
               p.id, p.localisation, p.horaire, p.places, p.prix, p.type1, p.couverture, cin_agent);  // Update agent
        } else {
            fprintf(temp, "%s", line);
        }
    }

    fclose(file);
    fclose(temp);
    remove("parc.txt");
    rename("temp_parc.txt", "parc.txt");

    // Update in agents.txt
    file = fopen("agents.txt", "r+");
    temp = fopen("temp_agents.txt", "w");

    while (fgets(line, sizeof(line), file)) {
        Agent agent;
        sscanf(line, "%19[^;];%49[^;];%49[^;];%d;%9[^;];%d", 
               agent.cin, agent.first_name, agent.last_name, &agent.age, agent.sexe, &agent.affecte);

        if (strcmp(agent.cin, cin_agent) == 0) {
            agent.affecte = 1;  // Mark as assigned
            fprintf(temp, "%s;%s;%s;%d;%s;%d\n", 
                    agent.cin, agent.first_name, agent.last_name, 
                    agent.age, agent.sexe, agent.affecte);
        } else {
            fprintf(temp, "%s", line);
        }
    }

    fclose(file);
    fclose(temp);
    remove("agents.txt");
    rename("temp_agents.txt", "agents.txt");
}
