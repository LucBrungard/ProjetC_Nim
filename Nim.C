#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define vmin 5
#define vmax 30

typedef struct // création d'une structure qui correspond à une case de tableau
{
    int ilig;
    int icol;
}T_case;


typedef struct //création d'une structure qui permet d'enregistrer les paramètres du partie
{
    int nlig;
    int ncol;
    int niveau;
    int next;
    int nban;
}params;

T_case T_tab_case[vmax*vmax]; //déclaration d'un tableau qui possède une taille en fonction du nombre maximum de ligne et de colonnes
T_case ban[vmax];
int nim[vmax][vmax];

int lire_entier(int a, int b) // fonction lire_entier qui permet de renrer  une valeur entre un minimum et un maximum
{
    int nb=0;
    while (nb>b||nb<a)
    {
        printf("Donner une valeur comprise entre %d et %d :\n",a, b);
        scanf("%d",&nb);
    }
    printf("\n");

    return nb;
}


int max(int n1, int n2) //fonction max qui permet de retourner le maximum entre 2 nombres
{
    int maximum;
    if (n1<n2)
        maximum=n2;
    else
        maximum=n1;

    return maximum;
}


params parametres() // fonction d'initialisation des paramètres
{
    params resultat;

    printf("Combien de lignes ?\n");
    resultat.nlig=lire_entier(vmin, vmax);

    printf("Combien de colonnes ?\n");
    resultat.ncol=lire_entier(vmin, vmax);

    printf("Quel niveau de jeu souhaitez-vous ?\n");
    printf("(1.Debutant  2.Intermediaire  3.Talentueux  4.Impossible)\n");
    resultat.niveau=lire_entier(1, 4);

    printf("Choisissez qui commence (1 pour l'ordinateur, 2 pour le joueur)\n");
    resultat.next=lire_entier(1, 2);

    resultat.nban=rand()% max(resultat.nlig,resultat.ncol)+1;

    return resultat;
}


//affichage de la table
void remplit(params tab)
{
    int i,j;
    for (i=0;i<tab.nlig;i++)
    {
        for(j=i*tab.ncol;j<tab.ncol*(i+1);j++)
        {
            T_tab_case[j].ilig=i;
            T_tab_case[j].icol=j%tab.ncol;
        }
    }
}

void affichet(params par, T_case pion)
{
    int i,j,k,bannie,compt=1;

    printf("   |");
    //affichage du numéro de colonne
    for (i=0; i<par.ncol; i++)
    {
        printf(" %2d ", compt);
        compt+=1;
    }
    printf("\n");

    //affichage d'une barre de séparation
    for (i=0; i<4*par.ncol+4; i++)
    {
        printf("_", compt);
        compt+=1;
    }
    printf("\n");

    for (i=0; i<par.nlig; i++)
    {
        printf("%2d |",i+1);
        for(j=i*par.ncol; j<par.ncol*(i+1); j++)
        {
            bannie=0;

            for (k=0; k<par.nban; k++)
            {
                if (T_tab_case[j].ilig==ban[k].ilig && T_tab_case[j].icol==ban[k].icol)
                    bannie=1;
            }

            if (T_tab_case[j].ilig == pion.ilig && T_tab_case[j].icol == pion.icol)
                printf("  O ");

            else if (bannie)
                printf("  X ");

            else
                printf("  - ");
        }
        printf("\n");
    }
}


int bannissable(int i, params tab)
{
    int j;
    // test des cases sur la bordure extérieure
    if ((T_tab_case[i].ilig==0) && (T_tab_case[i].icol==tab.ncol-1) || (T_tab_case[i].ilig==tab.nlig-1) && (T_tab_case[i].icol==0))
        return (1);

    if ((T_tab_case[i].ilig==0) || (T_tab_case[i].icol==0) || (T_tab_case[i].ilig==tab.nlig-1) || (T_tab_case[i].icol==tab.ncol-1))
        return (0);

    for (j=0; j<tab.nban; j++) //parcours du tableau de cases bannies
    {
        //si la case est déjà bannie, on retourne 0
        if ((T_tab_case[i].ilig==ban[j].ilig) && (T_tab_case[i].icol==ban[j].icol))
            return (0);

        //si la case en diagonale en haut à droite de celle actuelle est bannie, on retourne 0
        if ((T_tab_case[i-tab.ncol+1].ilig==ban[j].ilig) && (T_tab_case[i-tab.ncol+1].icol==ban[j].icol))
            return (0);

        //si la case en diagonale en bas à gauche de celle actuelle est bannie, on retourne 0
        if ((T_tab_case[i+tab.ncol-1].ilig==ban[j].ilig) && (T_tab_case[i+tab.ncol-1].icol==ban[j].icol))
            return (0);
    }
    return (1);
}

void hasard_ban(params tab)
{
    int i, i_tab_ban=0;

    //tant qu'on a pas atteint le nombre de cases bannies
    while (i_tab_ban < tab.nban)
    {
        //au hasard
        i = rand()%(tab.nlig*tab.ncol);

        //si la case est bannissable
        if (bannissable(i, tab))
        {
            ban[i_tab_ban].ilig=T_tab_case[i].ilig;
            ban[i_tab_ban].icol=T_tab_case[i].icol;
            i_tab_ban+=1;
        }
    }

    /*
    printf("cases bannies :");
    for (i=0;i<tab.nban;i++)
    {
        printf(" (%d:%d)",ban[i].ilig+1, ban[i].icol+1);
    }
    printf("\n");
    */
}


void calcul_nimbers(params tab)
{
    int i=tab.nlig-1, j=tab.ncol-1, k, full=1, vois;
    nim[i][j]=0;

    //tant que toutes les cases ne sont pas numérotées
    while (full<=tab.nlig*tab.ncol)
    {
        vois=0;

        //vérification si la case possède des voisines dans le tableau
        for (k=1; k<=2; k++)
        {
            //si la case est à l'intérieur du tableau
            if ((i+k)<tab.nlig)
                //si la case en dessous possède la valeur 0 (différent de 1)
                if (nim[i+k][j]!=1)
                    vois=1;

            if ((j+k)<tab.ncol)
                //si la case à droite possède la valeur 0 (différent de 1)
                if (nim[i][j+k]!=1)
                    vois=1;
        }

        //si une des cases voisines vérifiées possède la valeur 0, alors la case prend la valeur 1, sinon la case prend la valeur 0
        if (vois!=0)
            nim[i][j]=1;
        else
            nim[i][j]=0;

        //on rajoute +1 au compteur de cases notées
        full+=1;

        //on remonte d'une ligne
        i-=1;
        //si on est à la première ligne on recommence à la dernière ligne et on change de colonne
        if (i<0)
        {
            i=tab.nlig-1;
            j-=1;
        }
    }

    /*
    //affichage du tableau de nim
    for (i=0; i<tab.nlig; i++)
    {
        for (j=0; j<tab.ncol; j++)
        {
            printf(" %d ",nim[i][j]);
        }
        printf("\n");
    }
    */
}

int coup_pos(params par, T_case pion, T_case *tab)
{
    T_case npion;
    int mouvcol=1, mouvlig=1, i_coup_pos=0, i, j;

    //on cherche les cases sur la même colonne pour lesquelles le mouvement est possible
    for (j=1; j<=2; j++)
    {
        i=0;
        //on vérifie toutes les cases du tableau des cases bannies
        while (i<par.nban)
        {
            //si la case en dessous est bannie le mouvement n'est pas possible
            if ((pion.ilig+j==ban[i].ilig) && (pion.icol==ban[i].icol))
                mouvcol=0;

            if (pion.ilig==ban[i].ilig && pion.icol+j==ban[i].icol)
                mouvlig=0;

            //si la case est en dehors du tableau alors le mouvement n'est pas possible
            if (pion.ilig+j >= par.nlig)
                mouvcol=0;

            if (pion.icol+j >= par.ncol)
                mouvlig=0;

            i+=1;
        }

        if (mouvcol==1)
        {
            //et on rajoute cette case dans le tableau des coups possibles
            tab[i_coup_pos].ilig=pion.ilig+j;
            tab[i_coup_pos].icol=pion.icol;
            i_coup_pos+=1;
        }
        if (mouvlig==1)
        {
            tab[i_coup_pos].ilig=pion.ilig;
            tab[i_coup_pos].icol=pion.icol+j;
            i_coup_pos+=1;
        }
    }

    return i_coup_pos;
}

T_case coup_joueur(params par, T_case pion)
{
    int i, choix=0, nb_coup_pos;
    T_case tab_coup_pos[4], npion;

    nb_coup_pos=coup_pos(par, pion, tab_coup_pos);

    printf("C'est a vous de jouer !\n");
    printf("Vous pouvez vous deplacer aux cases :\n");
    for (i=0; i<nb_coup_pos; i++)
    {
        printf("%d.(%d:%d)  ",i+1 , tab_coup_pos[i].ilig+1, tab_coup_pos[i].icol+1);
    }

    //le joueur choisit ensuite sur quelle case se déplacer
    while (choix < 1 || choix > nb_coup_pos)
    {
        printf("Selectionner votre choix : ");
        scanf("%d", &choix);
    }
    npion.ilig=tab_coup_pos[choix-1].ilig;
    npion.icol=tab_coup_pos[choix-1].icol;

    //on effectue un nouvel affichage du tableau
    printf("Vous avez deplace le pion a la case (%d:%d)\n", npion.ilig+1, npion.icol+1);
    affichet(par, npion);
    printf("\n");

    //on retourne ses coordonnnées
    return (npion);
}

T_case coup_ordi_hasard(params par, T_case pion)
{
    int i, nb_coup_pos;
    T_case tab_coup_pos[4], npion;

    nb_coup_pos=coup_pos(par, pion, tab_coup_pos);
    i=rand()%nb_coup_pos;

    npion.ilig=tab_coup_pos[i].ilig;
    npion.icol=tab_coup_pos[i].icol;

    printf("C'est a l'ordinateur de jouer !\n");
    printf("L'ordinateur a deplace le pion a la case (%d:%d)\n\n", npion.ilig+1, npion.icol+1);
    affichet(par, npion);
    printf("\n");

    return (npion);
}

T_case coup_ordi_gagnant(params par, T_case pion, T_case *tab_gagnant)
{
    int i, j, k, nb_coup_pos, pos=0;
    T_case tab_coup_pos[4];
    T_case npion;

    nb_coup_pos=coup_pos(par, pion, tab_coup_pos);

    for (i=0; i<nb_coup_pos; i++)
    {
        for (j=0; j<par.nlig; j++)
        {
            for (k=0; k<par.ncol; k++)
            {
                if (tab_coup_pos[i].ilig==j && tab_coup_pos[i].icol==k && nim[j][k]==0)
                {
                    tab_gagnant[pos].ilig = tab_coup_pos[i].ilig;
                    tab_gagnant[pos].icol = tab_coup_pos[i].icol;
                    pos+=1;
                }
            }
        }
    }

    if (pos==0)
    {
        i=rand()%nb_coup_pos;
        npion.ilig=tab_coup_pos[i].ilig;
        npion.icol=tab_coup_pos[i].icol;
    }
    else
    {
        i=rand()%pos;
        npion.ilig = tab_gagnant[i].ilig;
        npion.icol = tab_gagnant[i].icol;
    }

    printf("L'ordinateur a deplace le pion a la case (%d:%d)\n\n", npion.ilig+1, npion.icol+1);
    affichet(par, npion);
    printf("\n");

    return (npion);
}



// programme principal
int main()
{
    // déclaration des variables
    int i, tour=0;
    params iparams;
    T_case tab_coup_pos[4], pion;
    pion.ilig=0;
    pion.icol=0;

    printf("BIENVENUE DANS LE JEU DE NIM !\n\n");

    iparams = parametres();

    remplit(iparams);
    hasard_ban(iparams);
    calcul_nimbers(iparams);

    printf("\nVoici le tableau de jeu :\n");
    affichet(iparams, pion);
    printf("\n");

    switch (iparams.next)
    {
        case 1:
                switch(iparams.niveau)
                {
                    case 1:
                        while (pion.ilig+1 != iparams.nlig || pion.icol+1 != iparams.ncol)
                        {
                            pion = coup_ordi_hasard(iparams, pion);
                            tour+=1;

                            if (pion.ilig+1 != iparams.nlig || pion.icol+1 != iparams.ncol)
                            {
                                pion = coup_joueur(iparams, pion);
                                tour+=1;
                            }
                        }

                        break;


                    case 2:
                        while (pion.ilig+1 != iparams.nlig || pion.icol+1 != iparams.ncol)
                        {
                            i=rand()%3;
                            switch(i)
                            {
                                case 0: pion = coup_ordi_hasard(iparams, pion);
                                        break;

                                case 1: pion = coup_ordi_hasard(iparams, pion);
                                        break;

                                case 2: pion = coup_ordi_gagnant(iparams, pion, tab_coup_pos);
                                        break;
                            }
                            tour+=1;

                            if (pion.ilig+1 != iparams.nlig || pion.icol+1 != iparams.ncol)
                            {
                                pion = coup_joueur(iparams, pion);
                                tour+=1;
                            }
                        }
                        break;


                    case 3:
                        while (pion.ilig+1 != iparams.nlig || pion.icol+1 != iparams.ncol)
                        {
                            i=rand()%3;
                            switch(i)
                            {
                                case 0: pion = coup_ordi_hasard(iparams, pion);
                                        break;

                                case 1: pion = coup_ordi_gagnant(iparams, pion, tab_coup_pos);
                                        break;

                                case 2: pion = coup_ordi_gagnant(iparams, pion, tab_coup_pos);
                                        break;
                            }
                            tour+=1;

                            if (pion.ilig+1 != iparams.nlig || pion.icol+1 != iparams.ncol)
                            {
                                pion = coup_joueur(iparams, pion);
                                tour+=1;
                            }
                        }
                        break;

                    case 4:
                        while (pion.ilig+1 != iparams.nlig || pion.icol+1 != iparams.ncol)
                        {
                            pion = coup_ordi_gagnant(iparams, pion, tab_coup_pos);
                            tour+=1;

                            if (pion.ilig+1 != iparams.nlig || pion.icol+1 != iparams.ncol)
                            {
                                pion = coup_joueur(iparams, pion);
                                tour+=1;
                            }
                        }
                        break;
                }

            if (tour%2!=0)
                printf("Dommage, l'ordinateur a gagne .. vous pouvez reesayer ;)");
            else
                printf("Bravo ! Vous avez gagne !");

            break;



        case 2: switch(iparams.niveau)
                {

                    case 1: while (pion.ilig+1 != iparams.nlig || pion.icol+1 != iparams.ncol)
                            {
                                pion = coup_joueur(iparams, pion);
                                tour+=1;

                                if (pion.ilig+1 != iparams.nlig || pion.icol+1 != iparams.ncol)
                                {
                                    pion = coup_ordi_hasard(iparams, pion);
                                    tour+=1;
                                }
                            }

                        break;


                    case 2:
                        while (pion.ilig+1 != iparams.nlig || pion.icol+1 != iparams.ncol)
                        {
                            pion = coup_joueur(iparams, pion);
                            tour+=1;

                            if (pion.ilig+1 != iparams.nlig || pion.icol+1 != iparams.ncol)
                            {
                                i=rand()%3;
                                switch(i)
                                {
                                    case 0: pion = coup_ordi_hasard(iparams, pion);
                                            break;

                                    case 1: pion = coup_ordi_hasard(iparams, pion);
                                            break;

                                    case 2: pion = coup_ordi_gagnant(iparams, pion, tab_coup_pos);
                                            break;
                                }
                                tour+=1;
                            }
                        }
                        break;


                    case 3:
                        while (pion.ilig+1 != iparams.nlig || pion.icol+1 != iparams.ncol)
                        {
                            pion = coup_joueur(iparams, pion);
                            tour+=1;

                            if (pion.ilig+1 != iparams.nlig || pion.icol+1 != iparams.ncol)
                            {
                                i=rand()%3;
                                switch(i)
                                {
                                    case 0: pion = coup_ordi_hasard(iparams, pion);
                                            break;

                                    case 1: pion = coup_ordi_gagnant(iparams, pion, tab_coup_pos);
                                            break;

                                    case 2: pion = coup_ordi_gagnant(iparams, pion, tab_coup_pos);
                                            break;
                                }
                                tour+=1;
                            }
                        }
                        break;

                    case 4:
                        while (pion.ilig+1 != iparams.nlig || pion.icol+1 != iparams.ncol)
                        {
                            pion = coup_joueur(iparams, pion);
                            tour+=1;

                            if (pion.ilig+1 != iparams.nlig || pion.icol+1 != iparams.ncol)
                            {
                                pion = coup_ordi_gagnant(iparams, pion, tab_coup_pos);
                                tour+=1;
                            }
                        }
                        break;
                }

        if (tour%2==0)
        {
            printf("Dommage, l'ordinateur a gagne .. vous pouvez reesayer ;)");
        }
        else
        {
            printf("Bravo ! Vous avez gagne !");
        }
        break;
    }
}
