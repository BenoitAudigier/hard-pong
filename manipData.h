#ifndef MENUDATA

#define MENUDATA

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

#endif // MENUDATA
