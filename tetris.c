//Librairie standard
#include <stdio.h>
#include <stdlib.h>
//Pour utiliser la SDL
#include <SDL.h>
#include <SDL_ttf.h>

//définition des fonctions utilisées par la suite
int Init( void );
int Draw( void );
void Frame( void );
void charger_images (void);
void afficher_niveau (void);
int Init_ttf( void );
void afficher_background (void);
void afficher_tableau (void);
void free_surface(void);

//La mémoire vidéo contenant ce qui s'affiche
SDL_Surface *ecran;
//Les évènements du programme
SDL_Event evenements;
//La mémoire vidéo contenant le fond du jeu
SDL_Surface *Background;
//La destination de la copie du fond
SDL_Rect rect;
//La police 
TTF_Font *police;
//La surface pour afficher le niveau
SDL_Surface *surface_niveau;
//La couleur du score (blanc);
SDL_Color couleur = {255,255,000};
SDL_Surface *ico;


void free_surface(void)
{
SDL_FreeSurface(ico);
SDL_FreeSurface(surface_niveau);

SDL_FreeSurface(Background);
SDL_FreeSurface(ecran);
}

int Init( void )
{
  if ( SDL_Init( SDL_INIT_VIDEO ) == -1 )
    {
      printf( "Echec lors du chargement de la vidéo : %s", SDL_GetError() );
      SDL_Quit();
    }
  else
    {
      if ( ecran = SDL_SetVideoMode( 640, 480, 16, SDL_HWSURFACE | SDL_DOUBLEBUF ) )
 
	{  //On marque teris dans la barre de titre	 
	  SDL_WM_SetCaption("Tetris !", NULL);
	  //On cache le curseur de la souris	
	  SDL_ShowCursor(SDL_DISABLE);
	  //On charge les images du jeu
	  charger_images();
          // une icone pour windows
	  SDL_WM_SetIcon(ico,NULL);
	  //On initialise la police du jeu
	  Init_ttf();
          //On charge la police du jeu
          charger_police();
	  return 1;}
      else
	SDL_Quit();
    }
  return 0;
}
void charger_images(void)
{
  //image du fond
  Background = SDL_LoadBMP ("bg.bmp");
  if (Background == NULL)
    {
      printf("Probleme de chargement du fond : %s\n", SDL_GetError());
      exit(-1);
    }
  //icone du jeu
  ico = SDL_LoadBMP("ico.png");
}	


int Draw( void )
{
  afficher_background ();
  afficher_niveau ();
  //afficher_tableau ();

  SDL_Flip( ecran );
  return 1;
}
void Frame( void )
{
  SDL_Event event;

  if (Draw() == 0)
    return;

  while (1)
    {

      SDL_WaitEvent(&event);

      switch (event.type)
        {
	case SDL_VIDEOEXPOSE:
	  Draw();
         
	  break;
	case SDL_QUIT:
            TTF_CloseFont(police); /* Doit être avant TTF_Quit() */
            TTF_Quit(); 
	  free_surface();
	  SDL_Quit();
	  return;
	}
    }
}

int Init_ttf( void ){

  if (TTF_Init()<0)
    {
      printf("Impossible d'initialiser la librairie SDL_ttf : %s\n", TTF_GetError());
      exit(-1);
    }
}
int charger_police( void ){
  //On charge  la police à utiliser
  police = TTF_OpenFont ("verdana.ttf", 16);
  if (police == NULL)
    {
      printf("Impossible de charger la police : %s\n", TTF_GetError());
      exit(-1);
    }


}

void afficher_niveau (void)
{
  char chaine_niveau[3];
  int niveau = 123;
  sprintf (chaine_niveau, "%d",niveau ) ;
  surface_niveau = TTF_RenderText_Solid (police,chaine_niveau , couleur);
  if (surface_niveau)
    {
      rect.x = 173;
      rect.y = 400;
	
      SDL_BlitSurface (surface_niveau, NULL, ecran, &rect);
              
    }
}

void afficher_background (void){
  rect.x = rect.y = 0;
  rect.w = 640;
  rect.h = 480;

  SDL_BlitSurface( Background, &rect, ecran, &rect );
}




int main( int argc, char* argv[] )
{
  if ( Init() )
    {
      Frame();
    }
    
  return 0;
}
