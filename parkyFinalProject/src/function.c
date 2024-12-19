#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "function.h"
#include <math.h>
#include <curl/curl.h>
// Fichier de stockage des données des services
#define FICHIER_SERVICES "services.txt"
#define FICHIER_RESERVATIONS "reservations.txt"

int is_valid_email(char *email) {
    return strchr(email, '@') != NULL;
}

int password_strength(char *password) {
    int length = strlen(password);
    int has_lower = 0, has_upper = 0, has_digit = 0, has_special = 0;

    if (length < 8) return 0; // Too short

    for (int i = 0; i < length; i++) {
        if (islower(password[i])) has_lower = 1;
        else if (isupper(password[i])) has_upper = 1;
        else if (isdigit(password[i])) has_digit = 1;
        else if (strchr("!@#$%^&*()-_=+[]{};:'\",.<>?/\\|", password[i])) has_special = 1;
    }

    if (has_lower && has_digit && has_upper && has_special) return 3; // Strong
    if (has_lower && has_digit) return 2;                            // Normal
    if (has_lower) return 1;                                         // Weak
    return 0;                                                        // Invalid
}

int signup(char *email, char *password, char *nom, char *prenom) {
    // Validate email
    if (!is_valid_email(email)) {
        printf("Invalid email! Please include an '@' symbol.\n");
        return 0;
    }

    // Validate password strength
    int strength = password_strength(password);
    if (strength == 0) {
        printf("Password must be at least 8 characters long and contain a mix of uppercase letters, digits, and symbols.\n");
        return 0;
    } else if (strength == 1) {
        printf("Weak password! Your password only contains lowercase letters.\n");
        return 0;
    } else if (strength == 2) {
        printf("Normal password. Consider adding symbols for better security.\n");
    } else if (strength == 3) {
        printf("Strong password! Well done.\n");
    }

    // Open file for appending
    FILE *f = fopen("signup.txt", "a");
    if (f == NULL) {
        printf("Error opening file for signup.\n");
        return 0;
    }

    // Save user details
    fprintf(f, "%s %s %s %s %s \n", nom, prenom, email, password,"citoyen");
    fclose(f);

    printf("Signup successful!\n");
    return 1;
}


int signin(char *email, char *password,char *userType) {
    // Open the file in read mode
    FILE *f = fopen("signup.txt", "r");
    if (f == NULL) {
        printf("Error: Could not open signup.txt.\n");
        return 0;
    }

    Users temp;
    int found = 0;

    // Read user data from the file
    while (fscanf(f, "%49s %49s %49s %49s %49s", temp.nom, temp.prenom, temp.email, temp.mdp,temp.type) == 5) {
        if (strcmp(temp.email, email) == 0 && strcmp(temp.mdp, password) == 0) {
            printf("Signin successful!\n"); 
	snprintf(userType, 256,temp.type);
            found = 1;
            break;
        }
    }

    fclose(f); // Ensure the file is closed
    return found;
} 

int emailExist(char *email,char *errorMessage){
    // Open the file in read mode
    FILE *f = fopen("signup.txt", "r");
    if (f == NULL) {
        printf("Error: Could not open signup.txt.\n");
       
        return 0;
    }

    Users temp;
    int found = 0;

    // Read user data from the file
    while (fscanf(f, "%49s %49s %49s %49s %49s", temp.nom, temp.prenom, temp.email, temp.mdp,temp.type) == 5) {
        if (strcmp(temp.email, email) == 0) {
            printf("email verified!\n");
            snprintf(errorMessage, 256, "email verified!");
            found = 1;
            break;
        }else {snprintf(errorMessage, 256, "email dont exist!");}
    }

    fclose(f); // Ensure the file is closed
    return found;
} 

int ajouter(char *citoyenFichier, citoyen c,char *errorMessage) {
     printf( "%s %s %d %s %s %s %d %s\n", c.nom, c.prenom, c.cin, c.ville, c.modele, c.matricule, c.age, c.sexe);
    FILE *f = fopen(citoyenFichier, "a+");
    if (f == NULL) {
        printf("Error opening file.\n");
        return 0;
    }

    citoyen temp;
    while (fscanf(f, "%s %s %d %s %s %s %d %s", temp.nom, temp.prenom, &temp.cin, temp.ville, temp.modele, temp.matricule, &temp.age, temp.sexe) != EOF) {
        if (temp.cin == c.cin) {
            printf("CIN already exists. Cannot add citoyen.\n");
snprintf(errorMessage, 256, "CIN already exists. Cannot add citoyen.");
            fclose(f);
            return 0;
        }
    }
    int length = (c.cin == 0) ? 1 : (int)log10(abs(c.cin)) + 1;
    if (length != 8) {
        printf("CIN should contain exactly 8 digits.\n");
 snprintf(errorMessage, 256, "CIN should contain exactly 8 digits.");
        fclose(f);
        return 0;
    }
    fprintf(f, "%s %s %d %s %s %s %d %s\n", c.nom, c.prenom, c.cin, c.ville, c.modele, c.matricule, c.age, c.sexe);
    fclose(f);
    return 1;
}

int modifier(char *citoyenFichier, int cin, citoyen nouv, char *errorMessage) {
    citoyen c;
    int done = 0;
    int length = (nouv.cin == 0) ? 1 : (int)log10(abs(nouv.cin)) + 1;
    if (length != 8) {
        printf("CIN should contain exactly 8 digits.\n");
snprintf(errorMessage, 256, "CIN should contain exactly 8 digits");
        return 0;
    }
int length2 = (cin == 0) ? 1 : (int)log10(abs(cin)) + 1;
    if (length2 != 8) {
        printf("CIN of the citoyen to update should contain exactly 8 digits.\n");
       snprintf(errorMessage, 256, "CIN of the citoyen to update should contain exactly 8 digits.");
        return 0;
    }

    FILE *f = fopen(citoyenFichier, "r");
    FILE *f2 = fopen("temp.txt", "w");

    if (f == NULL || f2 == NULL) {
        printf("Error opening file.\n");
        return 0;
    }

    while (fscanf(f, "%s %s %d %s %s %s %d %s", c.nom, c.prenom, &c.cin, c.ville, c.modele, c.matricule, &c.age, c.sexe) == 8) {
        if (c.cin == cin) {
            fprintf(f2, "%s %s %d %s %s %s %d %s\n", nouv.nom, nouv.prenom, nouv.cin, nouv.ville, nouv.modele, nouv.matricule, nouv.age, nouv.sexe);
            done = 1;
        } else {
            fprintf(f2, "%s %s %d %s %s %s %d %s\n", c.nom, c.prenom, c.cin, c.ville, c.modele, c.matricule, c.age, c.sexe);
        }
    }
    if(done==0){
printf("citoyen nexiste pas \n");
       snprintf(errorMessage, 256, "citoyen nexiste pas ");
   }

    fclose(f);
    fclose(f2);
    remove(citoyenFichier);
    rename("temp.txt", citoyenFichier);

    return done;
}

int supprimer(char *citoyenFichier, int cin) {
    int done = 0;
    citoyen c;
    FILE *f = fopen(citoyenFichier, "r");
    FILE *f2 = fopen("temp.txt", "w");

    if (f == NULL || f2 == NULL) return 0;

    while (fscanf(f, "%s %s %d %s %s %s %d %s", c.nom, c.prenom, &c.cin, c.ville, c.modele, c.matricule, &c.age, c.sexe) == 8) {
        if (c.cin != cin) {
            fprintf(f2, "%s %s %d %s %s %s %d %s\n", c.nom, c.prenom, c.cin, c.ville, c.modele, c.matricule, c.age, c.sexe);

        } else {
            done = 1;
        }
    }

    fclose(f);
    fclose(f2);

    remove(citoyenFichier);
    rename("temp.txt", citoyenFichier);

    return done;
}

int loadParkingData(char *filename, Parking parkings[], int maxCount) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Error opening parking file: %s\n", filename);
        return -1;
    }

    int count = 0;
    while (count < maxCount && fscanf(file, "%d,%d,%49s",
            &parkings[count].idParking, &parkings[count].priceParking, parkings[count].nameParking) == 3) {
        count++;
    }
    fclose(file);
    return count;
}

int loadServiceData(char *filename, Service services[], int maxCount) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Error opening service file: %s\n", filename);
        return -1;
    }

    int count = 0;
    while (count < maxCount && fscanf(file, "%d,%d,%49s",
            &services[count].idService, &services[count].priceService, services[count].nameService) == 3) {
        count++;
    }
    fclose(file);
    return count;
}

void generateFacture(const char *resFile, const char *srvFile, const char *prkFile, const char *factureFile, int targetIdCitoyen,char *totaleMessage) {
    Parking parkings[100];
    Service services[100];
    Reservation res;
    int parkingCount = loadParkingData(prkFile, parkings, 100);
    int serviceCount = loadServiceData(srvFile, services, 100);

    if (parkingCount < 0 || serviceCount < 0) {
        printf("Error loading data.\n");
        return;
    }

    FILE *resFilePtr = fopen(resFile, "r");
    FILE *factureFilePtr = fopen(factureFile, "w");

    if (!resFilePtr || !factureFilePtr) {
        printf("Error opening reservation or facture file.\n");
        return;
    }

    fprintf(factureFilePtr, "Date\t\tHour\t\tNameParking\t\tNameService\t\tPrice\n");
    int totalPrice = 0;
    int recordFound = 0;

    while (fscanf(resFilePtr, "%d,%10[^,],%5[^,],%d,%d", &res.idCitoyen, res.date, res.hour, &res.idParking, &res.idService) == 5) {
        // Only process reservation if the idCitoyen matches the target ID
        if (res.idCitoyen != targetIdCitoyen) {
            continue;  // Skip this reservation if the ID does not match
        }

        int parkingPrice = 0, servicePrice = 0;
        char parkingName[50] = "Unknown";
        char serviceName[50] = "Unknown";

        // Find the parking corresponding to the reservation
        for (int i = 0; i < parkingCount; i++) {
            if (parkings[i].idParking == res.idParking) {
                parkingPrice = parkings[i].priceParking;
                strcpy(parkingName, parkings[i].nameParking);
                break;
            }
        }

        // Find the service corresponding to the reservation
        for (int i = 0; i < serviceCount; i++) {
            if (services[i].idService == res.idService) {
                servicePrice = services[i].priceService;
                strcpy(serviceName, services[i].nameService);
                break;
            }
        }

        int price = parkingPrice + servicePrice;
        totalPrice += price;
        fprintf(factureFilePtr, "%s\t%-5s\t%-16s\t%-16s\t%d\n", res.date, res.hour, parkingName, serviceName, price);
        recordFound = 1;
    }

    if (!recordFound) {
        fprintf(factureFilePtr, "No reservations found for Citizen ID: %d.\n", targetIdCitoyen);
    } else {
        fprintf(factureFilePtr, "\nTotal\t\t\t\t\t\t\t%d\n", totalPrice);
   char str[350];
      sprintf(str, "%d", totalPrice);
       snprintf(totaleMessage, 256,str );
    }

    fclose(resFilePtr);
    fclose(factureFilePtr);

    printf("Facture generated in %s\n", factureFile);
}


void searchByMonth(const char *factureFile, int month,char *totaleMessage) {
    FILE *file = fopen(factureFile, "r");
    FILE *searchFile = fopen("search.txt", "w");

    if (!file || !searchFile) {
        printf("Error opening file.\n");
        return;
    }

    char line[256];       // Buffer to hold each line
    int recordFound = 0;  // Flag to indicate if any records are found
    int totalPrice=0 ;
    // Skip the header line
    fgets(line, sizeof(line), file);

    // Write the header to search.txt
    fprintf(searchFile, "Date\t\tHour\t\tNameParking\t\tNameService\t\tPrice\n");

    // Process each line in the file
    while (fgets(line, sizeof(line), file)) {
        char date[11], hour[6], nameParking[50], nameService[50];
        int price;

        // Parse the line including the hour field
        if (sscanf(line, "%10s\t%5s\t%49[^\t]\t%49[^\t]\t%d", date, hour, nameParking, nameService, &price) == 5) {
            int recordMonth;
            sscanf(date, "%*4d-%2d-%*2d", &recordMonth); // Extract month from date (YYYY-MM-DD format)

            if (recordMonth == month) {
                // Write the matching record to search.txt
                fprintf(searchFile, "%s\t%-5s\t%-16s\t%-16s\t%d\n", date, hour, nameParking, nameService, price);
                 totalPrice += price; 
                 recordFound = 1;
            }
        }
    }

    if (!recordFound) {
        fprintf(searchFile, "No records found for the specified month.\n");
         snprintf(totaleMessage, 256, "0");
    }else {
        char str[350];
      sprintf(str, "%d", totalPrice);
       snprintf(totaleMessage, 256,str );
    }

    fclose(file);
    fclose(searchFile);

    printf("Search results have been saved to search.txt\n");
}



	void add_row(GtkListStore *list_store, const gchar *attribute, const gchar *value) {
    GtkTreeIter iter;

    // Add a new row
    gtk_list_store_append(list_store, &iter);

    // Set values for the row (2 columns: attribute and value)
    gtk_list_store_set(list_store, &iter,
                       0, attribute,  // Column 0: Attribute name
                       1, value,      // Column 1: Value
                       -1);
}

void populate_treeview(GtkWidget *treeview, const char *filename, int target_cin) {
    FILE *file = fopen(filename, "r");
    citoyen c;
    char buffer[50];
    int found = 0;

    // Create the ListStore model
    GtkListStore *list_store = gtk_list_store_new(2, G_TYPE_STRING, G_TYPE_STRING);

    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    // Read the file using fscanf
    while (fscanf(file, "%s %s %d %s %s %s %d %s\n",
                  c.nom, c.prenom, &c.cin, c.ville, c.modele, c.matricule, &c.age, c.sexe) != EOF) {
        // Debugging: Print parsed data
        g_print("Parsed data: Name: %s, Surname: %s, CIN: %d, City: %s, Model: %s, Plate: %s, Age: %d, Gender: %s\n",
                c.nom, c.prenom, c.cin, c.ville, c.modele, c.matricule, c.age, c.sexe);

        // Compare CIN
        if (c.cin == target_cin) {
            found = 1;

            // Add rows for each attribute of the citoyen
            add_row(list_store, "Name", c.nom);
            add_row(list_store, "Surname", c.prenom);
            snprintf(buffer, sizeof(buffer), "%d", c.cin);  // Convert CIN to string
            add_row(list_store, "CIN", buffer);
            add_row(list_store, "City", c.ville);
            add_row(list_store, "Car Model", c.modele);
            add_row(list_store, "License Plate", c.matricule);
            snprintf(buffer, sizeof(buffer), "%d", c.age);  // Convert age to string
            add_row(list_store, "Age", buffer);
            add_row(list_store, "Gender", c.sexe);

            break;  // Exit loop after finding the target citoyen
        }
    }

    fclose(file);

    if (found==0) {
        g_print("Citizen with CIN %d not found in the file.\n", target_cin);
    } else {
        g_print("Citizen with CIN %d found.\n", target_cin);
    }

    // Set the ListStore model to the TreeView
    gtk_tree_view_set_model(GTK_TREE_VIEW(treeview), GTK_TREE_MODEL(list_store));
    g_object_unref(list_store);

    // Add columns dynamically if they are not already set
    if (gtk_tree_view_get_columns(GTK_TREE_VIEW(treeview)) == 0) {
        GtkCellRenderer *renderer;
        GtkTreeViewColumn *column;

        // Create the "Attribute" column
        renderer = gtk_cell_renderer_text_new();
        column = gtk_tree_view_column_new_with_attributes("Attribute", renderer, "text", 0, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);

        // Create the "Value" column
        renderer = gtk_cell_renderer_text_new();
        column = gtk_tree_view_column_new_with_attributes("Value", renderer, "text", 1, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);
    }
}  

void add_row2(GtkListStore *list_store, const char *date, const char *hour, const char *nameParking, const char *nameService, int price) {
    GtkTreeIter iter;
    gtk_list_store_append(list_store, &iter);
    gtk_list_store_set(list_store, &iter, 0, date, 1, hour, 2, nameParking, 3, nameService, 4, price, -1);
} 

void populate_treeview_with_file(GtkWidget *treeview, const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Error opening file: %s\n", filename);
        return;
    }

    // Create a ListStore with 5 columns
    GtkListStore *list_store = gtk_list_store_new(5, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_INT);

    // Read and parse the data from the file
    char line[256];
    while (fgets(line, sizeof(line), file)) {
        char date[11], hour[6], nameParking[50], nameService[50];
        int price;

        // Parse the line
        if (sscanf(line, "%10s\t%5s\t%49[^\t]\t%49[^\t]\t%d", date, hour, nameParking, nameService, &price) == 5) {
            // Add the row to the ListStore
            add_row2(list_store, date, hour, nameParking, nameService, price);
        }
    }

    fclose(file);

    // Set the ListStore model to the TreeView
    gtk_tree_view_set_model(GTK_TREE_VIEW(treeview), GTK_TREE_MODEL(list_store));
    g_object_unref(list_store);  // Unreference the list_store after setting the model

    // Add columns if not already added
    if (gtk_tree_view_get_columns(GTK_TREE_VIEW(treeview)) == 0) {
        GtkCellRenderer *renderer;
        GtkTreeViewColumn *column;

        // Create the "Date" column
        renderer = gtk_cell_renderer_text_new();
        column = gtk_tree_view_column_new_with_attributes("Date", renderer, "text", 0, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);

        // Create the "Hour" column
        renderer = gtk_cell_renderer_text_new();
        column = gtk_tree_view_column_new_with_attributes("Hour", renderer, "text", 1, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);

        // Create the "NameParking" column
        renderer = gtk_cell_renderer_text_new();
        column = gtk_tree_view_column_new_with_attributes("NameParking", renderer, "text", 2, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);

        // Create the "NameService" column
        renderer = gtk_cell_renderer_text_new();
        column = gtk_tree_view_column_new_with_attributes("NameService", renderer, "text", 3, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);

        // Create the "Price" column
        renderer = gtk_cell_renderer_text_new();
        column = gtk_tree_view_column_new_with_attributes("Price", renderer, "text", 4, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);
    }
} 




void show_facture(GtkWidget *treeview) {
    populate_treeview_with_file(treeview, "facture.txt");
}
void show_search(GtkWidget *treeview) {
    populate_treeview_with_file(treeview, "search.txt");
} 

// Function to clear all rows in a GtkTreeView
void viderTreeView(GtkWidget *treeview) {
    if (!treeview) {
        g_print("Error: TreeView widget is NULL.\n");
        return;
    }

    if (!GTK_IS_TREE_VIEW(treeview)) {
        g_print("Error: The widget provided is not a GtkTreeView.\n");
        return;
    }

    GtkTreeModel *model = gtk_tree_view_get_model(GTK_TREE_VIEW(treeview));

    if (GTK_IS_LIST_STORE(model)) {
        // If the model is a GtkListStore, clear it
        GtkListStore *list_store = GTK_LIST_STORE(model);
        gtk_list_store_clear(list_store);
    } else if (GTK_IS_TREE_STORE(model)) {
        // If the model is a GtkTreeStore, clear it
        GtkTreeStore *tree_store = GTK_TREE_STORE(model);
        gtk_tree_store_clear(tree_store);
    } else {
        g_print("Error: TreeView does not have a valid ListStore or TreeStore.\n");
    }
}

void add_row5(GtkListStore *list_store, const char *nom, const char *prenom, int cin,
             const char *ville, const char *modele, const char *matricule, int age, const char *sexe) {
    GtkTreeIter iter;
    gtk_list_store_append(list_store, &iter);
    gtk_list_store_set(list_store, &iter,
                       0, nom,
                       1, prenom,
                       2, cin,
                       3, ville,
                       4, modele,
                       5, matricule,
                       6, age,
                       7, sexe,
                       -1);
}
void populate_treeview_with_citoyens(GtkWidget *treeview, const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Error opening file: %s\n", filename);
        return;
    }

    // Create a ListStore with 8 columns
    GtkListStore *list_store = gtk_list_store_new(8, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_INT,
                                                  G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING,
                                                  G_TYPE_INT, G_TYPE_STRING);

    citoyen c;

    // Read and parse the data from the file
    while (fscanf(file, "%49s %49s %d %49s %49s %49s %d %9s",
                  c.nom, c.prenom, &c.cin, c.ville, c.modele, c.matricule, &c.age, c.sexe) == 8) {
        // Add the row to the ListStore
        add_row5(list_store, c.nom, c.prenom, c.cin, c.ville, c.modele, c.matricule, c.age, c.sexe);
    }

    fclose(file);

    // Set the ListStore model to the TreeView
    gtk_tree_view_set_model(GTK_TREE_VIEW(treeview), GTK_TREE_MODEL(list_store));
    g_object_unref(list_store);  // Unreference the list_store after setting the model

    // Add columns if not already added
    if (gtk_tree_view_get_columns(GTK_TREE_VIEW(treeview)) == 0) {
        GtkCellRenderer *renderer;
        GtkTreeViewColumn *column;

        // Create the "Name" column
        renderer = gtk_cell_renderer_text_new();
        column = gtk_tree_view_column_new_with_attributes("Name", renderer, "text", 0, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);

        // Create the "Surname" column
        renderer = gtk_cell_renderer_text_new();
        column = gtk_tree_view_column_new_with_attributes("Surname", renderer, "text", 1, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);

        // Create the "CIN" column
        renderer = gtk_cell_renderer_text_new();
        column = gtk_tree_view_column_new_with_attributes("CIN", renderer, "text", 2, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);

        // Create the "City" column
        renderer = gtk_cell_renderer_text_new();
        column = gtk_tree_view_column_new_with_attributes("City", renderer, "text", 3, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);

        // Create the "Car Model" column
        renderer = gtk_cell_renderer_text_new();
        column = gtk_tree_view_column_new_with_attributes("Car Model", renderer, "text", 4, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);

        // Create the "License Plate" column
        renderer = gtk_cell_renderer_text_new();
        column = gtk_tree_view_column_new_with_attributes("License Plate", renderer, "text", 5, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);

        // Create the "Age" column
        renderer = gtk_cell_renderer_text_new();
        column = gtk_tree_view_column_new_with_attributes("Age", renderer, "text", 6, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);

        // Create the "Gender" column
        renderer = gtk_cell_renderer_text_new();
        column = gtk_tree_view_column_new_with_attributes("Gender", renderer, "text", 7, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);
    }
}


// Callback function to write data (needed for reading the email body)
size_t payload_source(void *ptr, size_t size, size_t nmemb, void *userp) {
    const char **payload_text = (const char **)userp;
    size_t len = strlen(*payload_text);
    
    // Copy the data into ptr
    if (len) {
        size_t copy_len = size * nmemb < len ? size * nmemb : len;
        memcpy(ptr, *payload_text, copy_len);
        *payload_text += copy_len; // Move pointer forward
        return copy_len;
    }
    return 0;
}

void send_email(const char *recipient, const char *subject, const char *body) {
    CURL *curl;
    CURLcode res;
    struct curl_slist *recipients = NULL;
    
    const char *from = "parky.esprit@gmail.com";
    const char *password = "zvnkktsjxmgocwrr"; // Use App Password here
    
    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_USERNAME, from);
        curl_easy_setopt(curl, CURLOPT_PASSWORD, password);
        curl_easy_setopt(curl, CURLOPT_URL, "smtps://smtp.gmail.com:465");
        
        // Enable SSL
        curl_easy_setopt(curl, CURLOPT_USE_SSL, CURLUSESSL_ALL);
        
        // Sender
        curl_easy_setopt(curl, CURLOPT_MAIL_FROM, from);
        
        // Recipient
        recipients = curl_slist_append(recipients, recipient);
        curl_easy_setopt(curl, CURLOPT_MAIL_RCPT, recipients);
        
        // Create email body with headers and body content
        char payload[1024];  // Increased buffer size
        snprintf(payload, sizeof(payload),
                 "To: %s\r\n"
                 "From: %s\r\n"
                 "Subject: %s\r\n"
                 "\r\n"
                 "%s\r\n", recipient, from, subject, body);
        
        // Set up the payload source (use callback to handle the payload)
        const char *payload_text = payload;
        curl_easy_setopt(curl, CURLOPT_READFUNCTION, payload_source);
        curl_easy_setopt(curl, CURLOPT_READDATA, (void *)&payload_text);
        
        // Enable uploading of data
        curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
        
        // Debug output
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
        
        // Perform the request
        res = curl_easy_perform(curl);
        
        // Handle result
        if (res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        } else {
            printf("Email sent successfully!\n");
        }
        
        // Cleanup
        curl_slist_free_all(recipients);
        curl_easy_cleanup(curl);
    }
}

void changePassword(const char *email, const char *newPassword) {
    FILE *file = fopen("signup.txt", "r");
    if (file == NULL) {
        perror("Error opening signup.txt");
        return;
    }

    FILE *tempFile = fopen(TEMP_FILE, "w");
    if (tempFile == NULL) {
        perror("Error opening temporary file");
        fclose(file);
        return;
    }

    char firstName[51], lastName[51], currentEmail[101], currentPassword[51],type[51];
    int emailFound = 0;

    // Read each record from the file
    while (fscanf(file, "%50s %50s %100s %50s %50s", firstName, lastName, currentEmail, currentPassword,type) == 5) {
        if (strcmp(currentEmail, email) == 0) {
            emailFound = 1;
            fprintf(tempFile, "%s %s %s %s %s\n", firstName, lastName, currentEmail, newPassword,type);
        } else {
            fprintf(tempFile, "%s %s %s %s %s\n", firstName, lastName, currentEmail, currentPassword,type);
        }
    }

    fclose(file);
    fclose(tempFile);

    if (emailFound) {
        // Replace the original file with the updated file
        if (remove("signup.txt") != 0 || rename(TEMP_FILE, "signup.txt") != 0) {
            perror("Error updating signup.txt");
        } else {
            printf("Password updated successfully.\n");
        }
    } else {
        printf("Email not found.\n");
        remove(TEMP_FILE); // Remove the temporary file if email not found
    }
}
////////////////////////////////////////////////////////////////////////////////ilyessdalhoumi
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
///////////////////////////////////////////////////////////////itaf
int trouver_place_libre(int *places, int taille) {
    for (int i = 0; i < taille; i++) {
        if (places[i] == 0) { // Si la place est libre
            places[i] = 1;    // Marquer la place comme occupée
            return i + 1;     // Retourner le numéro de la place (index + 1)
        }
    }
    return -1; // Retourner -1 si aucune place n'est disponible
}

// Fonction pour ajouter une réservation
int ajouter(const char *filename, reservation *res, int *places, int taille) {
    // Trouver une place disponible
    res->place = trouver_place_libre(places, taille);
    if (res->place == -1) {
        printf("Erreur : Aucune place disponible.\n");
        return 0; // Retourne 0 si aucune place n'est disponible
    }

    // Ouvrir le fichier en mode ajout
    FILE *file = fopen(filename, "a");
    if (file == NULL) {
        printf("Erreur : Impossible d'ouvrir le fichier %s.\n", filename);
        return 0; // Retourne 0 en cas d'échec
    }

    // Écriture des informations de la réservation dans le fichier
    fprintf(file, "%d %d %04d-%02d-%02d %d\n", 
            res->cin, 
            res->place, 
            res->res_date.annee, 
            res->res_date.mois, 
            res->res_date.jour, 
            res->duree
    );

    fclose(file); // Fermeture du fichier
    printf("Réservation ajoutée avec succès : Place attribuée %d.\n", res->place);
    return 1; // Retourne 1 en cas de succès
}

// Fonction pour afficher les réservations d'une personne par CIN
int afficher(const char *filename, int cin, char *output) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Erreur : Impossible d'ouvrir le fichier %s.\n", filename);
        return 0;  // Return failure if file can't be opened
    }

    reservation res;
    int found = 0;

    while (fscanf(file, "%d %d %d-%d-%d %d\n", 
                  &res.cin, 
                  &res.place, 
                  &res.res_date.annee, 
                  &res.res_date.mois, 
                  &res.res_date.jour, 
                  &res.duree) == 6) {
        if (res.cin == cin) {
            // Format the output string for the label
            sprintf(output, "Réservations pour CIN %d:\nPlace: %d, Date: %04d-%02d-%02d, Durée: %d heures",
                    res.cin, res.place, res.res_date.annee, res.res_date.mois, res.res_date.jour, res.duree);
            found = 1;
        }
    }

    fclose(file);
    return found;  // Return 1 if found, 0 if not
}
void rechercher_par_place(const char *filename, int place, char *output) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Erreur : Impossible d'ouvrir le fichier %s.\n", filename);
        strcpy(output, "Erreur : impossible d'ouvrir le fichier.");  // Message d'erreur pour l'ouverture du fichier
        return;
    }

    reservation res;
    int found = 0;
    output[0] = '\0';  // Initialiser le buffer de sortie

    while (fscanf(file, "%d %d %d-%d-%d %d\n", 
                  &res.cin, 
                  &res.place, 
                  &res.res_date.annee, 
                  &res.res_date.mois, 
                  &res.res_date.jour, 
                  &res.duree) == 6) {
        if (res.place == place) {
            // Formatage et ajout de la réservation trouvée dans le buffer de sortie
            sprintf(output, "Réservation pour la place %d:\nCIN: %d\nDate: %04d-%02d-%02d\nDurée: %d heures", 
                    res.place, res.cin, res.res_date.annee, res.res_date.mois, res.res_date.jour, res.duree);
            found = 1;
            break;  // Sortir de la boucle une fois que la réservation est trouvée
        }
    }

    fclose(file);

    if (!found) {
        strcpy(output, "Aucune réservation trouvée pour cette place.");
    }
}
int modifier(const char *filename, int cin, int place, reservation *res) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Erreur : Impossible d'ouvrir le fichier %s en lecture.\n", filename);
        return 0;  // Échec de l'ouverture du fichier
    }

    reservation reservations[100];  // Tableau pour stocker temporairement les réservations
    int count = 0;                  // Nombre total de réservations
    int found = 0;                  // Indique si la réservation à modifier est trouvée

    // Lecture des réservations depuis le fichier
    while (fscanf(file, "%d %d %d-%d-%d %d\n", 
                  &reservations[count].cin, 
                  &reservations[count].place, 
                  &reservations[count].res_date.annee, 
                  &reservations[count].res_date.mois, 
                  &reservations[count].res_date.jour, 
                  &reservations[count].duree) == 6) {
        if (reservations[count].cin == cin && reservations[count].place == place) {
            // Mettre à jour la réservation trouvée
            reservations[count] = *res;
            found = 1;
        }
        count++;
    }

    fclose(file);  // Fermer le fichier en mode lecture

    if (!found) {
        printf("Erreur : Aucune réservation trouvée pour CIN %d et place %d.\n", cin, place);
        return 0;  // Échec si la réservation n'a pas été trouvée
    }

    // Réécriture des données dans le fichier
    file = fopen(filename, "w");
    if (file == NULL) {
        printf("Erreur : Impossible d'ouvrir le fichier %s en écriture.\n", filename);
        return 0;  // Échec de l'ouverture du fichier
    }

    for (int i = 0; i < count; i++) {
        fprintf(file, "%d %d %04d-%02d-%02d %d\n", 
                reservations[i].cin, 
                reservations[i].place, 
                reservations[i].res_date.annee, 
                reservations[i].res_date.mois, 
                reservations[i].res_date.jour, 
                reservations[i].duree);
    }

    fclose(file);  // Fermer le fichier après écriture

    // Affichage des réservations mises à jour pour le CIN
    printf("Réservations pour CIN %d après modification :\n", cin);
    for (int i = 0; i < count; i++) {
        if (reservations[i].cin == cin) {
            printf("Place : %d, Date : %04d-%02d-%02d, Durée : %d heures\n", 
                   reservations[i].place, 
                   reservations[i].res_date.annee, 
                   reservations[i].res_date.mois, 
                   reservations[i].res_date.jour, 
                   reservations[i].duree);
        }
    }

    return 1;  // Succès
}
// Fonction pour supprimer une réservation
int supprimer(const char *filename, int cin, int place) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Erreur : Impossible d'ouvrir le fichier %s en lecture.\n", filename);
        return 0;  // Échec de l'ouverture du fichier
    }

    reservation reservations[100];  // Tableau pour stocker temporairement les réservations
    int count = 0;                  // Nombre total de réservations
    int found = 0;                  // Indique si la réservation à supprimer est trouvée

    // Lecture des réservations depuis le fichier
    while (fscanf(file, "%d %d %d-%d-%d %d\n", 
                  &reservations[count].cin, 
                  &reservations[count].place, 
                  &reservations[count].res_date.annee, 
                  &reservations[count].res_date.mois, 
                  &reservations[count].res_date.jour, 
                  &reservations[count].duree) == 6) {
        if (reservations[count].cin == cin && reservations[count].place == place) {
            found = 1;  // Marquer comme trouvé, mais ne pas ajouter cette réservation
        } else {
            count++;  // Ajouter les autres réservations au tableau
        }
    }

    fclose(file);  // Fermer le fichier en mode lecture

    if (!found) {
        printf("Erreur : Aucune réservation trouvée pour CIN %d et place %d.\n", cin, place);
        return 0;  // Échec si la réservation n'a pas été trouvée
    }

    // Réécriture des données restantes dans le fichier
    file = fopen(filename, "w");
    if (file == NULL) {
        printf("Erreur : Impossible d'ouvrir le fichier %s en écriture.\n", filename);
        return 0;  // Échec de l'ouverture du fichier
    }

    for (int i = 0; i < count; i++) {
        fprintf(file, "%d %d %04d-%02d-%02d %d\n", 
                reservations[i].cin, 
                reservations[i].place, 
                reservations[i].res_date.annee, 
                reservations[i].res_date.mois, 
                reservations[i].res_date.jour, 
                reservations[i].duree);
    }

    fclose(file);  // Fermer le fichier après écriture

    printf("Réservation supprimée avec succès pour CIN %d et place %d.\n", cin, place);
    return 1;  // Succès
}


///////////////////////


void add_row_reservation(GtkListStore *list_store, int cin, int place, date res_date, int duree) {
    GtkTreeIter iter;
    // Convert date to string format YYYY-MM-DD
    char date_str[11];
    snprintf(date_str, sizeof(date_str), "%d-%02d-%02d", res_date.annee, res_date.mois, res_date.jour);

    // Append the new row with reservation details
    gtk_list_store_append(list_store, &iter);
    gtk_list_store_set(list_store, &iter,
                       0, cin,               // CIN
                       1, place,             // Place
                       2, date_str,          // Reservation Date
                       3, duree,             // Duration
                       -1);                  // End of arguments
}

// Function to populate the TreeView with reservation data
void populate_treeview_with_reservations(GtkWidget *treeview, const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Error opening file: %s\n", filename);
        return;
    }

    // Create a ListStore with 4 columns (CIN, Place, Date, Duration)
    GtkListStore *list_store = gtk_list_store_new(4, G_TYPE_INT, G_TYPE_INT, G_TYPE_STRING, G_TYPE_INT);

    reservation res;
    // Read and parse the data from the file
    while (fscanf(file, "%d %d %d-%d-%d %d", &res.cin, &res.place, &res.res_date.annee, &res.res_date.mois, &res.res_date.jour, &res.duree) == 6) {
        // Add the row to the ListStore
        add_row_reservation(list_store, res.cin, res.place, res.res_date, res.duree);
    }

    fclose(file);

    // Set the ListStore model to the TreeView
    gtk_tree_view_set_model(GTK_TREE_VIEW(treeview), GTK_TREE_MODEL(list_store));
    g_object_unref(list_store);  // Unreference the list_store after setting the model

    // Add columns if not already added
    if (gtk_tree_view_get_columns(GTK_TREE_VIEW(treeview)) == 0) {
        GtkCellRenderer *renderer;
        GtkTreeViewColumn *column;

        // Create the "CIN" column
        renderer = gtk_cell_renderer_text_new();
        column = gtk_tree_view_column_new_with_attributes("CIN", renderer, "text", 0, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);

        // Create the "Place" column
        renderer = gtk_cell_renderer_text_new();
        column = gtk_tree_view_column_new_with_attributes("Place", renderer, "text", 1, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);

        // Create the "Reservation Date" column
        renderer = gtk_cell_renderer_text_new();
        column = gtk_tree_view_column_new_with_attributes("Reservation Date", renderer, "text", 2, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);

        // Create the "Duration" column
        renderer = gtk_cell_renderer_text_new();
        column = gtk_tree_view_column_new_with_attributes("Duration", renderer, "text", 3, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);
    }
} 
//////////////// 

int compare_reservations(const void *a, const void *b) {
    reservation *res_a = (reservation *)a;
    reservation *res_b = (reservation *)b;
    return res_a->place - res_b->place;  // Sort by place (ascending)
}

// Function to read reservations from file, sort, and write to a new file
void sort_and_write_reservations(const char *input_filename, const char *output_filename) {
    FILE *input_file = fopen(input_filename, "r");
    if (!input_file) {
        printf("Error opening file: %s\n", input_filename);
        return;
    }

    // Read reservations into an array
    reservation *reservations = malloc(sizeof(reservation) * 100);  // Assuming there are at most 100 reservations
    int count = 0;

    while (fscanf(input_file, "%d %d %d-%d-%d %d", &reservations[count].cin, 
                  &reservations[count].place, &reservations[count].res_date.annee, 
                  &reservations[count].res_date.mois, &reservations[count].res_date.jour, 
                  &reservations[count].duree) == 6) {
        count++;
        if (count >= 100) break;  // Avoid overflow if there are more than 100 reservations
    }
    fclose(input_file);

    // Sort the reservations by place
    qsort(reservations, count, sizeof(reservation), compare_reservations);

    // Write the sorted reservations to a new file
    FILE *output_file = fopen(output_filename, "w");
    if (!output_file) {
        printf("Error opening file: %s\n", output_filename);
        free(reservations);
        return;
    }

    for (int i = 0; i < count; i++) {
        fprintf(output_file, "%d %d %d-%02d-%02d %d\n", reservations[i].cin, 
                reservations[i].place, reservations[i].res_date.annee, 
                reservations[i].res_date.mois, reservations[i].res_date.jour, 
                reservations[i].duree);
    }

    fclose(output_file);
    free(reservations);

    printf("Reservations sorted and written to %s\n", output_filename);
}
///////////////////////////////////////////////////////////////////////////wael
// Fonction pour ajouter un service
void ajouterService(Service service) {
    FILE *fichier = fopen(FICHIER_SERVICES, "a");
    if (fichier == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        return;
    }
    fprintf(fichier, "%d;%s;%s;%.2f\n", service.id, service.nom, service.description, service.tarif);
    fclose(fichier);
}

// Fonction pour modifier un service
void modifierService(int id, Service service) {
    FILE *fichier = fopen(FICHIER_SERVICES, "r");
    FILE *temp = fopen("temp_services.txt", "w");
    if (fichier == NULL || temp == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        return;
    }

    Service s;
    int found = 0;

    while (fscanf(fichier, "%d;%49s;%99s;%f\n", &s.id, s.nom, s.description, &s.tarif) != EOF) {
        if (s.id == id) {
            fprintf(temp, "%d;%s;%s;%.2f\n", service.id, service.nom, service.description, service.tarif);
            found = 1;
        } else {
            fprintf(temp, "%d;%s;%s;%.2f\n", s.id, s.nom, s.description, s.tarif);
        }
    }

    fclose(fichier);
    fclose(temp);

    remove(FICHIER_SERVICES);
    rename("temp_services.txt", FICHIER_SERVICES);

    if (!found) {
        printf("Service avec l'ID %d introuvable.\n", id);
    }
}

// Fonction pour supprimer un service
void supprimerService(int id) {
    FILE *fichier = fopen(FICHIER_SERVICES, "r");
    FILE *temp = fopen("temp_services.txt", "w");
    if (fichier == NULL || temp == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        return;
    }

    Service s;
    int found = 0;

    while (fscanf(fichier, "%d;%49s;%99s;%f\n", &s.id, s.nom, s.description, &s.tarif) != EOF) {
        if (s.id != id) {
            fprintf(temp, "%d;%s;%s;%.2f\n", s.id, s.nom, s.description, s.tarif);
        } else {
            found = 1;
        }
    }

    fclose(fichier);
    fclose(temp);

    remove(FICHIER_SERVICES);
    rename("temp_services.txt", FICHIER_SERVICES);

    if (!found) {
        printf("Service avec l'ID %d introuvable.\n", id);
    }
}

// Fonction pour afficher tous les services
void afficherService(GtkWidget *treeview) {
    GtkListStore *store;
    GtkTreeIter iter;
    FILE *fichier;
    Service service;

    // Créer le modèle pour le TreeView (GtkListStore)
    store = gtk_list_store_new(4, G_TYPE_INT, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_FLOAT);

    // Ouvrir le fichier contenant les services
    fichier = fopen(FICHIER_SERVICES, "r");
    if (!fichier) {
        perror("Erreur lors de l'ouverture de services.txt");
        return;
    }

    // Lire les services et les ajouter au modèle
    while (fscanf(fichier, "%d;%49s;%99s;%f\n", &service.id, service.nom, service.description, &service.tarif) != EOF) {
        gtk_list_store_append(store, &iter);
        gtk_list_store_set(store, &iter,
                           0, service.id,
                           1, service.nom,
                           2, service.description,
                           3, service.tarif,
                           -1);
    }
    fclose(fichier);

    // Associer le modèle au TreeView
    gtk_tree_view_set_model(GTK_TREE_VIEW(treeview), GTK_TREE_MODEL(store));

    // Ajouter les colonnes au TreeView (si elles n'existent pas déjà)
    if (gtk_tree_view_get_columns(GTK_TREE_VIEW(treeview)) == NULL) {
    GtkCellRenderer *renderer;

    // Colonne ID
    renderer = gtk_cell_renderer_text_new();
    gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(treeview),
                                                -1, "ID", renderer,
                                                "text", 0, NULL);

    // Colonne Nom
    renderer = gtk_cell_renderer_text_new();
    gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(treeview),
                                                -1, "Nom", renderer,
                                                "text", 1, NULL);

    // Colonne Description
    renderer = gtk_cell_renderer_text_new();
    gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(treeview),
                                                -1, "Description", renderer,
                                                "text", 2, NULL);

    // Colonne Tarif
    renderer = gtk_cell_renderer_text_new();
    gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(treeview),
                                                -1, "Tarif", renderer,
                                                "text", 3, NULL);
}

    // Libérer le modèle après utilisation
    g_object_unref(store);
}
 
 
 
 // Fonction pour affecter un service à une réservation
void affecterServiceAReservation(int id_reservation, int id_service, const char *date, const char *client) {
    // Ouvrir le fichier de réservations en mode ajout (append)
    FILE *fichier = fopen(FICHIER_RESERVATIONS, "a");
    if (fichier == NULL) {
        perror("Erreur lors de l'ouverture du fichier des réservations");
        return;
    }

    // Créer une réservation
    Reservation reservation;
    reservation.id_reservation = id_reservation;
    reservation.id_service = id_service;
    strncpy(reservation.date, date, sizeof(reservation.date) - 1);
    reservation.date[sizeof(reservation.date) - 1] = '\0'; // Assurer que la chaîne soit bien terminée
    strncpy(reservation.client, client, sizeof(reservation.client) - 1);
    reservation.client[sizeof(reservation.client) - 1] = '\0'; // Assurer que la chaîne soit bien terminée

    // Ajouter la réservation dans le fichier
    fprintf(fichier, "%d %d %s %s\n", reservation.id_reservation, reservation.id_service, reservation.date, reservation.client);

    // Fermer le fichier
    fclose(fichier);

    printf("Service %d affecté à la réservation %d pour le client %s à la date %s.\n", id_service, id_reservation, client, date);
}
//////////////////////////////////////////////////////////////////////ayoub
#include "agent.h"
#include <stdio.h>
#include <string.h>
#include <gtk/gtk.h>

void verif_CIN(agent *G)
{
    do
    {
        printf("\nEntrer CIN (8 digits): ");
        scanf("%d", &G->CIN);
    } while (G->CIN < 10000000 || G->CIN > 99999999);
}

void verif_numTEL(agent *G)
{
    do
    {
        printf("\nEntrer Telephone Number (8 digits): ");
        scanf("%d", &G->numTEL);
    } while (G->numTEL < 10000000 || G->numTEL > 99999999);
}

int verif_sexe(agent *G)
{
    do
    {
        printf("\nEntrer le sexe de l'agent (1: Homme / 0: Femme): ");
        scanf("%d", &G->sexe);
    } while (G->sexe != 0 && G->sexe != 1);
    return G->sexe;
}

int verif_horraires(agent *G)
{
    do
    {
        printf("\nEnter es horraires de travail (0: 00h-8h / 1: 8h-16h / 2: 16h-00h): ");
        scanf("%d", &G->horraires_de_travail);
    } while (G->horraires_de_travail < 0 || G->horraires_de_travail > 2);
    return G->horraires_de_travail;
}

int verif_poste(agent *G)
{
    do
    {
        printf("\nEnter position de travail (0, 1, 2, or 3): ");
        scanf("%d", &G->poste_de_travail);
    } while (G->poste_de_travail < 0 || G->poste_de_travail > 3);
    return G->poste_de_travail;
}

void saisir_agent(agent *G)
{
    printf("\nEntrer le nom de l'agent: ");
    scanf("%s", G->nom);
    printf("\nEntrer le prenom de l'agent: ");
    scanf("%s", G->prenom);
    verif_CIN(G);

    printf("\nEntrer la date de naissance de l'agent (day month year): ");
    scanf("%d %d %d", &G->date_naissance.jour, &G->date_naissance.mois, &G->date_naissance.annee);

    verif_sexe(G);

    printf("\nEntrer la ville de l'agent: ");
    scanf("%s", G->ville);

    verif_numTEL(G);

    printf("\nEntrer Email de l'agent: ");
    scanf("%s", G->email);

    printf("\nEntrer la date de recrutement de l'agent (day month year): ");
    scanf("%d %d %d", &G->date_recrutement.jour, &G->date_recrutement.mois, &G->date_recrutement.annee);

    verif_horraires(G);
    verif_poste(G);
}


int ajouter_agent(char *file_agent, agent G)
{
    FILE *f = fopen(file_agent, "a");
    if (f != NULL)
    {
        fprintf(f, "%s %s %d %02d/%02d/%04d %s %d %s %02d/%02d/%04d ",
                G.nom, G.prenom, G.CIN,
                G.date_naissance.jour, G.date_naissance.mois, G.date_naissance.annee,
                G.ville, G.numTEL, G.email,
                G.date_recrutement.jour, G.date_recrutement.mois, G.date_recrutement.annee);

        const char *gender = (G.sexe == 0) ? "Femme" : "Homme";
        fprintf(f, "%s ", gender);

        const char *work_hours = (G.horraires_de_travail == 0) ? "00h-8h" :
                                 (G.horraires_de_travail == 1) ? "8h-16h" : "16h-00h";
        fprintf(f, "%s ", work_hours);

        fprintf(f, "Poste%d\n", G.poste_de_travail);

        fclose(f);
        return 1;
    }
    return 0;
}


int modifier(char *file_agent, int CIN, agent nouvG)
{
    agent G;
    char gender_str[10], work_hours_str[10], work_poste[10];
    FILE *f = fopen(file_agent, "r");
    if (f == NULL) {
        printf("Error opening the input file: %s\n", file_agent);
        return 0;
    }

    FILE *f2 = fopen("nouv_agent.txt", "w");
    if (f2 == NULL) {
        printf("Error creating the output file: nouv_agent.txt\n");
        fclose(f);  // Close the input file before returning
        return 0;
    }


    while (fscanf(f, "%s %s %d %02d/%02d/%04d %s %d %s %02d/%02d/%04d %s %s %s",
                  G.nom, G.prenom, &G.CIN,
                  &G.date_naissance.jour, &G.date_naissance.mois, &G.date_naissance.annee,
                  G.ville, &G.numTEL, G.email,
                  &G.date_recrutement.jour, &G.date_recrutement.mois, &G.date_recrutement.annee,
                  gender_str, work_hours_str, work_poste) != EOF)
    {
        // Convert gender and work hours and work_poste from strings
        G.sexe = (strcmp(gender_str, "Femme") == 0) ? 0 : 1;
        
	if (strcmp(work_hours_str, "00h-8h") == 0)
            G.horraires_de_travail = 0;
        else if (strcmp(work_hours_str, "8h-16h") == 0)
            G.horraires_de_travail = 1;
        else
            G.horraires_de_travail = 2;

	if (strcmp(work_poste, "poste0") == 0)
            G.poste_de_travail = 0;
        else if (strcmp(work_poste, "poste1") == 0)
            G.poste_de_travail = 1;
        else if (strcmp(work_poste, "poste2") == 0)
            G.poste_de_travail = 2;        
	else
            G.poste_de_travail = 3;

        if (G.CIN == CIN)
        {
            fprintf(f2, "%s %s %d %02d/%02d/%04d %s %d %s %02d/%02d/%04d ",
                    nouvG.nom, nouvG.prenom, nouvG.CIN,
                    nouvG.date_naissance.jour, nouvG.date_naissance.mois, nouvG.date_naissance.annee,
                    nouvG.ville, nouvG.numTEL, nouvG.email,
                    nouvG.date_recrutement.jour, nouvG.date_recrutement.mois, nouvG.date_recrutement.annee);

            const char *gender = (nouvG.sexe == 0) ? "Femme" : "Homme";
            fprintf(f2, "%s ", gender);

            const char *work_hours = (nouvG.horraires_de_travail == 0) ? "00h-8h" :
                                      (nouvG.horraires_de_travail == 1) ? "8h-16h" : "16h-00h";
            fprintf(f2, "%s ", work_hours);

            fprintf(f2, "Poste%d\n", nouvG.poste_de_travail);

        }
        else
        {
            fprintf(f2, "%s %s %d %02d/%02d/%04d %s %d %s %02d/%02d/%04d %s %s Poste%d\n",
                    G.nom, G.prenom, G.CIN,
                    G.date_naissance.jour, G.date_naissance.mois, G.date_naissance.annee,
                    G.ville, G.numTEL, G.email,
                    G.date_recrutement.jour, G.date_recrutement.mois, G.date_recrutement.annee,
                    (G.sexe == 0) ? "Femme" : "Homme",
                    (G.horraires_de_travail == 0) ? "00h-8h" :
                    (G.horraires_de_travail == 1) ? "8h-16h" : "16h-00h",
                    G.poste_de_travail);
        }
    }

    fclose(f);
    fclose(f2);

    remove(file_agent);
    rename("nouv_agent.txt", file_agent);

    return 1;
}



int supprimer(char *file_agent, int CIN)
{
    agent G;
    char gender_str[10], work_hours_str[10], work_poste[10];
    int found = 0; // Pour vérifier si l'agent a été trouvé

    FILE *f = fopen(file_agent, "r");
    if (f == NULL) {
        printf("Erreur lors de l'ouverture du fichier : %s\n", file_agent);
        return 0;
    }

    FILE *f2 = fopen("nouv_agent.txt", "w");
    if (f2 == NULL) {
        printf("Erreur lors de la création du fichier temporaire : nouv_agent.txt\n");
        fclose(f);
        return 0;
    }

    // Lecture et écriture dans le fichier temporaire en excluant l'agent avec le CIN spécifié
    while (fscanf(f, "%s %s %d %02d/%02d/%04d %s %d %s %02d/%02d/%04d %s %s %s",
                  G.nom, G.prenom, &G.CIN,
                  &G.date_naissance.jour, &G.date_naissance.mois, &G.date_naissance.annee,
                  G.ville, &G.numTEL, G.email,
                  &G.date_recrutement.jour, &G.date_recrutement.mois, &G.date_recrutement.annee,
                  gender_str, work_hours_str, work_poste) != EOF)
    {
        // Si le CIN correspond, on marque cet agent comme trouvé et on le saute
        if (G.CIN == CIN) {
            found = 1;
        }
        else {
            fprintf(f2, "%s %s %d %02d/%02d/%04d %s %d %s %02d/%02d/%04d %s %s %s\n",
                    G.nom, G.prenom, G.CIN,
                    G.date_naissance.jour, G.date_naissance.mois, G.date_naissance.annee,
                    G.ville, G.numTEL, G.email,
                    G.date_recrutement.jour, G.date_recrutement.mois, G.date_recrutement.annee,
                    gender_str, work_hours_str, work_poste);

        }
    }

    fclose(f);
    fclose(f2);

    // Si l'agent n'a pas été trouvé, on supprime le fichier temporaire et on retourne 0
    if (found==0) {
        printf("Agent avec le CIN %d non trouvé.\n", CIN);
        remove("nouv_agent.txt");
        return 0;
    }

    // si l'agent est trouvé, on remplace le fichier original par le fichier temporaire et on retourne 1
    remove(file_agent);
    rename("nouv_agent.txt", file_agent);

    printf("Suppression de l'agent avec CIN %d réussie.\n", CIN);
    return 1;
}

int chercher(char *file_agent, int CIN, agent *pG)
{
    char gender_str[10], work_hours_str[10], work_poste[10];
    int found = 0;
    FILE *f = fopen(file_agent, "r");

    if (f == NULL) {
        printf("Error opening the input file: %s\n", file_agent);
        return 0;
    }

    while (fscanf(f, "%s %s %d %02d/%02d/%04d %s %d %s %02d/%02d/%04d %s %s %s",
                  pG->nom, pG->prenom, &pG->CIN,
                  &pG->date_naissance.jour, &pG->date_naissance.mois, &pG->date_naissance.annee,
                  pG->ville, &pG->numTEL, pG->email,
                  &pG->date_recrutement.jour, &pG->date_recrutement.mois, &pG->date_recrutement.annee,
                  gender_str, work_hours_str, work_poste) != EOF)
    {
        if (pG->CIN == CIN) {
            pG->sexe = (strcmp(gender_str, "Femme") == 0) ? 0 : 1;

            if (strcmp(work_hours_str, "00h-8h") == 0)
                pG->horraires_de_travail = 0;
            else if (strcmp(work_hours_str, "8h-16h") == 0)
                pG->horraires_de_travail = 1;
            else
                pG->horraires_de_travail = 2;

            if (strcmp(work_poste, "Poste0") == 0)
                pG->poste_de_travail = 0;
            else if (strcmp(work_poste, "Poste1") == 0)
                pG->poste_de_travail = 1;
            else if (strcmp(work_poste, "Poste2") == 0)
                pG->poste_de_travail = 2;
            else
                pG->poste_de_travail = 3;

            found = 1;
            break;
        }
    }
    fclose(f);
    return found;
}

int afficher_cherche_agent(char *file_agent, int CIN)
{
    agent G;
    int found;
    FILE *f2 = fopen("cherche_agent.txt", "w");
    if (f2 == NULL) {
        printf("Error creating the output file: cherche_agent.txt\n");
        return 0;
    }

    found = chercher(file_agent, CIN, &G);
    if (!found) {
        printf("\nAgent non trouvé\n");
    } else {
        const char *gender = (G.sexe == 0) ? "Femme" : "Homme";
        const char *work_hours = (G.horraires_de_travail == 0) ? "00h-8h" :
                                 (G.horraires_de_travail == 1) ? "8h-16h" : "16h-00h";
        
        fprintf(f2, "%s %s %d %02d/%02d/%04d %s %d %s %02d/%02d/%04d %s %s Poste%d\n",
                G.nom, G.prenom, G.CIN,
                G.date_naissance.jour, G.date_naissance.mois, G.date_naissance.annee,
                G.ville, G.numTEL, G.email,
                G.date_recrutement.jour, G.date_recrutement.mois, G.date_recrutement.annee,
                gender, work_hours, G.poste_de_travail);

        printf("\nAgent trouvé avec succès...vérifier le fichier cherche_agent\n");
    }
    fclose(f2);
    return found;
}

int chercher_reservation(char *file_reservation, int jour,int mois,int annee, reservation *pR)
{
    int found=0;
    FILE *f3 = fopen(file_reservation, "r");

    if (f3 == NULL) {
        printf("Error opening the input file: %s\n", file_reservation);
        return 0;
    }
    while (fscanf(f3,"%s %d %d %d %s %02d/%02d/%04d",
                  pR->nom, &pR->id, &pR->place, &pR->durree, pR->statut, &pR->date_reservation.jour, &pR->date_reservation.mois, &pR->date_reservation.annee ) != EOF)
    {
        if((pR->date_reservation.jour==jour) && (pR->date_reservation.mois==mois) && (pR->date_reservation.annee==annee))
        {
            found=1;
            break;
        }
    }
    fclose(f3);
    return found;
}

int afficher_reservation(char *file_reservation,int jour,int mois,int annee)
{
    reservation R;
    int x;
    FILE *f2 = fopen("cherche_reservation.txt", "w");
    if (f2 == NULL) {
        printf("Error creating the output file: cherche_reservation.txt\n");
        return 0;
    }
    x=chercher_reservation(file_reservation ,jour, mois ,annee, &R);
    if (!x) {
        printf("\nRéservation non trouvé\n");
    }
    else {
        fprintf(f2,"%s %d %d %d %s %02d/%02d/%04d\n",
                R.nom, R.id, R.place, R.durree, R.statut, R.date_reservation.jour, R.date_reservation.mois, R.date_reservation.annee );
        printf("\nRéservation trouvée avec succès...vérifier le fichier cherche_reservation\n");
    }
    fclose(f2);
    return x;
}



void afficher_agent(GtkWidget *liste) {
    GtkCellRenderer *renderer;
    GtkTreeViewColumn *column;
    GtkTreeIter iter;
    GtkTreeModel *store;
    FILE *f;
    char nom[30];
    char prenom[30];
    int cin;
    char date_naissance[15];
    char ville[30];
    int numtel;
    char email[30];
    char date_recrutement[15];
    char sexe[10];
    char work_hours[10];
    char work_poste[15];

    store = gtk_tree_view_get_model(GTK_TREE_VIEW(liste));
    if (store == NULL) {
        // Création des colonnes si elles n'existent pas encore
        renderer = gtk_cell_renderer_text_new();
        column = gtk_tree_view_column_new_with_attributes("Nom", renderer, "text", NOM, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(liste), column);

        renderer = gtk_cell_renderer_text_new();
        column = gtk_tree_view_column_new_with_attributes("Prénom", renderer, "text", PRENOM, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(liste), column);

        renderer = gtk_cell_renderer_text_new();
        column = gtk_tree_view_column_new_with_attributes("CIN", renderer, "text", CIN, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(liste), column);

        renderer = gtk_cell_renderer_text_new();
        column = gtk_tree_view_column_new_with_attributes("Date de naissance", renderer, "text", DN, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(liste), column);

        renderer = gtk_cell_renderer_text_new();
        column = gtk_tree_view_column_new_with_attributes("Ville", renderer, "text", VILLE, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(liste), column);

        renderer = gtk_cell_renderer_text_new();
        column = gtk_tree_view_column_new_with_attributes("Numéro de téléphone", renderer, "text", NUMTEL, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(liste), column);

        renderer = gtk_cell_renderer_text_new();
        column = gtk_tree_view_column_new_with_attributes("Email", renderer, "text", EMAIL, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(liste), column);

        renderer = gtk_cell_renderer_text_new();
        column = gtk_tree_view_column_new_with_attributes("Date de recrutement", renderer, "text", DR, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(liste), column);

        renderer = gtk_cell_renderer_text_new();
        column = gtk_tree_view_column_new_with_attributes("Sexe", renderer, "text", SEXE, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(liste), column);

        renderer = gtk_cell_renderer_text_new();
        column = gtk_tree_view_column_new_with_attributes("Heures de travail", renderer, "text", WH, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(liste), column);

        renderer = gtk_cell_renderer_text_new();
        column = gtk_tree_view_column_new_with_attributes("Poste", renderer, "text", WP, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(liste), column);
    }

    // Création du modèle
    store = gtk_list_store_new(COLUMNS, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_INT, G_TYPE_STRING,
                               G_TYPE_STRING, G_TYPE_INT, G_TYPE_STRING, G_TYPE_STRING,
                               G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING);

    // Lecture du fichier
    f = fopen("agents.txt", "r");
    if (f == NULL) {
        g_print("Erreur : Impossible d'ouvrir le fichier agents.txt\n");
        return; // On quitte la fonction en cas d'erreur
    }

    while (fscanf(f, "%s %s %d %s %s %d %s %s %s %s %s", nom, prenom, &cin, date_naissance,
                  ville, &numtel, email, date_recrutement, sexe, work_hours, work_poste) != EOF) {
        gtk_list_store_append(store, &iter);
        gtk_list_store_set(store, &iter,
                           NOM, nom,
                           PRENOM, prenom,
                           CIN, cin,
                           DN, date_naissance,
                           VILLE, ville,
                           NUMTEL, numtel,
                           EMAIL, email,
                           DR, date_recrutement,
                           SEXE, sexe,
                           WH, work_hours,
                           WP, work_poste,
                           -1);
    }

    fclose(f);

    // Associer le modèle à la vue
    gtk_tree_view_set_model(GTK_TREE_VIEW(liste), GTK_TREE_MODEL(store));
    g_object_unref(store);
}

/*********************************************************************************************/



void afficher_liste_reservation(GtkWidget *liste) {
    GtkCellRenderer *renderer;
    GtkTreeViewColumn *column;
    GtkTreeIter iter;
    GtkTreeModel *store;
    FILE *f;
    char nom[30];
    int id;
    int place;
    int duree;
    char statue[50];  // Augmenter la taille du tableau pour le statut
    char date_reservation[20];  // Augmenter la taille pour la date

    store = gtk_tree_view_get_model(GTK_TREE_VIEW(liste));
    if (store == NULL) {
        // Création des colonnes si elles n'existent pas encore
        renderer = gtk_cell_renderer_text_new();
        column = gtk_tree_view_column_new_with_attributes("Nom", renderer, "text", NNOM, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(liste), column);

        renderer = gtk_cell_renderer_text_new();
        column = gtk_tree_view_column_new_with_attributes("ID", renderer, "text", ID, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(liste), column);

        renderer = gtk_cell_renderer_text_new();
        column = gtk_tree_view_column_new_with_attributes("Place", renderer, "text", PLACE, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(liste), column);

        renderer = gtk_cell_renderer_text_new();
        column = gtk_tree_view_column_new_with_attributes("Duree", renderer, "text", DUREE, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(liste), column);

        renderer = gtk_cell_renderer_text_new();
        column = gtk_tree_view_column_new_with_attributes("Statue", renderer, "text", STATUE, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(liste), column);

        renderer = gtk_cell_renderer_text_new();
        column = gtk_tree_view_column_new_with_attributes("Date de reservation", renderer, "text", DRES, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(liste), column);
    }

    // Création du modèle avec des types spécifiques
    store = gtk_list_store_new(CCOLUMNS, G_TYPE_STRING, G_TYPE_INT, G_TYPE_INT, G_TYPE_INT, G_TYPE_STRING, G_TYPE_STRING);

    f = fopen("cherche_reservation.txt", "r");
    if (f == NULL) {
        return; // Si le fichier ne s'ouvre pas, quitter la fonction
    }

    while (fscanf(f, "%s %d %d %d %49s %19s", nom, &id, &place, &duree, statue, date_reservation) != EOF) {
        gtk_list_store_append(store, &iter);
        gtk_list_store_set(store, &iter, NNOM, nom, ID, id, PLACE, place, DUREE, duree, STATUE, statue, DRES, date_reservation, -1);
    }

    fclose(f); // Assurer la fermeture du fichier après lecture

    // Associer le modèle à la vue
    gtk_tree_view_set_model(GTK_TREE_VIEW(liste), GTK_TREE_MODEL(store));
    g_object_unref(store); // Libérer le modèle
}















