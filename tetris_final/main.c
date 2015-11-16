/*
  main.c
  ------
  Par Paul ROBIN et Louis DUDOT

  Rôle : ecran d'accueil
*/

#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_mixer.h>
#include "constantes.h"
#include "jeu.h"

int main(int argc, char *argv[])
{ SDL_Surface *ico =NULL ;
  SDL_Surface *ecran = NULL, *Menu = NULL, *Background = NULL;
  SDL_Rect positionBackground, positionMenu;
  SDL_Event event;

  //Initialisation de l'API Mixer
  if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024) == -1) //Initialisation de l'API Mixer
    { printf("Probleme de chargement audio: %s\n", Mix_GetError());
      SDL_Quit();
    }

  int continuer = 1;
  // lecture des images
  ico = IMG_Load("images/ico.png"); Background = IMG_Load("images/backgroud.jpg"); Menu = IMG_Load("images/menu.png");

  if( ( ico == NULL ) || ( Background == NULL ) || ( Menu == NULL ) ){ 
    printf("Probleme de chargement ressourse: %s\n", SDL_GetError());
    SDL_Quit(); } 

  if( SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER)== -1 ){
    printf( "Echec lors du chargement de la video : %s", SDL_GetError() );
    SDL_Quit();
  }
  SDL_WM_SetIcon(ico, NULL); // L'icône doit être chargée avant SDL_SetVideoMode
  ecran = SDL_SetVideoMode(LARGEUR_FENETRE, HAUTEUR_FENETRE, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
  SDL_WM_SetCaption("Tetris Clone", NULL);
  SDL_ShowCursor(SDL_DISABLE);
 
  positionBackground.x = 0;
  positionBackground.y = 0;
  // pas besoin car png et SDL_image.h
  /* SDL_SetColorKey(Menu, SDL_SRCCOLORKEY, SDL_MapRGB(Menu->format, 0,0,0)); */

  positionMenu.x = 30;
  positionMenu.y = 180;

  while (continuer)
    {
      SDL_WaitEvent(&event);
      switch(event.type)
        {
	case SDL_QUIT:
	  continuer = 0;
	  break;
	case SDL_KEYDOWN:
	  switch(event.key.keysym.sym)
	    {
	    case SDLK_ESCAPE: // Veut sortir
	      continuer = 0;
	      break;
	    case SDLK_j: // Demande à jouer
	      jouer(ecran);
	      break;
	    }
	  break;
        }

      // Effacement de l'écran puis affichage
      SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 0, 0, 0));
      SDL_BlitSurface(Background, NULL, ecran, &positionBackground);
      SDL_BlitSurface(Menu, NULL, ecran, &positionMenu);
      SDL_Flip(ecran);
    }
  // Libération des surfaces chargées et de l audio

  SDL_FreeSurface(Menu);
  SDL_FreeSurface(Background);
  SDL_FreeSurface(ecran);
  Mix_CloseAudio();
 
  SDL_Quit();

  return EXIT_SUCCESS;
}
