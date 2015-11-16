/*
  jeu.c
  -----

 Par Paul ROBIN et Louis DUDOT

  Rôle : fonctions du jeu.
*/

#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_mixer.h>
#include "constantes.h"
#include "jeu.h" 

// les variables 
int future_Tetramino,jeu_tetramino,compteur,suite,rotation,score,niveau,best_score,nbr_ligne, rythme,time,continuer,jeu_pas_fini,explose,stop,volume_son,volume_musique;
Uint32 timer,timer2;

// structure qui stoque les touches préssées
typedef struct{
  char key[SDLK_LAST];
  char quit;
} Input;

void UpdateEvents(Input* in){
  SDL_Event event;
  while(SDL_PollEvent(&event)){
    switch (event.type){
    case SDL_KEYDOWN:
      in->key[event.key.keysym.sym]=1;
      break;
    case SDL_KEYUP:
      in->key[event.key.keysym.sym]=0;
      break;
    case SDL_QUIT:
      in->quit = 1;
      break;
    default:
      break;
    }
  }
}

void jouer(SDL_Surface* ecran){
  Input in;
  // les surfaces
  SDL_Surface *Tetramino[7] = {NULL}, *Background2 = NULL,*surface_niveau = NULL,*surface_timer = NULL,*surface_score = NULL, *surface_best_score = NULL, *surface_nbr_ligne = NULL,*surface_perdu = NULL,*surface_aide = NULL ;
  // musiqe et sons et police
  Mix_Music *musique = NULL;
  TTF_Font *police;
  Mix_Chunk *boum = NULL;
  Mix_Chunk *son_stop = NULL;
  Mix_Chunk *game_over = NULL;

  SDL_Rect positionBackground2, positionFuturTetramino,positionTetramino,pos_niveau,pos_timer,pos_score,pos_best_score,pos_nbr_ligne,pos_perdu,position_aide;
  SDL_Event event;
  SDL_Color Vert = {0,255,0};
  // lecture des sons et musique
  musique = Mix_LoadMUS("musiques/tetris.ogx");
  boum = Mix_LoadWAV("musiques/boum.wav");
  son_stop = Mix_LoadWAV("musiques/stop.wav");
  game_over = Mix_LoadWAV("musiques/game_over.wav");
if(  musique == NULL || boum == NULL||son_stop == NULL ||son_stop == NULL) {
 printf("Probleme de chargement ressourse son ou musique");
    SDL_Quit();  
}
// reglages sons et musique
  volume_musique=50;
  Mix_VolumeMusic(volume_musique);
  Mix_PlayMusic(musique, -1);
  Mix_AllocateChannels(4);
  volume_son=50;
  Mix_VolumeChunk(son_stop,volume_son);
  Mix_VolumeChunk(boum,volume_son);
  Mix_VolumeChunk(game_over,volume_son);

  if (TTF_Init()<0){
    printf("Impossible d'initialiser la librairie SDL_ttf : %s\n", TTF_GetError());
  SDL_Quit();
  }
  // charger la police
  police = TTF_OpenFont ("ttf/DIGIT-LCD.ttf", 32);
  if (police == NULL){
    printf("Impossible de charger la police : %s\n", TTF_GetError());
  SDL_Quit();
  }
  // generation d'un numero entre 0 et 6
  future_Tetramino = hazard();
  // charger les images
  Background2 = IMG_Load("images/backgroud2.jpeg");
  surface_perdu = IMG_Load("images/perdue.png");
  surface_aide = IMG_Load("images/key.png");
if( (Background2 == NULL ) || (surface_perdu == NULL ) || ( surface_aide == NULL ) ){ 
    printf("Probleme de chargement ressourse: %s\n", SDL_GetError());
    SDL_Quit(); } 
// initialisation variable
  positionBackground2.x = 0;
  positionBackground2.y = 0;
  pos_perdu.x=160;
  pos_perdu.y=20;
  jeu_pas_fini=1;
  continuer = 1;
  nbr_ligne = 0;
  time=0;
  score=0;
  niveau= 1 ;
  explose=0;
  stop=0;
  volume_son=50;
  volume_musique=50;
  // vitesse de chute en fonction du niveau
  if(niveau<=10){
    rythme=1000-(90*niveau);}
  //lecture du score dans un fichier
  FILE* fichier = NULL;
  fichier = fopen("best_score.s", "r");
  if (fichier == NULL){
    printf("Impossible de charger le fichier: %s\n", TTF_GetError());
    exit(-1);
  }
  if (fichier != NULL){ 
    fscanf(fichier, "%d", &best_score);
    fclose(fichier);
  }
  // charge les images du tetramino
  charger_images(Tetramino);
  // creer une matrice et la remplir
  int  matrice_jeu[NB_BLOCS_LARGEUR][NB_BLOCS_HAUTEUR] /* = {0} */;
  creer_matrice_jeu(NB_BLOCS_LARGEUR,NB_BLOCS_HAUTEUR,matrice_jeu);
  creer_tetramino(NB_BLOCS_LARGEUR ,NB_BLOCS_HAUTEUR,matrice_jeu);
  // init des deux timer
  timer = SDL_GetTicks();
  timer2 = SDL_GetTicks();
  
  memset(&in,0,sizeof(in));
  while (continuer){
    UpdateEvents(&in);
    // lecture des touches et actions
    if (in.key[SDLK_UP]){ 
      in.key[SDLK_UP] = 0; 
      tourner_piece (NB_BLOCS_LARGEUR ,NB_BLOCS_HAUTEUR,matrice_jeu);
    }
    if (in.key[SDLK_DOWN]){
      in.key[SDLK_c]=0;
      in.key[SDLK_DOWN] = 0; 
      suite=chute_du_tetramino(NB_BLOCS_LARGEUR ,NB_BLOCS_HAUTEUR,matrice_jeu);
    }
    if (in.key[SDLK_c]){
      in.key[SDLK_DOWN] = 0; 
      suite=chute_du_tetramino(NB_BLOCS_LARGEUR ,NB_BLOCS_HAUTEUR,matrice_jeu);
      if (suite == 1){
	in.key[SDLK_c]=0;
      }
    }
    if (in.key[SDLK_RIGHT]){
      in.key[SDLK_RIGHT]=0;
      deplacementdroite(NB_BLOCS_LARGEUR ,NB_BLOCS_HAUTEUR,matrice_jeu);
    }
    if (in.key[SDLK_LEFT]){
      in.key[SDLK_LEFT]=0;
      deplacementgauche(NB_BLOCS_LARGEUR ,NB_BLOCS_HAUTEUR,matrice_jeu);
    }
    if (in.key[SDLK_KP_PLUS] && !in.key[SDLK_s]){
      in.key[SDLK_KP_PLUS]=0;
      if(volume_musique < MIX_MAX_VOLUME-5 && volume_musique >= 0 ){
	volume_musique=volume_musique+5;
	Mix_VolumeMusic(volume_musique);
      }
     
    } 
    if (in.key[SDLK_KP_MINUS]&& !in.key[SDLK_s]){
      in.key[SDLK_KP_MINUS]=0;
      if(volume_musique >=5 && volume_musique <= MIX_MAX_VOLUME){
	volume_musique=volume_musique-5;
        Mix_VolumeMusic(volume_musique);

      }
    }
    if (in.key[SDLK_KP_PLUS]&& in.key[SDLK_s]){
      in.key[SDLK_KP_PLUS]=0;
      if(volume_son < MIX_MAX_VOLUME-5 && volume_son >= 0 ){
	volume_son = volume_son +5;
	Mix_VolumeChunk(son_stop,volume_son);
	Mix_VolumeChunk(boum,volume_son);
	Mix_VolumeChunk(game_over,volume_son);
      }
    } 
    if (in.key[SDLK_KP_MINUS] && in.key[SDLK_s]){
      in.key[SDLK_KP_MINUS]=0;
      if(volume_son >= 5 && volume_son <= MIX_MAX_VOLUME){
	volume_son = volume_son - 5;;
	Mix_VolumeChunk(son_stop,volume_son);
	Mix_VolumeChunk(boum,volume_son);
	Mix_VolumeChunk(game_over,volume_son);
      }
    }
    // timeur de chute
    if (SDL_GetTicks()-timer>rythme && jeu_pas_fini){    
      in.key[SDLK_DOWN] = 1;    
      timer = SDL_GetTicks();
    }    
    // pour le temps
    if (SDL_GetTicks()-timer2>1000) {
      time++;
      timer2 = SDL_GetTicks();
    }
    // enreistrement meilleur score
    if (score>best_score){
      fichier = fopen("best_score.s", "w");
      if (fichier == NULL){
	printf("Impossible de charger le fichier: %s\n", TTF_GetError());
	exit(-1);
      }
      if (fichier != NULL){ 
	fprintf(fichier, "%d",score);
	fclose(fichier);
      }
    }
    //son si ligne complete 
    if (explose){
      Mix_PlayChannel(-1,boum, 0); explose=0;
    }
    // son jeu fini
    if (stop){
      Mix_PlayChannel(-1,son_stop, 0);
      stop=0;
    }
    // affichages
    SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 255, 255, 255));
    SDL_BlitSurface(Background2, NULL, ecran, &positionBackground2);
    afficher_futur_Tetramino(positionFuturTetramino,Tetramino,ecran );
    afficher_score(Vert,police,pos_score,surface_score,ecran);
    afficher_nbr_ligne (Vert,police,pos_score,surface_score,ecran);
    afficher_timer(Vert,police,pos_timer,surface_timer,ecran);
    afficher_niveau(Vert,police,pos_niveau,surface_niveau,ecran);
    afficher_best_score(Vert,police,pos_best_score,surface_best_score,ecran);
    afficher_matrice_jeu (NB_BLOCS_LARGEUR ,NB_BLOCS_HAUTEUR,matrice_jeu,positionTetramino,Tetramino,ecran );
    //ecran d'aide
    if (in.key[SDLK_COMMA]||in.key[SDLK_QUESTION]){
      position_aide.x=0;
      position_aide.y=0;
      // bliter aide
      SDL_BlitSurface(surface_aide, NULL, ecran, &position_aide);
    } 
    // jeu fini 
    if(!jeu_pas_fini){
      Mix_HaltMusic();
      Mix_PlayChannel(-1,game_over, 0);
      SDL_SetColorKey(surface_perdu, SDL_SRCCOLORKEY, SDL_MapRGB(surface_perdu->format, 0,0,0));
      SDL_BlitSurface(surface_perdu, NULL, ecran, &pos_perdu);
      SDL_Flip(ecran);
      SDL_Delay (7000);
      continuer=0;}
    SDL_Flip(ecran);
  }
  // Libération des surfaces chargées des polices des son et de la musique
  TTF_Quit();
  SDL_FreeSurface(Background2);
  SDL_FreeSurface(surface_niveau);
  SDL_FreeSurface(surface_timer);
  SDL_FreeSurface(surface_score);
  SDL_FreeSurface(surface_best_score);
  SDL_FreeSurface(surface_nbr_ligne);
  SDL_FreeSurface(surface_perdu);
  SDL_FreeSurface(surface_aide);
  for (compteur = 0 ; compteur < 7 ; compteur++)
    { SDL_FreeSurface(Tetramino[compteur]);}  
  Mix_FreeChunk(boum);
  Mix_FreeChunk(son_stop);
  Mix_FreeChunk(game_over);
  Mix_FreeMusic(musique);
}

int hazard(void){
  return SDL_GetTicks()%7;
}


void afficher_futur_Tetramino(SDL_Rect positionFuturTetramino,SDL_Surface *Tetramino[7], SDL_Surface *ecran ){
  int d_x = 480;
  int d_y = 50;
  int px = TAILLE_BLOC;
	
  switch (future_Tetramino) {
  case 0:
    //en carré
    positionFuturTetramino.x = d_x + (px*1);
    positionFuturTetramino.y = d_y + (px*2);
    SDL_BlitSurface (Tetramino[future_Tetramino], NULL, ecran, &positionFuturTetramino);
    positionFuturTetramino.x = d_x + (px*2);
    positionFuturTetramino.y = d_y + (px*2);
    SDL_BlitSurface (Tetramino[future_Tetramino], NULL, ecran, &positionFuturTetramino);
    positionFuturTetramino.x = d_x + (px*1);
    positionFuturTetramino.y = d_y + (px*1);
    SDL_BlitSurface (Tetramino[future_Tetramino], NULL, ecran, &positionFuturTetramino);
    positionFuturTetramino.x = d_x + (px*2);
    positionFuturTetramino.y = d_y + (px*1);
    SDL_BlitSurface (Tetramino[future_Tetramino], NULL, ecran, &positionFuturTetramino);
    break;
  case 1:
    //en ligne droite
    positionFuturTetramino.x = d_x + (px*1);
    positionFuturTetramino.y = d_y + (px*2);
    SDL_BlitSurface (Tetramino[future_Tetramino], NULL, ecran, &positionFuturTetramino);
    positionFuturTetramino.x = d_x + (px*2);
    positionFuturTetramino.y = d_y + (px*2);
    SDL_BlitSurface (Tetramino[future_Tetramino], NULL, ecran, &positionFuturTetramino);
    positionFuturTetramino.x = d_x + (px*3);
    positionFuturTetramino.y = d_y + (px*2);
    SDL_BlitSurface (Tetramino[future_Tetramino], NULL, ecran, &positionFuturTetramino);
    positionFuturTetramino.x = d_x + (px*4);
    positionFuturTetramino.y = d_y + (px*2);
    SDL_BlitSurface (Tetramino[future_Tetramino], NULL, ecran, &positionFuturTetramino);
    break;
  case 2:
    // en L
    positionFuturTetramino.x = d_x + (px*1);
    positionFuturTetramino.y = d_y + (px*1);
    SDL_BlitSurface (Tetramino[future_Tetramino], NULL, ecran, &positionFuturTetramino);
    positionFuturTetramino.x = d_x + (px*1);
    positionFuturTetramino.y = d_y + (px*2);
    SDL_BlitSurface (Tetramino[future_Tetramino], NULL, ecran, &positionFuturTetramino);
    positionFuturTetramino.x = d_x + (px*2);
    positionFuturTetramino.y = d_y + (px*2);
    SDL_BlitSurface (Tetramino[future_Tetramino], NULL, ecran, &positionFuturTetramino);
    positionFuturTetramino.x = d_x + (px*3);
    positionFuturTetramino.y = d_y + (px*2);
    SDL_BlitSurface (Tetramino[future_Tetramino], NULL, ecran, &positionFuturTetramino);
    break;
  case 3:
    //en _|
    positionFuturTetramino.x = d_x + (px*1);
    positionFuturTetramino.y = d_y + (px*2);
    SDL_BlitSurface (Tetramino[future_Tetramino], NULL, ecran, &positionFuturTetramino);
    positionFuturTetramino.x = d_x + (px*2);
    positionFuturTetramino.y = d_y + (px*2);
    SDL_BlitSurface (Tetramino[future_Tetramino], NULL, ecran, &positionFuturTetramino);
    positionFuturTetramino.x = d_x + (px*3);
    positionFuturTetramino.y = d_y + (px*2);
    SDL_BlitSurface (Tetramino[future_Tetramino], NULL, ecran, &positionFuturTetramino);
    positionFuturTetramino.x = d_x + (px*3);
    positionFuturTetramino.y = d_y + (px*1);
    SDL_BlitSurface (Tetramino[future_Tetramino], NULL, ecran, &positionFuturTetramino);
    break;
  case 4:
    //en T
    positionFuturTetramino.x = d_x + (px*1);
    positionFuturTetramino.y = d_y + (px*1);
    SDL_BlitSurface (Tetramino[future_Tetramino], NULL, ecran, &positionFuturTetramino);
    positionFuturTetramino.x = d_x + (px*2);
    positionFuturTetramino.y = d_y + (px*1);
    SDL_BlitSurface (Tetramino[future_Tetramino], NULL, ecran, &positionFuturTetramino);
    positionFuturTetramino.x = d_x + (px*2);
    positionFuturTetramino.y = d_y + (px*2);
    SDL_BlitSurface (Tetramino[future_Tetramino], NULL, ecran, &positionFuturTetramino);
    positionFuturTetramino.x = d_x + (px*3);
    positionFuturTetramino.y = d_y + (px*1);
    SDL_BlitSurface (Tetramino[future_Tetramino], NULL, ecran, &positionFuturTetramino);
    break;
  case 5:
    // en _|-
    positionFuturTetramino.x = d_x + (px*1);
    positionFuturTetramino.y = d_y + (px*2);
    SDL_BlitSurface (Tetramino[future_Tetramino], NULL, ecran, &positionFuturTetramino);
    positionFuturTetramino.x = d_x + (px*2);
    positionFuturTetramino.y = d_y + (px*2);
    SDL_BlitSurface (Tetramino[future_Tetramino], NULL, ecran, &positionFuturTetramino);
    positionFuturTetramino.x = d_x + (px*2);
    positionFuturTetramino.y = d_y + (px*1);
    SDL_BlitSurface (Tetramino[future_Tetramino], NULL, ecran, &positionFuturTetramino);
    positionFuturTetramino.x = d_x + (px*3);
    positionFuturTetramino.y = d_y + (px*1);
    SDL_BlitSurface (Tetramino[future_Tetramino], NULL, ecran, &positionFuturTetramino);
    break;
  case 6:
    //en -|_
    positionFuturTetramino.x = d_x + (px*1);
    positionFuturTetramino.y = d_y + (px*1);
    SDL_BlitSurface (Tetramino[future_Tetramino], NULL, ecran, &positionFuturTetramino);
    positionFuturTetramino.x = d_x + (px*2);
    positionFuturTetramino.y = d_y + (px*1);
    SDL_BlitSurface (Tetramino[future_Tetramino], NULL, ecran, &positionFuturTetramino);
    positionFuturTetramino.x = d_x + (px*2);
    positionFuturTetramino.y = d_y + (px*2);
    SDL_BlitSurface (Tetramino[future_Tetramino], NULL, ecran, &positionFuturTetramino);
    positionFuturTetramino.x = d_x + (px*3);
    positionFuturTetramino.y = d_y + (px*2);
    SDL_BlitSurface (Tetramino[future_Tetramino], NULL, ecran, &positionFuturTetramino);
    break;
  default:
    break;
  }
}

void charger_images(SDL_Surface *Tetramino[7]){
  Tetramino[0] = SDL_LoadBMP ("images/violet.bmp");
  if (Tetramino[0] == NULL){
    printf("Probleme de chargement du fichier : %s\n", SDL_GetError());
   SDL_Quit(); 
  }
  Tetramino[1] = SDL_LoadBMP ("images/vert.bmp");
  if (Tetramino[1] == NULL){
    printf("Probleme de chargement du fichier : %s\n", SDL_GetError());
    SDL_Quit(); 
  }
  Tetramino[2] = SDL_LoadBMP ("images/bleu.bmp");
  if (Tetramino[2] == NULL){
    printf("Probleme de chargement du fichier : %s\n", SDL_GetError());
    SDL_Quit(); 
  }
  Tetramino[3] = SDL_LoadBMP ("images/gris.bmp");
  if (Tetramino[3] == NULL) {
    printf("Probleme de chargement du fichier : %s\n", SDL_GetError());
   SDL_Quit(); 
  }
  Tetramino[4] = SDL_LoadBMP ("images/jaune.bmp");
  if (Tetramino[4] == NULL) {
    printf("Probleme de chargement du fichier : %s\n", SDL_GetError());
    SDL_Quit(); 
  }
  Tetramino[5] = SDL_LoadBMP ("images/orange.bmp");
  if (Tetramino[5] == NULL){
    printf("Probleme de chargement du fichier : %s\n", SDL_GetError());
     SDL_Quit(); 
  }
  Tetramino[6] = SDL_LoadBMP ("images/bleuc.bmp");
  if (Tetramino[6] == NULL){
    printf("Probleme de chargement du fichier : %s\n", SDL_GetError());
    SDL_Quit(); 
  }
}	

void creer_matrice_jeu(int i ,int j,int matrice_jeu[i][j]){  
  int l,h;
  for (h=0;h<NB_BLOCS_HAUTEUR;h++) {
    for (l=0;l<NB_BLOCS_LARGEUR;l++){
      matrice_jeu[l][h] = 0;
    }
  }
  for (l=0;l<NB_BLOCS_LARGEUR;l++){
    matrice_jeu[l][NB_BLOCS_HAUTEUR-1] = 255; 
  }
  //On initialise les deux colonnes sur les côtés
  for (h=0;h<NB_BLOCS_HAUTEUR;h++){
    matrice_jeu[0][h] = 255;
    matrice_jeu[11][h] = 255;
  }
  /* verif_matrice_jeu (NB_BLOCS_LARGEUR,NB_BLOCS_HAUTEUR ,matrice_jeu);  */
}

void verif_matrice_jeu (int l ,int h ,int  matrice_jeu[l][h]){
  int i = 0;
  int j = 0;

  /* for (i = 0; i < h; i++) { */
  /*   for(j = 0; j < l; j++) { */
  /*     printf("Tab[%d][%d] = %d\n", j, i, matrice_jeu[j][i]); */
  /*   } */
  /* } */
  printf(" \n Tableau : \n");
  //Pour chaque ligne
  for (i=0;i<h;i++)
    {
      //Pour chaque colonne
      for (j=0;j<l;j++)
	{
	  printf(" %3d ",matrice_jeu[j][i]);
	}
      printf("\n");
    }
}
void creer_tetramino(int l ,int h ,int  matrice_jeu[l][h]){
  // on initialise le tetramino de la matrice
  jeu_tetramino = future_Tetramino; 
  switch (jeu_tetramino)
    {
    case 0:
      // en carré
      matrice_jeu[4][0] = jeu_tetramino + 1 ;
      matrice_jeu[4][1] = jeu_tetramino + 1 ;
      matrice_jeu[5][0] = jeu_tetramino + 1 ;
      matrice_jeu[5][1] = jeu_tetramino + 1;
      break;
    case 1:
      // en ligne
      matrice_jeu[3][0] = jeu_tetramino + 1;
      matrice_jeu[4][0] = jeu_tetramino + 1;
      matrice_jeu[5][0] = jeu_tetramino + 1 ;
      matrice_jeu[6][0] = jeu_tetramino + 1 ;
      break;
    case 2:
      // en |_	       
      matrice_jeu[4][0] = jeu_tetramino + 1 ;
      matrice_jeu[4][1] = jeu_tetramino + 1 ;
      matrice_jeu[5][1] = jeu_tetramino + 1 ;
      matrice_jeu[6][1] = jeu_tetramino + 1 ;
      break;
    case 3:
      // en _|	       
      matrice_jeu[4][1] = jeu_tetramino + 1 ;
      matrice_jeu[5][1] = jeu_tetramino + 1 ;
      matrice_jeu[6][1] = jeu_tetramino + 1 ;
      matrice_jeu[6][0] = jeu_tetramino + 1 ;
      break;
    case 4:
      //en T	       
      matrice_jeu[4][0] = jeu_tetramino + 1 ;
      matrice_jeu[5][0] = jeu_tetramino + 1 ;
      matrice_jeu[5][1] = jeu_tetramino + 1 ;
      matrice_jeu[6][0] = jeu_tetramino + 1 ;
      break;
    case 5:
      // en _|-	       
      matrice_jeu[4][1] = jeu_tetramino + 1 ;
      matrice_jeu[5][1] = jeu_tetramino + 1 ;
      matrice_jeu[5][0] = jeu_tetramino + 1 ;
      matrice_jeu[6][0] = jeu_tetramino + 1 ;
      break;
    case 6:
      //en -|_	       
      matrice_jeu[4][0] = jeu_tetramino + 1 ;
      matrice_jeu[5][0] = jeu_tetramino + 1 ;
      matrice_jeu[5][1] = jeu_tetramino + 1 ;
      matrice_jeu[6][1] = jeu_tetramino + 1 ;
      break;      
    }
  /* verif_matrice_jeu (NB_BLOCS_LARGEUR,NB_BLOCS_HAUTEUR ,matrice_jeu); */
  // generation nouveau tetramino
  future_Tetramino = (SDL_GetTicks()%7);
  rotation=0;
}


void afficher_matrice_jeu (int l ,int h ,int  matrice_jeu[l][h],SDL_Rect positionTetramino,SDL_Surface *Tetramino[7], SDL_Surface *ecran ){
  int i;
  int j;
  int decalage_x = (640-(12*20))/2;
  int decalage_y = 20;
  int px = TAILLE_BLOC;
  for (i=0;i<h-1;i++){
    for (j=1;j<(l-1);j++){
      if (matrice_jeu[j][i] > 0){
	positionTetramino.x = decalage_x + (px*j);
	positionTetramino.y = decalage_y + (px*i);
	if (matrice_jeu[j][i]>10){
	  //Affichage d'un carré fixé
	  SDL_BlitSurface (Tetramino[matrice_jeu[j][i]-11], NULL, ecran, &positionTetramino);
	}
	else{						     
	  SDL_BlitSurface (Tetramino[matrice_jeu[j][i]-1], NULL, ecran, &positionTetramino);
	}
      }
    }
  }
}

int chute_du_tetramino(int l ,int h ,int  matrice_jeu[l][h])
{ int i,j;
  int chute_possible,suite;
  chute_possible = 1;
  suite=0;
  // tester si chute possible
  for (i=21;i>=0 && chute_possible;i--){	
    for (j=1;j<11 && chute_possible;j++){
      if (matrice_jeu[j][i] < 10 && matrice_jeu[j][i]>0 ){
	if (matrice_jeu[j][i] < matrice_jeu[j][i+1]){   
	  chute_possible = 0;
	}
      }
    }
  }
  if (chute_possible){
    for (i=21;i>=0 && chute_possible ;i--){
      for (j=1;j<11&& chute_possible;j++)
	{if (matrice_jeu[j][i] < 10 && matrice_jeu[j][i]>0){
	    //chute des carre
	    matrice_jeu[j][i+1] = matrice_jeu[j][i];
	    matrice_jeu[j][i] = 0;
	  }
	}
    }
  }
  else  {
    stop=1;// les carres ce bloque
    for (i=23;i>=0;i--){
      for (j=1;j<11;j++){
	if (matrice_jeu[j][i] < 10 && matrice_jeu[j][i]>0){					
	  matrice_jeu[j][i] = matrice_jeu[j][i] + 10;
	  suite=1;
	  if (i<=2){jeu_pas_fini=0;//tetramino ce bloque dans les carre du haut le jeu et fini
	  }
	}			
      }
    }     	
    score=score + 1+ (10*(niveau-1));
    // verification si ligne complete et chute
    chute_ligne(NB_BLOCS_LARGEUR ,NB_BLOCS_HAUTEUR,matrice_jeu);
    /* SDL_Delay (300); */
    if(jeu_pas_fini){
      SDL_Delay (300);
      //si jeu pas fini on rejoue avec le tetramino  suivant
      creer_tetramino(NB_BLOCS_LARGEUR ,NB_BLOCS_HAUTEUR,matrice_jeu);}
  } 
  /* verif_matrice_jeu (NB_BLOCS_LARGEUR,NB_BLOCS_HAUTEUR ,matrice_jeu); */
  return suite; 
}

void tourner_piece (int l ,int h ,int  matrice_jeu[l][h]){
  int i;
  int j;
  int tetramino_trouve = 0;
  /* printf("piece : %d \n", jeu_tetramino); */
  /* printf("rotation : %d \n", rotation); */
  for (i=22;i>=0 && !tetramino_trouve;i--){
    for (j=1;j<(l-1) &&  !tetramino_trouve;j++){
      if (matrice_jeu[j][i] < 10 && matrice_jeu[j][i]>0){
	switch (jeu_tetramino){  
	case 0:
	  tetramino_trouve = 1;
	  break;
	case 1:
	  switch (rotation) { 
	  case 0: //On vérifie les cases a remplire
	    if (matrice_jeu[j+2][i+2]==0 && matrice_jeu[j+2][i-1]==0 && matrice_jeu[j+2][i+1]==0){
	      //Ok, on pivote et  on enregistre la nouvelle rotation							
	      matrice_jeu[j+2][i+2] = matrice_jeu[j][i];
	      matrice_jeu[j+2][i+1] = matrice_jeu[j+1][i];
	      matrice_jeu[j+2][i-1] = matrice_jeu[j+3][i];
	      matrice_jeu[j][i] = 0;
	      matrice_jeu[j+1][i] = 0;
	      matrice_jeu[j+3][i] = 0;
	      rotation=1;
	    }
	    break; 
	  case 1:
	    if (matrice_jeu[j-2][i-2]==0 && matrice_jeu[j-1][i-2]==0 && matrice_jeu[j+1][i-2]==0){		
	      matrice_jeu[j-2][i-2] = matrice_jeu[j][i];
	      matrice_jeu[j-1][i-2] = matrice_jeu[j][i-1];
	      matrice_jeu[j+1][i-2] = matrice_jeu[j][i-3];
	      matrice_jeu[j][i] = 0;
	      matrice_jeu[j][i-1] = 0;
	      matrice_jeu[j][i-3] = 0;
	      rotation = 0;
	    }
	    break;
	  }
	  tetramino_trouve = 1; // on arrete
	  break;
	case 2:
	  switch (rotation){
	  case 0:
	    if (matrice_jeu[j+1][i-1]==0 && matrice_jeu[j][i+1] ==0 && matrice_jeu[j+1][i+1]==0){						
	      matrice_jeu[j+1][i-1] = matrice_jeu[j][i];
	      matrice_jeu[j][i+1] = matrice_jeu[j][i-1];
	      matrice_jeu[j+1][i+1] = matrice_jeu[j+2][i];
	      matrice_jeu[j][i] = 0;
	      matrice_jeu[j][i-1] = 0;
	      matrice_jeu[j+2][i] = 0;
	      rotation = 1;	
	    }
	    break;
	  case 1:
	    if (matrice_jeu[j][i-1]==0 && matrice_jeu[j+2][i]==0 && matrice_jeu[j+2][i-1]==0){				
	      matrice_jeu[j][i-1] = matrice_jeu[j][i];
	      matrice_jeu[j+2][i] = matrice_jeu[j+1][i];
	      matrice_jeu[j+2][i-1] = matrice_jeu[j+1][i-2];
	      matrice_jeu[j][i] = 0;
	      matrice_jeu[j+1][i] = 0;
	      matrice_jeu[j+1][i-2] = 0;
	      rotation = 2;
	    }
	    break;
	  case 2:
	    if (matrice_jeu[j-1][i]==0 && matrice_jeu[j-1][i-2]==0 && matrice_jeu[j][i-2]==0){						
	      matrice_jeu[j-1][i] = matrice_jeu[j][i];
	      matrice_jeu[j-1][i-2] = matrice_jeu[j][i-1];
	      matrice_jeu[j][i-2] = matrice_jeu[j-2][i-1];
	      matrice_jeu[j][i] = 0;
	      matrice_jeu[j][i-1] = 0;
	      matrice_jeu[j-2][i-1] = 0;
	      rotation = 3;	
	    }
	    break;
	  case 3:
	    if (matrice_jeu[j-1][i-2]==0 && matrice_jeu[j-1][i-1]==0 && matrice_jeu[j+1][i-1]==0){
	      matrice_jeu[j-1][i-2] = matrice_jeu[j][i];
	      matrice_jeu[j-1][i-1] = matrice_jeu[j][i-2];
	      matrice_jeu[j+1][i-1] = matrice_jeu[j+1][i-2];
	      matrice_jeu[j][i] = 0;
	      matrice_jeu[j][i-2] = 0;
	      matrice_jeu[j+1][i-2] = 0;
	      rotation = 0;
	    }
	    break;
	  }
	  tetramino_trouve = 1;
	  break;
	case 3:
	  switch (rotation) {
	  case 0:
	    if (matrice_jeu[j+1][i+1]==0 && matrice_jeu[j][i-1]==0 && matrice_jeu[j+1][i-1]==0){			
	      matrice_jeu[j+1][i+1] = matrice_jeu[j][i];
	      matrice_jeu[j][i-1] = matrice_jeu[j+2][i-1];
	      matrice_jeu[j+1][i-1] = matrice_jeu[j+2][i];
	      matrice_jeu[j][i] = 0;
	      matrice_jeu[j+2][i-1] = 0;
	      matrice_jeu[j+2][i] = 0;
	      rotation = 1;
	    }
	    break;
	  case 1:
	    if (matrice_jeu[j-1][i]==0 && matrice_jeu[j-2][i-1]==0 && matrice_jeu[j+1][i-1]==0){				
	      matrice_jeu[j-1][i] = matrice_jeu[j][i];
	      matrice_jeu[j-1][i-1] = matrice_jeu[j-1][i-2];
	      matrice_jeu[j+1][i-1] = matrice_jeu[j][i-2];
	      matrice_jeu[j][i] = 0;
	      matrice_jeu[j-1][i-2] = 0;
	      matrice_jeu[j][i-2] = 0;
	      rotation = 2;
	    }
	    break;
	  case 2:
	    if (matrice_jeu[j+1][i-2]==0 && matrice_jeu[j+1][i]==0 && matrice_jeu[j+2][i]==0){						
	      matrice_jeu[j+1][i-2] = matrice_jeu[j][i];
	      matrice_jeu[j+1][i] = matrice_jeu[j][i-1];
	      matrice_jeu[j+2][i] = matrice_jeu[j+2][i-1];
	      matrice_jeu[j][i] = 0;
	      matrice_jeu[j][i-1] = 0;
	      matrice_jeu[j+2][i-1] = 0;
	      rotation = 3;					
	    }
	    break;
	  case 3:
	    if (matrice_jeu[j-1][i-1]==0 && matrice_jeu[j+1][i-1]==0 && matrice_jeu[j+1][i-2]==0){				
	      matrice_jeu[j-1][i-1] = matrice_jeu[j][i];
	      matrice_jeu[j+1][i-1] = matrice_jeu[j+1][i];
	      matrice_jeu[j+1][i-2] = matrice_jeu[j][i-2];
	      matrice_jeu[j][i] = 0;
	      matrice_jeu[j+1][i] = 0;
	      matrice_jeu[j][i-2] = 0;
	      rotation = 0;
	    }
	    break;
	  }
	  tetramino_trouve = 1;
	  break;
	case 4:
	  switch (rotation) {
	  case 0:
	    if (matrice_jeu[j-1][i-2]==0){
	      matrice_jeu[j][i-2] = matrice_jeu[j-1][i-1];
	      matrice_jeu[j-1][i-1] = 0;
	      rotation=1;
	    }
	    break;
	  case 1:
	    if (matrice_jeu[j-1][i-1]==0){
	      matrice_jeu[j-1][i-1] = matrice_jeu[j][i];
	      matrice_jeu[j][i] = 0;
	      rotation=2;	
	    }
	    break;
	  case 2:
	    if (matrice_jeu[j+1][i+1]==0){
	      matrice_jeu[j+1][i+1] = matrice_jeu[j+2][i];
	      matrice_jeu[j+2][i] = 0;
	      rotation=3;
	    }
	    break;
	  case 3:
	    if (matrice_jeu[j+1][i-1]==0){
	      matrice_jeu[j+1][i-1] = matrice_jeu[j][i-2];
	      matrice_jeu[j][i-2] = 0;
	      rotation=0;
	    }
	    break;
	  }
	  tetramino_trouve = 1;
	  break;
	case 5:
	  switch (rotation){
	  case 0:
	    if (matrice_jeu[j+1][i-2]==0 && matrice_jeu[j+2][i]==0){
	      matrice_jeu[j+1][i-2] = matrice_jeu[j][i];
	      matrice_jeu[j+2][i] = matrice_jeu[j+1][i];
	      matrice_jeu[j][i] = 0;
	      matrice_jeu[j+1][i] = 0;
	      rotation=1;
	    }
	    break;
	  case 1:
	    if (matrice_jeu[j-2][i]==0 && matrice_jeu[j-1][i]==0){
	      matrice_jeu[j-2][i] = matrice_jeu[j][i];
	      matrice_jeu[j-1][i] = matrice_jeu[j-1][i-2];
	      matrice_jeu[j][i] = 0;
	      matrice_jeu[j-1][i-2] = 0;
	      rotation=0;
	    }
	    break;
	  }
	  tetramino_trouve = 1;
	  break;
	case 6:
	  switch (rotation){				
	  case 0:
	    if (matrice_jeu[j+1][i-2]==0 && matrice_jeu[j+1][i-1]==0){
	      matrice_jeu[j+1][i-1] = matrice_jeu[j-1][i-1];
	      matrice_jeu[j+1][i-2] = matrice_jeu[j+1][i];
	      matrice_jeu[j+1][i] = 0;
	      matrice_jeu[j-1][i-1] = 0;
	      rotation=1;	
	    }
	    break;				
	  case 1:
	    if (matrice_jeu[j+1][i]==0 && matrice_jeu[j-1][i-1]==0){
	      matrice_jeu[j+1][i] = matrice_jeu[j+1][i-1];
	      matrice_jeu[j-1][i-1] = matrice_jeu[j+1][i-2];
	      matrice_jeu[j+1][i-1] = 0;
	      matrice_jeu[j+1][i-2] = 0;
	      rotation=0;	
	    }
	    break;
	  }
	  tetramino_trouve = 1;
	  break;
	}
      }
    }
  }
}

void deplacementgauche(int l ,int h ,int  matrice_jeu[l][h]){

  int i,j;
  int gauche_possible;
  gauche_possible = 1;
  // tester si deplacement gauche est possible
  for (i=21;i>=0 && gauche_possible;i--){
    for (j=1;j<11 && gauche_possible;j++){
      if (matrice_jeu[j][i] < 10 && matrice_jeu[j][i]>0){
	if (matrice_jeu[j][i] < matrice_jeu[j-1][i]){  
	  gauche_possible = 0;
	}
      }
    }
  }
  if (gauche_possible){// on deplace
    for (i=21;i>=0 && gauche_possible ;i--){
      for (j=1;j<11&& gauche_possible;j++){
	if (matrice_jeu[j][i] < 10 && matrice_jeu[j][i]>0){
	  matrice_jeu[j-1][i] = matrice_jeu[j][i];
	  matrice_jeu[j][i] = 0;
	}
      }
    }
  }
}

void deplacementdroite(int l ,int h ,int  matrice_jeu[l][h]){

  int i,j;
  int droite_possible;
  droite_possible = 1;
  // tester si deplacement droite est possible
  for (i=21;i>=0 && droite_possible;i--){
    for (j=1;j<11 && droite_possible;j++){
      if (matrice_jeu[j][i] < 10 && matrice_jeu[j][i]>0){
	if (matrice_jeu[j][i] < matrice_jeu[j+1][i]){  
	  droite_possible = 0;
	}
      }
    }
  }
  if (droite_possible){
    for (i=21;i>=0 && droite_possible ;i--){
      for (j=11;j>=1 && droite_possible;j--){
	if (matrice_jeu[j][i] < 10 && matrice_jeu[j][i]>0){
	  matrice_jeu[j+1][i] = matrice_jeu[j][i];
	  matrice_jeu[j][i] = 0;
	}
      }
    }
  }
}


void chute_ligne (int l ,int h ,int  matrice_jeu[l][h])
{
  int i;
  int j;
  int i2;
  int ligne_pleine;
  int total = 0; // nombre de lignes effacées
  for (i=21;i>=2;i--){
    ligne_pleine=1;
    for (j=1;j<11;j++){	
      if (matrice_jeu[j][i] == 0){
	//la ligne n'est pas pleine
	ligne_pleine = 0;
      }
    }
    if (ligne_pleine)
      {  explose=1;
	for (i2=i;i2>=3;i2--){
	  for (j=1;j<11;j++){
	    matrice_jeu[j][i2] = matrice_jeu[j][i2-1];
	  }
	}
	i ++;
	total ++;	 
      }
  }
  nbr_ligne=nbr_ligne+total;
  score= score + (total*2*niveau);
}


void afficher_niveau (SDL_Color Vert,TTF_Font *police,SDL_Rect pos_niveau,SDL_Surface *surface_niveau, SDL_Surface *ecran)
{ 
  char chaine_niveau[5];
  if (score>50){ niveau =2;}
  if (score>100){ niveau =3;}
  if (score>200){ niveau =4;}
  if (score>400){ niveau =5;}
  if (score>800){ niveau =6;}
  if (score>1500){ niveau =7;} 
  if (score>3000){ niveau =8;}
  if (score>9000){ niveau =9;} 
  if (score>50000){ niveau =10;}
  if (niveau <= 99999){
    sprintf (chaine_niveau, "%d",niveau ) ;
    surface_niveau = TTF_RenderText_Solid (police,chaine_niveau ,Vert);
    if (surface_niveau) {
      pos_niveau.x = 500;
      pos_niveau.y = 230;	
      SDL_BlitSurface (surface_niveau, NULL, ecran, &pos_niveau);              
    } 
  }
}



void afficher_timer (SDL_Color Vert,TTF_Font *police,SDL_Rect pos_timer,SDL_Surface *surface_timer, SDL_Surface *ecran)
{
  char chaine_timer [6];
  if ( time/60 <= 999){// on test les depacements
    sprintf ( chaine_timer, "%d  %d",(time/60)%60,time%60);
    surface_timer = TTF_RenderText_Solid ( police,chaine_timer ,Vert);
    if(surface_timer) {
      pos_timer.x = 65;
      pos_timer.y = 380;
      SDL_BlitSurface(surface_timer, NULL, ecran , &pos_timer);
    }
  }
}
void afficher_score(SDL_Color Vert,TTF_Font *police,SDL_Rect pos_score,SDL_Surface *surface_score, SDL_Surface *ecran)
{
  char chaine_score [5];
  if ( best_score <= 99999){
    sprintf ( chaine_score, "%d", score);
    surface_score = TTF_RenderText_Solid ( police,chaine_score ,Vert);
    if(surface_score){
      pos_score.x = 65;
      pos_score.y = 85;
      SDL_BlitSurface(surface_score, NULL, ecran , &pos_score);
    }
  }
}

void afficher_best_score(SDL_Color Vert,TTF_Font *police,SDL_Rect pos_best_score,SDL_Surface *surface_best_score, SDL_Surface *ecran)
{
  char chaine_best_score [5];
  if ( best_score <= 99999){
    sprintf ( chaine_best_score, "%d", best_score);
    surface_best_score = TTF_RenderText_Solid ( police,chaine_best_score ,Vert);
    if(surface_best_score){
      pos_best_score.x = 65;
      pos_best_score.y = 230;
      SDL_BlitSurface(surface_best_score, NULL, ecran , &pos_best_score);
    }
  }
}
void afficher_nbr_ligne(SDL_Color Vert,TTF_Font *police,SDL_Rect pos_nbr_ligne,SDL_Surface *surface_nbr_ligne, SDL_Surface *ecran)
{ 
  if (  nbr_ligne <= 99999){
    char chaine_nbr_ligne [5];
    sprintf ( chaine_nbr_ligne, "%d", nbr_ligne);
    surface_nbr_ligne = TTF_RenderText_Solid ( police,chaine_nbr_ligne ,Vert);
    if(surface_nbr_ligne){
      pos_nbr_ligne.x = 500;
      pos_nbr_ligne.y = 380;
      SDL_BlitSurface(surface_nbr_ligne, NULL, ecran , &pos_nbr_ligne);
    }
  }
}
