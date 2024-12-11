#include <stdio.h>
#include <stdlib.h>
#include <string.h>
    typedef struct{
    char nom[20];
    int cin;
    char maladie[20];
    }patient;
   typedef struct{
   char nom[20];
   char hote[20];
   char des[100];
   }variant;
   typedef struct lit{
   int id;
   int valable;//1 si ce lit est valable 0 sinon
   int cin_patient;//-1 si ce lit est non utilisé
   struct lit* suiv;
   } lit;
   typedef struct secteur{
   char nom[20];
   lit *lits;
   struct secteur *suiv;
   } secteur;
   ////pour le stockage////
   typedef struct {
    char code[20];
    char name[20];
    char info[20];
} Entry;
typedef struct Node {
    char question[256];
    float probability; // Ajout d'un champ pour la probabilité
    struct Node *yes;
    struct Node *no;
} Node;
typedef struct ElementPile {
    char action[100];
    struct ElementPile* suivant;
} ElementPile;
//fonction pour verifier si le variant existe//
   int check_existance(const char *name){
   FILE *F;
   char ligne[100];
   char* partie_nom;
   F=fopen("./variant.txt","r");
   if (F==NULL){return 0;}
   while(fgets(ligne,sizeof(ligne),F)){
    ligne[strcspn(ligne,"\n")]='\0';
    partie_nom=strtok(ligne,";");
    if(((partie_nom)!=NULL) && (strcmp(partie_nom,name)==0)){
        fclose(F);
        return 1;
    }
    }
   fclose(F);
   return 0;
   }
////procedure pour ajouter variant///
   void ajouter_variant(){
   FILE*F;
   char name[20];
   char ht[20];
   variant variant;
   F=fopen("./variant.txt","a");
   if (F==NULL){
   printf("erreur:impossible d'ouvrir le fichier.\n");
   return;
   }
   printf("\n entrer le nom de ce variant :");
   scanf("%s",name);
   if(check_existance(name)==1){
    printf("\n ce variant existe deja.\n");
    fclose(F);
    return;
   }
   else{
    strcpy(variant.nom, name);
    printf("\ndonner le nom de l'hote :");
    scanf("%s",ht);
    strcpy(variant.hote, ht);
    getchar();
    printf("\n donner la description de ce variant :");
    fgets(variant.des,sizeof(variant.des),stdin);
    variant.des[strcspn(variant.des, "\n")] = '\0';
    fprintf(F,"%s;%s;%s\n",variant.nom,variant.hote,variant.des);
    fclose(F);
    printf("le variant a été ajouté avec succès.\n");
   }
   };
   //pour afficher le variant//
   void afficher_variant(const char *name){
   FILE *F;
   char ligne[100];
   char *partie_nom,*partie_hote,*partie_des;
   F=fopen("./variant.txt","r");
   if(F==NULL){
    printf("erreur:impossible d'ouvrir le fichier \n");
    return;
   }
   if(check_existance(name)==1){
   while(fgets(ligne,sizeof(ligne),F)){
    ligne[strcspn(ligne,"\n")]='\0';
    partie_nom=strtok(ligne,";");
    partie_hote=strtok(NULL,";");
    partie_des=strtok(NULL,";");
    if (strcmp(partie_nom,name)==0){
    printf("hote : %s\n",partie_hote);
    printf("description : %s\n ",partie_des);
    fclose(F);
    return;
   }
   }
   }
    else{printf("variant n'existe pas!");
    fclose(F);
   }
   }
   void menu_principale(){
       int c;
       printf("enter 0 pour aller au menue principale ");
            scanf("%d",&c);
            if(c==0){
                main();
            }
   }

    void rechercher_par_description(const char *mot_recherche) {
    FILE *F = fopen("./variant.txt", "r");
    if (F == NULL) {
        printf("Erreur lors de l'ouverture du fichier\n");
        return;
    }

    char ligne[200];
    char maladie[100], hote[100], description[100];
    int trouve = 0;

    printf("Les lignes où le mot '%s' existe dans la description :\n", mot_recherche);

    while (fgets(ligne, sizeof(ligne), F)) {
        if (sscanf(ligne, "%99[^;];%99[^;];%99[^\n]", maladie, hote, description) == 3) {
            if (strstr(description, mot_recherche) != NULL) {
                printf("%s", ligne);
                trouve = 1;
            }
        }
    }

    if (!trouve) {
        printf("Aucune ligne contenant '%s' dans la description.\n", mot_recherche);
    }

    fclose(F);
}
//ajouter lit en utilisant liste chainee//
   secteur *ajouter_lit(secteur *debut, const char *secteur_nom, int lit_id, int validite, int cin_patient) {
    secteur *s = debut;
    while (s != NULL && strcmp(s->nom, secteur_nom) != 0) {
        s = s->suiv;
    }    if (s == NULL) {
        // Création du secteur si non existant
        s = (secteur *)malloc(sizeof(secteur));
        strcpy(s->nom, secteur_nom);
        s->lits = NULL;
        s->suiv = debut;
        debut = s;
    }

    // Création du lit
    lit *l = (lit *)malloc(sizeof(lit));
    l->id = lit_id;
    l->valable = validite;
    l->cin_patient = cin_patient;
    l->suiv = s->lits;
    s->lits = l;

    return debut;
}
   void sauvegarder(secteur *debut, const char *nom_fichier) {
    FILE *F = fopen("./lits.txt", "a");
    if (!F) {
        printf("Erreur d'ouverture du fichier pour la sauvegarde.\n");
        return;
    }

    secteur *s = debut;
    while (s != NULL) {
        fprintf(F, "%s;", s->nom);
        lit *l = s->lits;
        while (l != NULL) {
            fprintf(F, "%d;%d;%d\n", l->id, l->valable,l->cin_patient);
            l = l->suiv;
        }
        s = s->suiv;
    }

    fclose(F);
}
   void rechercher_par_secteur(const char *nom_recherche) {
    FILE *F = fopen("./lits.txt", "r");
    if (F == NULL) {
        printf("Erreur lors de l'ouverture du fichier\n");
        return;
    }

    char ligne[100];
    char nom[50];
    int numero, valable, cin;
    int s = 0;

    printf("Les numéros associés à '%s' qui sont valable :\n", nom_recherche);

    // Lire le fichier ligne par ligne
    while (fgets(ligne, sizeof(ligne), F)) {
        // Utilisation de sscanf pour parser la ligne avec des séparateurs ';'
        if (sscanf(ligne, "%49[^;];%d;%d;%d", nom, &numero, &valable, &cin) == 4) {
            if (strcmp(nom, nom_recherche) == 0 && valable == 0) {
                printf("%d\n", numero);
                s++;
            }
        }
    }

    printf("Il existe %d lits valables dans ce secteur\n", s);

    fclose(F);
   }
void ajouter_patient(){
   FILE*F;
   char n_maladie[20];
   int cin_p;
   patient patient;
   F=fopen("./patient.txt","a");
   if (F==NULL){
   printf("erreur:impossible d'ouvrir le fichier.\n");
   return;
   }
    printf("\n entrer le CIN du patient :");
    scanf("%d",&cin_p);
    patient.cin=cin_p;
    getchar();
    printf("\n donner le nom du patient :");
    fgets(patient.nom,sizeof(patient.nom),stdin);
    patient.nom[strcspn(patient.nom, "\n")] = '\0';
    printf("\ndonner le nom de maladie :");
    scanf("%s",n_maladie);
    strcpy(patient.maladie,n_maladie);
    fprintf(F,"%d;%s;%s\n",patient.cin,patient.nom,patient.maladie);
    fclose(F);
    printf("le patient a ete ajouté avec succes.\n");
   }
   void supprimer_patient(int cin_patient) {
       FILE *F_lits = fopen("./lits.txt", "r");
       FILE *F_temp = fopen("./temp_lits.txt", "w");

       if (F_lits == NULL || F_temp == NULL) {
           printf("Erreur lors de l'ouverture des fichiers.\n");
           return;
       }
       char ligne[100];
       int lit_modifie = 0; // Indicateur pour savoir si le lit a été modifié
       while (fgets(ligne, sizeof(ligne), F_lits)) {
           char secteur[50];
           int numero_lit, valable, cin;

           // Lecture des données d'une ligne
           sscanf(ligne, "%[^;];%d;%d;%d", secteur, &numero_lit, &valable, &cin);

           // Si le CIN correspond, on libère le lit
           if (cin == cin_patient) {
               cin = -1; // Libération du lit
               valable=0;
               lit_modifie = 1;
           }

           // Écriture des données (modifiées ou non) dans le fichier temporaire
           fprintf(F_temp, "%s;%d;%d;%d\n", secteur, numero_lit, valable, cin);
       }

       fclose(F_lits);
       fclose(F_temp);

       // Remplace le fichier original par le fichier temporaire
       remove("./lits.txt");
       rename("./temp_lits.txt", "./lits.txt");

       if (lit_modifie) {
           printf("Le patient avec CIN %d a été supprimé avec succès de la base de donnée.\n", cin_patient);
       } else {
           printf("Aucun lit associé au CIN %d n'a été trouvé.\n", cin_patient);
       }
   }
   void traiter_suppression() {
       int cin;
       printf("Entrez le CIN du patient à exclure : ");
       scanf("%d", &cin);

       supprimer_patient(cin);
   }
   // Fonction pour retirer les caractères de retour à la ligne d'une chaîne
void trim_newline(char *str) {
    size_t len = strlen(str);
    if (len > 0 && str[len - 1] == '\n') {
        str[len - 1] = '\0';
    }
}

// Fonction principale pour le traitement des variantes par secteur
void variant_par_secteur() {
    FILE *filePatient, *fileLits;
    char line[100];
    Entry entries[100];
    int entryCount = 0;
    // Ouvrir le fichier "patient.txt" en mode lecture
    filePatient = fopen("./patient.txt", "r");
    if (filePatient == NULL) {
        printf("Impossible d'ouvrir patient.txt\n");
        return;
    }
    // Lire et stocker les données du fichier patient
    while (fgets(line, sizeof(line), filePatient)) {
        Entry entry;
        char *token = strtok(line, ";");

        if (token != NULL) {
            strcpy(entry.code, token); // Lire le code
            trim_newline(entry.code); // Supprimer le retour à la ligne
            token = strtok(NULL, ";");
        }

        if (token != NULL) {
            strcpy(entry.name, token); // Lire le nom
            trim_newline(entry.name);
            token = strtok(NULL, ";");
        }

        if (token != NULL) {
            strcpy(entry.info, token); // Lire les informations
            trim_newline(entry.info);
        }

        entries[entryCount++] = entry;
    }
    fclose(filePatient);
    // Ouvrir le fichier "lits.txt" en mode lecture
    fileLits = fopen("./lits.txt", "r");
    if (fileLits == NULL) {
        printf("Impossible d'ouvrir lits.txt\n");
        return;
    }
    // Lire et traiter chaque ligne du fichier lits
    while (fgets(line, sizeof(line), fileLits)) {
        char category[20], codeToCheck[20];
        char *token = strtok(line, ";");
        if (token != NULL) {
            strcpy(category, token); // Lire la catégorie (exemple : "A")
            trim_newline(category);
            token = strtok(NULL, ";"); // Ignorer le deuxième champ
        }
        token = strtok(NULL, ";"); // Ignorer le troisième champ
        if (token != NULL) {
            token = strtok(NULL, ";"); // Obtenir le code à vérifier
            strcpy(codeToCheck, token);
            trim_newline(codeToCheck);
        }

        int found = 0;
        for (int i = 0; i < entryCount; i++) {
            if (strcmp(entries[i].code, codeToCheck) == 0) {
                printf("Dans %s, il y a %s\n", category, entries[i].info);
                found = 1;
                break;
            }
        }

    }

    fclose(fileLits);
}
void rechercher_patient_par_cin(const char *cin_recherche) {
    FILE *F = fopen("./patient.txt", "r");
    if (F == NULL) {
        printf("Erreur lors de l'ouverture du fichier.\n");
        return;
    }
    char ligne[200];
    int trouve = 0;
    while (fgets(ligne, sizeof(ligne), F)) {
        char cin[50], nom[50], maladie[100];

        // Extraire les champs de la ligne
        if (sscanf(ligne, "%49[^;];%49[^;];%99[^\n]", cin, nom, maladie) == 3) {
            if (strcmp(cin, cin_recherche) == 0) {
                printf("Ligne trouvée : %s\n", ligne);
                trouve = 1;
                break; // On peut sortir après avoir trouvé le CIN
            }
        }
    }

    fclose(F);

    if (!trouve) {
        printf("Le patient avec le CIN '%s' n'a pas été trouvé.\n", cin_recherche);
    }
}
void modifier_description_variant(const char *nom_variant, const char *nouvelle_description) {
    FILE *F = fopen("./variant.txt", "r");
    FILE *temp = fopen("./temp_variant.txt", "w");
    if (F == NULL || temp == NULL) {
        printf("Erreur lors de l'ouverture des fichiers.\n");
        if (F) fclose(F);
        if (temp) fclose(temp);
        return;
    }
    char ligne[200];
    int trouve = 0;
    while (fgets(ligne, sizeof(ligne), F)) {
        char nom[50], hote[50], description[100];
        if (sscanf(ligne, "%49[^;];%49[^;];%99[^\n]", nom, hote, description) == 3) {
            if (strcmp(nom, nom_variant) == 0) {
                fprintf(temp, "%s;%s;%s\n", nom, hote, nouvelle_description);
                trouve = 1;
            } else {
                fprintf(temp, "%s;%s;%s\n", nom, hote, description);
            }
        }
    }
    fclose(F);
    fclose(temp);
    if (trouve) {
        remove("./variant.txt");
        rename("./temp_variant.txt", "./variant.txt");
        printf("La description du variant '%s' a été mise à jour avec succès.\n", nom_variant);
    } else {
        printf("Le variant '%s' n'existe pas dans le fichier.\n", nom_variant);
        remove("./temp_variant.txt");
    }
}




void reseau(const char *secteur_nom, const char *maladie_critique) {
    FILE *F_lits = fopen("./lits.txt", "r");
    FILE *F_patients = fopen("./patient.txt", "r");
    FILE *F_autres_secteurs = fopen("./autres_secteurs.txt", "a");
    FILE *F_transferts = fopen("./transferts.txt", "a"); // Fichier pour enregistrer les transferts

    if (F_lits == NULL || F_patients == NULL || F_autres_secteurs == NULL || F_transferts == NULL) {
        printf("Erreur lors de l'ouverture des fichiers.\n");
        return;
    }

    int total_lits = 0, lits_occupees = 0, total_maladies = 0, maladie_count = 0;
    char ligne[100], secteur[50], maladie[50];
    int numero_lit, valable, cin;

    while (fgets(ligne, sizeof(ligne), F_lits)) {
        ligne[strcspn(ligne, "\n")] = '\0';
        char *token = strtok(ligne, ";");
        if (token != NULL) {
            strcpy(secteur, token);
            token = strtok(NULL, ";");
            numero_lit = atoi(token);
            token = strtok(NULL, ";");
            valable = atoi(token);
            token = strtok(NULL, ";");
            cin = atoi(token);

            if (strcmp(secteur, secteur_nom) == 0) {
                total_lits++;
                if (valable == 1 && cin != -1) {
                    lits_occupees++;
                }
            }
        }
    }
    fclose(F_lits);

    while (fgets(ligne, sizeof(ligne), F_patients)) {
        ligne[strcspn(ligne, "\n")] = '\0';
        char *token = strtok(ligne, ";");
        if (token != NULL) {
            cin = atoi(token);
            token = strtok(NULL, ";");
            token = strtok(NULL, ";");
            strcpy(maladie, token);

            if (strcmp(maladie_critique, maladie) == 0) {
                maladie_count++;
            }
            total_maladies++;
        }
    }
    fclose(F_patients);

    float prob_influence = (total_maladies > 0) ? (float)maladie_count / total_maladies : 0.0;

    if (prob_influence > 0.75) {
        fprintf(F_transferts, "Probabilité d'influence %.2f dépasse 0.75. Transfert général requis pour le secteur '%s'.\n", prob_influence, secteur_nom);
        printf("Transfert général requis pour le secteur '%s'.\n", secteur_nom);

        while (fgets(ligne, sizeof(ligne), F_autres_secteurs)) {
            fprintf(F_transferts, "Transfert vers : %s\n", ligne);
            printf("Transfert vers : %s\n", ligne);
        }
    } else if (total_lits == lits_occupees) {
        fprintf(F_transferts, "Stock complet dans le secteur '%s'. Recherche d'un secteur similaire.\n", secteur_nom);
        printf("Recherche d'un secteur similaire dans un autre hôpital.\n");

        char secteur_similaire[50], hopital_nom[50];
        while (fgets(ligne, sizeof(ligne), F_autres_secteurs)) {
            ligne[strcspn(ligne, "\n")] = '\0';
            char *token = strtok(ligne, ";");
            if (token != NULL) {
                strcpy(hopital_nom, token);
                token = strtok(NULL, ";");
                strcpy(secteur_similaire, token);

                if (strcmp(secteur_nom, secteur_similaire) == 0) {
                    fprintf(F_transferts, "Transfert possible vers '%s' à '%s'.\n", secteur_similaire, hopital_nom);
                    printf("Transfert possible vers '%s' à '%s'.\n", secteur_similaire, hopital_nom);
                }
            }
        }
    } else {
        fprintf(F_transferts, "Aucun transfert nécessaire pour le secteur '%s'.\n", secteur_nom);
        printf("Aucun transfert nécessaire pour le secteur '%s'.\n", secteur_nom);
    }

    fclose(F_autres_secteurs);
    fclose(F_transferts);
}
void traiter_reseau() {
       char secteur[50], maladie[50];
       printf("Entrez le nom du secteur à analyser : ");
       scanf("%s", secteur);
       printf("Entrez le nom de la maladie critique : ");
       scanf("%s", maladie);

       reseau(secteur, maladie);
       printf("\nAnalyse terminée. Consultez les résultats dans transferts.txt.\n");
   }

   void supprimer_ligne_par_numero(int numero) {
    const char *nom_fichier = "./lits.txt";
    FILE *fichier = fopen(nom_fichier, "r");
    FILE *temp = fopen("temp.txt", "w");

    if (fichier == NULL || temp == NULL) {
        printf("Erreur lors de l'ouverture des fichiers.\n");
        return;
    }

    char ligne[256];
    while (fgets(ligne, sizeof(ligne), fichier)) {
        char ligne_copy[256];
        strcpy(ligne_copy, ligne);
        char *token = strtok(ligne_copy, ";");

        int compteur = 0;
        int num = -1;

        while (token != NULL) {
            if (compteur == 1) {
                num = atoi(token);
                break;
            }
            token = strtok(NULL, ";");
            compteur++;
        }

        if (num != numero) {
            fputs(ligne, temp);
        }
    }

    fclose(fichier);
    fclose(temp);
    remove(nom_fichier);
    rename("temp.txt", nom_fichier);
    menu_principale();
}

// Fonction pour créer un nouveau nud
Node* createNode(const char* question, float probability) {
    Node* node = (Node*)malloc(sizeof(Node));
    strcpy(node->question, question);
    node->probability = probability;
    node->yes = NULL;
    node->no = NULL;
    return node;
}

// Fonction pour libérer l'arbre
void freeTree(Node* node) {
    if (node == NULL) return;
    freeTree(node->yes);
    freeTree(node->no);
    free(node);
}

// Fonction pour parcourir l'arbre
void traverseTree(Node* node) {
    char response[3];

    if (node == NULL) return;

    // Si c'est une feuille, afficher le résultat et la probabilité
    if (node->yes == NULL && node->no == NULL) {
        printf("Résultat : %s\n", node->question);
        printf("Probabilité estimée : %.2f%%\n", node->probability * 100);
        return;
    }

    // Poser la question
    printf("%s (oui/non) : ", node->question);
    scanf("%s", response);

    // Suivre la branche correspondante
    if (strcmp(response, "oui") == 0) {
        traverseTree(node->yes);
    } else if (strcmp(response, "non") == 0) {
        traverseTree(node->no);
    } else {
        printf("Réponse invalide. Veuillez répondre par 'oui' ou 'non'.\n");
        traverseTree(node);
    }
}

void empiler(ElementPile** sommet, const char* action) {
    ElementPile* nouveau = malloc(sizeof(ElementPile));
    if (!nouveau) {
        perror("Erreur : mémoire insuffisante");
        return;
    }
    strncpy(nouveau->action, action, sizeof(nouveau->action) - 1);
    nouveau->action[sizeof(nouveau->action) - 1] = '\0'; // Assurer la terminaison
    nouveau->suivant = *sommet;
    *sommet = nouveau;
    printf("Action '%s' ajoutée à la pile.\n", action);
}
void depiler(ElementPile** sommet) {
    if (!*sommet) {
        printf("Pile vide, aucune action à dépiler.\n");
        return;
    }
    ElementPile* temp = *sommet;
    printf("Action dépilée : '%s'\n", temp->action);
    *sommet = temp->suivant;
    free(temp);
}
void afficher_pile(const ElementPile* sommet) {
    if (!sommet) {
        printf("Pile vide.\n");
        return;
    }
    printf("Actions dans la pile :\n");
    while (sommet) {
        printf("- %s\n", sommet->action);
        sommet = sommet->suivant;
    }
}


    void Menu() {
    printf("-------------------------------------------------------\n");
    printf("\n   **      classification des maladies et l'organisation de l'hopital lors d'une pandemie avec PandemieCore      **     \n\n");
    printf("           < WELCOME TO THE MAIN MENU >\n\n");
    printf("-------------------------------------------------------\n");
    printf("1-gerrer variants.\n");
    printf("2-gerrer secteur.\n");
    printf("3-gereer patient.\n");
    printf("4-gereer actions.\n");
    printf("5-reseau hopital.\n");
    printf("------------------------------------------------------\n\n");
    printf("Choisissez une option : ");
    }
    void SMenu1() {
    printf("-------------------------------------------------------\n");
    printf("\n                  **mofification des variant**     \n\n");
    printf("-------------------------------------------------------\n");
    printf("1-afficher les informations d'un varaiant.\n");
    printf("2-ajouter variant.\n");
    printf("3-rechercher_par description.\n");
    printf("4-modifier variant\n");
    printf("5-estimation covid\n");
    printf("Choisissez une option : ");
    }
    void SMenu2() {
    printf("-------------------------------------------------------\n");
    printf("\n                  **modifier secteur**     \n\n");
    printf("-------------------------------------------------------\n");
    printf("1-afficher lit valable par secteur.\n");
    printf("2-ajouter lit.\n");
    printf("3-supprimer lit.\n");
    printf("------------------------------------------------------\n\n");
    printf("Choisissez une option : ");
    }
    void SMenu3() {
    printf("-------------------------------------------------------\n");
    printf("\n                  **gerrer patient**     \n");
    printf("-------------------------------------------------------\n");
    printf("1-ajouter patient.\n");
    printf("2-suppression d'un patient.\n");
    printf("3-donner les information d'un patient.\n");
    printf("------------------------------------------------------\n\n");
    printf("Choisissez une option : ");
    }
    void menup() {
    printf("-------------------------------------------------------\n");
    printf("\n                  **gerrer les actions**     \n");
    printf("-------------------------------------------------------\n");
    printf("1-ajouter une action\n");
    printf("2-action precedente\n");
    printf("3-Afficher les actions\n");
    printf("4-Quitter\n");
}
int main() {
    int choix,c,lid,v,cin,t[500],l,p;
    int exists,nb_maladies;
    char vname[20],sname[20],ch[3],nom[10],nom_m[20], maladies[500][50];
    secteur *L=NULL;
        Menu();
        scanf("%d", &choix);
        switch (choix) {
            case 1: {
    SMenu1();
    scanf("%d", &choix);
    switch (choix) {
        case 1: {
        printf("ecrire le nom du variant à chercher ses informations\n");
        scanf("%s",vname);
        afficher_variant(vname);
        menu_principale();
        }

        case 2: {
            ajouter_variant();
            menu_principale();
            }
        case 3:{
         char mot_recherche[100];
         printf("Entrez un mot à rechercher dans la description : ");
         scanf("%99s", mot_recherche);
         rechercher_par_description(mot_recherche);
         menu_principale();
        }
        case 4:{
           char nom[50], nouvelle_desc[100];

    printf("Entrez le nom du variant : ");
    scanf("%49s", nom);
    getchar();

    printf("Entrez la nouvelle description : ");
    fgets(nouvelle_desc, sizeof(nouvelle_desc), stdin);
    nouvelle_desc[strcspn(nouvelle_desc, "\n")] = '\0';

    modifier_description_variant(nom, nouvelle_desc);
    menu_principale();

        case 5:{
            Node* root = createNode("Température>38C ?", 0.0);
    root->yes = createNode("Toux sèche ?", 0.0);
    root->no = createNode("Température < 37C ?", 0.0);

    root->yes->yes = createNode("Perte d'odorat ougoût ?", 0.0);
    root->yes->no = createNode("Fatigue excessive ?", 0.0);
    root->no->yes = createNode("Symptômes grippaux ?", 0.0);
    root->no->no = createNode("Contact avec un cas confirmé ?", 0.0);

    root->yes->yes->yes = createNode("COVID probable", 0.90);
    root->yes->yes->no = createNode("COVID possible", 0.75);
    root->yes->no->yes = createNode("COVID possible", 0.60);
    root->yes->no->no = createNode("COVID peu probable", 0.30);
    root->no->yes->yes = createNode("Grippe ou autre infection", 0.50);
    root->no->yes->no = createNode("Pas de COVID", 0.10);
    root->no->no->yes = createNode("COVID possible (asymptomatique)", 0.40);
    root->no->no->no = createNode("Pas de COVID", 0.05);

    // Parcourir l'arbre
    printf("Prédiction COVID-19 avec probabilité\n");
    traverseTree(root);
    menu_principale();
    return 0;
            }
}

    }
            }
           case 2:{
        SMenu2();
        scanf("%d",&choix);
        switch(choix){
            case 2:{
            printf("vous allez ajouter un lit\n");
            variant_par_secteur();
            printf("donner le nom du secteur\n");
            scanf("%s",sname);
            printf("donner le numero du lit\n");
            scanf("%d",&lid);
            printf("ce lit est t'il valable ?(1 pour non,0 pour oui)\n");
            scanf("%d",&v);
            printf("donner le cin de lutilisateur");
            scanf("%d",&cin);
            L=ajouter_lit(L,sname,lid,v,cin);
            printf("lit ajouté avec succes");
            printf("voulez vous le sauvegarder ?(ecrire oui ou non)");
            scanf("%s",&ch);
            if (strcmp(ch, "oui") == 0){
                sauvegarder(L,"./lits.txt");
            }
            menu_principale();
            }
            case 1:{
                printf("donner le nom du secteur\n");
                scanf("%s",sname);
                rechercher_par_secteur(sname);
                menu_principale();
            }
            case 3:{
                int numero;

                printf("Entrez le numero à supprimer : ");
                scanf("%d", &numero);

                supprimer_ligne_par_numero(numero);
                printf("La ligne avec le numero %d a été supprimée.\n", numero);

    return 0;

    return 0;


               }
        }

        }
        case 3:{
        SMenu3();
        scanf("%d",&choix);
        if (choix==1){
         ajouter_patient();
         menu_principale();}
        else if (choix==2){
            traiter_suppression();
                menu_principale();
                break;
        }
        else if (choix==3){
                char cin[50];
                printf("Entrez le CIN du patient : ");
                scanf("%49s", cin);
                rechercher_patient_par_cin(cin);
                menu_principale();
                break;
}
        }
        case 4:{
        ElementPile* pile = NULL; // Initialiser la pile comme vide
    char action[100]; // Tableau pour stocker l'action saisie
    int choix;

    do {
        menup(); // Afficher le menu
        printf("Entrez votre choix : ");
        scanf("%d", &choix); // Lire le choix de l'utilisateur
        getchar(); // Consommer le '\n' restant après scanf

        switch (choix) {
            case 1: // Empiler une action
                printf("Entrez l'action à empiler : ");
                fgets(action, sizeof(action), stdin); // Lire l'action
                action[strcspn(action, "\n")] = '\0'; // Supprimer le saut de ligne
                empiler(&pile, action); // Empiler l'action
                break;

            case 2: // Dépiler une action
                depiler(&pile); // Dépiler une action
                break;

            case 3: // Afficher la pile
                afficher_pile(pile); // Afficher la pile
                break;

            case 4: // Quitter
                printf("Au revoir!\n");
                break;

            default:
                printf("Choix invalide. Veuillez essayer à nouveau.\n");
        }
    } while (choix != 4); // Quitter lorsque l'utilisateur choisit '4'



        menu_principale();
        return;
        }
       case 5:{
                traiter_reseau();
                menu_principale();
                break;
            }

       }
        }
