#ifndef JEU

#define JEU

int jeu(SDL_Surface *ecran);
void afficherJeu(SDL_Surface *ecran, SDL_Surface *barre, SDL_Surface *balle, SDL_Rect positionBarre, SDL_Rect positionBalle, SDL_Surface *mur, SDL_Rect positionMur, SDL_Surface *cote, SDL_Rect positionCoteHaut, SDL_Rect positionCoteBas);
int pause(SDL_Surface *ecran);
void afficherPause(SDL_Surface *ecran, SDL_Surface *textePause, SDL_Surface *texteReprendre, SDL_Surface *retourAuMenu, SDL_Rect positionPause, SDL_Rect positionReprendre, SDL_Rect positionRetourAuMenu);
void faireAvancerBalleModeNormal(int *x, int *y, double *coefficient, int vitesse, int *sensDeLaBalle);
int verifDefaite(SDL_Rect positionBalle);
int ecranDefaite(SDL_Surface *ecran, int valeurScoreChiffre, int valeurMeilleurScoreChiffre);
double calculCoefApresCollision(SDL_Rect positionBalle,SDL_Rect positionBarre, int ancienCoef);
int collision(SDL_Rect positionBalle, SDL_Rect positionBarre);
double calculCoefImpact(SDL_Rect positionBalle, SDL_Rect positionBarre);
int nombreAuHasard(int minimum, int maximum);
int debutJeu(SDL_Surface *ecran);
void gestionTemps(int tempsEcouleDepuisLancementJeu, int *vitesse);
int facetieux (int probabilite);// 1 sur envoyé

#endif // JEU
