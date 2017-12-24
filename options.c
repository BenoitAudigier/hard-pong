#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include <math.h>
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_image.h>
#include "options.h"
#include "manipData.h"
#define LARGEUR_ECRAN 1300
#define HAUTEUR_ECRAN 600
#define MODE_FACILE 1
#define MODE_NORMAL 2
#define MODE_DIFFICILE 3
#define SENSIBILITE_FAIBLE 1
#define SENSIBILITE_NORMALE 2
#define SENSIBILITE_FORTE 3

int menuOptions(SDL_Surface *ecran)
{
    FILE *data = fopen("data-dll.txt", "r+");
    if (data == NULL)
    {
        data = fopen("data-dll.txt", "w+");
        reinitialiserData(data);
    }

    int sensibiliteSauvegardee = 2;
    int modeSauvegarde = 2;
    int meilleurScoreSauvegarde = 0;
    recupererTotaliteDonnees(data, &sensibiliteSauvegardee, &modeSauvegarde, &meilleurScoreSauvegarde);


    TTF_Font *policeTexteOptions = TTF_OpenFont("crimesTimesSix.ttf", 30);
    TTF_Font *policeRetour = TTF_OpenFont("crimesTimesSix.ttf", 40);
    TTF_Font *policeTitreOptions = TTF_OpenFont("crimesTimesSix.ttf", 100);

    if (policeTitreOptions == NULL || policeTexteOptions == NULL || policeRetour == NULL)
    {
        fprintf(stderr, "Impossible d'ouvrir la police (regarder s'il manque pas un .ttf; %s", TTF_GetError());
        exit(EXIT_FAILURE);
    }


    SDL_Color bleu = {0, 0, 150};
    SDL_Color blanc = {255, 255, 255};
    SDL_Color bordeau = {190, 0, 0};
    SDL_Color noir = {0, 0, 0};
    SDL_Color bleuClair = {0, 120, 255};

    //tout ce qui est normal
    SDL_Surface *titreOptions = TTF_RenderText_Shaded(policeTitreOptions, "Options", bleuClair, noir);
    SDL_Surface *retour = TTF_RenderText_Shaded(policeRetour, "Retour", blanc, noir);
    SDL_Surface *modeFacile = TTF_RenderText_Shaded(policeTexteOptions, "Facile", blanc, noir);
    SDL_Surface *modeNormal = TTF_RenderText_Shaded(policeTexteOptions, "Normal", blanc, noir);
    SDL_Surface *modeDifficile = TTF_RenderText_Shaded(policeTexteOptions, "Difficile", blanc, noir);
    SDL_Surface *sensibliteFaible = TTF_RenderText_Shaded(policeTexteOptions, "Faible", blanc, noir);
    SDL_Surface *sensibliteNormale = TTF_RenderText_Shaded(policeTexteOptions, "Normale", blanc, noir);
    SDL_Surface *sensibliteForte = TTF_RenderText_Shaded(policeTexteOptions, "Forte", blanc, noir);
    SDL_Surface *flecheHaut = IMG_Load("flecheHaut.png");
    SDL_Surface *flecheBas = IMG_Load("flecheBas.png");
    SDL_Surface *flecheHaut2 = IMG_Load("flecheHaut2.png");
    SDL_Surface *flecheBas2 = IMG_Load("flecheBas2.png");
    TTF_SetFontStyle(policeTexteOptions, TTF_STYLE_UNDERLINE);//soulignes
    TTF_SetFontStyle(policeRetour, TTF_STYLE_UNDERLINE);
    SDL_Surface *retour2 = TTF_RenderText_Shaded(policeRetour, "Retour", bordeau, noir);
    SDL_Surface *mode = TTF_RenderText_Shaded(policeTexteOptions, "Mode : ", bleu, noir);
    SDL_Surface *sensibilite = TTF_RenderText_Shaded(policeTexteOptions, "Sensiblite", bleu, noir);

    TTF_CloseFont(policeTexteOptions);
    TTF_CloseFont(policeTitreOptions);
    TTF_CloseFont(policeRetour);

    int espaceEntreParagraphes = 50;

    //coordonnees titre
    SDL_Rect positionTitreOptions;
    positionTitreOptions.x = LARGEUR_ECRAN/2 - titreOptions->w/2;
    positionTitreOptions.y = titreOptions->h/5;
    SDL_Rect positionRetour;
    positionRetour.x = LARGEUR_ECRAN/2 - retour->w/2;
    positionRetour.y = HAUTEUR_ECRAN - 2*(retour->h);


    //coordonnees mode et tout
    SDL_Rect positionMode;
    positionMode.x = (LARGEUR_ECRAN/5)*2;
    positionMode.y = (HAUTEUR_ECRAN/3);
    SDL_Rect positionFlecheHautMode;
    positionFlecheHautMode.x = positionMode.x + sensibilite->w + modeDifficile->w/2 + 10;
    positionFlecheHautMode.y = positionMode.y - flecheHaut->h - 2;
    SDL_Rect positionFlecheBasMode;
    positionFlecheBasMode.x = positionFlecheHautMode.x;
    positionFlecheBasMode.y = positionMode.y + mode->h + 2;

    SDL_Rect positionModeFacile;
    positionModeFacile.x = (positionFlecheHautMode.x + flecheHaut->w/2) - modeFacile->w/2;
    positionModeFacile.y = positionMode.y;
    SDL_Rect positionModeNormal;
    positionModeNormal.x = (positionFlecheHautMode.x + flecheHaut->w/2) - modeNormal->w/2;
    positionModeNormal.y = positionMode.y;
    SDL_Rect positionModeDifficile;
    positionModeDifficile.x = (positionFlecheHautMode.x + flecheHaut->w/2) - modeDifficile->w/2;
    positionModeDifficile.y = positionMode.y;


    //coordonnees sensibilite et tout
    SDL_Rect positionFlecheHautSensibilite;
    positionFlecheHautSensibilite.x = positionFlecheBasMode.x;
    positionFlecheHautSensibilite.y = positionFlecheBasMode.y + flecheBas->h + espaceEntreParagraphes;
    SDL_Rect positionSensibilite;
    positionSensibilite.x = positionMode.x;
    positionSensibilite.y = positionFlecheHautSensibilite.y + flecheBas->h + 2;
    SDL_Rect positionFlecheBasSensibilite;
    positionFlecheBasSensibilite.x = positionFlecheBasMode.x;
    positionFlecheBasSensibilite.y = positionSensibilite.y + sensibilite->h + 2;

    SDL_Rect positionSensibiliteFaible;
    positionSensibiliteFaible.x = positionFlecheHautSensibilite.x + flecheHaut->w/2 - sensibliteFaible->w/2;
    positionSensibiliteFaible.y = positionSensibilite.y;
    SDL_Rect positionSensibiliteNormale;
    positionSensibiliteNormale.x = positionFlecheHautSensibilite.x + flecheHaut->w/2 - sensibliteNormale->w/2;
    positionSensibiliteNormale.y = positionSensibilite.y;
    SDL_Rect positionSensibiliteForte;
    positionSensibiliteForte.x = positionFlecheHautSensibilite.x + flecheHaut->w/2 - sensibliteForte->w/2;
    positionSensibiliteForte.y = positionSensibilite.y;


    //affichage premier ecran
    SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 0, 0, 0));
    SDL_BlitSurface(titreOptions, NULL, ecran, &positionTitreOptions);
    SDL_BlitSurface(mode, NULL, ecran, &positionMode);
    SDL_BlitSurface(sensibilite, NULL, ecran, &positionSensibilite);
    SDL_BlitSurface(retour, NULL, ecran, &positionRetour);
    SDL_BlitSurface(flecheHaut, NULL, ecran, &positionFlecheHautMode);
    SDL_BlitSurface(flecheHaut, NULL, ecran, &positionFlecheHautSensibilite);
    SDL_BlitSurface(flecheBas, NULL, ecran, &positionFlecheBasMode);
    SDL_BlitSurface(flecheBas, NULL, ecran, &positionFlecheBasSensibilite);

    if (modeSauvegarde == 1)
    {
        SDL_BlitSurface(modeFacile, NULL, ecran, &positionModeFacile);
    }
    else if (modeSauvegarde == 2)
    {
        SDL_BlitSurface(modeNormal, NULL, ecran, &positionModeNormal);
    }
    else if (modeSauvegarde == 3)
    {
        SDL_BlitSurface(modeDifficile, NULL, ecran, &positionModeDifficile);
    }

    if (sensibiliteSauvegardee == 1)
    {
        SDL_BlitSurface(sensibliteFaible, NULL, ecran, &positionSensibiliteFaible);
    }
    else if (sensibiliteSauvegardee == 2)
    {
        SDL_BlitSurface(sensibliteNormale, NULL, ecran, &positionSensibiliteNormale);
    }
    else if (sensibiliteSauvegardee == 3)
    {
        SDL_BlitSurface(sensibliteForte, NULL, ecran, &positionSensibiliteForte);
    }

    SDL_Flip(ecran);

    int retourTotal = 0;
    int continuerMenuOptions = 1;
    SDL_Event event;
    while (continuerMenuOptions)
    {
        SDL_WaitEvent(&event);
        switch(event.type)
        {
        case SDL_QUIT:
            continuerMenuOptions = 0;
            retourTotal = 1;
            break;
        case SDL_KEYUP:
            if ((event.key.keysym.sym & SDLK_ESCAPE) == SDLK_ESCAPE)
            {
                continuerMenuOptions = 0;
            }
            break;
        case SDL_MOUSEBUTTONUP:
            if (event.motion.x > positionFlecheHautMode.x && event.motion.x < positionFlecheHautMode.x + flecheHaut->w)
            {
                if (event.motion.y > positionFlecheHautMode.y && event.motion.y < positionFlecheHautMode.y + flecheHaut->h)
                {
                    SDL_BlitSurface(flecheHaut2, NULL, ecran, &positionFlecheHautMode);
                    modeSauvegarde++;
                    if (modeSauvegarde > 3)
                    {
                        modeSauvegarde = 3;
                    }
                }
                else if (event.motion.y > positionFlecheBasMode.y && event.motion.y < positionFlecheBasMode.y + flecheBas->h)
                {
                    SDL_BlitSurface(flecheBas2, NULL, ecran, &positionFlecheBasMode);
                    modeSauvegarde--;
                    if (modeSauvegarde < 1)
                    {
                        modeSauvegarde = 1;
                    }
                }
                else if (event.motion.y > positionFlecheHautSensibilite.y && event.motion.y < positionFlecheHautSensibilite.y + flecheHaut->h)
                {
                    SDL_BlitSurface(flecheHaut2, NULL, ecran, &positionFlecheHautSensibilite);
                    sensibiliteSauvegardee++;
                    if (sensibiliteSauvegardee > 3)
                    {
                        sensibiliteSauvegardee = 3;
                    }
                }
                else if (event.motion.y > positionFlecheBasSensibilite.y && event.motion.y < positionFlecheBasSensibilite.y + flecheBas->h)
                {
                    SDL_BlitSurface(flecheBas2, NULL, ecran, &positionFlecheBasSensibilite);
                    sensibiliteSauvegardee--;
                    if (sensibiliteSauvegardee < 1)
                    {
                        sensibiliteSauvegardee = 1;
                    }
                }
                else if (event.motion.x > positionRetour.x && event.motion.x < positionRetour.x + retour->w
                         && event.motion.y > positionRetour.y && event.motion.y < positionRetour.y + retour->h)
                {
                    continuerMenuOptions = 0;
                }
            }
            else if (event.motion.x > positionRetour.x && event.motion.x < positionRetour.x + retour->w
            && event.motion.y > positionRetour.y && event.motion.y < positionRetour.y + retour->h)
            {
                continuerMenuOptions = 0;
            }
            break;
        default:
            break;
        }


         //affichage ecran
        SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 0, 0, 0));
        SDL_BlitSurface(titreOptions, NULL, ecran, &positionTitreOptions);
        SDL_BlitSurface(mode, NULL, ecran, &positionMode);
        SDL_BlitSurface(sensibilite, NULL, ecran, &positionSensibilite);
        if (modeSauvegarde == 1)
        {
            SDL_BlitSurface(modeFacile, NULL, ecran, &positionModeFacile);
        }
        else if (modeSauvegarde == 2)
        {
            SDL_BlitSurface(modeNormal, NULL, ecran, &positionModeNormal);
        }
        else if (modeSauvegarde == 3)
        {
            SDL_BlitSurface(modeDifficile, NULL, ecran, &positionModeDifficile);
        }

        if (sensibiliteSauvegardee == 1)
        {
            SDL_BlitSurface(sensibliteFaible, NULL, ecran, &positionSensibiliteFaible);
        }
        else if (sensibiliteSauvegardee == 2)
        {
            SDL_BlitSurface(sensibliteNormale, NULL, ecran, &positionSensibiliteNormale);
        }
        else if (sensibiliteSauvegardee == 3)
        {
            SDL_BlitSurface(sensibliteForte, NULL, ecran, &positionSensibiliteForte);
        }
        //collage des fleches et retour
        if (event.motion.x > positionFlecheHautMode.x && event.motion.x < positionFlecheHautMode.x + flecheHaut->w
            && event.motion.y > positionFlecheHautMode.y && event.motion.y < positionFlecheHautMode.y + flecheHaut->h)
            {
                SDL_BlitSurface(flecheHaut2, NULL, ecran, &positionFlecheHautMode);
            }
            else
            {
                SDL_BlitSurface(flecheHaut, NULL, ecran, &positionFlecheHautMode);
            }

        if (event.motion.x > positionFlecheHautMode.x && event.motion.x < positionFlecheHautMode.x + flecheHaut->w
            && event.motion.y > positionFlecheBasMode.y && event.motion.y < positionFlecheBasMode.y + flecheBas->h)
            {
                SDL_BlitSurface(flecheBas2, NULL, ecran, &positionFlecheBasMode);
            }
            else
            {
                SDL_BlitSurface(flecheBas, NULL, ecran, &positionFlecheBasMode);
            }

        if (event.motion.x > positionFlecheHautMode.x && event.motion.x < positionFlecheHautMode.x + flecheHaut->w
            && event.motion.y > positionFlecheHautSensibilite.y && event.motion.y < positionFlecheHautSensibilite.y + flecheHaut->h)
            {
                SDL_BlitSurface(flecheHaut2, NULL, ecran, &positionFlecheHautSensibilite);
            }
            else
            {
                SDL_BlitSurface(flecheHaut, NULL, ecran, &positionFlecheHautSensibilite);
            }

        if (event.motion.x > positionFlecheHautMode.x && event.motion.x < positionFlecheHautMode.x + flecheHaut->w
            && event.motion.y > positionFlecheBasSensibilite.y && event.motion.y < positionFlecheBasSensibilite.y + flecheBas->h)
            {
                SDL_BlitSurface(flecheBas2, NULL, ecran, &positionFlecheBasSensibilite);
            }
            else
            {
                SDL_BlitSurface(flecheBas, NULL, ecran, &positionFlecheBasSensibilite);
            }

         if (event.motion.x > positionRetour.x && event.motion.x < positionRetour.x + retour->w
            && event.motion.y > positionRetour.y && event.motion.y < positionRetour.y + retour->h)
            {
                SDL_BlitSurface(retour2, NULL, ecran, &positionRetour);
            }
            else
            {
                SDL_BlitSurface(retour, NULL, ecran, &positionRetour);
            }

        SDL_Flip(ecran);
    }

    reecrireFichier(data, sensibiliteSauvegardee, modeSauvegarde, meilleurScoreSauvegarde);
    fclose(data);

    SDL_FreeSurface(titreOptions);
    SDL_FreeSurface(retour);
    SDL_FreeSurface(retour2);
    SDL_FreeSurface(mode);
    SDL_FreeSurface(modeFacile);
    SDL_FreeSurface(modeNormal);
    SDL_FreeSurface(modeDifficile);
    SDL_FreeSurface(sensibilite);
    SDL_FreeSurface(sensibliteFaible);
    SDL_FreeSurface(sensibliteNormale);
    SDL_FreeSurface(sensibliteForte);
    SDL_FreeSurface(flecheBas);
    SDL_FreeSurface(flecheBas2);
    SDL_FreeSurface(flecheHaut);
    SDL_FreeSurface(flecheHaut2);

    return retourTotal;
}
