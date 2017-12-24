#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include <math.h>
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_image.h>
#include "manipData.h"

void recupererTotaliteDonnees(FILE *data, int *sensibilite, int *mode, int *meilleurScore);
int recupererSensibilite(FILE *data);
int recupererSensibilite1(FILE *data);
int recupererSensibilite2(FILE *data);
int recupererMode(FILE *data);
int recupererMode1(FILE *data);
int recupererMode2(FILE *data);
int recupererMeilleurScore(FILE *data);
int recupererMeilleurScore1(FILE *data);
int recupererMeilleurScore2(FILE *data);
void encoderCode1(int *nombreAEncoder);
void decoderCode1(int *nombreADecoder);
void encoderCode2(int *nombreAEncoder);
void decoderCode2(int *nombreADecoder);
void deplacerCurseur(FILE *data, int numeroLigne);
void reecrireFichier(FILE *data,int sensibilite,int mode,int meilleurScore);
void reinitialiserData(FILE *data);

/*
CODE :

1 avertissement
2 mode1
3 senibilite2
4 meilleurScore2
5 sensibilite1
6 dla merde
7 mode2
8 meilleurScore 1
9 dla merde

*/

//Récupération données
void recupererTotaliteDonnees(FILE *data, int *sensibilite, int *mode, int *meilleurScore)
{
    *sensibilite = recupererSensibilite(data);
    *mode = recupererMode(data);
    *meilleurScore = recupererMeilleurScore(data);

    if (*sensibilite < 0 || *mode < 0 || *meilleurScore < 0 || *sensibilite > 3 || *mode > 3)
    {
        reinitialiserData(data);
        *sensibilite = recupererSensibilite(data);
        *mode = recupererMode(data);
        *meilleurScore = recupererMeilleurScore(data);
    }
}

int recupererSensibilite(FILE *data)
{
    int sensibiliteSauvegardee;
    int sensibilite1 = recupererSensibilite1(data);
    int sensibilite2 = recupererSensibilite2(data);
    decoderCode1(&sensibilite1);
    decoderCode2(&sensibilite2);

    if (sensibilite1 == sensibilite2)
    {
        sensibiliteSauvegardee = sensibilite1;
    }
    else
    {
        sensibiliteSauvegardee = -10;

    }
    return sensibiliteSauvegardee;
}

int recupererMeilleurScore(FILE *data)
{
    int meilleurScore;
    int meilleurScore1 = recupererMeilleurScore1(data);
    int meilleurScore2 = recupererMeilleurScore2(data);

    decoderCode1(&meilleurScore1);
    decoderCode2(&meilleurScore2);

    if (meilleurScore1 == meilleurScore2)
    {
        meilleurScore = meilleurScore1;
    }
    else
    {
        meilleurScore = -10;
    }
    return meilleurScore;
}

int recupererMode(FILE *data)
{
    int mode;
    int mode1 = recupererMode1(data);
    int mode2 = recupererMode2(data);

    decoderCode1(&mode1);
    decoderCode2(&mode2);

    if (mode1 == mode2)
    {
        mode = mode1;
    }
    else
    {
        mode = -10;
    }
    return mode;
}

int recupererSensibilite1(FILE *data)
{
    int sensibilite1 = 0;
    deplacerCurseur(data, 5);
    fscanf(data, "%d", &sensibilite1);
    return sensibilite1;
}

int recupererSensibilite2(FILE *data)
{
    int sensibilite2 = 0;
    deplacerCurseur(data, 3);
    fscanf(data, "%d", &sensibilite2);
    return sensibilite2;
}

int recupererMode1(FILE *data)
{
    int mode1 = 0;
    deplacerCurseur(data, 2);
    fscanf(data, "%d", &mode1);
    return mode1;
}

int recupererMode2(FILE *data)
{
    int mode2 = 0;
    deplacerCurseur(data, 7);
    fscanf(data, "%d", &mode2);
    return mode2;
}

int recupererMeilleurScore1(FILE *data)
{
    int meilleurScore1 = 0;
    deplacerCurseur(data, 8);
    fscanf(data, "%d", &meilleurScore1);
    return meilleurScore1;
}

int recupererMeilleurScore2(FILE *data)
{
    int meilleurScore2 = 0;
    deplacerCurseur(data, 4);
    fscanf(data, "%d", &meilleurScore2);
    return meilleurScore2;
}


//Deplacement curseur
void deplacerCurseur(FILE *data, int numeroLigne)
{
    rewind(data);
    int i = 1;
    int caractere = 0;
    for (i = 1; i < numeroLigne; i++)
    {
        fgetc(data);
        while (caractere != '\n')
        {
            caractere = fgetc(data);
            if (caractere == EOF)
            {
                fprintf(stderr, "Le fichier data-dll est corompu ou illisible, merci de contacter un admin");
                exit(EXIT_FAILURE);
            }
        }
        caractere = 1;
    }
}



//tout ce qui est code
void encoderCode1(int *nombreAEncoder)
{
    int clef1 = 7, clef2 = 4, clef3 = 2, clef4 = 5, clef5 = 7;

    *nombreAEncoder = *nombreAEncoder + 41625;//protection sup'

//vigénaire
    int chiffre1 = *nombreAEncoder/10000;
    *nombreAEncoder = *nombreAEncoder - (10000*chiffre1);

    int chiffre2 = *nombreAEncoder/1000;
    *nombreAEncoder = *nombreAEncoder - (1000*chiffre2);

    int chiffre3 = *nombreAEncoder/100;
    *nombreAEncoder = *nombreAEncoder - (100*chiffre3);

    int chiffre4 = *nombreAEncoder/10;
    *nombreAEncoder = *nombreAEncoder - (10*chiffre4);

    int chiffre5 = *nombreAEncoder;

    chiffre1 = (chiffre1 + clef1)%10;
    chiffre2 = (chiffre2 + clef2)%10;
    chiffre3 = (chiffre3 + clef3)%10;
    chiffre4 = (chiffre4 + clef4)%10;
    chiffre5 = (chiffre5 + clef5)%10;

    *nombreAEncoder = (chiffre1*10000) + (chiffre2*1000) + (chiffre3*100) + (chiffre4*10) + chiffre5;
}

void decoderCode1(int *nombreADecoder)
{
    int clef1 = 7, clef2 = 4, clef3 = 2, clef4 = 5, clef5 = 7;

    int chiffre1 = *nombreADecoder/10000;
    *nombreADecoder = *nombreADecoder - (10000*chiffre1);

    int chiffre2 = *nombreADecoder/1000;
    *nombreADecoder = *nombreADecoder - (1000*chiffre2);

    int chiffre3 = *nombreADecoder/100;
    *nombreADecoder = *nombreADecoder - (100*chiffre3);

    int chiffre4 = *nombreADecoder/10;
    *nombreADecoder = *nombreADecoder - (10*chiffre4);

    int chiffre5 = *nombreADecoder;

    chiffre1 = (chiffre1 - clef1 + 10)%10;
    chiffre2 = (chiffre2 - clef2 + 10)%10;
    chiffre3 = (chiffre3 - clef3 + 10)%10;
    chiffre4 = (chiffre4 - clef4 + 10)%10;
    chiffre5 = (chiffre5 - clef5 + 10)%10;

    *nombreADecoder = (chiffre1*10000) + (chiffre2*1000) + (chiffre3*100) + (chiffre4*10) + chiffre5;
    *nombreADecoder = *nombreADecoder - 41625;
}

void encoderCode2(int *nombreAEncoder)
{

    int clef1 = 6, clef2 = 2, clef3 = 2, clef4 = 1, clef5 = 3;
    *nombreAEncoder = 91423 - *nombreAEncoder;

    int chiffre1 = *nombreAEncoder/10000;
    *nombreAEncoder = *nombreAEncoder - (10000*chiffre1);

    int chiffre2 = *nombreAEncoder/1000;
    *nombreAEncoder = *nombreAEncoder - (1000*chiffre2);

    int chiffre3 = *nombreAEncoder/100;
    *nombreAEncoder = *nombreAEncoder - (100*chiffre3);

    int chiffre4 = *nombreAEncoder/10;
    *nombreAEncoder = *nombreAEncoder - (10*chiffre4);

    int chiffre5 = *nombreAEncoder;

    chiffre1 = (chiffre1 + clef1)%10;
    chiffre2 = (chiffre2 + clef2)%10;
    chiffre3 = (chiffre3 + clef3)%10;
    chiffre4 = (chiffre4 + clef4)%10;
    chiffre5 = (chiffre5 + clef5)%10;

    *nombreAEncoder = (chiffre1*10000) + (chiffre2*1000) + (chiffre3*100) + (chiffre4*10) + chiffre5;
}

void decoderCode2(int *nombreADecoder)
{

    int clef1 = 6, clef2 = 2, clef3 = 2, clef4 = 1, clef5 = 3;

    int chiffre1 = *nombreADecoder/10000;
    *nombreADecoder = *nombreADecoder - (10000*chiffre1);

    int chiffre2 = *nombreADecoder/1000;
    *nombreADecoder = *nombreADecoder - (1000*chiffre2);

    int chiffre3 = *nombreADecoder/100;
    *nombreADecoder = *nombreADecoder - (100*chiffre3);

    int chiffre4 = *nombreADecoder/10;
    *nombreADecoder = *nombreADecoder - (10*chiffre4);

    int chiffre5 = *nombreADecoder;

    chiffre1 = (chiffre1 - clef1 + 10)%10;
    chiffre2 = (chiffre2 - clef2 + 10)%10;
    chiffre3 = (chiffre3 - clef3 + 10)%10;
    chiffre4 = (chiffre4 - clef4 + 10)%10;
    chiffre5 = (chiffre5 - clef5 + 10)%10;

    *nombreADecoder = (chiffre1*10000) + (chiffre2*1000) + (chiffre3*100) + (chiffre4*10) + chiffre5;
    *nombreADecoder = 91423 - *nombreADecoder;
}


//tout ce qui est écriture du texte
void reecrireFichier(FILE *data,int sensibilite,int mode,int meilleurScore)
{
    rewind(data);
    int sensibilite2 = sensibilite;
    encoderCode1(&sensibilite);
    encoderCode2(&sensibilite2);

    int mode2 = mode;
    encoderCode1(&mode);
    encoderCode2(&mode2);

    int meilleurScore2 = meilleurScore;
    encoderCode1(&meilleurScore);
    encoderCode2(&meilleurScore2);

    fprintf(data, "TOUTE MODIFICATION A CES DONNEES PROVOQUERA LEUR REINITIALISATION A 0\n");
    fprintf(data, "%d\n", mode);
    fprintf(data, "%d\n", sensibilite2);
    fprintf(data, "%d\n", meilleurScore2);
    fprintf(data, "%d\n", sensibilite);
    fprintf(data, "43981\n");
    fprintf(data, "%d\n", mode2);
    fprintf(data, "%d\n", meilleurScore);
    fprintf(data, "34827\n");
}

void reinitialiserData(FILE *data)
{
    reecrireFichier(data, 2, 2, 0);
}

