#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

//ici
File *fic=fopen("Sauvegarde.txt",wb);

int grille[3][3];
pthread_mutex_t verrou;



void createGrid(){

    for(int i=0; i<3; i++){
        for(int j=0; j<3; j++){
            grille[i][j] = 0;
            printf(" %d |",grille[i][j]);

        }

        printf("\n------------\n");
    }
}

void afficheGrille(){
    for(int i=0; i<3; i++){
        for(int j=0; j<3; j++){
            printf(" %d |",grille[i][j]);

        }
        printf("\n------------\n");
    }
}


int aGagne(){
    int win=0;
    for(int i=0; i<3; i++){
       for( int j=0; j<3; j++){
           if(grille[i][j]!=0){
                if(grille[i][j]==grille[i][j+1] && grille[i][j]==grille[i][j+2]) {
                    win=1;
                }
                if(grille[i][j]==grille[i+1][j] && grille[i][j]==grille[i+2][j+2]) {
                    win=1;
                }
           }
       }
    }

    if((grille[0][0]!=0)&&(grille[0][0] == grille[1][1]) && (grille[0][0] == grille[2][2])){
        win = 1;
    }
    if((grille[0][2]!=0)&&(grille[0][2] == grille[2][2]) && (grille[2][0] == grille[2][2])){
        win = 1;
    }
    return win;
}

int grillePleine(){
    int plein = 0;
    int nbJoue;
    for(int i =0; i<3; i++){
        for(int j=0; j<3; j++){
            if(grille[i][j] != 0){
                nbJoue++;
            }
        }
    }

    if (nbJoue == 9){
        plein = 1;
        return plein;
    }
    return plein;
}

int posMarquee(int i, int j){
    int estMarquee = 0;
    if(grille[i][j]!=0) estMarquee = 1;
    return estMarquee;
}

void jouerOrdis(int symbJeu){
    srand(time(NULL));
    int li, col, max=2;
    do{
        li = rand() % (max + 1);
        col = rand() % (max + 1);
    }while (posMarquee(li,col) == 1);
     grille[li][col] = symbJeu;
     //ici
    char text[]="a jouer à la case";
    fprintf(fic,"%s %d %d"text,col,fic);
    fwrite(grille,fic);
    //ici
   
}
void jouer(int symbJeu){
    int li, col;
    printf("Entrer la ligne [1-3] et la colonne [1-3] à jouer: ");
    scanf("%d %d", &li,&col);
     grille[li-1][col-1] = symbJeu;
    //ici
    char text[]="a jouer à la case";
    fprintf(fic,"%s %d %d"text,li,col);
    fwrite(grille,fic);
    //ici
   
}

void * jouerPC1(){
    printf("C'est au tour du thread 1 de jouer !\n");
    pthread_mutex_lock(&verrou);
    jouerOrdis(1);
    afficheGrille();
    if(aGagne()==1){
        printf("Le THREAD 1 a gagné !");
        return;
    }
    pthread_mutex_unlock(&verrou);
    sleep(1);
    return NULL;
}

void * jouerPC2(){
    printf("C'est au tour du thread 2 de jouer !\n");
    pthread_mutex_lock(&verrou);
    jouerOrdis(2);
    afficheGrille();
    if(aGagne()==1){
        printf("Le THREAD 2 a gagné !");
        return;
    }
    pthread_mutex_unlock(&verrou);
    sleep(1);
    return NULL;
}

int main()
{
    int choix;
    char reponse[10];
    char j1[10], j2[10], j[10];
    int tourDeJeu = 1;
    printf("Bienvenue dans le jeu de TIC TAC TOE \n");
    printf("1. Joueur vs. Joueur\n");
    printf("2. Joueur vs. PC\n");
    printf("3. PC vs. PC\n");
    printf("Entrez votre choix : ");
    scanf("%d", &choix);

   do{
        if (choix == 1){ // 1 vs 1
                printf(" 1 -> Symbole de jeu du joueur 1 \n");
                printf(" 2 -> ------------------------ 2 \n");
                printf("Nom du joueur1 : ");
                scanf("%s",j1);
                printf("Nom du joueur2 : ");
                scanf("%s",j2);
                createGrid();
                while (grillePleine() != 1){
                    if (tourDeJeu == 1){
                        printf("C'est a %s de jouer !\n", j1);
                        //ici
                        fprintf(fic,"%s"j1);
                        //ici
                        jouer(1);
                        afficheGrille();
                        if (aGagne()==1){
                            printf("%s a gagne !", j1);
                             //ici
                            char tt="a gagner!";
                            fprintf(fic,"%s %s"j1,tt);
                            break;
                        }
                        tourDeJeu++;
                    }
                    if(tourDeJeu == 2){
                        printf("C'est a %s de jouer !\n", j2);
                        //ici
                        fprintf(fic,"%s"j1);
                        //ici
                        jouer(2);
                        afficheGrille();
                        if (aGagne()==1){
                            printf("%s a gagne !", j2);
                            //ici
                            char tt="a gagner!";
                            fprintf(fic,"%s %s"j2,tt);
                            //ici
                            break;
                        }
                        tourDeJeu--;
                    }
                }

        }

        if(choix == 2){
            printf(" 1 -> Symbole de jeu du joueur  \n");
            printf(" 2 -> -------------- de la machine \n");
            printf("Nom du joueur1 : ");
            scanf("%s",j);
            createGrid();
            while (grillePleine() != 1){
                    if (tourDeJeu == 1){
                        printf("C'est a %s de jouer !\n", j);
                        //ici
                        fprintf(fic,"%s"j1);
                        //ici
                        jouer(1);
                        afficheGrille();
                        if (aGagne()==1){
                            printf("%s a gagne !", j);
                             //ici
                            char tt="a gagner!";
                            fprintf(fic,"%s %s"j,tt);
                            //ici
                            break;
                        }
                        tourDeJeu++;
                    }
                    if(tourDeJeu == 2){
                        printf("C'est a la machine de jouer : \n");
                        jouerOrdis(2);
                        afficheGrille();
                        if (aGagne()==1){
                            printf("%s a gagne !", j2);
                             //ici
                            char tt="a gagner!";
                            fprintf(fic,"%s %s"j2,tt);
                            //ici
                            break;
                        }
                        tourDeJeu--;
                    }

                }

        }
        if(choix==3){
        afficheGrille();
        printf("\n");
        while(grillePleine()!=1){
            pthread_t t1, t2;
            pthread_mutex_init (&verrou, NULL);
            pthread_create(&t1, NULL, jouerPC1, (void *) grille );

            pthread_create(&t2, NULL, jouerPC2, (void *) grille );

            pthread_join(t1, NULL);
            pthread_join(t2, NULL);
            //pthread_mutex_destroy(&verrou);

        }
            exit(0);
        }
        printf("Voulez vous toujours jouer (O/N)?");
        scanf("%s",reponse);
    }while(reponse=='o' || reponse=='O');
    fclose(fic);

}


