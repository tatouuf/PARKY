#include <gtk/gtk.h>
#include "callbacks.h"
#include "interface.h"
#include "support.h"
#include "function.h"
#include <curl/curl.h>

int clientId ; 
int selected_agent_id;

void on_buttonSaveAjI_clicked(GtkWidget *objet_graphique, gpointer user_data) {
     char *citoyenFichier = "citoyen.txt";

    GtkWidget *entryNom = lookup_widget(objet_graphique, "entryNomAjI");
    GtkWidget *entryPrenom = lookup_widget(objet_graphique, "entryPrenomAjI");
    GtkWidget *entryCin = lookup_widget(objet_graphique, "entryCinAjI");
    GtkWidget *entryModele = lookup_widget(objet_graphique, "entryModeleAjI");
    GtkWidget *entryMatricule = lookup_widget(objet_graphique, "entryMatriculeAjI");
    GtkWidget *comboBoxVille = lookup_widget(objet_graphique, "comboboxentryVilleAjI");
    GtkWidget *spinButtonAge = lookup_widget(objet_graphique, "spinbuttonAgeAjI");
    GtkWidget *radioButtonMale = lookup_widget(objet_graphique, "radiobuttonMaleAjI");
    GtkWidget *radioButtonFemale = lookup_widget(objet_graphique, "radiobuttonFemaleAjI");
GtkWidget *labelError = lookup_widget(objet_graphique, "labelErrorAjI");
    
    citoyen c;
    char errorMessage[256];

    // Check if all entries are filled
    if (!entryNom || !gtk_entry_get_text(GTK_ENTRY(entryNom))[0] ||
        !entryPrenom || !gtk_entry_get_text(GTK_ENTRY(entryPrenom))[0] ||
        !entryCin || !gtk_entry_get_text(GTK_ENTRY(entryCin))[0] ||
        !entryModele || !gtk_entry_get_text(GTK_ENTRY(entryModele))[0] ||
        !entryMatricule || !gtk_entry_get_text(GTK_ENTRY(entryMatricule))[0] ||
        !comboBoxVille || !gtk_combo_box_get_active_text(GTK_COMBO_BOX(comboBoxVille)) ||
        !spinButtonAge || gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spinButtonAge)) == 0 ||
        (!radioButtonMale && !radioButtonFemale)) {
        snprintf(errorMessage, 256, "All fields must be filled.");
        gtk_label_set_text(GTK_LABEL(labelError), errorMessage);
        return;
    }


    if (entryNom) {
        strcpy(c.nom, gtk_entry_get_text(GTK_ENTRY(entryNom)));

    } else {
        strcpy(c.nom, "Unknown");
    }

    if (entryPrenom) {
        strcpy(c.prenom, gtk_entry_get_text(GTK_ENTRY(entryPrenom)));

    } else {
        strcpy(c.prenom, "Unknown");
    }

    if (entryCin) {
        c.cin = atoi(gtk_entry_get_text(GTK_ENTRY(entryCin)));

        clientId=atoi(gtk_entry_get_text(GTK_ENTRY(entryCin)));
    } else {
        c.cin = 0;
    }

    if (entryModele) {
        strcpy(c.modele, gtk_entry_get_text(GTK_ENTRY(entryModele)));

    } else {
        strcpy(c.modele, "Unknown");
    }

    if (entryMatricule) {
        strcpy(c.matricule, gtk_entry_get_text(GTK_ENTRY(entryMatricule)));

    } else {
        strcpy(c.matricule, "Unknown");
    }

    if (spinButtonAge) {
        c.age = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spinButtonAge));

    } else {
        c.age = 0;
    }

    if (comboBoxVille) {
        const char *selectedVille = gtk_combo_box_get_active_text(GTK_COMBO_BOX(comboBoxVille));
        if (selectedVille) {
            strcpy(c.ville, selectedVille);

        } else {
            strcpy(c.ville, "Unknown");
        }
    } else {
        strcpy(c.ville, "Unknown");
    }

    if (radioButtonMale && gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(radioButtonMale))) {
        strcpy(c.sexe, "Male");

    } else if (radioButtonFemale && gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(radioButtonFemale))) {
        strcpy(c.sexe, "Female");

    } else {
        strcpy(c.sexe, "Unknown");
    }

   if (ajouter(citoyenFichier, c, errorMessage)) {
        gtk_label_set_text(GTK_LABEL(labelError), "Citoyen added successfully.");
    } else {
        gtk_label_set_text(GTK_LABEL(labelError), errorMessage);
    } 
}

void on_radiobuttonFemaleAjI_toggled(GtkToggleButton *togglebutton, gpointer user_data) {
    // Your code here
}

void on_radiobuttonMaleAjI_toggled(GtkToggleButton *togglebutton, gpointer user_data) {
    // Your code here
}


void on_buttonSignupI_clicked(GtkButton *objet_graphique, gpointer user_data) { 
    GtkWidget *citoyenDashboardI;
    GtkWidget *SignUpI;
    citoyenDashboardI = create_citoyenDashboardI ();
    //SignUpI = create_SignUpI();  
    
    // Retrieve widgets
    GtkWidget *entryNom = lookup_widget(GTK_WIDGET(objet_graphique), "entryNomSignupI");
    GtkWidget *entryPrenom = lookup_widget(GTK_WIDGET(objet_graphique), "entryPrenomSignupI");
    GtkWidget *entryEmail = lookup_widget(GTK_WIDGET(objet_graphique), "entryEmailSignupI");
    GtkWidget *entryPwd = lookup_widget(GTK_WIDGET(objet_graphique), "entryPwdSignupI");
    GtkWidget *entryVerifierPwd = lookup_widget(GTK_WIDGET(objet_graphique), "entryVerifierPwdSignupI");

    GtkWidget *labelFeedbackEmail = lookup_widget(GTK_WIDGET(objet_graphique), "labelFeedbackSignupEmailI");
    GtkWidget *labelFeedbackPwd = lookup_widget(GTK_WIDGET(objet_graphique), "labelFeedbackSignupPWDI");
    GtkWidget *labelFeedbackVerifier = lookup_widget(GTK_WIDGET(objet_graphique), "labelErrorVirifierI");
    GtkWidget *labelError = lookup_widget(GTK_WIDGET(objet_graphique), "labelErrorSingUpI");

    // Reset feedback labels
    gtk_label_set_text(GTK_LABEL(labelFeedbackEmail), "");
    gtk_label_set_text(GTK_LABEL(labelFeedbackPwd), "");
    gtk_label_set_text(GTK_LABEL(labelFeedbackVerifier), "");
    gtk_label_set_text(GTK_LABEL(labelError), "");

    // Retrieve input values
    const char *nom = gtk_entry_get_text(GTK_ENTRY(entryNom));
    const char *prenom = gtk_entry_get_text(GTK_ENTRY(entryPrenom));
    const char *email = gtk_entry_get_text(GTK_ENTRY(entryEmail));
    const char *password = gtk_entry_get_text(GTK_ENTRY(entryPwd));
    const char *verifierPassword = gtk_entry_get_text(GTK_ENTRY(entryVerifierPwd));

    char feedbackMessage[256];

    // Validate all fields
    if (!nom[0] || !prenom[0] || !email[0] || !password[0]) {
        snprintf(feedbackMessage, sizeof(feedbackMessage), "All fields must be filled.");
        gtk_label_set_text(GTK_LABEL(labelError), feedbackMessage);
        return;
    }

    // Validate email format
    if (!is_valid_email(email)) {
        snprintf(feedbackMessage, sizeof(feedbackMessage), "Include a valid '@' symbol and domain.");
        gtk_label_set_text(GTK_LABEL(labelFeedbackEmail), feedbackMessage);
        return;
    }

    // Validate password strength
    int passwordStrength = password_strength(password);
    if (passwordStrength == 0) {
        snprintf(feedbackMessage, sizeof(feedbackMessage),
                 "Password must be at least 8 characters long ");
        gtk_label_set_text(GTK_LABEL(labelFeedbackPwd), feedbackMessage);
        return;
    } else if (passwordStrength == 1) {
        snprintf(feedbackMessage, sizeof(feedbackMessage), "Weak password! It contains only lowercase letters.");
        gtk_label_set_text(GTK_LABEL(labelFeedbackPwd), feedbackMessage);
        return;
    } else if (passwordStrength == 2) {
        snprintf(feedbackMessage, sizeof(feedbackMessage), "Normal password. Consider adding symbols for better security.");
        gtk_label_set_text(GTK_LABEL(labelFeedbackPwd), feedbackMessage);
    } else if (passwordStrength == 3) {
        snprintf(feedbackMessage, sizeof(feedbackMessage), "Strong password! Well done.");
        gtk_label_set_text(GTK_LABEL(labelFeedbackPwd), feedbackMessage);
    }

    // Verify password confirmation
    if (strcmp(verifierPassword, password) != 0) {
        snprintf(feedbackMessage, sizeof(feedbackMessage), "Passwords do not match!");
        gtk_label_set_text(GTK_LABEL(labelFeedbackVerifier), feedbackMessage);
        return;
    }

    // Perform signup
    int signupResult = signup(email, password, nom, prenom);
    if (signupResult == 0) {
        snprintf(feedbackMessage, sizeof(feedbackMessage),
                 "Signup failed. Ensure the email and password meet requirements.");
        gtk_label_set_text(GTK_LABEL(labelError), feedbackMessage);
    } else {
        SignUpI =lookup_widget(GTK_WIDGET(objet_graphique),"SignUpI");
        snprintf(feedbackMessage, sizeof(feedbackMessage), "Signup successful!");
        gtk_label_set_text(GTK_LABEL(labelError), feedbackMessage);
        gtk_widget_show (citoyenDashboardI); 
        gtk_widget_destroy(SignUpI);
      
     
    }
}



void
on_buttonSigninI_clicked               (GtkWidget *objet_graphique,
                                        gpointer         user_data)
{  

  GtkWidget *entryEmail = lookup_widget(GTK_WIDGET(objet_graphique), "entryEmailSigninI");
GtkWidget *entryPwd = lookup_widget(GTK_WIDGET(objet_graphique), "entryPwdSigninI");
GtkWidget *labelError = lookup_widget(GTK_WIDGET(objet_graphique), "labelErrorSigninI"); 
GtkWidget *labelFeedbackEmail = lookup_widget(GTK_WIDGET(objet_graphique), "labelErrorEmailSignInI"); 
 
     GtkWidget *CitoyenDashboardI;
     GtkWidget *AdminDashboardI;
    GtkWidget *SignInI;
    CitoyenDashboardI = create_citoyenDashboardI ();
    AdminDashboardI = create_adminDashboardI ();
    // Reset feedback labels
    gtk_label_set_text(GTK_LABEL(labelFeedbackEmail), "");
    gtk_label_set_text(GTK_LABEL(labelError), "");

 const char *email = gtk_entry_get_text(GTK_ENTRY(entryEmail));
 const char *password = gtk_entry_get_text(GTK_ENTRY(entryPwd));
    char feedbackMessage[256];
    char userType[256];


  // Validate all fields
    if (!email[0] || !password[0]) {
        snprintf(feedbackMessage, sizeof(feedbackMessage), "All fields must be filled.");
        gtk_label_set_text(GTK_LABEL(labelError), feedbackMessage);
        return;
    }  
    // Validate email format 
    if (!is_valid_email(email)) {
        snprintf(feedbackMessage, sizeof(feedbackMessage), " include '@'and domain.");
        gtk_label_set_text(GTK_LABEL(labelFeedbackEmail), feedbackMessage);
        return;
    } 
// Perform signin
    int signinResult = signin(email, password,userType);
    if (signinResult == 0) {
        snprintf(feedbackMessage, sizeof(feedbackMessage),
                 "Invalid Email or Password!!");
        gtk_label_set_text(GTK_LABEL(labelError), feedbackMessage); 
         } else { 
        if(strcmp(userType,"citoyen")==0){
	SignInI =lookup_widget(GTK_WIDGET(objet_graphique),"signInI");
        snprintf(feedbackMessage, sizeof(feedbackMessage), "Signin successful!");
        gtk_label_set_text(GTK_LABEL(labelError), feedbackMessage);
	 gtk_widget_show (CitoyenDashboardI); 
        gtk_widget_destroy(SignInI); 
    } 
   else {   
     gtk_widget_show (AdminDashboardI); 
        gtk_widget_destroy(SignInI);
}

    }

}
void
on_buttonEditAjI_clicked               (GtkButton       *objet,
                                        gpointer         user_data)
{
// Step 1: Retrieve the parent notebook widget
    GtkWidget *notebook = lookup_widget(GTK_WIDGET(objet), "notebookProfileI");
    
    if (notebook == NULL) {
        g_print("Error: Notebook widget 'notebookProfileI' not found.\n");
        return;
    }

    // Step 2: Set the current page of the notebook
    int pageIndex = 1; // The second page (index 1)
    gtk_notebook_set_current_page(GTK_NOTEBOOK(notebook), 2);

    g_print("Switched to page %d in the notebook.\n",2);
}


void
on_buttonDeleteAjI_clicked             (GtkButton       *button,
                                        gpointer         user_data)
{

    GtkWidget *DeleteAlert;
    DeleteAlert = create_deleteAlert ();
	 gtk_widget_show (DeleteAlert); 
       
}


void
on_radiobuttonMaleUpI_toggled          (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{

}


void
on_radiobuttonFemaleUpI_toggled        (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{

}


void on_buttonSaveUpI_clicked(GtkButton *objet_graphique, gpointer user_data) {
    char *citoyenFichier = "citoyen.txt";

    GtkWidget *entryNom = lookup_widget(objet_graphique, "entryNomUpI");
    GtkWidget *entryPrenom = lookup_widget(objet_graphique, "entryPrenomUpI");
    //GtkWidget *entryCin = lookup_widget(objet_graphique, "entryCinUpI");
    GtkWidget *entryModele = lookup_widget(objet_graphique, "entryModeleUpI");
    GtkWidget *entryMatricule = lookup_widget(objet_graphique, "entryMatriculeUpI");
    GtkWidget *comboBoxVille = lookup_widget(objet_graphique, "comboboxentryVilleUpI");
    GtkWidget *spinButtonAge = lookup_widget(objet_graphique, "spinbuttonAgeUpI");
    GtkWidget *radioButtonMale = lookup_widget(objet_graphique, "radiobuttonMaleUpI");
    GtkWidget *radioButtonFemale = lookup_widget(objet_graphique, "radiobuttonFemaleUpI");
    GtkWidget *labelError = lookup_widget(objet_graphique, "labelErrorUpI");
    
    citoyen c;
    //const char *CIN = gtk_entry_get_text(GTK_ENTRY(entryCin));
    if (!entryNom || !gtk_entry_get_text(GTK_ENTRY(entryNom))[0] ||
    !entryPrenom || !gtk_entry_get_text(GTK_ENTRY(entryPrenom))[0] ||
   // !entryCin || !gtk_entry_get_text(GTK_ENTRY(entryCin))[0] ||
    !entryModele || !gtk_entry_get_text(GTK_ENTRY(entryModele))[0] ||
    !entryMatricule || !gtk_entry_get_text(GTK_ENTRY(entryMatricule))[0] ||
    !comboBoxVille || !gtk_combo_box_get_active_text(GTK_COMBO_BOX(comboBoxVille)) ||
    !spinButtonAge || gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spinButtonAge)) == 0 ||
    (!radioButtonMale && !radioButtonFemale)) {
    gtk_label_set_text(GTK_LABEL(labelError), "Error: All fields must be filled.");
    return;
}

    // Validate CIN
 /*   if (strlen(CIN) != 8 || atoi(CIN) == 0) {
        gtk_label_set_text(GTK_LABEL(labelError), "Error: CIN must be exactly 8 digits.");
        return;
    }*/
    //int cin = atoi(CIN);
    char errorMessage[250];
    // Input validation for empty fields
    if (!gtk_entry_get_text(GTK_ENTRY(entryNom))[0] || 
    !gtk_entry_get_text(GTK_ENTRY(entryPrenom))[0] ||
    !gtk_entry_get_text(GTK_ENTRY(entryModele))[0] || 
    !gtk_entry_get_text(GTK_ENTRY(entryMatricule))[0]) {
    gtk_label_set_text(GTK_LABEL(labelError), "Error: All fields must be filled.");
    return;
}


    // Populate citizen structure
    strcpy(c.nom, gtk_entry_get_text(GTK_ENTRY(entryNom)));
    strcpy(c.prenom, gtk_entry_get_text(GTK_ENTRY(entryPrenom)));
    c.cin = clientId;
    strcpy(c.modele, gtk_entry_get_text(GTK_ENTRY(entryModele)));
    strcpy(c.matricule, gtk_entry_get_text(GTK_ENTRY(entryMatricule)));
    c.age = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spinButtonAge));
    strcpy(c.ville, gtk_combo_box_get_active_text(GTK_COMBO_BOX(comboBoxVille)));

    if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(radioButtonMale)))
        strcpy(c.sexe, "Male");
    else
        strcpy(c.sexe, "Female");

    if (modifier(citoyenFichier, clientId, c,errorMessage)) {
        gtk_label_set_text(GTK_LABEL(labelError), "Citoyen updated successfully.");
        GtkWidget *UpdateAlertI = create_updateAlertI();
        gtk_widget_show(UpdateAlertI);
    } else {
        gtk_label_set_text(GTK_LABEL(labelError), errorMessage);
    
    }
}


void
on_buttonSearchFacI_clicked            (GtkButton       *objet,
                                        gpointer         user_data)
{ 
 char totaleMessage[250];
 GtkWidget *labelTotale = lookup_widget(objet, "label5TotaleI");
 GtkWidget *window =lookup_widget(objet, "UserProfileI");
GtkWidget *notebook = GTK_WIDGET(lookup_widget(window, "notebookProfileI"));
GtkWidget *treeview1 = GTK_WIDGET(lookup_widget(window, "treeviewFactureI"));
GtkWidget *spin = lookup_widget(objet, "spinbuttonMoisFacI");
int mois = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spin));
    if (notebook == NULL) {
        g_print("Error: Notebook widget not found.\n");
        return;
    }
    if (treeview1 == NULL) {
        g_print("Error: TreeView widget not found.\n");
        return;
    } 
  
  
    searchByMonth("facture.txt",mois,totaleMessage);
    gtk_label_set_text(GTK_LABEL(labelTotale), totaleMessage);
    // Populate the TreeView with data from the file
    show_search(treeview1);  // Replace with your actual file name
}


void
on_chekbuttonSendEmailSignUp_toggled   (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{

}





void
on_buttonDontI_clicked                 (GtkButton       *objet_graphique,
                                        gpointer         user_data)
{
GtkWidget *SignUpI;
    GtkWidget *SignInI;
    SignUpI = create_SignUpI ();
    SignInI =lookup_widget(GTK_WIDGET(objet_graphique),"signInI");
	 gtk_widget_show (SignUpI); 
        gtk_widget_destroy(SignInI);
}


void
on_buttonYesDeleteI_clicked            (GtkButton       *objet,
                                        gpointer         user_data)
{ 
   char feedbackMessage[250];
// GtkWidget *labelTotale = lookup_widget(objet, "label5TotaleI");
   supprimer("citoyen.txt",clientId ); 
  GtkWidget *deleteAlert;
    deleteAlert =lookup_widget(GTK_WIDGET(objet),"deleteAlert");
    gtk_widget_destroy(deleteAlert); 

 


}


void
on_buttonCancelDeleteI_clicked         (GtkButton       *objet_graphique,
                                        gpointer         user_data)
{
 GtkWidget *deleteAlert;
    deleteAlert =lookup_widget(GTK_WIDGET(objet_graphique),"deleteAlert");
    gtk_widget_destroy(deleteAlert);
}


void
on_buttonYesUpdateI_clicked            (GtkButton       *button,
                                        gpointer         user_data)
{

}


void
on_buttonCancelUpdateI_clicked         (GtkButton       *button,
                                        gpointer         user_data)
{

}


void
on_notebookProfileI_switch_page        (GtkNotebook     *notebook,
                                        GtkNotebookPage *page,
                                        guint            page_num,
                                        gpointer         user_data)
{
     char totaleMessage[250];
   GtkWidget *labelTotale = lookup_widget(notebook, "label5TotaleI");
   if (page_num == 1) {
        // Ensure parent widget is properly passed or accessible
        GtkWidget *window = lookup_widget(GTK_WIDGET(notebook), "UserProfileI");
        if (window == NULL) {
            g_print("Error: Parent window 'UserProfileI' not found.\n");
            return;
        }

        GtkWidget *notebookWidget = GTK_WIDGET(lookup_widget(window, "notebookProfileI"));
        if (notebookWidget == NULL) {
            g_print("Error: Notebook widget 'notebookProfileI' not found.\n");
            return;
        }

        GtkWidget *treeviewAI = GTK_WIDGET(lookup_widget(window, "treeviewAI"));
        if (treeviewAI == NULL) {
            g_print("Error: TreeView widget 'treeview1' not found.\n");
            return;
        }

        
        populate_treeview(treeviewAI, "citoyen.txt", clientId);
        g_print("TreeView populated with data for CIN %d.\n", clientId);
    }else if(page_num == 3){
      GtkWidget *window =lookup_widget(notebook, "UserProfileI");
GtkWidget *notebook = GTK_WIDGET(lookup_widget(window, "notebookProfileI"));
GtkWidget *treeview1 = GTK_WIDGET(lookup_widget(window, "treeviewFactureI"));

    if (notebook == NULL) {
        g_print("Error: Notebook widget not found.\n");
        return;
    }
    if (treeview1 == NULL) {
        g_print("Error: TreeView widget not found.\n");
        return;
    }
 
     generateFacture("reservation.txt", "services.txt","parking.txt","facture.txt",15915999,totaleMessage); 
    gtk_label_set_text(GTK_LABEL(labelTotale), totaleMessage);
    // Populate the TreeView with data from the file
    show_facture(treeview1);  // Replace with your actual file name
}
}


void
on_buttonRefrechI_clicked              (GtkButton       *objet,
                                        gpointer         user_data)
{ 
GtkWidget *window =lookup_widget(objet, "adminCitoyen");
//GtkWidget *notebook = GTK_WIDGET(lookup_widget(window, "notebookProfileI"));
GtkWidget *treeview3 = GTK_WIDGET(lookup_widget(window, "treeviewAdmineI"));
 populate_treeview_with_citoyens(treeview3,"citoyen.txt");
}
void
on_treeviewAdmineI_row_activated       (GtkTreeView     *treeview,
                                        GtkTreePath     *path,
                                        GtkTreeViewColumn *column,
                                        gpointer         user_data)
{
 GtkTreeModel *model;
    GtkTreeIter iter;
    int value; // To store the value from the first column

    // Get the model from the tree view
    model = gtk_tree_view_get_model(treeview);

    // Get the iter for the activated row
    if (gtk_tree_model_get_iter(model, &iter, path)) {
        // Get the value from the first column (column 0)
        gtk_tree_model_get(model, &iter,2, &value, -1);

        // Print the value (or process it)
        g_print("Activated row value: %d\n", value);
        selected_agent_id=value;

}
}

void
on_buttonDeleteAdminI_clicked          (GtkButton       *button,
                                        gpointer         user_data)
{ 

supprimer("citoyen.txt",selected_agent_id);
}




void
on_buttonForgetPI_clicked              (GtkButton       *objet_graphique,
                                        gpointer         user_data)
{ 
GtkWidget *source_entry = lookup_widget(objet_graphique, "entryEmailSigninI"); 
/*GtkWidget *second_window= lookup_widget(GTK_WIDGET(objet_graphique), "sendEmail"); 
  //GtkWindow *second_window= lookup_widget(GTK_WIDGET(objet_graphique), "sendEmail");
   //GtkWidget *destination_entry = lookup_widget(second_window, "entryGmailSendI"); 
const char *text = gtk_entry_get_text(GTK_ENTRY(source_entry));
    //gtk_entry_set_text(GTK_ENTRY(destination_entry), text);*/ 

//g_print("%s",text);

GtkWidget *SignInI;
    GtkWidget *send;
    send = create_sendEmail ();
    SignInI =lookup_widget(GTK_WIDGET(objet_graphique),"signInI");
	 gtk_widget_show (send); 
        gtk_widget_destroy(SignInI); 
   
}


void
on_buttonSendPI_clicked                (GtkButton       *objet,
                                        gpointer         user_data)
{
char email[100];
char errorMessage[250];
GtkWidget *labelErrorSend = lookup_widget(objet, "labelErrorSendI");
GtkWidget *Email = lookup_widget(objet, "entryGmailSendI");
strcpy(email, gtk_entry_get_text(GTK_ENTRY(Email))); 
  if (!is_valid_email(email)) {
        snprintf(errorMessage, sizeof(errorMessage), "Include a valid '@' symbol and domain.");
        gtk_label_set_text(GTK_LABEL(labelErrorSend), errorMessage);
        return;
    } 
 if (!Email || !gtk_entry_get_text(GTK_ENTRY(Email))[0]){
        snprintf(errorMessage, 256, "All fields must be filled.");
        gtk_label_set_text(GTK_LABEL(labelErrorSend), errorMessage);
        return;
} 
if(emailExist(email,errorMessage)==1) { 
//gtk_label_set_text(GTK_LABEL(labelErrorSend),errorMessage);
const char *recipient = email;
    const char *subject = "New Password";
    const char *body = "Your new password is: Az88r50@80";  // Or generate this programmatically

    send_email(recipient, subject, body); 
    //snprintf(errorMessage, 256, "Email send successfully!");
changePassword(email,"Az88r50@80");
}else{gtk_label_set_text(GTK_LABEL(labelErrorSend),errorMessage);
}}

void
on_buttonCancelSendI_clicked           (GtkButton       *objet_graphique,
                                        gpointer         user_data)
{
GtkWidget *SignInI;
    GtkWidget *send;
    SignInI = create_signInI();
    send =lookup_widget(GTK_WIDGET(objet_graphique),"sendEmail");
	 gtk_widget_show (SignInI); 
        gtk_widget_destroy(send);
}


void
on_buttonGAI_clicked                   (GtkButton       *button,
                                        gpointer         user_data)
{

}


void
on_buttonGSI_clicked                   (GtkButton       *button,
                                        gpointer         user_data)
{

}


void
on_buttonBackADI_clicked               (GtkButton       *button,
                                        gpointer         user_data)
{

}


void
on_buttonLogoutADI_clicked             (GtkButton       *objet_graphique,
                                        gpointer         user_data)
{
GtkWidget *adminDashboardI;
    GtkWidget *SignInI;
    SignInI = create_signInI ();
    adminDashboardI =lookup_widget(GTK_WIDGET(objet_graphique),"adminDashboardI");
	 gtk_widget_show (SignInI); 
        gtk_widget_destroy(adminDashboardI);
}


void
on_buttonGPI_clicked                   (GtkButton       *button,
                                        gpointer         user_data)
{

}


void
on_buttonGRI_clicked                   (GtkButton       *button,
                                        gpointer         user_data)
{

}


void
on_buttonGCI_clicked                   (GtkButton       *objet_graphique,
                                        gpointer         user_data)
{
GtkWidget *citoyenDashboardI;
    GtkWidget *UserProfileI;
    UserProfileI = create_UserProfileI ();
    citoyenDashboardI =lookup_widget(GTK_WIDGET(objet_graphique),"citoyenDashboardI");
	 gtk_widget_show (UserProfileI); 
        gtk_widget_destroy(citoyenDashboardI);
}


void
on_buttonLogoutCDI_clicked             (GtkButton       *objet_graphique,
                                        gpointer         user_data)
{
 GtkWidget *citoyenDashboardI;
    GtkWidget *SignInI;
    SignInI = create_signInI ();
    citoyenDashboardI =lookup_widget(GTK_WIDGET(objet_graphique),"citoyenDashboardI");
	 gtk_widget_show (SignInI); 
        gtk_widget_destroy(citoyenDashboardI);
}


void
on_buttonBackCDI_clicked               (GtkButton       *objet_graphique,
                                        gpointer         user_data)
{
 GtkWidget *citoyenDashboardI;
    GtkWidget *SignInI;
    SignInI = create_signInI ();
    citoyenDashboardI =lookup_widget(GTK_WIDGET(objet_graphique),"citoyenDashboardI");
	 gtk_widget_show (SignInI); 
        gtk_widget_destroy(citoyenDashboardI);
}
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
void on_ajouter_clicked(GtkButton *objet_graphique, gpointer user_data) {
	GtkWidget *input_nom;
	GtkWidget *input_prenom;
	GtkWidget *input_CIN;
	GtkWidget *input_ville;
	GtkWidget *input_numTEL;
	GtkWidget *input_email;
	GtkWidget *dn_jour;
	GtkWidget *dn_mois;
	GtkWidget *dn_annee;
	GtkWidget *dr_jour;
	GtkWidget *dr_mois;
	GtkWidget *dr_annee;
	GtkWidget *Combobox1;
	GtkWidget *Combobox2;
	GtkWidget *toggle_button_h;
	GtkWidget *toggle_button_f;
	GtkWidget *output;
	int n;


	agent new_agent;  

    // Récupérer les widgets
	input_nom = lookup_widget(GTK_WIDGET(objet_graphique), "nom");
	input_prenom = lookup_widget(GTK_WIDGET(objet_graphique), "prenom");
	input_CIN = lookup_widget(GTK_WIDGET(objet_graphique), "CIN");
	input_ville = lookup_widget(GTK_WIDGET(objet_graphique), "ville");
	input_numTEL = lookup_widget(GTK_WIDGET(objet_graphique), "numTEL");
	input_email = lookup_widget(GTK_WIDGET(objet_graphique), "email");
	output = lookup_widget(GTK_WIDGET(objet_graphique), "output");
	dn_jour = lookup_widget(GTK_WIDGET(objet_graphique), "datedenaissancejour");
	dn_mois = lookup_widget(GTK_WIDGET(objet_graphique), "datedenaissancemois");
	dn_annee = lookup_widget(GTK_WIDGET(objet_graphique), "datedenaissanceannee");
	dr_jour = lookup_widget(GTK_WIDGET(objet_graphique), "datederecrutementjour");
	dr_mois = lookup_widget(GTK_WIDGET(objet_graphique), "datederecrutementmois");
	dr_annee = lookup_widget(GTK_WIDGET(objet_graphique), "datederecrutementannee");
	Combobox1 = lookup_widget(GTK_WIDGET(objet_graphique), "horraires_de_travail");
	Combobox2 = lookup_widget(GTK_WIDGET(objet_graphique), "poste_de_travail");
	toggle_button_h = lookup_widget(GTK_WIDGET(objet_graphique), "homme");
	toggle_button_f = lookup_widget(GTK_WIDGET(objet_graphique), "femme");

	/* Récupérer les valeurs des boutons radio en utilisant la fonction gtk_toggle_button_set_active */
	if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(toggle_button_h))) {
		new_agent.sexe=1;
	}
	else
		if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(toggle_button_f))) {
			new_agent.sexe=0;
		}


	/* Récupérer les valeurs des spins buttons en utulisant la fonction gtk_spin_button_get_value_as_int qui retourne l'entier choisi par l'utilisateur */    
	new_agent.date_naissance.jour=gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(dn_jour));
	new_agent.date_naissance.mois=gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(dn_mois));
	new_agent.date_naissance.annee=gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(dn_annee));
	new_agent.date_recrutement.jour=gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(dr_jour));
	new_agent.date_recrutement.mois=gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(dr_mois));
	new_agent.date_recrutement.annee=gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(dr_annee));


    // Récupérer les textes entrés par l'utilisateur
	strcpy(new_agent.nom, gtk_entry_get_text(GTK_ENTRY(input_nom)));
	strcpy(new_agent.prenom, gtk_entry_get_text(GTK_ENTRY(input_prenom)));
	strcpy(new_agent.ville, gtk_entry_get_text(GTK_ENTRY(input_ville)));
	strcpy(new_agent.email, gtk_entry_get_text(GTK_ENTRY(input_email)));

    // Conversion de CIN et numTEL en entier
	new_agent.CIN = atoi(gtk_entry_get_text(GTK_ENTRY(input_CIN)));
	new_agent.numTEL = atoi(gtk_entry_get_text(GTK_ENTRY(input_numTEL)));

/* Récupérer les valeurs des combobox en utilisant la fonction gtk_combo_box_get_active_text qui retourne la chaine de caractère choisis par l'utilisateur */
	if (strcmp("00h==>8h",gtk_combo_box_get_active_text(GTK_COMBO_BOX(Combobox1)))==0) {
		new_agent.horraires_de_travail=0;	
	}
	else
		if (strcmp("8h==>16h",gtk_combo_box_get_active_text(GTK_COMBO_BOX(Combobox1)))==0) {
			new_agent.horraires_de_travail=1;
		}
		else {
			new_agent.horraires_de_travail=2;
		}

	
	
	if (strcmp("poste0",gtk_combo_box_get_active_text(GTK_COMBO_BOX(Combobox2)))==0) {
		new_agent.poste_de_travail=0;	
	}
	else
		if (strcmp("poste1",gtk_combo_box_get_active_text(GTK_COMBO_BOX(Combobox2)))==0) {
			new_agent.poste_de_travail=1;
		}
		else
			if (strcmp("poste2",gtk_combo_box_get_active_text(GTK_COMBO_BOX(Combobox2)))==0) {
				new_agent.poste_de_travail=2;
			}
			else {
				new_agent.poste_de_travail=3;
			}


	


    // Vérification des champs
	if (strlen(new_agent.nom) == 0 || strlen(new_agent.prenom) == 0 || new_agent.CIN == 0 ||
		strlen(new_agent.ville) == 0 || new_agent.numTEL == 0 || strlen(new_agent.email) == 0) {
		gtk_label_set_text(GTK_LABEL(output), "Tous les champs doivent être remplis !");
	} 
	else {
        // Si les champs sont valides, ajoutez l'agent
		n=ajouter_agent("agents.txt", new_agent);
		if (n != 0) {
			gtk_label_set_text(GTK_LABEL(output), "Agent ajouté avec succès !");
		} 
		else {
			gtk_label_set_text(GTK_LABEL(output), "Erreur lors de l'ajout de l'agent.");
        	}
    	}
}


/******************************************************************************************************/

void on_continuer_clicked(GtkButton *objet_graphique, gpointer user_data) {
	GtkWidget *output;
	GtkWidget *input_CIN;
	int CIN;
	int n;
	input_CIN = lookup_widget(GTK_WIDGET(objet_graphique), "CIN");
	output = lookup_widget(GTK_WIDGET(objet_graphique), "output");
	CIN = atoi(gtk_entry_get_text(GTK_ENTRY(input_CIN)));
	n=supprimer("agents.txt",CIN);
	if (!n) {
		gtk_label_set_text(GTK_LABEL(output), "Agent non trouvé(e) !");
	}
	else {
		gtk_label_set_text(GTK_LABEL(output), "Agent supprimé(e) avec succès !"); 
	}

}

/*****************************************************************************************************/
void on_modifier_clicked(GtkButton *objet_graphique, gpointer user_data)
{
    GtkWidget *output;
    GtkWidget *input_nom, *input_prenom, *input_CIN, *input_ville, *input_numTEL, *input_email;
    GtkWidget *Combobox1, *Combobox2;
    agent new_agent;
    int cin, n;

    // Initialisation des widgets
    output = lookup_widget((GtkWidget *)objet_graphique, "output");
    input_nom = lookup_widget((GtkWidget *)objet_graphique, "nom");
    input_prenom = lookup_widget((GtkWidget *)objet_graphique, "prenom");
    input_CIN = lookup_widget((GtkWidget *)objet_graphique, "CIN");
    input_ville = lookup_widget((GtkWidget *)objet_graphique, "ville");
    input_numTEL = lookup_widget((GtkWidget *)objet_graphique, "numTEL");
    input_email = lookup_widget((GtkWidget *)objet_graphique, "email");
    Combobox1 = lookup_widget((GtkWidget *)objet_graphique, "horraires");
    Combobox2 = lookup_widget((GtkWidget *)objet_graphique, "poste");

    // Vérification des champs
    if (strlen(gtk_entry_get_text(GTK_ENTRY(input_nom))) == 0 ||
        strlen(gtk_entry_get_text(GTK_ENTRY(input_prenom))) == 0 ||
        strlen(gtk_entry_get_text(GTK_ENTRY(input_CIN))) == 0 ||
        strlen(gtk_entry_get_text(GTK_ENTRY(input_ville))) == 0 ||
        strlen(gtk_entry_get_text(GTK_ENTRY(input_email))) == 0) {
        gtk_label_set_text(GTK_LABEL(output), "Tous les champs doivent être remplis !");
        return;
    }

    // Récupération des valeurs
    strcpy(new_agent.nom, gtk_entry_get_text(GTK_ENTRY(input_nom)));
    strcpy(new_agent.prenom, gtk_entry_get_text(GTK_ENTRY(input_prenom)));
    strcpy(new_agent.ville, gtk_entry_get_text(GTK_ENTRY(input_ville)));
    strcpy(new_agent.email, gtk_entry_get_text(GTK_ENTRY(input_email)));

    // Conversion des champs numériques
    new_agent.CIN = atoi(gtk_entry_get_text(GTK_ENTRY(input_CIN)));
    new_agent.numTEL = atoi(gtk_entry_get_text(GTK_ENTRY(input_numTEL)));

    // Récupération des combobox
    if (strcmp("00h==>8h", gtk_combo_box_get_active_text(GTK_COMBO_BOX(Combobox1))) == 0) {
        new_agent.horraires_de_travail = 0;
    } else if (strcmp("8h==>16h", gtk_combo_box_get_active_text(GTK_COMBO_BOX(Combobox1))) == 0) {
        new_agent.horraires_de_travail = 1;
    } else {
        new_agent.horraires_de_travail = 2;
    }

    // Appeler la fonction `on_ccontinuer_clicked` pour obtenir le CIN
    cin = on_ccontinuer_clicked(objet_graphique, user_data);
    n=modifier("agents.txt", cin, new_agent);
    if (!cin) {
        gtk_label_set_text(GTK_LABEL(output), "Erreur : CIN invalide !");
        return;
    }

    // Modifier l'agent
    if (!n) {
        gtk_label_set_text(GTK_LABEL(output), "Erreur lors de la modification de l'agent !");
    } else {
	n=modifier("agents.txt", cin, new_agent);
        gtk_label_set_text(GTK_LABEL(output), "Modification réussie !");
    }
}
 

/**********************************************************************************************************************************/
int on_ccontinuer_clicked(GtkButton *objet_graphique, gpointer user_data)
{
    GtkWidget *output;
    GtkWidget *input_CIN;
    int n, CIN;
    char msg[100];
    agent G;
    GtkWidget *fenetre_chercher;
    GtkWidget *fenetre_modifier;

    // Cast objet_graphique to GtkWidget*
    fenetre_chercher = lookup_widget((GtkWidget *)objet_graphique, "demande modifier cin agent");
    fenetre_modifier = lookup_widget((GtkWidget *)objet_graphique, "mis à jour compte agent");
    input_CIN = lookup_widget((GtkWidget *)objet_graphique, "CIN");
    output = lookup_widget((GtkWidget *)objet_graphique, "output");

    // Get input from the entry field
    strcpy(msg, gtk_entry_get_text(GTK_ENTRY(input_CIN)));

    // Check if input is empty
    if (strlen(msg) == 0) {
        gtk_label_set_text(GTK_LABEL(output), "Le champ doit être complis");
        return 0; // Exit function if input is empty
    }

    // Convert input to integer and search for the agent
    CIN = atoi(msg);
    n = chercher("agents.txt", CIN, &G);

    // Handle search results
    if (!n) {
        gtk_label_set_text(GTK_LABEL(output), "Agent non trouvé(e) !");
    } else {
        gtk_widget_destroy(fenetre_chercher);
        
        // Ensure create_mis____jour_compte_agent is implemented or corrected
        fenetre_modifier = create_mis____jour_compte_agent(); 
        gtk_widget_show(fenetre_modifier);
    }
    return CIN;
}






/**********************************************************************************************/

void
on_treeview1_row_activated             (GtkTreeView     *treeview,
                                        GtkTreePath     *path,
                                        GtkTreeViewColumn *column,
                                        gpointer         user_data)
{

}

/**************************************************************************************************/
void on_supprimerlabprincipale_clicked(GtkButton *objet_graphique, gpointer user_data)
{
    GtkWidget *fenetre_principale;
    GtkWidget *fenetre_supprimer;
    fenetre_principale = lookup_widget((GtkWidget *)objet_graphique, "labelprincipale");
    fenetre_supprimer = lookup_widget((GtkWidget *)objet_graphique, "supprimer un agent");
    fenetre_supprimer = create_Supprimer_un_agent ();
    gtk_widget_show (fenetre_supprimer);
    
}

void on_modifierlabprincipale_clicked(GtkButton *objet_graphique, gpointer user_data)
{
    GtkWidget *fenetre_principale;
    GtkWidget *fenetre_modifier;
    fenetre_principale = lookup_widget((GtkWidget *)objet_graphique, "labelprincipale");
    fenetre_modifier = lookup_widget((GtkWidget *)objet_graphique, "demande modifier cin agent");
    fenetre_modifier = create_demande_modifier_cin_agent ();
    gtk_widget_show (fenetre_modifier);

}


void on_ajouterlabprincipale_clicked(GtkButton *objet_graphique, gpointer user_data)
{
    GtkWidget *fenetre_principale;
    GtkWidget *fenetre_ajouter;
    fenetre_principale = lookup_widget((GtkWidget *)objet_graphique, "labelprincipale");
    fenetre_ajouter = lookup_widget((GtkWidget *)objet_graphique, "Ajouter un agent");
    fenetre_ajouter = create_Ajouter_un_agent ();
    gtk_widget_show (fenetre_ajouter);
}

void on_listereservation_clicked(GtkButton *objet_graphique, gpointer user_data)
{
    GtkWidget *fenetre_date_reservation;
    fenetre_date_reservation = create_date_de_r__servation ();
    gtk_widget_show (fenetre_date_reservation);


}


void on_okreservation_clicked(GtkButton *objet_graphique, gpointer user_data)
{
    GtkWidget *dres_jour;
    GtkWidget *dres_mois;
    GtkWidget *dres_annee;
    GtkWidget *output;
    GtkWidget *fenetre_liste_reservation;
    GtkWidget *treeview2;
    char buffer[50];
    reservation R;
    int n, x;

    dres_jour = lookup_widget(GTK_WIDGET(objet_graphique), "datedereservationjour");
    dres_mois = lookup_widget(GTK_WIDGET(objet_graphique), "datedereservationmois");
    dres_annee = lookup_widget(GTK_WIDGET(objet_graphique), "datedereservationannee");
    output = lookup_widget(GTK_WIDGET(objet_graphique), "output");

    R.date_reservation.jour = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(dres_jour));
    R.date_reservation.mois = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(dres_mois));
    R.date_reservation.annee = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(dres_annee));

    n = chercher_reservation("re.txt", R.date_reservation.jour, R.date_reservation.mois, R.date_reservation.annee, &R);

    if (!n) {
        gtk_label_set_text(GTK_LABEL(output), "Réservation non trouvée !");
    } else {
        // Construire le message avec la date de réservation
        sprintf(buffer, "%d / %d / %d", R.date_reservation.jour, R.date_reservation.mois, R.date_reservation.annee);
        gtk_label_set_text(GTK_LABEL(output), buffer);

        x = afficher_reservation("re.txt", R.date_reservation.jour, R.date_reservation.mois, R.date_reservation.annee);
        fenetre_liste_reservation = create_liste_des_r__servations();
        gtk_widget_show(fenetre_liste_reservation);
        treeview2 = lookup_widget(fenetre_liste_reservation, "treeview2");
        afficher_liste_reservation(treeview2);
    }
}

void
on_btn_ajouter_ww_clicked              (GtkButton       *button,
                                        gpointer         user_data)
{
    GtkWidget *window_ajouter;

    // Créer la fenêtre "Ajouter" si elle n'existe pas déjà
    window_ajouter = create_window_ajouter_ww();

    // Afficher la fenêtre "Ajouter"
    gtk_widget_show(window_ajouter);
}


void
on_btn_modifier_ww_clicked             (GtkWidget       *button,
                                        gpointer         user_data)
{
    GtkWidget *window_modifier, *combobox;

    g_print("Tentative de création de la fenêtre Modifier...\n");

    // Créer la fenêtre "Modifier"
    window_modifier = create_window_modifier_ww();
    if (!window_modifier) {
        g_print("Erreur : Impossible de créer ou de trouver la fenêtre Modifier.\n");
        return;
    }

    // Récupérer la combobox
    combobox = lookup_widget(window_modifier, "comboboxentry_modifier_ww");
    if (!combobox) {
        g_print("Erreur : Impossible de trouver la combobox Modifier.\n");
        return;
    }

    // Remplir la combobox
    remplir_combobox_ids(combobox);

    // Afficher la fenêtre "Modifier"
    gtk_widget_show_all(window_modifier);

    g_print("Fenêtre Modifier affichée avec succès.\n");
}


void
on_btn_supprimer_ww_clicked            (GtkWidget       *button,
                                        gpointer         user_data)
{
 GtkWidget *window_supprimer, *combobox;

    // Créer la fenêtre "Supprimer"
    window_supprimer = create_window_supprimer_ww();

    // Récupérer la combobox
    combobox = lookup_widget(window_supprimer, "comboboxentry_supprimer_ww");

    // Remplir la combobox avec les IDs disponibles
    remplir_combobox_ids(combobox);

    // Afficher la fenêtre
    gtk_widget_show_all(window_supprimer);
}


void
on_btn_afficher_ww_clicked             (GtkButton       *button,
                                        gpointer         user_data)
{
GtkWidget *window_afficher, *treeview;

    // Créer la fenêtre "Afficher" si elle n'existe pas encore
    window_afficher = create_window_afficher_ww();
    if (!window_afficher) {
        g_print("Erreur : Impossible de créer ou de trouver la fenêtre Afficher.\n");
        return;
    }

    // Récupérer le TreeView dans la fenêtre d'affichage
    treeview = lookup_widget(window_afficher, "treeview_ww");

    // Appeler la fonction d'affichage des services
    afficherService(treeview);

    // Afficher la fenêtre "Afficher"
    gtk_widget_show_all(window_afficher);
}


void
on_btn_affecter_ww_clicked             (GtkButton       *button,
                                        gpointer         user_data)
{

}


void
on_btn_retour1_clicked                 (GtkButton       *button,
                                        gpointer         user_data)
{

}


void on_btn_ajouterservice_ww_clicked(GtkWidget *widget, gpointer user_data) {
    // Déclaration des widgets
    GtkWidget *entry_id, *entry_nom, *entry_description, *entry_prix, *output;
    Service service;
    char message[200];

    // Récupérer les widgets par leur nom
    entry_id = lookup_widget(widget, "entry_id_ww");
    entry_nom = lookup_widget(widget, "entry_nom_ww");
    entry_description = lookup_widget(widget, "entry_description_ww");
    entry_prix = lookup_widget(widget, "entry_prix_ww");
    output = lookup_widget(widget, "label_ajouter_ww");

    // Récupérer les données des widgets
    const char *id_str = gtk_entry_get_text(GTK_ENTRY(entry_id));
    const char *nom = gtk_entry_get_text(GTK_ENTRY(entry_nom));
    const char *description = gtk_entry_get_text(GTK_ENTRY(entry_description));
    const char *prix_str = gtk_entry_get_text(GTK_ENTRY(entry_prix));

    // Vérifications de base sur les champs
    if (strlen(id_str) == 0 || strlen(nom) == 0 || strlen(description) == 0 || strlen(prix_str) == 0) {
        gtk_label_set_text(GTK_LABEL(output), "Erreur : Tous les champs sont obligatoires.");
        return;
    }

    // Conversion et vérification des données
    service.id = atoi(id_str);
    service.tarif = atof(prix_str);
    if (service.id <= 0 || service.tarif <= 0) {
        gtk_label_set_text(GTK_LABEL(output), "Erreur : ID et tarif doivent être des valeurs positives.");
        return;
    }

    // Stocker les chaînes dans la structure Service
    strncpy(service.nom, nom, sizeof(service.nom) - 1);
    strncpy(service.description, description, sizeof(service.description) - 1);

    // Appeler la fonction pour ajouter le service
    ajouterService(service);

    // Afficher un message de confirmation
    snprintf(message, sizeof(message), "Service ajouté : ID=%d, Nom=%s", service.id, service.nom);
    gtk_label_set_text(GTK_LABEL(output), message);

    // Réinitialiser les champs après ajout
    gtk_entry_set_text(GTK_ENTRY(entry_id), "");
    gtk_entry_set_text(GTK_ENTRY(entry_nom), "");
    gtk_entry_set_text(GTK_ENTRY(entry_description), "");
    gtk_entry_set_text(GTK_ENTRY(entry_prix), "");
}

void on_btn_retour_ww_clicked(GtkWidget *button, gpointer user_data) {
    GtkWidget *current_window, *main_window;

    // Récupérer la fenêtre actuelle (celle contenant le bouton "Retour")
    current_window = gtk_widget_get_toplevel(button);

    // Récupérer la fenêtre principale
    main_window = lookup_widget(button, "main_window_ww");

    // Cacher la fenêtre actuelle
    gtk_widget_hide(current_window);

    // Afficher la fenêtre principale
    gtk_widget_show(main_window);
}


void on_btn_supprimerservice_ww_clicked(GtkButton *button, gpointer user_data) {
    GtkWidget *comboboxentry_id, *output_label;
    GtkTreeModel *model;
    GtkTreeIter iter;
    const gchar *id_str = NULL;
    int id;

    // Récupérer les widgets nécessaires
    comboboxentry_id = lookup_widget(GTK_WIDGET(button), "comboboxentry_supprimer_ww");
    output_label = lookup_widget(GTK_WIDGET(button), "label_message_supprimer");

    // Vérifier si un ID a été sélectionné dans la combobox
    model = gtk_combo_box_get_model(GTK_COMBO_BOX(comboboxentry_id));
    if (gtk_combo_box_get_active_iter(GTK_COMBO_BOX(comboboxentry_id), &iter)) {
        gtk_tree_model_get(model, &iter, 0, &id_str, -1);
    }

    if (id_str == NULL || strlen(id_str) == 0) {
        gtk_label_set_text(GTK_LABEL(output_label), "Erreur : Aucun ID sélectionné.");
        return;
    }

    // Conversion de l'ID en entier
    id = atoi(id_str);

    // Appeler la fonction pour supprimer le service
    supprimerService(id);

    // Afficher un message de confirmation
    char message[100];
    snprintf(message, sizeof(message), "Service avec ID=%d supprimé avec succès.", id);
    gtk_label_set_text(GTK_LABEL(output_label), message);

    // Optionnel : Réinitialiser la combobox après suppression
    remplir_combobox_ids(comboboxentry_id);
}


void
on_btn_affecterservice_ww_clicked      (GtkButton       *button,
                                        gpointer         user_data)
{

}

void remplir_combobox_ids(GtkWidget *combobox) {
    GtkListStore *store;
    GtkTreeIter iter;

    // Récupérer le modèle de la combobox
    store = GTK_LIST_STORE(gtk_combo_box_get_model(GTK_COMBO_BOX(combobox)));
    if (!store) return;

    // Supprimer les anciennes options
    gtk_list_store_clear(store);

    // Ouvrir le fichier contenant les services
    FILE *fichier = fopen("services.txt", "r");
    if (fichier == NULL) {
        perror("Erreur lors de l'ouverture du fichier services.txt");
        return;
    }

    // Lire les IDs depuis le fichier et les ajouter dans le combobox
    Service service;
    while (fscanf(fichier, "%d;%49s;%99s;%f\n", &service.id, service.nom, service.description, &service.tarif) != EOF) {
        char id_str[10];
        snprintf(id_str, sizeof(id_str), "%d", service.id);

        // Ajouter l'ID dans le modèle
        gtk_list_store_append(store, &iter);
        gtk_list_store_set(store, &iter, 0, id_str, -1);
    }

    fclose(fichier);
}

void on_btn_modifierservice_ww_clicked(GtkButton *button, gpointer user_data) {
    // Déclaration des widgets
    GtkWidget *comboboxentry_id, *entry_nom, *entry_description, *entry_prix, *output_label;
    GtkTreeModel *model;
    GtkTreeIter iter;
    const gchar *id_str = NULL;
    Service service;
    char message[200];
    int found = 0;

    // Récupérer les widgets par leur nom
    comboboxentry_id = lookup_widget(GTK_WIDGET(button), "comboboxentry_modifier_ww");
    entry_nom = lookup_widget(GTK_WIDGET(button), "entry_modifiernom_ww");
    entry_description = lookup_widget(GTK_WIDGET(button), "entry_modifierdescription_ww");
    entry_prix = lookup_widget(GTK_WIDGET(button), "entry_modifierprix_ww");
    output_label = lookup_widget(GTK_WIDGET(button), "label_modifier_ww");

    // Récupérer l'ID sélectionné dans le GtkComboBoxEntry
    model = gtk_combo_box_get_model(GTK_COMBO_BOX(comboboxentry_id));
    if (gtk_combo_box_get_active_iter(GTK_COMBO_BOX(comboboxentry_id), &iter)) {
        gtk_tree_model_get(model, &iter, 0, &id_str, -1); // Récupère l'ID en tant que chaîne
    }

    if (id_str == NULL || strlen(id_str) == 0) {
        gtk_label_set_text(GTK_LABEL(output_label), "Erreur : Aucun ID sélectionné.");
        return;
    }

    // Conversion de l'ID
    service.id = atoi(id_str);

    // Vérifier si l'ID existe dans le fichier (lecture du fichier)
    FILE *fichier = fopen("services.txt", "r");
    if (fichier == NULL) {
        gtk_label_set_text(GTK_LABEL(output_label), "Erreur : Impossible d'ouvrir le fichier des services.");
        return;
    }

    // Rechercher l'ID dans le fichier
    Service temp_service;
    while (fscanf(fichier, "%d;%49s;%99s;%f\n", &temp_service.id, temp_service.nom, temp_service.description, &temp_service.tarif) != EOF) {
        if (temp_service.id == service.id) {
            found = 1;
            break;
        }
    }
    fclose(fichier);

    if (!found) {
        gtk_label_set_text(GTK_LABEL(output_label), "Erreur : ID non trouvé.");
        return;
    }

    // Récupérer les nouvelles données des champs GtkEntry
    const char *nom = gtk_entry_get_text(GTK_ENTRY(entry_nom));
    const char *description = gtk_entry_get_text(GTK_ENTRY(entry_description));
    const char *prix_str = gtk_entry_get_text(GTK_ENTRY(entry_prix));

    if (strlen(nom) == 0 || strlen(description) == 0 || strlen(prix_str) == 0) {
        gtk_label_set_text(GTK_LABEL(output_label), "Erreur : Tous les champs doivent être remplis.");
        return;
    }

    // Conversion du tarif
    service.tarif = atof(prix_str);
    if (service.tarif <= 0) {
        gtk_label_set_text(GTK_LABEL(output_label), "Erreur : Le tarif doit être une valeur positive.");
        return;
    }

    // Stocker les données dans la structure Service
    strncpy(service.nom, nom, sizeof(service.nom) - 1);
    strncpy(service.description, description, sizeof(service.description) - 1);

    // Appeler la fonction de modification
    modifierService(service.id, service);

    // Afficher un message de confirmation
    snprintf(message, sizeof(message), "Service modifié avec succès : ID=%d, Nom=%s", service.id, service.nom);
    gtk_label_set_text(GTK_LABEL(output_label), message);

    // Réinitialiser les champs après la modification (si nécessaire)
    gtk_entry_set_text(GTK_ENTRY(entry_nom), "");
    gtk_entry_set_text(GTK_ENTRY(entry_description), "");
    gtk_entry_set_text(GTK_ENTRY(entry_prix), "");
}


void
on_btn_afficherservice_ww_clicked      (GtkButton       *button,
                                        gpointer         user_data)
{
 GtkWidget *treeview;
    GtkListStore *store;

    // Récupérer le widget TreeView dans la fenêtre actuelle
    treeview = lookup_widget(GTK_WIDGET(button), "treeview_ww");

    if (!treeview) {
        g_print("Erreur : treeview_ww non trouvé.\n");
        return;
    }

    // Effacer les anciennes données (si nécessaire)
    store = GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(treeview)));
    if (store) {
        gtk_list_store_clear(store);
    }

    // Appeler la fonction d'affichage des services de service.c
    afficherService(treeview);
}




