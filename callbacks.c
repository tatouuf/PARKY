#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif
#include "reservation.h"
#include <gtk/gtk.h>
#include "callbacks.h"
#include "interface.h"
#include "support.h"


void on_itafbutton1_clicked(GtkButton *objet_graphique, gpointer user_data)
{
    // Déclarer la variable `reservation`
    reservation res; 

    // Récupérer les valeurs des widgets
    GtkWidget *spin1 = lookup_widget(GTK_WIDGET(objet_graphique), "itafspinbutton1");
    res.res_date.annee = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spin1));

    GtkWidget *spin2 = lookup_widget(GTK_WIDGET(objet_graphique), "itafspinbutton2");
    res.res_date.mois = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spin2));

    GtkWidget *spin3 = lookup_widget(GTK_WIDGET(objet_graphique), "itafspinbutton3");
    res.res_date.jour = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spin3));

    GtkWidget *comboBoxduree = lookup_widget(GTK_WIDGET(objet_graphique), "comboboxentry1");
    const gchar *duree_text = gtk_combo_box_get_active_text(GTK_COMBO_BOX(comboBoxduree));
    res.duree = atoi(duree_text);

    GtkWidget *input = lookup_widget(GTK_WIDGET(objet_graphique), "itafentry1");
    const gchar *cin_text = gtk_entry_get_text(GTK_ENTRY(input));
    res.cin = atoi(cin_text);

    // Récupérer le label pour afficher le résultat
    GtkWidget *output = lookup_widget(GTK_WIDGET(objet_graphique), "itafllabel2");

    // Gestion des places de parking (tableau statique comme exemple)
    static int places[1000] = {0}; // Tableau représentant les places (0 = libre, 1 = occupée)

    // Ajouter la réservation
    if (ajouter("reservations.txt", &res, places, 1000)) {
        char buffer[256];
        sprintf(buffer, "Réservation ajoutée avec succès :\nPlace attribuée %d", res.place);
        gtk_label_set_text(GTK_LABEL(output), buffer); // Mettre à jour le texte du label
    } else {
        gtk_label_set_text(GTK_LABEL(output), "Echec de l'ajout : aucune place disponible.");
    }
}

void on_itafbuttonn_clicked(GtkButton *objet_graphique, gpointer user_data) {
    reservation res;
    
    // Récupérer le texte du champ d'entrée pour le CIN
    GtkWidget *input = lookup_widget(GTK_WIDGET(objet_graphique), "itafeentry111");
    const gchar *cin_text = gtk_entry_get_text(GTK_ENTRY(input));
    res.cin = atoi(cin_text);

    // Trouver le label de sortie où afficher les résultats
    GtkWidget *output = lookup_widget(GTK_WIDGET(objet_graphique), "itafllabel1");
    char result_text[1024];  // Buffer pour stocker le texte formaté des résultats

    // Appel à la fonction afficher pour récupérer les réservations de l'utilisateur
    if (afficher("reservations.txt", res.cin, result_text)) {
        // Si des réservations ont été trouvées, afficher dans le label
        gtk_label_set_text(GTK_LABEL(output), result_text);
    } else {
        // Si aucune réservation n'a été trouvée, afficher un message d'erreur
        gtk_label_set_text(GTK_LABEL(output), "Echec : aucune place reliee a ce CIN.");
    }
}
void on_itafbutton4_clicked(GtkButton *objet_graphique, gpointer user_data) {
    reservation res;

    // Récupérer le texte de l'entrée et le convertir en entier (numéro de place)
    GtkWidget *input = lookup_widget(GTK_WIDGET(objet_graphique), "itafentry5");
    const gchar *place_text = gtk_entry_get_text(GTK_ENTRY(input));
    res.place = atoi(place_text);

    // Trouver le label où afficher le résultat
    GtkWidget *output = lookup_widget(GTK_WIDGET(objet_graphique), "itaflllabel4");

    char message[2000];  // Buffer pour le message de résultat

    // Appeler la fonction pour rechercher la réservation par numéro de place
    rechercher_par_place("reservations.txt", res.place, message);

    // Vérifier si un message a été trouvé (message vide ou non)
    if (strlen(message) > 0) {
        // Si des réservations ont été trouvées, afficher dans le label
        gtk_label_set_text(GTK_LABEL(output), message);
    } else {
        // Si aucune réservation n'a été trouvée, afficher un message d'erreur
        gtk_label_set_text(GTK_LABEL(output), "Echec : aucune réservation reliée à cette place.");
    }
}

void on_itafbuttonmodifier_clicked(GtkButton *objet_graphique, gpointer user_data)  
{
    reservation res; 
    const char *filename = "reservations.txt";  // Nom du fichier contenant les réservations
    
    // Récupération du CIN
    GtkWidget *input_cin = lookup_widget(GTK_WIDGET(objet_graphique), "itafentryy2");
    const gchar *cin_text = gtk_entry_get_text(GTK_ENTRY(input_cin));
    res.cin = atoi(cin_text);

    // Récupération de la place
    GtkWidget *input_place = lookup_widget(GTK_WIDGET(objet_graphique), "entry3");
    const gchar *place_text = gtk_entry_get_text(GTK_ENTRY(input_place));
    int place = atoi(place_text);

    // Récupération de la nouvelle date
    GtkWidget *spin_annee = lookup_widget(GTK_WIDGET(objet_graphique), "itafspinbuttonannee");
    res.res_date.annee = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spin_annee));

    GtkWidget *spin_mois = lookup_widget(GTK_WIDGET(objet_graphique), "itafspinbuttonmois");
    res.res_date.mois = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spin_mois));

    GtkWidget *spin_jour = lookup_widget(GTK_WIDGET(objet_graphique), "itafspinbuttonjour");
    res.res_date.jour = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spin_jour));

    // Récupération de la durée
    GtkWidget *combo_duree = lookup_widget(GTK_WIDGET(objet_graphique), "itafcomboboxentryy");
    const gchar *duree_text = gtk_combo_box_get_active_text(GTK_COMBO_BOX(combo_duree));
    res.duree = atoi(duree_text);

    // Modification de la réservation
    GtkWidget *output = lookup_widget(GTK_WIDGET(objet_graphique), "itaflllabel2");
    if (modifier(filename, res.cin, place, &res)) {
        // Message de succès et affichage des informations mises à jour
        char output_text[256];
        sprintf(output_text, 
                "Modification réussie pour CIN %d :\nPlace : %d\nDate : %04d-%02d-%02d\nDurée : %d heures", res.cin, res.place, res.res_date.annee, res.res_date.mois, res.res_date.jour, res.duree);
        gtk_label_set_text(GTK_LABEL(output), output_text);
    } else {
        // Message d'erreur en cas d'échec
        gtk_label_set_text(GTK_LABEL(output), "Erreur : Réservation non trouvée ou modification échouée.");
    }
}
void on_itafbutton3_clicked(GtkButton *objet_graphique, gpointer user_data) {
    // Widgets pour récupérer les entrées utilisateur
    GtkWidget *input_cin = lookup_widget(GTK_WIDGET(objet_graphique), "itafentry3");
    GtkWidget *input_place = lookup_widget(GTK_WIDGET(objet_graphique), "itafentry4");
    GtkWidget *output_label = lookup_widget(GTK_WIDGET(objet_graphique), "itaflllabel3");

    // Récupérer les données d'entrée
    const gchar *cin_text = gtk_entry_get_text(GTK_ENTRY(input_cin));
    const gchar *place_text = gtk_entry_get_text(GTK_ENTRY(input_place));

    // Valider les données d'entrée
    if (cin_text == NULL || place_text == NULL || strlen(cin_text) == 0 || strlen(place_text) == 0) {
        gtk_label_set_text(GTK_LABEL(output_label), "Erreur : Veuillez remplir tous les champs.");
        return;
    }

    // Convertir les données en entier
    int cin = atoi(cin_text);
    int place = atoi(place_text);

    // Vérifier si les conversions sont valides
    if (cin <= 0 || place <= 0) {
        gtk_label_set_text(GTK_LABEL(output_label), "Erreur : CIN ou place invalide.");
        return;
    }

    // Appeler la fonction supprimer depuis reservation.c
    if (supprimer("reservations.txt", cin, place)) {
        // Succès de la suppression
        gtk_label_set_text(GTK_LABEL(output_label), "Réservation supprimée avec succès.");
    } else {
        // Échec de la suppression
        gtk_label_set_text(GTK_LABEL(output_label), "Erreur : Réservation non trouvée.");
    }
}
void on_itafbuttontri_clicked(GtkButton *objet_graphique, gpointer user_data) {
    GtkWidget *window = lookup_widget(GTK_WIDGET(objet_graphique), "itafwineffichagetouteslesreser");
            GtkWidget *treeviewAI = GTK_WIDGET(lookup_widget(window, "treeview"));   
sort_and_write_reservations("reservations.txt","reservationsTrie.txt"); 
 populate_treeview_with_reservations(treeviewAI, "reservationsTrie.txt");
}

void
on_itafbutton2_clicked                 (GtkButton       *objet_graphique,
                                        gpointer         user_data)
{
    GtkWidget *itafwinremerciment                               =GTK_WIDGET(gtk_widget_get_toplevel(GTK_WIDGET(objet_graphique)));

    gtk_widget_destroy(itafwinremerciment);
}


void
on_buttonajouter1_clicked(GtkButton *objet_graphique, gpointer user_data)
{
	GtkWidget *itafwinajouter;
	itafwinajouter = create_itafwinajouter ();
  	gtk_widget_show (itafwinajouter);

}


void
on_bouttonaffichageres2_clicked(GtkButton *objet_graphique, gpointer user_data)
{
	GtkWidget *itafwinaffichage;
 	itafwinaffichage = create_itafwinaffichage ();
 	 gtk_widget_show (itafwinaffichage);

}


void
on_buttonaffichageplace3_clicked(GtkButton *objet_graphique, gpointer user_data)
{
	GtkWidget *itafwinrechercherparplace;	 
	itafwinrechercherparplace = create_itafwinrechercherparplace ();
 	 gtk_widget_show (itafwinrechercherparplace);

}


void
on_buttonmodifierres4_clicked(GtkButton *objet_graphique, gpointer user_data)
{
	GtkWidget *itafwinmodifier;
	itafwinmodifier = create_itafwinmodifier ();
	gtk_widget_show (itafwinmodifier);
}


void
on_buttonaffichagedesresers5_clicked(GtkButton *objet_graphique, gpointer user_data)
{
	GtkWidget *itafwineffichagetouteslesreser;
  	itafwineffichagetouteslesreser = create_itafwineffichagetouteslesreser ();
  	gtk_widget_show (itafwineffichagetouteslesreser);

}


void
on_buttonsupprimer6_clicked(GtkButton *objet_graphique, gpointer user_data)
{
	GtkWidget *itafwinsupprimer;
	itafwinsupprimer = create_itafwinsupprimer ();
  	gtk_widget_show (itafwinsupprimer);

}


void
on_quitter_clicked(GtkButton *objet_graphique, gpointer user_data)
{
  	GtkWidget *itafwinremerciment;

	 itafwinremerciment = create_itafwinremerciment ();
 	 gtk_widget_show (itafwinremerciment);

}

