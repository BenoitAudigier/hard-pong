#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include <math.h>
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_image.h>
#include "regles.h"
#define LARGEUR_ECRAN 1300
#define HAUTEUR_ECRAN 600

int menuRegles(SDL_Surface *ecran)
{
    TTF_Font *policeTexteRegles = TTF_OpenFont("crimesTimesSix.ttf", 30);
    TTF_Font *policeRetour = TTF_OpenFont("crimesTimesSix.ttf", 40);
    TTF_Font *policeTitreRegles = TTF_OpenFont("crimesTimesSix.ttf", 100);

    SDL_Color bleu = {0, 0, 150};
    SDL_Color bleuClair = {0, 120, 255};
    SDL_Color blanc = {255, 255, 255};
    SDL_Color bordeau = {190, 0, 0};
    SDL_Color noir = {0, 0, 0};


    //tout ce qui est retour
    SDL_Surface *retour = TTF_RenderText_Shaded(policeRetour, "Retour", blanc, noir);
    TTF_SetFontStyle(policeRetour, TTF_STYLE_NORMAL | TTF_STYLE_UNDERLINE);//souligné
    SDL_Surface *retour2 = TTF_RenderText_Shaded(policeRetour, "Retour", bordeau, noir);
    SDL_Rect positionRetour;
    positionRetour.x = LARGEUR_ECRAN/2 - retour->w/2;
    positionRetour.y = HAUTEUR_ECRAN - 2*(retour->h);

    //le titre
    SDL_Surface *titreRegles = TTF_RenderText_Shaded(policeTitreRegles, "Regles", bleuClair, noir);
    SDL_Rect positionTitreRegles;
    positionTitreRegles.x = LARGEUR_ECRAN/2 - titreRegles->w/2;
    positionTitreRegles.y = titreRegles->h/5;

    //tout ce qui est normal
    SDL_Surface *nePasPerdre = TTF_RenderText_Shaded(policeTexteRegles, "Ne pas perdre", blanc, noir);
    SDL_Surface *enGagnant = TTF_RenderText_Shaded(policeTexteRegles, "En gagnant, quelle question", blanc, noir);
    SDL_Surface *fleches = TTF_RenderText_Shaded(policeTexteRegles, "Fleches directionnelles", blanc, noir);

    TTF_SetFontStyle(policeTexteRegles, TTF_STYLE_UNDERLINE);//souligné
    SDL_Surface *butDujeu = TTF_RenderText_Shaded(policeTexteRegles, "But du jeu :", bleu, noir);
    SDL_Surface *comment = TTF_RenderText_Shaded(policeTexteRegles, "Comment :", bleu, noir);
    SDL_Surface *seDeplacer = TTF_RenderText_Shaded(policeTexteRegles, "Se deplacer :", bleu, noir);

    SDL_Rect positionNePasPerdre, positionEnGagnant, positionFleches, positionButDuJeu, positionComment, positionSeDeplacer;
    positionNePasPerdre.x = LARGEUR_ECRAN/2 - nePasPerdre->w/2;
    positionEnGagnant.x = LARGEUR_ECRAN/2 - enGagnant->w/2;
    positionFleches.x = LARGEUR_ECRAN/2 - fleches->w/2;
    positionButDuJeu.x = LARGEUR_ECRAN/2 - butDujeu->w/2;
    positionComment.x = LARGEUR_ECRAN/2 - comment->w/2;
    positionSeDeplacer.x = LARGEUR_ECRAN/2 - seDeplacer->w/2;

    int placePourTexte = positionRetour.y - retour->h - (7/4)*(titreRegles->h);
    int espaceEntrePhrases = 4;
    int espaceEntreParagraphes = 40;

    positionComment.y = (3/2)*(titreRegles->h) + placePourTexte/2 - espaceEntrePhrases/2 - comment->h;
    positionEnGagnant.y = (3/2)*(titreRegles->h) + placePourTexte/2 + espaceEntrePhrases/2;
    positionSeDeplacer.y = positionEnGagnant.y + enGagnant->h + espaceEntreParagraphes;
    positionFleches.y = positionSeDeplacer.y + seDeplacer->h + espaceEntrePhrases;
    positionNePasPerdre.y = positionComment.y - espaceEntreParagraphes - nePasPerdre->h;
    positionButDuJeu.y = positionNePasPerdre.y - espaceEntrePhrases - butDujeu->h;


    TTF_CloseFont(policeRetour);
    TTF_CloseFont(policeTexteRegles);

    SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 0, 0, 0));//premier affichage
    SDL_BlitSurface(titreRegles, NULL, ecran, &positionTitreRegles);
    SDL_BlitSurface(butDujeu, NULL, ecran, &positionButDuJeu);
    SDL_BlitSurface(nePasPerdre, NULL, ecran, &positionNePasPerdre);
    SDL_BlitSurface(comment, NULL, ecran, &positionComment);
    SDL_BlitSurface(enGagnant, NULL, ecran, &positionEnGagnant);
    SDL_BlitSurface(seDeplacer, NULL, ecran, &positionSeDeplacer);
    SDL_BlitSurface(fleches, NULL, ecran, &positionFleches);
    SDL_BlitSurface(retour, NULL, ecran, &positionRetour);
    SDL_Flip(ecran);//fin premier affichage

    SDL_Event event;
    int curseur = 0;
    int retourTotal = 0;
    int continuer = 1;
    while(continuer)
    {
        SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 0, 0, 0)); //affichage regle
        SDL_BlitSurface(titreRegles, NULL, ecran, &positionTitreRegles);
        SDL_BlitSurface(butDujeu, NULL, ecran, &positionButDuJeu);
        SDL_BlitSurface(nePasPerdre, NULL, ecran, &positionNePasPerdre);
        SDL_BlitSurface(comment, NULL, ecran, &positionComment);
        SDL_BlitSurface(enGagnant, NULL, ecran, &positionEnGagnant);
        SDL_BlitSurface(seDeplacer, NULL, ecran, &positionSeDeplacer);
        SDL_BlitSurface(fleches, NULL, ecran, &positionFleches);
        SDL_BlitSurface(retour, NULL, ecran, &positionRetour);
        if (curseur)
        {
            SDL_BlitSurface(retour2, NULL, ecran, &positionRetour);
        }
        else
        {
            SDL_BlitSurface(retour, NULL, ecran, &positionRetour);
        }
        SDL_Flip(ecran);//fin affichage
        curseur = 0;
        SDL_WaitEvent(&event);
        switch(event.type)
        {
        case SDL_QUIT:
            continuer = 0;
            retourTotal = 1;
            break;
        case SDL_KEYUP:
            if ((event.key.keysym.sym & SDLK_ESCAPE) == SDLK_ESCAPE)
            {
                continuer = 0;
            }
            break;
        case SDL_MOUSEMOTION:
            if (event.motion.x > positionRetour.x //si le curseur est sur le retour
                && event.motion.x < (positionRetour.x + retour->w)
                && event.motion.y > positionRetour.y
                && event.motion.y < (positionRetour.y + retour->h))
            {
                curseur = 1;
            }
            break;
        case SDL_MOUSEBUTTONUP:
            if (event.motion.x > positionRetour.x //si le curseur est sur le retour
                && event.motion.x < (positionRetour.x + retour->w)
                && event.motion.y > positionRetour.y
                && event.motion.y < (positionRetour.y + retour->h))
                {
                    curseur = 1;
                    continuer = 0;
                }
            break;
        default:
            continuer = 1;
            break;
        }
    }

    SDL_FreeSurface(retour);
    SDL_FreeSurface(retour2);
    SDL_FreeSurface(titreRegles);
    SDL_FreeSurface(nePasPerdre);
    SDL_FreeSurface(enGagnant);
    SDL_FreeSurface(fleches);
    SDL_FreeSurface(butDujeu);
    SDL_FreeSurface(comment);
    SDL_FreeSurface(seDeplacer);

    return retourTotal;
}

