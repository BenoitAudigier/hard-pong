#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include <math.h>
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_image.h>
#include "menu.h"
#include "regles.h"
#include "regles.c"
#include "jeu.h"
#include "jeu.c"
#include "options.h"
#include "options.c"
#include "manipData.h"
#include "manipData.c"

#define CURSEUR_SUR_PLAY 1
#define CURSEUR_SUR_QUIT 2
#define CURSEUR_SUR_REGLES 3
#define CURSEUR_SUR_OPTIONS 4

#define LARGEUR_ECRAN 1300
#define HAUTEUR_ECRAN 600

#define ECRAN_ROUGE 0
#define ECRAN_VERT 0
#define ECRAN_BLEU 0


static SDL_Surface *titre = NULL;
static SDL_Surface *jouer = NULL;
static SDL_Surface *quitter = NULL;
static SDL_Surface *regles = NULL;
static SDL_Surface *jouer2 = NULL;
static SDL_Surface *quitter2 = NULL;
static SDL_Surface *regles2 = NULL;
static SDL_Surface *options = NULL;
static SDL_Surface *options2 = NULL;

static SDL_Rect positionJouer, positionQuitter, positionTitre, positionRegles, positionOptions;

int main ( int argc, char** argv )
{
    FILE *data = fopen("data-dll.txt", "r");
    if (data == NULL)
    {
        data = fopen("data-dll.txt", "w+");
        reinitialiserData(data);
    }
    fclose(data);

    srand(time(NULL));
    if ( SDL_Init( SDL_INIT_VIDEO ) < 0 )
    {
        printf( "Unable to init SDL: %s\n", SDL_GetError() );
        exit(EXIT_FAILURE);
    }
    atexit(SDL_Quit);

    if (TTF_Init() < 0)
    {
        fprintf(stderr, "Erreur lors de l'ouverture de ttf.h (checker dll) : %s", TTF_GetError());
        exit(EXIT_FAILURE);
    }
    atexit(TTF_Quit);


    TTF_Font *policeTitre = TTF_OpenFont("crimesTimesSix.ttf", 190);
    TTF_Font *policeReste = TTF_OpenFont("crimesTimesSix.ttf", 40);
    TTF_Font *policeIcone = TTF_OpenFont("crimesTimesSix.ttf", 60);

    if (policeTitre == NULL || policeReste == NULL || policeIcone == NULL)
    {
        fprintf(stderr, "Erreur lors de l'ouverture de la police (manque un .ttf quoi) : %s", TTF_GetError());
        exit(EXIT_FAILURE);
    }


    SDL_Color blanc = {255, 255, 255};
    SDL_Color bordeau = {190, 0, 0};
    SDL_Color noir = {0, 0, 0};
    SDL_Color bleuClair = {0, 120, 255};

    SDL_Surface *icone = TTF_RenderText_Blended(policeIcone, " P ", bordeau);
    SDL_WM_SetIcon(icone, NULL);
    SDL_WM_SetCaption("Pong", "Icone Pong");
    SDL_FreeSurface(icone);



    SDL_Surface* ecran = SDL_SetVideoMode(LARGEUR_ECRAN, HAUTEUR_ECRAN, 32, SDL_HWSURFACE|SDL_DOUBLEBUF);
    if (ecran == NULL)
    {
        printf("Unable to open a window (resolution ?): %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }



    titre = TTF_RenderText_Shaded(policeTitre, "PONG", bleuClair, noir);
    jouer = TTF_RenderText_Shaded(policeReste, "Jouer", blanc, noir);
    quitter = TTF_RenderText_Shaded(policeReste, "Quitter", blanc, noir);
    regles = TTF_RenderText_Shaded(policeReste, "Regles", blanc, noir);
    options = TTF_RenderText_Shaded(policeReste, "Options", blanc, noir);

    TTF_SetFontStyle(policeReste, TTF_STYLE_UNDERLINE);

    jouer2 = TTF_RenderText_Shaded(policeReste, "Jouer", bordeau, noir);
    quitter2 = TTF_RenderText_Shaded(policeReste, "Quitter", bordeau, noir);
    regles2 = TTF_RenderText_Shaded(policeReste, "Regles", bordeau, noir);
    options2 = TTF_RenderText_Shaded(policeReste, "Options", bordeau, noir);

    TTF_CloseFont(policeTitre);
    TTF_CloseFont(policeReste);

    positionTitre.x = LARGEUR_ECRAN/2 - titre->w/2;
    positionTitre.y = titre->h/5;

    positionJouer.x = LARGEUR_ECRAN/2 - jouer->w/2;
    positionQuitter.x = LARGEUR_ECRAN/2 - quitter->w/2;
    positionRegles.x = LARGEUR_ECRAN/2 - regles->w/2;
    positionOptions.x = LARGEUR_ECRAN/2 - options->w/2;

    positionJouer.y = (HAUTEUR_ECRAN - (7/5)*(titre->h))/2 + 1.5*(jouer->h);
    positionRegles.y = positionJouer.y + 1.6*(jouer->h);
    positionOptions.y = positionJouer.y + 1.6*2*(jouer->h);
    positionQuitter.y = positionJouer.y + 1.6*3*(jouer->h);




    while (menu(ecran));


    SDL_FreeSurface(ecran);
    SDL_FreeSurface(titre);
    SDL_FreeSurface(jouer);
    SDL_FreeSurface(jouer2);
    SDL_FreeSurface(quitter);
    SDL_FreeSurface(quitter2);
    SDL_FreeSurface(regles);
    SDL_FreeSurface(regles2);
    SDL_FreeSurface(options);
    SDL_FreeSurface(options2);


    return EXIT_SUCCESS;
}

int menu(SDL_Surface* ecran)
{
    afficherMenu(ecran, 0);

    SDL_Event event;
    int menu = 1;
    while (menu)
    {
        SDL_WaitEvent(&event);
        switch(event.type)
        {
            case SDL_MOUSEMOTION:
                if (event.motion.x > positionJouer.x   // Si le curseur est sur l'image play
                    && event.motion.x < (positionJouer.x + jouer->w)
                    && event.motion.y > positionJouer.y
                    && event.motion.y < (positionJouer.y + jouer->h) )
                    {
                        afficherMenu(ecran, CURSEUR_SUR_PLAY);
                    }
                else if (event.motion.x > positionQuitter.x   // Si le curseur est sur l'image quit
                    && event.motion.x < (positionQuitter.x + quitter->w)
                    && event.motion.y > positionQuitter.y
                    && event.motion.y < (positionQuitter.y + quitter->h) )
                    {
                        afficherMenu(ecran, CURSEUR_SUR_QUIT);
                    }
                else if (event.motion.x > positionRegles.x   // Si le curseur est sur l'image regles
                    && event.motion.x < (positionRegles.x + regles->w)
                    && event.motion.y > positionRegles.y
                    && event.motion.y < (positionRegles.y + regles->h) )
                    {
                        afficherMenu(ecran, CURSEUR_SUR_REGLES);
                    }
                else if (event.motion.x > positionOptions.x   // Si le curseur est sur l'image options
                    && event.motion.x < (positionOptions.x + options->w)
                    && event.motion.y > positionOptions.y
                    && event.motion.y < (positionOptions.y + options->h) )
                    {
                        afficherMenu(ecran, CURSEUR_SUR_OPTIONS);
                    }
                else
                    {
                        afficherMenu(ecran, 0);
                    }
                break;
            case SDL_MOUSEBUTTONUP:
                if (event.motion.x > positionQuitter.x   // Si le curseur était sur l'image quit
                    && event.motion.x < (positionQuitter.x + quitter->w)
                    && event.motion.y > positionQuitter.y
                    && event.motion.y < (positionQuitter.y + quitter->h)
                    && event.button.button == SDL_BUTTON_LEFT)
                    {
                        menu = 0;
                    }
                else if (event.motion.x > positionJouer.x   // Si le curseur était sur l'image play
                    && event.motion.x < (positionJouer.x + jouer->w)
                    && event.motion.y > positionJouer.y
                    && event.motion.y < (positionJouer.y + jouer->h) )
                    {
                        if (jeu(ecran) == 1)
                        {
                            menu = 0;
                        }
                        afficherMenu(ecran, 0);
                    }
                else if (event.motion.x > positionRegles.x   // Si le curseur était sur l'image regles
                    && event.motion.x < (positionRegles.x + regles->w)
                    && event.motion.y > positionRegles.y
                    && event.motion.y < (positionRegles.y + regles->h) )
                {
                    int curseur = menuRegles(ecran);

                    if (curseur)
                    {
                        menu = 0;
                    }
                    afficherMenu(ecran, 0);
                }
                else if (event.motion.x > positionOptions.x   // Si le curseur est sur l'image options
                    && event.motion.x < (positionOptions.x + options->w)
                    && event.motion.y > positionOptions.y
                    && event.motion.y < (positionOptions.y + options->h) )
                    {
                        int curseur = menuOptions(ecran);
                        if (curseur)
                        {
                            menu = 0;
                        }
                        afficherMenu(ecran, 0);
                    }
                break;
            case SDL_KEYUP:
                if ((event.key.keysym.sym & SDLK_ESCAPE) == SDLK_ESCAPE)
                {
                    menu = 0;
                }
                else if ((event.key.keysym.sym & SDLK_j) == SDLK_j)
                {
                    if (jeu(ecran) == 1)
                    {
                        menu = 0;
                    }
                    afficherMenu(ecran, 0);
                }
                break;
            case SDL_QUIT:
                menu = 0;
                break;
            default:
                break;
        }
    }
    return menu;
}

void afficherMenu(SDL_Surface *ecran, int curseur)
{
    SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, ECRAN_ROUGE, ECRAN_VERT, ECRAN_BLEU));

    if (curseur == CURSEUR_SUR_PLAY)
    {
        SDL_BlitSurface(jouer2, NULL, ecran, &positionJouer);
    }
    else
    {
        SDL_BlitSurface(jouer, NULL, ecran, &positionJouer);
    }


    if (curseur == CURSEUR_SUR_QUIT)
    {
        SDL_BlitSurface(quitter2, NULL, ecran, &positionQuitter);
    }
    else
    {
        SDL_BlitSurface(quitter, NULL, ecran, &positionQuitter);
    }


    if (curseur == CURSEUR_SUR_REGLES)
    {
        SDL_BlitSurface(regles2, NULL, ecran, &positionRegles);
    }
    else
    {
        SDL_BlitSurface(regles, NULL, ecran, &positionRegles);
    }

    if (curseur == CURSEUR_SUR_OPTIONS)
    {
        SDL_BlitSurface(options2, NULL, ecran, &positionOptions);
    }
    else
    {
        SDL_BlitSurface(options, NULL, ecran, &positionOptions);
    }

    SDL_BlitSurface(titre, NULL, ecran, &positionTitre);

    SDL_Flip(ecran);
}


