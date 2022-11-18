#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#define MAX 10


FILE * fic; // fichier pour l'historique d'un jeu
FILE * sav; // fichier qui permet de simuler à partierd'un grille renseignée
int taille; // taille de la grille de jeu (3 à 10)
char grille[MAX][MAX];/*={{' ',' ',' '},{' ',' ',' '},{' ',' ',' '}};*/ // tableau dont la taille represente la taille max qu'on peut jouer (10)
pthread_mutex_t verrou; //semaphore
int tourDeJeu,  temoin=0, temoin1=0;
int nbVictoire1=0, nbVictoire2=0; //socke le nombre de fois gagnes par les threads1 ou j1 et thread2 ou j2

pthread_t pc1, pc2;

/*permet d'initialiser les cases de la grille à 0 */
void initGrid(){
    for(int i=0; i<taille; i++){
        for(int j=0; j<taille; j++){
            grille[i][j] = ' ';
        }
    }
}

/* formatte l'affichage de la grille de jeu selon la taille */
void afficheGrille(){
    for(int i=0; i<taille; i++){
        for(int j=0; j<taille; j++){
            printf(" %c |",grille[i][j]);
            fprintf(fic," %c |",grille[i][j]);
            fprintf(sav, "%c",grille[i][j]);
        }

        switch(taille){
            case 3:
            printf("\n------------\n"); //4*taille
            fputs("\n------------\n",fic);
            break;
            case 4:
            printf("\n----------------\n"); //4*taille
            fputs("\n----------------\n",fic);
            break;
            case 5:
            printf("\n--------------------\n"); //4*taille
            fputs("\n--------------------\n",fic);
            break;
            case 6:
            printf("\n------------------------\n"); //4*taille
            fputs("\n------------------------\n",fic);
            break;
            case 7:
            printf("\n----------------------------\n"); //4*taille
            fputs("\n----------------------------\n",fic);
            break;
            case 8:
            printf("\n--------------------------------\n"); //4*taille
            fputs("\n--------------------------------\n",fic);
            break;
            case 9:
            printf("\n------------------------------------\n"); //4*taille
            fputs("\n------------------------------------\n",fic);
            break;
            case 10:
            printf("\n----------------------------------------\n"); //4*taille
            fputs("\n----------------------------------------\n",fic);
            break;
        }

    }
    printf("\n");
    fputs("\n",fic);
}

/* cette fonction prend rien en parametre, et retourne un entier:1 permettant de verifier si quelqu'un a gagne */
int aGagne(){
    int win, val1, val2, val3, val4;
    int nb1=0, nb2=0, nb3=0, nb4=0;
    for(int i=0; i<taille; i++){
           for( int j=0; j<taille; j++){
                if((grille[i][0]!=' ') &&(grille[i][0]==grille[i][j])){
                    nb1++;
                }
                if((grille[0][i]!=' ') &&(grille[0][i]==grille[j][i])){
                    nb2++;
                }
                if((grille[0][0]!=' ')  && (grille[0][0]==grille[j][j])){
                    nb3++;
                }
                if((grille[0][taille-1]!=' ') &&(grille[0][taille-1]==grille[j][taille-1-j])) {
                    nb4++;
                }
            }
            if((nb1==taille)||(nb2==taille)||(nb3==taille)||(nb4==taille)){
                win = 1;
                return win;
            }

            else {
                nb1=0;
                nb2=0;
                nb3=0;
                nb4=0;
            }

    }


}

/* permet de verifier si la grille de jeu est pleine en retournant 1 ou 0 sinon */
int grillePleine(){
    for(int i =0; i<taille; i++){
        for(int j=0; j<taille; j++){
            if(grille[i][j] == ' '){
                return 0;
            }
        }
    }
    return 1;
}

/* permet de verifier si une case a ete deja joue */
int caseJouee(int i, int j){
    int caseJouee = 0;
    if(grille[i][j]!= ' ') caseJouee = 1;
    return caseJouee;
}

/*permet au pc de jouer une case en prenant en parametre un symbole de jeu (entier)*/
void jouerOrdis(char c){
    srand(time(NULL));
    int li, col, max=taille-1;
    do{
        li = rand() % (max + 1);
        col = rand() % (max + 1);
    }while (caseJouee(li,col) == 1);
    grille[li][col] = c;
    fprintf(fic,"La machine%c a joue : %d %d\n",c,li,col);
}

void jouerPCs(){
    srand(time(NULL));
    int li, col, max=taille-1;
    do{
        li = rand() % (max + 1);
        col = rand() % (max + 1);
    }while (caseJouee(li,col) == 1);
//    if(tourDeJeu == 1) grille[li][col] = 1;
 //   if(tourDeJeu == 2) grille[li][col] = 2;
    //grille[li][col] = symbJeu;
    //fprintf(fic,"La machine%d a joue : %d %d\n",symbJeu,li,col);
}

/*permet à un joueur de jouer sur une case, le parametre est le symbole de jeu dans la grille*/
void jouer(char symbJeu){
    int li, col;
    do{
        printf("Entrer la ligne [1-%d] et la colonne [1-%d] à jouer: ",taille,taille);
        scanf("%d %d", &li,&col);
    }while (caseJouee(li-1,col-1) == 1);
    grille[li-1][col-1] = symbJeu;
    fprintf(fic,": %d %d\n",li,col);
}

/*permet au thread1 de pouvoir jouer */
void * jouerPC(){
    pthread_mutex_lock(&verrou);
    if(pthread_equal(pc1, pthread_self())) {

        if(aGagne()!=1){
            if(grillePleine()==1){
                printf("\nPartie NULLE !\n");
                fputs("\nPartie NULLE !\n",fic);
                temoin = 1;
                PTHREAD_CANCELED;
                return NULL;
            }
            else{
                printf("C'est au tour du THREAD1 de jouer !\n");
                fputs("C'est au tour du THREAD1 de jouer !\n",fic);
            }

        }
        jouerOrdis('x');
        afficheGrille();

        if(aGagne()==1){
            printf("Le THREAD1 a gagne! \n");
            fputs("Le THREAD1 a gagne! \n",fic);
            nbVictoire1++;
            temoin=1;
            PTHREAD_CANCELED;
            return NULL;
        }

    }
    if(pthread_equal(pc2, pthread_self())) {

        if(aGagne()!=1){
            if(grillePleine()==1){
                printf("\nPartie NULLE !\n");
                fputs("\NPartie NULLE !\n",fic);
                temoin = 1;
                PTHREAD_CANCELED;
                return NULL;

            }
            else{
                printf("C'est au tour du THREAD2 de jouer !\n");
                fputs("C'est au tour du THREAD2 de jouer !\n",fic);
            }

        }
        jouerOrdis('o');
        afficheGrille();
        if (grillePleine()==1) temoin1=1;
        if(aGagne()==1){
            printf("Le THREAD2 a gagne! \n");
            fputs("Le THREAD2 a gagne! \n",fic);
            nbVictoire2++;
            temoin = 1;
            PTHREAD_CANCELED;
            return NULL;

        }
    }
    pthread_mutex_unlock(&verrou);
    sleep(1);
    return NULL;
}

/*permet au thread 2 de jouer
void * jouerPC2(){
    if(aGagne(2)!=1){
        if(grillePleine()==1){
            printf("\nPartie NULLE!\n");
            return;
        }
        printf("C'est au tour du thread 2 de jouer !\n");
    }

    else{
        return;
    }
    pthread_mutex_lock(&verrou);
    jouerOrdis(2);
    afficheGrille();
    if(aGagne(2)==1){
        printf("Le THREAD 2 a gagné !\n");
        fputs("Le THREAD 2 a gagne ! \n",fic);
        nbVictoire2++;
        return;
    }
    pthread_mutex_unlock(&verrou);
    sleep(1);
    return NULL;
}*/

/*permet d'indiquer qui joue en premier dans l'option jouerVSjouer et jueurVSpc*/
int joueEnPremier(){
    srand(time(NULL));
    int nb = rand()%2;
    if(nb==0) return 1;
    else return 2;
}

/* represente le jeu de l'option pcVSpc, prend ch en parametre qui indique le thread jouant en premier*/
void pcVSpc(int ch){
        initGrid();
        afficheGrille();
        srand(time(NULL));
        int ordreJeu;
        if(ch==1) ordreJeu=0;
        if(ch==2) ordreJeu=1;
        if(ch==3) ordreJeu = rand()%2;


        //pthread_t pc1, pc2;
        pthread_mutex_init (&verrou, NULL);

        //tourDeJeu = joueEnPremier();

        //pthread_create(&pc1, NULL, jouerPC('x'), (void *) grille );
        //pthread_create(&pc2, NULL, jouerPC('o'), (void *) grille );


        int nb =1;
        while(1){
            //if (tourDeJeu==1){
                //pthread_create(&pc1, NULL, jouerPC('x'), (void *) grille );
                //tourDeJeu++;
            //}
            //if (tourDeJeu==2){
                //pthread_create(&pc2, NULL, jouerPC('o'), (void *) grille );
                //tourDeJeu--;

            //}


            switch(ordreJeu){
                case 0:
                    pthread_create(&pc2, NULL, jouerPC, (void *) grille );
                    pthread_create(&pc1, NULL, jouerPC, (void *) grille );
                    break;
                default:
                    pthread_create(&pc1, NULL, jouerPC, (void *) grille );
                    pthread_create(&pc2, NULL, jouerPC, (void *) grille );

            }

            //pthread_create(&pc1, NULL, jouerPC, (void *) grille );
            //if(pthread_equal(pc1, pthread_self())) jouerOrdis('o');
            //printf("\n%d\n", grillePleine());
            //pthread_create(&pc2, NULL, jouerPC, (void *) grille );
            pthread_join(pc1, NULL);
            pthread_join(pc2, NULL);

            if(temoin==1  || grillePleine()==1) break;
            //nb++;

            //pthread_cancel(pc1);
            //pthread_cancel(pc2);
            /*pthread_cancel(pc1);
            pthread_cancel(pc2);*/


        }


}

/*permet de faire des simulations de pcVSpc avec des stats*/
void statistique(){
    int nbSimulations, pctage1, pctage2, pctage3, nbNull, cpt=0, ch;
    printf("Entrez le nombre de simulations à faire : ");
    scanf("%d",&nbSimulations);
    printf("Quel thread joue en premier ? 1.Thread1 2.Thread2 3.Aleatoirement :");
    scanf("%d",&ch);
    while(cpt<nbSimulations){
        pcVSpc(ch);
        initGrid();
        cpt++;
        temoin=0;
    }
    nbNull = nbSimulations - (nbVictoire1+nbVictoire2);
    pctage1 = (int)nbVictoire1*100/nbSimulations;
    pctage2 = (int)nbVictoire2*100/nbSimulations;
    pctage3 = 100 - (pctage1+pctage2);
    printf("\nSur %d simulations le THREAD1 a gagné %d pourcent (%d) des parties et le THREAD2 %d pourcent (%d); et il y'a %d pourcent (%d) de partie nulle. \n",nbSimulations,pctage1,nbVictoire1,pctage2,nbVictoire2,pctage3,nbNull);
    fprintf(fic,"Sur %d simulations le THREAD1 a gagné %d pourcent  (%d) des parties et le THREAD2 %d pourcent (%d); et il y'a %d pourcent (%d) de partie nulle. \n",nbSimulations,pctage1,nbVictoire1,pctage2,nbVictoire2,pctage3,nbNull);

}


int main()
{
    /*menu du jeu */
    fic=fopen("Historique.txt","w");
    sav=fopen("Sauvegarde.txt","r");
    int choix, nbParties;
    char reponse[1];
    char val[1]="o";
    char j1[10], j2[10], j[10];
    int tourDeJeu;
    //int tourDeJeu = joueEnPremier();
    printf("*** Bienvenue dans le jeu de TIC TAC TOE ***\n\n");
    printf("1. Joueur vs. Joueur\n");
    printf("2. Joueur vs. PC\n");
    printf("3. PC vs. PC\n");
    printf("4. Simuler à partir de l'historique \n");
    printf("5. Statistiques \n");

    do{
        printf("\nEntrez votre choix : ");
        scanf("%d", &choix);
    }while(!(choix>=1 && choix<=5));

    do{
        printf("\nChoisir votre taille de grille de morpion (de 3 à 10) : ");
        scanf("%d",&taille);
    }while(!(taille>=3 && taille<=10));



    do{
        switch(choix){
        case 1:
                printf("\nSur combien de parties voulez vous jouer ? : ");
                scanf("%d",&nbParties);
                printf(" 1 -> Symbole de jeu du joueur 1 \n");
                printf(" 2 -> ------------------------ 2 \n");
                printf("Nom du joueur1 : ");
                scanf("%s",j1);
                printf("Nom du joueur2 : ");
                scanf("%s",j2);
                for(int i=0;i<nbParties;i++){
                    initGrid();
                    afficheGrille();
                    tourDeJeu = joueEnPremier();
                    while (grillePleine() != 1){
                        if (tourDeJeu == 1){
                            printf("C'est a %s de jouer !\n", j1);
                            fprintf(fic,"%s",j1);
                            fputs(" a joué a la case d'indices ",fic);
                            jouer('x');
                            afficheGrille();
                            if (aGagne()==1){
                                nbVictoire1++;
                                printf("%s a gagne !\n", j1);
                                fprintf(fic,"%s a gagne ! \n",j1);
                                break;
                            }
                            tourDeJeu++;
                        }
                        if(tourDeJeu == 2){
                            printf("C'est a %s de jouer !\n", j2);
                            fprintf(fic,"%s",j2);
                            fputs(" a joue a la case d'indices ",fic);
                            jouer('o');
                            afficheGrille();
                            if (aGagne()==1){
                                nbVictoire2++;
                                printf("%s a gagne !\n", j2);
                                fprintf(fic,"%s a gagne ! \n",j2);
                                break;
                            }
                            tourDeJeu--;
                        }
                    }
                    if(aGagne(1)!=1){
                        printf("\nPartie NULLE! ");
                    }
                }
                printf("\n*** Sur %d parties ***\n%s %d : %d %s",nbParties,j1,nbVictoire1,nbVictoire2,j2);
                fprintf(fic,"\n!! Sur %d parties !!\n%s %d : %d %s",nbParties,j1,nbVictoire1,nbVictoire2,j2);
                if(nbVictoire1>nbVictoire2) {
                    printf("\nLe gagnant final est : %s\n", j1);
                    fprintf(fic,"\nLe gagnant final est : %s\n", j1);
                }
                if(nbVictoire1<nbVictoire2) {
                    printf("\nLe gagnant final est : %s\n", j2);
                    fprintf(fic,"\nLe gagnant final est : %s\n", j2);
                }
                if(nbVictoire1==nbVictoire2) {
                    printf("\nEGALITE ! Personne n'a gagné. \n");
                    fputs("\nEGALITE ! Personne n'a gagné. \n", fic);
                }
                break;

        case 2:
            printf("\nSur combien de parties voulez vous jouer ? : ");
            scanf("%d",&nbParties);
            printf(" 1 -> Symbole de jeu du joueur  \n");
            printf(" 2 -> -------------- de la machine \n");
            printf("Nom du joueur1 : ");
            scanf("%s",j);
            for(int i=0;i<nbParties;i++){
                initGrid();
                afficheGrille();
                tourDeJeu = joueEnPremier();
                while (grillePleine() != 1){
                        if (tourDeJeu == 1){
                            printf("C'est a %s de jouer !\n", j);
                            fprintf(fic,"%s",j);
                            fputs(" a joue a la case d'indices ",fic);
                            jouer('x');
                            afficheGrille();
                            if (aGagne()==1){
                            nbVictoire1++;
                                printf("%s a gagne !\n", j);
                                fprintf(fic,"%s a gagne !\n",j);
                                break;
                            }
                            tourDeJeu++;
                        }
                        if(tourDeJeu == 2){
                            printf("C'est a la machine de jouer : \n");
                            jouerOrdis('o');
                            afficheGrille();
                            if (aGagne()==1){
                                nbVictoire2++;
                                printf("Le machine a gagne !\n");
                                fputs("La machine a gagne !\n",fic);
                                break;
                            }
                            tourDeJeu--;
                        }
                }
                if(aGagne(2)!=1){
                    printf("\nPartie NULLE! ");
                }
            }
            printf("\n*** Sur %d parties ***\n%s %d : %d machine",nbParties,j,nbVictoire1,nbVictoire2);
            fprintf(fic,"\n!! Sur %d parties !!\n%s %d : %d machine",nbParties,j,nbVictoire1,nbVictoire2);
            if(nbVictoire1>nbVictoire2) {
                printf("\nLe gagnant final est : %s\n", j);
                fprintf(fic,"\nLe gagnant final est : %s\n", j);
            }
            if(nbVictoire1<nbVictoire2) {
                printf("\nLe gagnant final est la machine");
                fputs("\nLe gagnant final est la machine",fic);
            }
            if(nbVictoire1==nbVictoire2) {
                printf("\nEGALITE ! Personne n'a gagné. \n");
                fputs("\nEGALITE ! Personne n'a gagné. \n", fic);
            }
            break;
        case 3:
            pcVSpc(3);
            break;

        case 4:
            char c;
            int nb, i=0, r=0, tab[MAX*MAX];
            while((c=fgetc(sav))!=EOF && i<MAX*MAX){
                int nb= (int)c-'0';
                tab[i] = nb;
                i++;
            }
            for(int i=0;i<taille;i++){
                for(int j=0;j<taille;j++){
                    grille[i][j]= tab[r];
                    r++;
                }
            }
            pcVSpc(3);
            fclose(sav);
            break;

        case 5:
            statistique();
            break;

        }

        fclose(fic);
        printf("Voulez vous jouer encore (o/n)? : ");
        scanf("%s",reponse);



    }while(reponse=='o' || reponse=='O')/*while(strcmp(val,reponse)==111)*/;


}



