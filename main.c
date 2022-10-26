#include <stdio.h>
#include <stdlib.h>

int grille[3][3];

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
        printf("\n---------\n");
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
    int li, col, max=0;
    do{
        li = rand() % (max + 1);
        col = rand() % (max + 1);
    }while (posMarquee(li,col) == 1);
    grille[li][col] = symbJeu;
}
void jouer(int symbJeu){
    int li, col;
    printf("Entrer la ligne [1-3] et la colonne [1-3] à jouer: ");
    scanf("%d %d", &li,&col);
    grille[li-1][col-1] = symbJeu;
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
                        jouer(1);
                        afficheGrille();
                        if (aGagne()==1){
                            printf("%s a gagne !", j1);
                            break;
                        }
                        tourDeJeu++;
                    }
                    if(tourDeJeu == 2){
                        printf("C'est a %s de jouer !\n", j2);
                        jouer(2);
                        afficheGrille();
                        if (aGagne()==1){
                            printf("%s a gagne !", j2);
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
                        jouer(1);
                        afficheGrille();
                        if (aGagne()==1){
                            printf("%s a gagne !", j);
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
                            break;
                        }
                        tourDeJeu--;
                    }
                }

        }


        printf("Voulez vous toujours jouer (O/N)?");
        scanf("%s",reponse);
    }while(reponse=='o' || reponse=='O');

}
