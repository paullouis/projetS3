/*
jeu.h
-----

 Par Paul ROBIN et louis DUDOT

Rôle : prototypes des fonctions du jeu.
*/

#ifndef DEF_JEU
#define DEF_JEU
void jouer(SDL_Surface* ecran);
int hazard(void);
void afficher_futur_Tetramino(SDL_Rect positionFuturTetramino,SDL_Surface *Tetramino[7], SDL_Surface *ecran );
void charger_images(SDL_Surface *Tetramino[7]);
void creer_matrice_jeu(int i ,int j,int matrice[i][j]);
void verif_matrice_jeu  (int l ,int h ,int  matrice_jeu[l][h]);
void creer_tetramino(int l ,int h ,int  matrice_jeu[l][h]);
void afficher_matrice_jeu (int l ,int h ,int  matrice_jeu[l][h],SDL_Rect positionTetramino,SDL_Surface *Tetramino[7], SDL_Surface *ecran );
Uint32 chute_tetramino (Uint32 intervalle, void *param);
int chute_du_tetramino(int l ,int h ,int matrice_jeu[l][h]);
void tourner_piece (int l ,int h ,int  matrice_jeu[l][h]);
void deplacementdroite(int l ,int h ,int  matrice_jeu[l][h]);
void deplacementgauche(int l ,int h ,int  matrice_jeu[l][h]);
void chute_ligne (int l ,int h ,int  matrice_jeu[l][h]);
void nouveau_score (int etat);
void change_niveau ();
void afficher_score(SDL_Color Vert,TTF_Font *police,SDL_Rect pos_score,SDL_Surface *surface_score, SDL_Surface *ecran );
void afficher_nbr_ligne(  SDL_Color Vert,TTF_Font *police,SDL_Rect pos_nbr_ligne,SDL_Surface *surface_nbr_ligne, SDL_Surface *ecran);
void afficher_timer (SDL_Color Vert,TTF_Font *police,SDL_Rect pos_timer,SDL_Surface *surface_timer, SDL_Surface *ecran);
void afficher_niveau (SDL_Color Vert,TTF_Font *police,SDL_Rect pos_niveau,SDL_Surface *surface_niveau, SDL_Surface *ecran);
void afficher_best_score(SDL_Color Vert,TTF_Font *police,SDL_Rect pos_best_score,SDL_Surface *surface_best_score, SDL_Surface *ecran);
#endif
