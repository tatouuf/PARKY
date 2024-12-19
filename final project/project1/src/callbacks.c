#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <gtk/gtk.h>
#include <stdio.h>
#include "callbacks.h"
#include "interface.h"
#include "support.h"
#include "fct.h"














void on_ED_button3_gestionparking_clicked(GtkButton *button, gpointer user_data) {
    GtkWidget *ADD_MOD; 
    GtkWidget *Layout;

    // Afficher un message dans la console
    printf("dkhalna west gestion !!!!!!!!!!!!\n");
    fflush(stdout);

    // Cacher la fenêtre actuelle
    Layout = lookup_widget(GTK_WIDGET(button), "Layout");
    if (Layout != NULL) {
        gtk_widget_hide(Layout);
    } else {
        g_print("Erreur : Widget 'Layout' introuvable.\n");
    }

    // Créer et afficher la nouvelle fenêtre
    ADD_MOD = create_ADD_MOD();
    if (ADD_MOD != NULL) {
        gtk_widget_show(ADD_MOD);
    } else {
        g_print("Erreur : Impossible de créer la fenêtre ADD_MOD.\n");
    }
}

void on_ED_button4_gestionparking_clicked(GtkButton *button, gpointer user_data) {
    GtkWidget *ADD_MOD; 
    GtkWidget *Layout;

    Layout = lookup_widget(GTK_WIDGET(button), "Layout");
    if (Layout) {
        gtk_widget_hide(Layout);
    }

    ADD_MOD = create_ADD_MOD();
    if (ADD_MOD) {
        gtk_widget_show(ADD_MOD);
    }
}

void on_ED_button5_gestionparking_clicked(GtkButton *button, gpointer user_data) {
    GtkWidget *layout, *supp;

    // Récupérer la fenêtre actuelle et la masquer
    layout = lookup_widget(GTK_WIDGET(button), "Layout");
    gtk_widget_hide(layout);

    // Afficher la fenêtre Supp
    supp = create_Supp();
    gtk_widget_show(supp);

    // Récupérer le TreeView et afficher tous les parkings
    GtkWidget *treeview = lookup_widget(supp, "ED_treeview1_gestionparking");
    afficher_parkings(GTK_TREE_VIEW(treeview), NULL); // NULL signifie qu'on affiche tous les parkings
}


void on_ED_button6_gestionparking_clicked(GtkButton *button, gpointer user_data) {
    GtkWidget *Affec_agent; 
    GtkWidget *Layout;

    Layout = lookup_widget(GTK_WIDGET(button), "Layout");
    if (Layout) {
        gtk_widget_hide(Layout);
    }

    Affec_agent = create_Affec_agent();
    if (Affec_agent) {
        gtk_widget_show(Affec_agent);
    }
}

void on_ED_button10_gestionparking_clicked(GtkButton *button, gpointer user_data) {
    GtkWidget *Affich; 
    GtkWidget *Layout;
    GtkWidget *treeview;

    // Masquer l'ancienne fenêtre (Layout)
    Layout = lookup_widget(GTK_WIDGET(button), "Layout");
    if (Layout) {
        gtk_widget_hide(Layout);
    }

    // Créer la nouvelle fenêtre (Affich)
    Affich = create_Affich();
    if (Affich) {
        gtk_widget_show(Affich);

        // Initialiser et afficher les données dans le TreeView
        treeview = lookup_widget(Affich, "ED_treeview11_gestionparking");
        if (treeview) {
            afficher_parkings(GTK_TREE_VIEW(treeview), NULL); // Affiche tous les parkings
        } else {
            g_print("Erreur : Widget 'ED_treeview11_gestionparking' introuvable dans la fenêtre Affich.\n");
        }
    } else {
        g_print("Erreur : Impossible de créer la fenêtre Affich.\n");
    }
}


void on_ED_button1_gestionparking_clicked(GtkButton *button, gpointer user_data) {
    printf("dkhalna west quit\n");
    fflush(stdout);
    gtk_main_quit();
}

void on_ED_button14_gestionparking_clicked(GtkButton *button, gpointer user_data) {
    GtkWidget *ADD_MOD; 
    GtkWidget *Layout;

    ADD_MOD = lookup_widget(GTK_WIDGET(button), "ADD_MOD");
    if (ADD_MOD) {
        gtk_widget_hide(ADD_MOD);
    }

    Layout = create_Layout();
    if (Layout) {
        gtk_widget_show(Layout);
    }
}

void on_ED_button9_gestionparking_clicked(GtkButton *button, gpointer user_data) {
    GtkWidget *Supp; 
    GtkWidget *Layout;

    Supp = lookup_widget(GTK_WIDGET(button), "Supp");
    if (Supp) {
        gtk_widget_hide(Supp);
    }

    Layout = create_Layout();
    if (Layout) {
        gtk_widget_show(Layout);
    }
}

void on_ED_button12_gestionparking_clicked(GtkButton *button, gpointer user_data) {
    GtkWidget *Affec_agent; 
    GtkWidget *Layout;

    Affec_agent = lookup_widget(GTK_WIDGET(button), "Affec_agent");
    if (Affec_agent) {
        gtk_widget_hide(Affec_agent);
    }

    Layout = create_Layout();
    if (Layout) {
        gtk_widget_show(Layout);
    }
}

void on_ED_button2_gestionparking_clicked(GtkButton *button, gpointer user_data) {
    GtkWidget *Affich; 
    GtkWidget *Layout;

    Affich = lookup_widget(GTK_WIDGET(button), "Affich");
    if (Affich) {
        gtk_widget_hide(Affich);
    }

    Layout = create_Layout();
    if (Layout) {
        gtk_widget_show(Layout);
    }
}

void on_ED_button8_gestionparking_clicked(GtkButton *button, gpointer user_data) {
    GtkWidget *treeview = lookup_widget(GTK_WIDGET(button), "ED_treeview1_gestionparking");
    GtkWidget *CONFIR_supp;
    GtkTreeSelection *selection;
    GtkTreeModel *model;
    GtkTreeIter iter;
    gchar *selected_id = NULL;

    // Vérifier si un parking est sélectionné dans le TreeView
    selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(treeview));
    if (gtk_tree_selection_get_selected(selection, &model, &iter)) {
        // Récupérer l'ID du parking sélectionné
        gtk_tree_model_get(model, &iter, 0, &selected_id, -1); // Colonne 0 pour l'ID

        if (selected_id) {
            // Créer et afficher la fenêtre CONFIR_supp
            CONFIR_supp = create_CONFIR_supp();
            gtk_widget_show(CONFIR_supp);

            // Associer l'ID sélectionné à CONFIR_supp
            g_object_set_data(G_OBJECT(CONFIR_supp), "selected_id", g_strdup(selected_id));
            g_free(selected_id); // Libérer l'ID après l'avoir associé
        } else {
            g_print("Erreur : Aucun parking sélectionné.\n");
        }
    } else {
        g_print("Erreur : Aucun parking sélectionné.\n");
    }
}

void on_ED_button200_gestionparking_clicked(GtkButton *button, gpointer user_data) {
    GtkWidget *CONFIR_supp; 
    GtkWidget *Supp;

    CONFIR_supp = lookup_widget(GTK_WIDGET(button), "CONFIR_supp");
    if (CONFIR_supp) {
        gtk_widget_hide(CONFIR_supp);
    }

    Supp = create_Supp();
    if (Supp) {
        gtk_widget_show(Supp);
    }
}

void on_ED_checkbutton1_gestionparking_toggled(GtkToggleButton *togglebutton, gpointer user_data) {
    // Ajoutez ici votre logique pour le bouton à bascule
}
void afficher_localisation(GtkComboBoxText *combo_localisation) {
    // Récupérer le texte actif du GtkComboBoxText
    const gchar *localisation = gtk_combo_box_text_get_active_text(combo_localisation);

    if (localisation) {
        // Afficher le contenu dans la console
        printf("Localisation sélectionnée : %s\n", localisation);
        fflush(stdout); // S'assurer que le message s'affiche immédiatement
    } else {
        // Aucun élément sélectionné
        printf("Aucune localisation sélectionnée.\n");
        fflush(stdout);
    }
}

// Fonction pour ajouter un parking
void on_ED_button13_gestionparking_clicked(GtkWidget *objet_gra, gpointer user_data) {
    GtkWidget *entry_parking_id;
    GtkWidget *combo_localisation, *combo_horaire;
    GtkWidget *spin_places, *spin_prix;
    GtkWidget *check_type1, *check_type2, *check_type3;
    GtkWidget *radio_couvert, *radio_non_couvert;
    GtkWidget *label_message;

    const gchar *parking_id, *localisation, *horaire;
    int places, prix;
    int type1, type2, type3, couvert;

    // Nouvelle variable pour l'ID agent
    const gchar *id_agent = "non_affecter";

    // Récupérer les widgets
    entry_parking_id = lookup_widget(objet_gra, "ED_entry4_gestionparking");
    combo_localisation = lookup_widget(objet_gra, "ED_comboboxentry1_gestionparking");
    combo_horaire = lookup_widget(objet_gra, "ED_comboboxentry2_gestionparking");
    spin_places = lookup_widget(objet_gra, "ED_spinbutton4_gestionparking");
    spin_prix = lookup_widget(objet_gra, "ED_spinbutton1_gestionparking");
    check_type1 = lookup_widget(objet_gra, "ED_checkbutton1_gestionparking");
    check_type2 = lookup_widget(objet_gra, "ED_checkbutton2_gestionparking");
    check_type3 = lookup_widget(objet_gra, "ED_checkbutton3_gestionparking");
    radio_couvert = lookup_widget(objet_gra, "ED_radiobutton1_gestionparking");
    radio_non_couvert = lookup_widget(objet_gra, "ED_radiobutton2_gestionparking");
    label_message = lookup_widget(objet_gra, "label_message");

    // Effacer les anciens messages
    gtk_label_set_text(GTK_LABEL(label_message), "");

    // Récupération des valeurs des widgets
    parking_id = gtk_entry_get_text(GTK_ENTRY(entry_parking_id));
    localisation = gtk_combo_box_get_active_text(GTK_COMBO_BOX(combo_localisation));
    horaire = gtk_combo_box_get_active_text(GTK_COMBO_BOX(combo_horaire));
    places = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spin_places));
    prix = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spin_prix));
    type1 = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(check_type1));
    type2 = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(check_type2));
    type3 = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(check_type3));
    couvert = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(radio_couvert));

    // Validation stricte de l'ID
    if (g_strcmp0(parking_id, "") == 0 || !est_id_valide(parking_id)) {
        gtk_label_set_text(GTK_LABEL(label_message), "Erreur : L'ID doit contenir uniquement des chiffres.");
        return;
    }

    // Vérification des champs obligatoires
    if (localisation == NULL) {
        gtk_label_set_text(GTK_LABEL(label_message), "Erreur : La localisation est obligatoire.");
        return;
    }
    if (horaire == NULL) {
        gtk_label_set_text(GTK_LABEL(label_message), "Erreur : L'horaire est obligatoire.");
        return;
    }
    if (places <= 0) {
        gtk_label_set_text(GTK_LABEL(label_message), "Erreur : Le nombre de places doit être supérieur à zéro.");
        return;
    }
    if (prix <= 0) {
        gtk_label_set_text(GTK_LABEL(label_message), "Erreur : Le prix doit être supérieur à zéro.");
        return;
    }
    if (!type1 && !type2 && !type3) {
        gtk_label_set_text(GTK_LABEL(label_message), "Erreur : Vous devez sélectionner au moins un type de parking.");
        return;
    }

    // Vérification de l'unicité de l'ID
    if (verifier_id_existant(parking_id)) {
        gtk_label_set_text(GTK_LABEL(label_message), "Erreur : Cet ID existe déjà !");
        return;
    }

    // Déterminer le type de parking
    const char *type_parking = "";
    if (type1) {
        type_parking = "Parking privé";
    } else if (type2) {
        type_parking = "Parking public";
    } else if (type3) {
        type_parking = "Sous-sol";
    }

    // Enregistrer les données dans le fichier
    FILE *file = fopen("parc.txt", "a");
    if (file == NULL) {
        gtk_label_set_text(GTK_LABEL(label_message), "Erreur : Impossible d'ouvrir le fichier parc.txt.");
        return;
    }

    // Enregistrement avec l'ID agent par défaut "non_affecter"
    fprintf(file, "%s;%s;%s;%d;%d;%s;%s;%s\n",
            parking_id,
            localisation,
            horaire,
            places,
            prix,
            type_parking,
            couvert ? "Couvert" : "Non couvert",
            id_agent);

    fclose(file);

    // Afficher un message de succès
    gtk_label_set_text(GTK_LABEL(label_message), "Données enregistrées avec succès !");
    g_print("Les données ont été enregistrées avec succès dans le fichier parc.txt.\n");
}















// button modifier
void on_ED_button111_gestionparking_clicked(GtkButton *button, gpointer user_data) {
    GtkWidget *entry_parking_id;
    GtkWidget *combo_localisation, *combo_horaire;
    GtkWidget *spin_places, *spin_prix;
    GtkWidget *check_type1, *check_type2, *check_type3;
    GtkWidget *radio_couvert;
    GtkWidget *label_message;

    const gchar *parking_id, *localisation, *horaire;
    int places, prix;
    int type1, type2, type3, couvert;

    // Nouvelle variable pour ID agent par défaut
    const gchar *id_agent = "non_affecter";

    // Récupérer les widgets
    GtkWidget *objet_gra = GTK_WIDGET(button);
    entry_parking_id = lookup_widget(objet_gra, "ED_entry4_gestionparking");
    combo_localisation = lookup_widget(objet_gra, "ED_comboboxentry1_gestionparking");
    combo_horaire = lookup_widget(objet_gra, "ED_comboboxentry2_gestionparking");
    spin_places = lookup_widget(objet_gra, "ED_spinbutton4_gestionparking");
    spin_prix = lookup_widget(objet_gra, "ED_spinbutton1_gestionparking");
    check_type1 = lookup_widget(objet_gra, "ED_checkbutton1_gestionparking");
    check_type2 = lookup_widget(objet_gra, "ED_checkbutton2_gestionparking");
    check_type3 = lookup_widget(objet_gra, "ED_checkbutton3_gestionparking");
    radio_couvert = lookup_widget(objet_gra, "ED_radiobutton1_gestionparking");
    label_message = lookup_widget(objet_gra, "label_message");

    // Effacer les anciens messages
    gtk_label_set_text(GTK_LABEL(label_message), "");

    // Récupération des valeurs des widgets
    parking_id = gtk_entry_get_text(GTK_ENTRY(entry_parking_id));
    localisation = gtk_combo_box_get_active_text(GTK_COMBO_BOX(combo_localisation));
    horaire = gtk_combo_box_get_active_text(GTK_COMBO_BOX(combo_horaire));
    places = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spin_places));
    prix = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spin_prix));
    type1 = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(check_type1));
    type2 = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(check_type2));
    type3 = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(check_type3));
    couvert = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(radio_couvert));

    // Validation stricte de l'ID
    if (g_strcmp0(parking_id, "") == 0 || !est_id_valide(parking_id)) {
        gtk_label_set_text(GTK_LABEL(label_message), "Erreur : L'ID doit contenir uniquement des chiffres.");
        return;
    }

    // Charger les données existantes depuis le fichier
    FILE *file = fopen("parc.txt", "r");
    if (file == NULL) {
        gtk_label_set_text(GTK_LABEL(label_message), "Erreur : Impossible d'ouvrir le fichier parc.txt pour lecture.");
        return;
    }

    Parking parkings[100];
    int count = 0;
    int id_found = 0;
    char ligne[256];

    while (fgets(ligne, sizeof(ligne), file)) {
        sscanf(ligne, "%49[^;];%49[^;];%49[^;];%d;%d;%19[^;];%19[^;];%49[^\n]",
               parkings[count].id,
               parkings[count].localisation,
               parkings[count].horaire,
               &parkings[count].places,
               &parkings[count].prix,
               parkings[count].type1,
               parkings[count].couverture,
               parkings[count].id_agent);

        // Si l'ID correspond, modifier les données
        if (strcmp(parkings[count].id, parking_id) == 0) {
            strcpy(parkings[count].localisation, localisation);
            strcpy(parkings[count].horaire, horaire);
            parkings[count].places = places;
            parkings[count].prix = prix;

            if (type1) {
                strcpy(parkings[count].type1, "Parking privé");
            } else if (type2) {
                strcpy(parkings[count].type1, "Parking public");
            } else if (type3) {
                strcpy(parkings[count].type1, "Sous-sol");
            } else {
                strcpy(parkings[count].type1, "");
            }

            strcpy(parkings[count].couverture, couvert ? "Couvert" : "Non couvert");

            // Si l'ID agent est vide, ajouter "non_affecter"
            if (strcmp(parkings[count].id_agent, "") == 0) {
                strcpy(parkings[count].id_agent, id_agent);
            }

            id_found = 1;
        }
        count++;
    }

    fclose(file);

    // Vérifier si l'ID a été trouvé
    if (!id_found) {
        gtk_label_set_text(GTK_LABEL(label_message), "Erreur : Aucun parking avec cet ID trouvé.");
        return;
    }

    // Réécrire les données dans le fichier
    file = fopen("parc.txt", "w");
    if (file == NULL) {
        gtk_label_set_text(GTK_LABEL(label_message), "Erreur : Impossible d'ouvrir le fichier parc.txt pour écriture.");
        return;
    }

    for (int i = 0; i < count; i++) {
        fprintf(file, "%s;%s;%s;%d;%d;%s;%s;%s\n",
                parkings[i].id,
                parkings[i].localisation,
                parkings[i].horaire,
                parkings[i].places,
                parkings[i].prix,
                parkings[i].type1,
                parkings[i].couverture,
                parkings[i].id_agent);
    }

    fclose(file);

    // Afficher un message de succès
    gtk_label_set_text(GTK_LABEL(label_message), "Données modifiées avec succès !");
    g_print("Les données ont été modifiées avec succès dans le fichier parc.txt.\n");
}









void
on_ED_treeview1_gestionparking_row_activated
                                        (GtkTreeView     *treeview,
                                        GtkTreePath     *path,
                                        GtkTreeViewColumn *column,
                                        gpointer         user_data)
{
    GtkTreeModel *model;
    GtkTreeIter iter;
    gchar *id;

    // Obtenir le modèle associé au TreeView
    model = gtk_tree_view_get_model(treeview);

    // Récupérer l'itérateur correspondant à la ligne activée
    if (gtk_tree_model_get_iter(model, &iter, path)) {
        // Extraire l'ID du parking (colonne 0)
        gtk_tree_model_get(model, &iter, 0, &id, -1);

        // Afficher l'ID sélectionné pour debug
        g_print("Parking sélectionné : %s\n", id);

        // Créer la fenêtre de confirmation CONFIR_supp
        GtkWidget *confir_supp = create_CONFIR_supp();
        if (confir_supp) {
            // Associer l'ID sélectionné à CONFIR_supp
            g_object_set_data(G_OBJECT(confir_supp), "selected_id", g_strdup(id));

            // Afficher la fenêtre CONFIR_supp
            gtk_widget_show(confir_supp);
        } else {
            g_print("Erreur : Impossible de créer la fenêtre CONFIR_supp.\n");
        }

        // Libérer la mémoire de l'ID
        g_free(id);
    } else {
        g_print("Erreur : Impossible de récupérer les informations de la ligne sélectionnée.\n");
    }
}


void
on_ED_button100_gestionparking_clicked (GtkButton       *button,
                                        gpointer         user_data)
{
    
    GtkWidget *CONFIR_supp = GTK_WIDGET(gtk_widget_get_toplevel(GTK_WIDGET(button)));
    const gchar *selected_id;

    // Récupérer l'ID du parking stocké dans CONFIR_supp
    selected_id = g_object_get_data(G_OBJECT(CONFIR_supp), "selected_id");

    if (selected_id) {
        // Supprimer le parking avec l'ID stocké
        int success = supprimer_parking_par_id("parc.txt", selected_id);
        if (success) {
            g_print("Le parking avec l'ID %s a été supprimé.\n", selected_id);

            // Mettre à jour l'affichage dans la fenêtre principale
            GtkWidget *treeview = lookup_widget(GTK_WIDGET(button), "ED_treeview1_gestionparking");
            if (treeview) {
                afficher_parkings(GTK_TREE_VIEW(treeview), NULL); // Affiche tous les parkings
            }

            // Fermer la fenêtre de confirmation
            gtk_widget_destroy(CONFIR_supp);
        } else {
            g_print("Erreur : Échec de la suppression du parking avec l'ID %s.\n", selected_id);
        }
    } else {
        g_print("Erreur : Aucun ID à supprimer.\n");
    }

}


void
on_ED_treeview11_gestionparking_row_activated
                                        (GtkTreeView     *treeview,
                                        GtkTreePath     *path,
                                        GtkTreeViewColumn *column,
                                        gpointer         user_data)
{
    GtkTreeModel *model;
    GtkTreeIter iter;
    gchar *id;

    // Obtenir le modèle du TreeView
    model = gtk_tree_view_get_model(treeview);

    // Obtenir l'itérateur correspondant à la ligne activée
    if (gtk_tree_model_get_iter(model, &iter, path)) {
        // Récupérer l'ID du parking à partir de la colonne 0 (colonne ID)
        gtk_tree_model_get(model, &iter, 0, &id, -1);

        // Afficher l'ID sélectionné (debug ou log, optionnel)
        g_print("Parking sélectionné : %s\n", id);

        // Sauvegarder l'ID sélectionné dans les données utilisateur (si nécessaire)
        GtkWidget *window_supp = lookup_widget(GTK_WIDGET(treeview), "Supp");
        g_object_set_data(G_OBJECT(window_supp), "id_selectionne", g_strdup(id));

        // Passer à la fenêtre CONFIR_supp
        GtkWidget *confir_supp = create_CONFIR_supp();
        gtk_widget_show(confir_supp);

        // Masquer la fenêtre actuelle si nécessaire
        gtk_widget_hide(window_supp);

        // Libérer la mémoire (si nécessaire)
        g_free(id);
    } else {
        g_print("Erreur : Impossible de récupérer les informations de la ligne sélectionnée.\n");
    }


}


void
on_ED_entry2_AFFgestionparking_changed (GtkEditable     *editable,
                                        gpointer         user_data)
{
    GtkWidget *treeview = lookup_widget(GTK_WIDGET(editable), "ED_treeview11_gestionparking");

    // Récupérer le texte entré dans l'entry
    const gchar *parking_id = gtk_entry_get_text(GTK_ENTRY(editable));

    // Si l'ID est vide, afficher tous les parkings
    if (g_strcmp0(parking_id, "") == 0) {
        afficher_parkings(GTK_TREE_VIEW(treeview), NULL); // Affiche tous les parkings
    } else {
        // Sinon, afficher uniquement les parkings correspondant à l'ID
        afficher_parkings(GTK_TREE_VIEW(treeview), parking_id);
    }
}










void
on_ED_entry33_gestionparking_changed   (GtkEditable     *editable,
                                        gpointer         user_data)
{
    const char *id_recherche = gtk_entry_get_text(GTK_ENTRY(editable));
    GtkWidget *treeview = lookup_widget(GTK_WIDGET(editable), "ED_treeview1_gestionparking");

    if (strlen(id_recherche) > 0) {
        afficher_parkings(GTK_TREE_VIEW(treeview), id_recherche);
    } else {
        afficher_parkings(GTK_TREE_VIEW(treeview), NULL); // Si aucun ID saisi, afficher tout
    }
}


void
on_Actualiser_clicked                  (GtkButton       *button,
                                        gpointer         user_data)
{
    GtkWidget *window_supp, *treeview1;

    // Récupérer la fenêtre Supp
    window_supp = lookup_widget(GTK_WIDGET(button), "Supp");

    // Récupérer le TreeView dans la fenêtre Supp
    treeview1 = lookup_widget(window_supp, "ED_treeview1_gestionparking");

    // Appeler la fonction afficher_parkings pour mettre à jour le TreeView
    afficher_parkings(GTK_TREE_VIEW(treeview1), NULL);

    // Afficher un message de confirmation
    g_print("Actualisation terminée.\n");
}


void
on_Rech_clicked                        (GtkButton       *button,
                                        gpointer         user_data)
{
    GtkWidget *entry_parking_id;
    GtkWidget *combo_localisation, *combo_horaire;
    GtkWidget *spin_places, *spin_prix;
    GtkWidget *check_type1, *check_type2, *check_type3;
    GtkWidget *radio_couvert, *radio_non_couvert;
    GtkWidget *label_message;

    const gchar *parking_id;
    char localisation[50], horaire[50], type_parking[50], couverture[50];
    int places, prix, id_found = 0;

    // Récupérer les widgets nécessaires
    GtkWidget *objet_gra = GTK_WIDGET(button);
    entry_parking_id = lookup_widget(objet_gra, "ED_entry4_gestionparking");
    combo_localisation = lookup_widget(objet_gra, "ED_comboboxentry1_gestionparking");
    combo_horaire = lookup_widget(objet_gra, "ED_comboboxentry2_gestionparking");
    spin_places = lookup_widget(objet_gra, "ED_spinbutton4_gestionparking");
    spin_prix = lookup_widget(objet_gra, "ED_spinbutton1_gestionparking");
    check_type1 = lookup_widget(objet_gra, "ED_checkbutton1_gestionparking");
    check_type2 = lookup_widget(objet_gra, "ED_checkbutton2_gestionparking");
    check_type3 = lookup_widget(objet_gra, "ED_checkbutton3_gestionparking");
    radio_couvert = lookup_widget(objet_gra, "ED_radiobutton1_gestionparking");
    radio_non_couvert = lookup_widget(objet_gra, "ED_radiobutton2_gestionparking");
    label_message = lookup_widget(objet_gra, "label_message");

    // Effacer les anciens messages
    gtk_label_set_text(GTK_LABEL(label_message), "");

    // Récupérer l'ID saisi
    parking_id = gtk_entry_get_text(GTK_ENTRY(entry_parking_id));

    // Vérifier si l'ID est vide
    if (g_strcmp0(parking_id, "") == 0) {
        gtk_label_set_text(GTK_LABEL(label_message), "Erreur : Le champ ID est vide !");
        return;
    }

    // Ouvrir le fichier parc.txt pour rechercher l'ID
    FILE *file = fopen("parc.txt", "r");
    if (file == NULL) {
        gtk_label_set_text(GTK_LABEL(label_message), "Erreur : Impossible d'ouvrir le fichier parc.txt.");
        return;
    }

    char ligne[256];
    while (fgets(ligne, sizeof(ligne), file)) {
        char id[50];

        // Extraire les champs de la ligne
        sscanf(ligne, "%49[^;]; %49[^;]; %49[^;]; %d; %d; %49[^;]; %49[^\n]",
               id, localisation, horaire, &places, &prix, type_parking, couverture);

        // Vérifier si l'ID correspond
        if (strcmp(id, parking_id) == 0) {
            id_found = 1;
            break;
        }
    }
    fclose(file);

    // Si l'ID est trouvé, remplir les champs
    if (id_found) {
        // Trouver l'index correspondant à la localisation et l'horaire
        gint index_localisation = trouver_index_combobox(GTK_COMBO_BOX(combo_localisation), localisation);
        gint index_horaire = trouver_index_combobox(GTK_COMBO_BOX(combo_horaire), horaire);

        // Définir l'élément actif des combobox
        gtk_combo_box_set_active(GTK_COMBO_BOX(combo_localisation), index_localisation);
        gtk_combo_box_set_active(GTK_COMBO_BOX(combo_horaire), index_horaire);

        // Remplir les autres widgets
        gtk_spin_button_set_value(GTK_SPIN_BUTTON(spin_places), places);
        gtk_spin_button_set_value(GTK_SPIN_BUTTON(spin_prix), prix);

        // Cocher les cases correspondant au type de parking
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(check_type1), strcmp(type_parking, "Parking privé") == 0);
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(check_type2), strcmp(type_parking, "Parking public") == 0);
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(check_type3), strcmp(type_parking, "Sous-sol") == 0);

        // Définir la couverture
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(radio_couvert), strcmp(couverture, "Couvert") == 0);
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(radio_non_couvert), strcmp(couverture, "Non couvert") == 0);

        gtk_label_set_text(GTK_LABEL(label_message), "Parking trouvé avec succès !");
    } else {
        gtk_label_set_text(GTK_LABEL(label_message), "Erreur : Aucun parking avec cet ID trouvé.");
    }
}






void
on_button1_gestionparking_clicked      (GtkButton       *button,
                                        gpointer         user_data)
{
    GtkWidget *entry_id_parking = lookup_widget(GTK_WIDGET(button), "aa_entry1_ag");  // Updated entry name
    GtkWidget *entry_cin_agent = lookup_widget(GTK_WIDGET(button), "aa_entry_ag");  // Updated entry name
    GtkWidget *label_message = lookup_widget(GTK_WIDGET(button), "aa_label_ag");  // Updated label name

    const char *id_parking = gtk_entry_get_text(GTK_ENTRY(entry_id_parking));
    const char *cin_agent = gtk_entry_get_text(GTK_ENTRY(entry_cin_agent));

    affecter_agent(id_parking, cin_agent);
    gtk_label_set_text(GTK_LABEL(label_message), "Affectation réussie !");

}

void
on_aaaaah_clicked                      (GtkButton       *button,
                                        gpointer         user_data)
{
GtkWidget *entry_id_parking = lookup_widget(GTK_WIDGET(button), "aa_entry1_ag");  // Updated entry name
    GtkWidget *entry_cin_agent = lookup_widget(GTK_WIDGET(button), "aa_entry_ag");  // Updated entry name
    GtkWidget *label_message = lookup_widget(GTK_WIDGET(button), "aa_label_ag");  // Updated label name

    const char *id_parking = gtk_entry_get_text(GTK_ENTRY(entry_id_parking));
    const char *cin_agent = gtk_entry_get_text(GTK_ENTRY(entry_cin_agent));

    if (strlen(id_parking) == 0 || strlen(cin_agent) == 0) {
        gtk_label_set_text(GTK_LABEL(label_message), "Erreur : Remplissez tous les champs.");
        return;
    }

    if (!verifier_parking(id_parking)) {
        gtk_label_set_text(GTK_LABEL(label_message), "Erreur : Parking introuvable ou déjà affecté.");
        return;
    }

    if (!verifier_agent(cin_agent)) {
        gtk_label_set_text(GTK_LABEL(label_message), "Erreur : Agent introuvable ou déjà affecté.");
        return;
    }

    gtk_label_set_text(GTK_LABEL(label_message), "Parking et agent trouvés. Cliquez sur Confirmer.");
}

