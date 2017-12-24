#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include <math.h>
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_image.h>
#include "jeu.h"
#include "manipData.h"
#define LARGEUR_ECRAN 1300
#define HAUTEUR_ECRAN 600
#define LARGEUR_BARRE 5
#define HAUTEUR_BARRE 100
#define HAUTEUR_MUR (HAUTEUR_ECRAN - 10)
#define LARGEUR_MUR 5
#define COTE_BALLE 10
#define HAUTEUR_COTE 5
#define ELOIGNE 1
#define REVIENT 2

static int modeSauvegarde;

int jeu(SDL_Surface* ecran)
{
    SDL_Surface *cote = SDL_CreateRGBSurface(SDL_HWSURFACE, LARGEUR_ECRAN, HAUTEUR_COTE, 32, 0, 0, 0, 0);
    SDL_Surface *barre = SDL_CreateRGBSurface(SDL_HWSURFACE, LARGEUR_BARRE, HAUTEUR_BARRE, 32, 0, 0, 0, 0);
    SDL_Surface *balle = SDL_CreateRGBSurface(SDL_HWSURFACE, COTE_BALLE, COTE_BALLE, 32, 0, 0, 0, 0);
    SDL_Surface *mur = SDL_CreateRGBSurface(SDL_HWSURFACE, LARGEUR_MUR, HAUTEUR_MUR, 32, 0, 0, 0, 0);
    SDL_FillRect(cote, NULL, SDL_MapRGB(ecran->format, 255, 255, 255));
    SDL_FillRect(mur, NULL, SDL_MapRGB(ecran->format, 255, 255, 255));
    SDL_FillRect(balle, NULL, SDL_MapRGB(ecran->format, 0, 120, 255));
    SDL_FillRect(barre, NULL, SDL_MapRGB(ecran->format, 255, 255, 255));
    SDL_Rect positionBarre, positionBalle, positionMur, positionCoteHaut, positionCoteBas;
    positionMur.x = LARGEUR_ECRAN - mur->w;
    positionMur.y = HAUTEUR_ECRAN/2 - HAUTEUR_MUR/2;
    positionBarre.x = 10;
    positionBarre.y = HAUTEUR_ECRAN/2 - HAUTEUR_BARRE/2;
    positionBalle.x = LARGEUR_ECRAN/2 - LARGEUR_BARRE/2;
    positionBalle.y = HAUTEUR_ECRAN/2 - COTE_BALLE/2;
    positionCoteBas.x = 0;
    positionCoteBas.y = HAUTEUR_ECRAN - cote->h;
    positionCoteHaut.x = positionCoteBas.x;
    positionCoteHaut.y = 0;

    int retourTotal = 0;
    int continuer = 1;
    int verifDebutJeu = debutJeu(ecran);
    if (verifDebutJeu == 0)
    {
        afficherJeu(ecran, barre, balle, positionBarre, positionBalle, mur, positionMur, cote, positionCoteHaut, positionCoteBas);
    }
    else if (verifDebutJeu == -1)
    {
        retourTotal = 1;
        continuer = 0;
    }
    else if (verifDebutJeu == 1)
    {
        continuer = 0;
    }




    int sensibiliteSauvegardee = 2;
    modeSauvegarde = 2;
    int meilleurScoreSauvegarde = 0;
    FILE *data = fopen("data-dll.txt", "r");
    if (data == NULL)
    {
        data = fopen("data-dll.txt", "w+");
        reinitialiserData(data);

    }
    recupererTotaliteDonnees(data, &sensibiliteSauvegardee, &modeSauvegarde, &meilleurScoreSauvegarde);
    fclose(data);

    const int tempsAuLancementJeu = SDL_GetTicks();
    int vitesse = 3;
    double coefficient = nombreAuHasard(-1, 1);
    int x = 0;
    int y = 0;
    int bonnassiteOrdi = 3;
    int sensDeLaBalle = ELOIGNE;

    SDL_EnableKeyRepeat((4 - sensibiliteSauvegardee)*3, (4 - sensibiliteSauvegardee)*3);//à revoir

    SDL_Event event;

    int verifRetour = 0;//utile pour le cas de la pause (sdlk_escape)
    int tempsPrecedent = 0, tempsActuel = 0;
    while (continuer)
    {
    tempsActuel = SDL_GetTicks();
    if (tempsActuel - tempsPrecedent > bonnassiteOrdi)
    {
        event.key.keysym.sym = SDLK_F15;//initialiser la touche de l'event (sinon il rerentre dans le truc sdlk_escape si déjà tapé).
        SDL_PollEvent(&event);
        x = positionBalle.x;
        y = positionBalle.y;
        faireAvancerBalleModeNormal(&x, &y, &coefficient, vitesse, &sensDeLaBalle);
        positionBalle.x = x;
        positionBalle.y = y;

        switch(event.type)
        {
        case SDL_QUIT:
            continuer = 0;
            retourTotal = 1;
            break;
        case SDL_KEYUP:
            switch(event.key.keysym.sym)
            {
                case SDLK_ESCAPE:
                    verifRetour = pause(ecran);
                    afficherJeu(ecran, barre, balle, positionBarre, positionBalle, mur, positionMur, cote, positionCoteHaut, positionCoteBas);
                    if(verifRetour == 1)
                    {
                        continuer = 0;
                    }
                    else if (verifRetour == 2)
                    {
                        continuer = 0;
                        retourTotal = 1;
                    }
                    else if (verifRetour == 0)
                    {
                        continuer = 1;
                        retourTotal = 0;
                    }
                    verifRetour = 0;
                    break;
                default:
                    break;
            }
        case SDL_KEYDOWN:
            switch(event.key.keysym.sym)
            {

                case SDLK_UP:
                    positionBarre.y = positionBarre.y - 6;//y en a un trois ligne en dessous
                    if (positionBarre.y < positionCoteHaut.y + cote->h + 2)
                        {
                            positionBarre.y = positionCoteHaut.y + cote->h + 2;
                        }
                    break;
                case SDLK_DOWN:
                    positionBarre.y = positionBarre.y + 6;//y en a un trois ligne au dessus
                    if (positionBarre.y > HAUTEUR_ECRAN - cote->h - barre->h - 2)
                        {
                            positionBarre.y = HAUTEUR_ECRAN - cote->h - barre->h - 2;
                        }
                    break;
                default:
                    break;
            }
            break;
        default:
            break;
        }
        afficherJeu(ecran, barre, balle, positionBarre, positionBalle, mur, positionMur, cote, positionCoteHaut, positionCoteBas);

        if (verifDefaite(positionBalle))
        {
            continuer = 0;
            int score = ((SDL_GetTicks() - tempsAuLancementJeu)/1000)*modeSauvegarde;

            data = fopen("data-dll.txt", "r+");
            if (data == NULL)
            {
                data = fopen("data-dll.txt", "w+");
                reinitialiserData(data);
            }

            recupererTotaliteDonnees(data, &sensibiliteSauvegardee, &modeSauvegarde, &meilleurScoreSauvegarde);

            if (meilleurScoreSauvegarde < score)
            {
                meilleurScoreSauvegarde = score;
            }

            retourTotal = ecranDefaite(ecran, score, meilleurScoreSauvegarde);

            reecrireFichier(data, sensibiliteSauvegardee, modeSauvegarde, meilleurScoreSauvegarde);
            fclose(data);

        }
        if (collision(positionBalle, positionBarre))
        {
            sensDeLaBalle = ELOIGNE;
            coefficient = calculCoefApresCollision(positionBalle, positionBarre, coefficient);
        }

        tempsPrecedent = tempsActuel;
    }
    else
    {
        SDL_Delay(30 - (tempsActuel - tempsPrecedent));
    }

    }

    SDL_FreeSurface(barre);
    SDL_FreeSurface(balle);
    SDL_FreeSurface(mur);
    SDL_FreeSurface(cote);
    return retourTotal;
}

void afficherJeu(SDL_Surface *ecran, SDL_Surface *barre, SDL_Surface *balle, SDL_Rect positionBarre, SDL_Rect positionBalle, SDL_Surface *mur, SDL_Rect positionMur, SDL_Surface *cote, SDL_Rect positionCoteHaut, SDL_Rect positionCoteBas)
{
    SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 0, 0, 0));
    SDL_BlitSurface(cote, NULL, ecran, &positionCoteHaut);
    SDL_BlitSurface(cote, NULL, ecran, &positionCoteBas);
    SDL_BlitSurface(barre, NULL, ecran, &positionBarre);
    SDL_BlitSurface(balle, NULL, ecran, &positionBalle);
    SDL_BlitSurface(mur, NULL, ecran, &positionMur);
    SDL_Flip(ecran);
}

int pause(SDL_Surface *ecran)
{
    SDL_Event event;
    int reprendre = 0, retourTotal = 0;

    SDL_Color blanc = {255, 255, 255};
    SDL_Color bordeau = {190, 0, 0};
    SDL_Color noir = {0, 0, 0};
    SDL_Color bleuClair = {0, 120, 255};

    TTF_Font *policePause = TTF_OpenFont("crimesTimesSix.ttf", 150);
    TTF_Font *policeReste = TTF_OpenFont("crimesTimesSix.ttf", 40);

    SDL_Surface *textePause = TTF_RenderText_Shaded(policePause, "Pause", bleuClair, noir);
    SDL_Surface *texteReprendre = TTF_RenderText_Shaded(policeReste, "Reprendre le jeu", blanc, noir);
    SDL_Surface *retourAuMenu = TTF_RenderText_Shaded(policeReste, "Retour au Menu", blanc, noir);
    TTF_SetFontStyle(policeReste, TTF_STYLE_UNDERLINE);
    SDL_Surface *texteReprendre2 = TTF_RenderText_Shaded(policeReste, "Reprendre le jeu", bordeau, noir);
    SDL_Surface *retourAuMenu2 = TTF_RenderText_Shaded(policeReste, "Retour au Menu", bordeau, noir);

    SDL_Rect positionPause, positionReprendre, positionRetourAuMenu;
    positionPause.x = LARGEUR_ECRAN/2 - textePause->w/2;
    positionReprendre.x = LARGEUR_ECRAN/2 - texteReprendre->w/2;
    positionRetourAuMenu.x = LARGEUR_ECRAN/2 - retourAuMenu->w/2;

    positionPause.y = textePause->h/4;
    positionReprendre.y = (HAUTEUR_ECRAN + textePause->h)/2 - ((5/2)*(texteReprendre->h));
    positionRetourAuMenu.y = (HAUTEUR_ECRAN + textePause->h)/2 + ((0/2)*(retourAuMenu->h));

    afficherPause(ecran, textePause, texteReprendre, retourAuMenu, positionPause, positionReprendre, positionRetourAuMenu);
    while (!reprendre)
    {
        SDL_WaitEvent(&event);
        switch (event.type)
        {
        case SDL_QUIT:
            reprendre = 1;
            retourTotal = 2;
            break;
        case SDL_KEYUP:
            switch (event.key.keysym.sym)
            {
            case SDLK_ESCAPE:
                reprendre = 1;
                break;
            default:
                break;
            }
        case SDL_MOUSEMOTION:
            if (event.motion.x > positionReprendre.x // si le curseur est sur le reprendre
                && event.motion.x < positionReprendre.x + texteReprendre->w
                && event.motion.y > positionReprendre.y
                && event.motion.y < positionReprendre.y + texteReprendre->h)
            {
                afficherPause(ecran, textePause, texteReprendre2, retourAuMenu, positionPause, positionReprendre, positionRetourAuMenu);
            }
            else if (event.motion.x > positionRetourAuMenu.x // si le curseur est sur le retour au menu
                && event.motion.x < positionRetourAuMenu.x + retourAuMenu->w
                && event.motion.y > positionRetourAuMenu.y
                && event.motion.y < positionRetourAuMenu.y + retourAuMenu->h)
            {
                afficherPause(ecran, textePause, texteReprendre, retourAuMenu2, positionPause, positionReprendre, positionRetourAuMenu);
            }
            else
            {
                afficherPause(ecran, textePause, texteReprendre, retourAuMenu, positionPause, positionReprendre, positionRetourAuMenu);
            }
            break;
        case SDL_MOUSEBUTTONUP:
            if (event.motion.x > positionReprendre.x // si le curseur est sur le reprendre
                && event.motion.x < positionReprendre.x + texteReprendre->w
                && event.motion.y > positionReprendre.y
                && event.motion.y < positionReprendre.y + texteReprendre->h)
            {
                afficherPause(ecran, textePause, texteReprendre2, retourAuMenu, positionPause, positionReprendre, positionRetourAuMenu);
                reprendre = 1;
                retourTotal = 0;
            }
            else if (event.motion.x > positionRetourAuMenu.x // si le curseur est sur le retour au menu
                && event.motion.x < positionRetourAuMenu.x + retourAuMenu->w
                && event.motion.y > positionRetourAuMenu.y
                && event.motion.y < positionRetourAuMenu.y + retourAuMenu->h)
            {
                afficherPause(ecran, textePause, texteReprendre, retourAuMenu2, positionPause, positionReprendre, positionRetourAuMenu);
                reprendre = 1;
                retourTotal = 1;
            }
            break;
        default:
            break;
        }
    }


    SDL_FreeSurface(textePause);
    SDL_FreeSurface(texteReprendre);
    SDL_FreeSurface(retourAuMenu);
    SDL_FreeSurface(texteReprendre2);
    SDL_FreeSurface(retourAuMenu2);
    return retourTotal;
}

void afficherPause(SDL_Surface *ecran, SDL_Surface *textePause, SDL_Surface *texteReprendre, SDL_Surface *retourAuMenu, SDL_Rect positionPause, SDL_Rect positionReprendre, SDL_Rect positionRetourAuMenu)
{
    SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 0, 0, 0));
    SDL_BlitSurface(textePause, NULL, ecran, &positionPause);
    SDL_BlitSurface(texteReprendre, NULL, ecran, &positionReprendre);
    SDL_BlitSurface(retourAuMenu, NULL, ecran, &positionRetourAuMenu);
    SDL_Flip(ecran);
}

int verifDefaite(SDL_Rect positionBalle)
{
    int retour = 0;
    if (positionBalle.x < 0)
    {
        retour = 1;
    }
    return retour;
}

void faireAvancerBalleModeNormal(int *x, int *y, double *coefficient, int vitesse, int *sensDeLaBalle)
{
    if (*coefficient >= -0.4 && *coefficient <= 0)// éviter un coef trop petit
    {
        *coefficient = -0.4;
    }
    else if (*coefficient <= 0.4 && *coefficient >= 0)//eviter un coef trop petit
    {
        *coefficient = 0.4;
    }

    if (*y < HAUTEUR_COTE)//rebond sur le coté
    {
        if (facetieux(10))
        {
            *y = HAUTEUR_ECRAN - HAUTEUR_COTE - COTE_BALLE;
        }
        else
        {
            *coefficient = - *coefficient;
            *y = HAUTEUR_COTE;
        }
    }
    else if (*y > HAUTEUR_ECRAN - HAUTEUR_COTE - COTE_BALLE)
    {
        if (facetieux(10))
        {
            *y = HAUTEUR_COTE;
        }
        else
        {
            *coefficient = - *coefficient;
            *y = HAUTEUR_ECRAN - HAUTEUR_COTE - COTE_BALLE;
        }
    }

    if (*coefficient == 0)
    {
        *coefficient = - 0.5;
    }

    if (*x > LARGEUR_ECRAN - LARGEUR_MUR - COTE_BALLE)//rebond sur le mur
    {
        *sensDeLaBalle = REVIENT;
    }
    int xRechange = LARGEUR_ECRAN - *x;// pour le changement de sens
    double coefficientRechange = *coefficient*10;

    if(*coefficient < 0.5 && *coefficient > -0.5 && *coefficient != 0)//si coef trop petit
    {
        if (*sensDeLaBalle == ELOIGNE)//déplacement vers le mur
        {
            *y = ((*x + vitesse)*(coefficientRechange))/10 + *y - ((*x)*(coefficientRechange))/10;
            *x = *x + vitesse;
        }
        else if (*sensDeLaBalle == REVIENT)//deplacement vers le joueur
        {
            *y = (xRechange + vitesse)*(coefficientRechange)/10 + *y - (xRechange)*(coefficientRechange)/10;
            *x = *x - vitesse;
        }
    }
    else//si coefficient normal
    {
        if (*sensDeLaBalle == ELOIGNE)//déplacement vers le mur
        {
            *y = (*x + vitesse)*(*coefficient) + *y - (*x)*(*coefficient);
            *x = *x + vitesse;
        }
        else if (*sensDeLaBalle == REVIENT)//deplacement vers le joueur
        {
            *y = (xRechange + vitesse)*(*coefficient) + *y - (xRechange)*(*coefficient);
            *x = *x - vitesse;
        }
    }


}

int ecranDefaite(SDL_Surface *ecran, int valeurScoreChiffre, int valeurMeilleurScoreChiffre)
{
    int retourTotal = 0;
    int espaceEntreTexteEtScore = 20;
    char valeurScore [6] = "";
    char valeurMeilleurScore [6] = "";

    sprintf(valeurScore, "%d", valeurScoreChiffre);
    sprintf(valeurMeilleurScore, "%d", valeurMeilleurScoreChiffre);

    TTF_Font *policeTitrePerdu = TTF_OpenFont("crimesTimesSix.ttf", 150);
    TTF_Font *policeReste = TTF_OpenFont("crimesTimesSix.ttf", 40);

    SDL_Color blanc = {255, 255, 255};
    SDL_Color bordeau = {190, 0, 0};
    SDL_Color noir = {0, 0, 0};
    SDL_Color bleuClair = {0, 120, 255};
    /*SDL_Color bleu = {0, 0, 150};*/

    SDL_Surface *titrePerdu = TTF_RenderText_Shaded(policeTitrePerdu, "Perdu", bleuClair, noir);
    SDL_Surface *votreScore = TTF_RenderText_Shaded(policeReste, "Votre score : ", blanc, noir);
    SDL_Surface *texteMeilleurScore = TTF_RenderText_Shaded(policeReste, "Meilleur score : ", blanc, noir);
    SDL_Surface *retourAuMenu = TTF_RenderText_Shaded(policeReste, "Retour au menu", blanc, noir);
    SDL_Surface *score = TTF_RenderText_Shaded(policeReste, valeurScore, blanc, noir);
    SDL_Surface *meilleurScore = TTF_RenderText_Shaded(policeReste, valeurMeilleurScore, blanc, noir);
    TTF_SetFontStyle(policeReste, TTF_STYLE_UNDERLINE);
    SDL_Surface *retourAuMenu2 = TTF_RenderText_Shaded(policeReste, "Retour au menu", bordeau, noir);

    TTF_CloseFont(policeReste);
    TTF_CloseFont(policeTitrePerdu);

    SDL_Rect positionTitrePerdu, positionVotreScore, positionRetourAuMenu, positionScore, positionMeilleurScore, positionTexteMeilleurScore;
    positionTitrePerdu.x = LARGEUR_ECRAN/2 - titrePerdu->w/2;
    positionTitrePerdu.y = titrePerdu->h/4;
    positionRetourAuMenu.x = LARGEUR_ECRAN/2 - retourAuMenu->w/2;
    positionRetourAuMenu.y = HAUTEUR_ECRAN - 2*(retourAuMenu->h);
    positionVotreScore.x = LARGEUR_ECRAN/2 - (votreScore->w + espaceEntreTexteEtScore + score->w)/2;
    positionVotreScore.y = (HAUTEUR_ECRAN - ( (positionTitrePerdu.y + titrePerdu->h) + (HAUTEUR_ECRAN - positionRetourAuMenu.y) ) )/*epace laissé*/ /2 - votreScore->h/2 + positionTitrePerdu.y + titrePerdu->h/2; //espace au milieu de titre et retour, centré
    positionTexteMeilleurScore.x = LARGEUR_ECRAN/2 - (texteMeilleurScore->w + espaceEntreTexteEtScore + meilleurScore->w)/2;
    positionTexteMeilleurScore.y = positionVotreScore.y + votreScore->h + 45;
    positionMeilleurScore.x = positionTexteMeilleurScore.x + texteMeilleurScore->w + espaceEntreTexteEtScore;
    positionMeilleurScore.y = positionTexteMeilleurScore.y;
    positionScore.x = positionVotreScore.x + votreScore->w + espaceEntreTexteEtScore;
    positionScore.y = positionVotreScore.y;

    SDL_Event event;
    int continuer = 1;

    SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 0, 0, 0));
    SDL_BlitSurface(titrePerdu, NULL, ecran, &positionTitrePerdu);
    SDL_BlitSurface(votreScore, NULL, ecran, &positionVotreScore);
    SDL_BlitSurface(score, NULL, ecran, &positionScore);
    SDL_BlitSurface(texteMeilleurScore, NULL, ecran, &positionTexteMeilleurScore);
    SDL_BlitSurface(meilleurScore, NULL, ecran, &positionMeilleurScore);
    SDL_BlitSurface(retourAuMenu, NULL, ecran, &positionRetourAuMenu);
    SDL_Flip(ecran);
    while (continuer)
    {
        SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 0, 0, 0));
        SDL_BlitSurface(titrePerdu, NULL, ecran, &positionTitrePerdu);
        SDL_BlitSurface(votreScore, NULL, ecran, &positionVotreScore);
        SDL_BlitSurface(score, NULL, ecran, &positionScore);
        SDL_BlitSurface(texteMeilleurScore, NULL, ecran, &positionTexteMeilleurScore);
        SDL_BlitSurface(meilleurScore, NULL, ecran, &positionMeilleurScore);

        SDL_WaitEvent(&event);
        switch(event.type)
        {
        case SDL_QUIT:
            continuer = 0;
            retourTotal = 1;
            break;
        case SDL_MOUSEBUTTONUP:
            if (event.motion.x > positionRetourAuMenu.x && event.motion.x < positionRetourAuMenu.x + retourAuMenu->w
                && event.motion.y > positionRetourAuMenu.y && event.motion.y < positionRetourAuMenu.y + retourAuMenu->h)
            {
                continuer = 0;
                retourTotal = 0;
            }
            break;
        case SDL_KEYUP:
            if ((event.key.keysym.sym & SDLK_ESCAPE) == SDLK_ESCAPE)
            {
                continuer = 0;
            }
        default:
            break;
        }

        if (event.motion.x > positionRetourAuMenu.x && event.motion.x < positionRetourAuMenu.x + retourAuMenu->w
            && event.motion.y > positionRetourAuMenu.y && event.motion.y < positionRetourAuMenu.y + retourAuMenu->h)
        {
            SDL_BlitSurface(retourAuMenu2, NULL, ecran, &positionRetourAuMenu);
        }
        else
        {
            SDL_BlitSurface(retourAuMenu, NULL, ecran, &positionRetourAuMenu);
        }
        SDL_Flip(ecran);
    }
    SDL_FreeSurface(titrePerdu);
    SDL_FreeSurface(votreScore);
    SDL_FreeSurface(score);
    SDL_FreeSurface(texteMeilleurScore);
    SDL_FreeSurface(meilleurScore);
    SDL_FreeSurface(retourAuMenu);
    SDL_FreeSurface(retourAuMenu2);
    return retourTotal;
}

double calculCoefApresCollision(SDL_Rect positionBalle, SDL_Rect positionBarre, int ancienCoef)
{
    double coefImpact = calculCoefImpact(positionBalle, positionBarre);
    double newCoef = (ancienCoef + coefImpact)/2;
    if (newCoef >= 5)
    {
        newCoef = 4.5;
    }
    else if (newCoef <= -5)
    {
        newCoef = -4.5;
    }

    return newCoef;
}

int collision(SDL_Rect positionBalle, SDL_Rect positionBarre)
{
    int valeurCollision = 0;

    if (positionBalle.x >= positionBarre.x && positionBalle.x <= (positionBarre.x + LARGEUR_BARRE)
        && (positionBalle.y + COTE_BALLE) >= positionBarre.y && positionBalle.y <= positionBarre.y + HAUTEUR_BARRE)//si la balle tape sur la barre
    {
        valeurCollision = 1;
    }

    return valeurCollision;

}

double calculCoefImpact(SDL_Rect positionBalle, SDL_Rect positionBarre)
{
    int yImpact = positionBalle.y + COTE_BALLE/2 - positionBarre.y;
    double coefImpact = 0;

    if (yImpact < 0)
    {
        yImpact = 0;
    }

    if (yImpact < HAUTEUR_BARRE/10)
    {
        coefImpact = -4;
    }
    else if (yImpact >= HAUTEUR_BARRE/10 && yImpact < 2*HAUTEUR_BARRE/10)
    {
        coefImpact = -2.5;
    }
    else if (yImpact >= 2*HAUTEUR_BARRE/10 && yImpact < 3*HAUTEUR_BARRE/10)
    {
        coefImpact = -1.5;
    }
    else if (yImpact >= 3*HAUTEUR_BARRE/10 && yImpact < 4*HAUTEUR_BARRE/10)
    {
        coefImpact = -0.5;
    }

    else if (yImpact >= 4*HAUTEUR_BARRE/10 && yImpact < 5*HAUTEUR_BARRE/10)
    {
        coefImpact = -0.4;
    }

    else if (yImpact >= 5*HAUTEUR_BARRE/10 && yImpact < 6*HAUTEUR_BARRE/10)
    {
        coefImpact = 0.4;
    }

    else if (yImpact >= 6*HAUTEUR_BARRE/10 && yImpact < 7*HAUTEUR_BARRE/10)
    {
        coefImpact = 0.5;
    }

    else if (yImpact >= 7*HAUTEUR_BARRE/10 && yImpact < 8*HAUTEUR_BARRE/10)
    {
        coefImpact = 1.5;
    }

    else if (yImpact >= 8*HAUTEUR_BARRE/10 && yImpact < 9*HAUTEUR_BARRE/10)
    {
        coefImpact = 2.5;
    }
    else if (yImpact >= 9*HAUTEUR_BARRE/10 && yImpact <= 10*HAUTEUR_BARRE/10)
    {
        coefImpact = 4;
    }

    return coefImpact;
}

int nombreAuHasard(int minimum, int maximum)
{
    int nombreMystere = (rand() % (maximum - minimum + 1)) + minimum;
    return nombreMystere;
}

int debutJeu(SDL_Surface *ecran)
{
    TTF_Font *police = TTF_OpenFont("crimesTimesSix.ttf", 40);
    SDL_Color blanc = {255, 255, 255};
    SDL_Color noir = {0, 0, 0};
    if (police == NULL)
    {
        fprintf(stderr, "il manque un fichier .ttf");
        exit(EXIT_FAILURE);
    }
    SDL_Surface *texte = TTF_RenderText_Shaded(police, "Tapez sur une touche pour commencer le jeu", blanc, noir);
    TTF_CloseFont(police);
    SDL_Rect positionTexte;
    positionTexte.x = LARGEUR_ECRAN/2 - texte->w/2;
    positionTexte.y = HAUTEUR_ECRAN/2 - texte->h/2;

    int continuer = 1;
    int retourTotal = 0;
    SDL_Event event;
    while (continuer)
    {
        SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 0, 0, 0));//affichage ecran
        SDL_BlitSurface(texte, NULL, ecran, &positionTexte);
        SDL_Flip(ecran);
        event.key.keysym.sym = SDLK_F15;//'initialiser' la touche de l'event
        SDL_WaitEvent(&event);
        switch (event.type)
        {
        case SDL_QUIT:
            continuer = 0;
            retourTotal = -1;
            break;
        case SDL_KEYUP:
            if ((event.key.keysym.sym & SDLK_ESCAPE) == SDLK_ESCAPE)
            {
                continuer = 0;
                retourTotal = 1;
            }
            else
            {
                continuer = 0;
                retourTotal = 0;
            }
        }
    }
    SDL_FreeSurface(texte);
    return retourTotal;
}

void gestionTemps(int tempsEcouleDepuisLancementJeu, int *vitesse)
{
    tempsEcouleDepuisLancementJeu = tempsEcouleDepuisLancementJeu/1000;//comversion en seconde
    tempsEcouleDepuisLancementJeu = tempsEcouleDepuisLancementJeu/60;//temps en minutes
    *vitesse = tempsEcouleDepuisLancementJeu + *vitesse;
    if (*vitesse > 3)
    {
        *vitesse = 3;
    }
}

int facetieux (int probabilite)// 1 sur envoyé
{
    int reponse = 1;
    if (modeSauvegarde == 3)
    {
        if (probabilite < 6)
        {
            reponse = 0;
        }

        int nombreHasard = nombreAuHasard(1, probabilite);

        if (nombreHasard == 4 && reponse == 1)
        {
            reponse = 1;
        }
        else
        {
            reponse = 0;
        }
    }
    else
    {
        reponse = 0;
    }
    return reponse;
}


