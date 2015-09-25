//Librairie standard
#include <stdio.h>
#include <stdlib.h>
//Pour utiliser la SDL
#include <SDL.h>
#include <SDL_ttf.h>

//d√©finition des fonctions utilis√©es par la suite
int Init( void );
int Draw( void );
void Frame( void );
void charger_images (void);
void afficher_niveau (void);
int Init_ttf( void );
void afficher_background (void);
void afficher_tableau (void);
void free_surface(void);
void creer_piece(void);
void afficher_timer(void);
void afficher_score(void);
void afficher_best_score(void);
void afficher_nbr_ligne(void);


//La m√©moire vid√©o contenant ce qui s'affiche
SDL_Surface *ecran;
//Les √©v√®nements du programme
SDL_Event evenements;
//La m√©moire vid√©o contenant le fond du jeu
SDL_Surface *Background;
//La destination de la copie du fond
SDL_Rect rect;
//La police 
TTF_Font *police;
//La surface pour afficher le niveau
SDL_Surface *surface_niveau;
//La surface pour afficher le timer
SDL_Surface *surface_timer;
//La Surface pour afficher le score
SDL_Surface *surface_score;
//La Surface pour afficher le score
SDL_Surface *surface_best_score;
//La Surface pour afficher le nombre de ligne
SDL_Surface *surface_nbr_ligne;

//La couleur du score (blanc);
SDL_Color couleur = {255,255,255};
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
  if ( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_TIMER) == -1 )
    {
      printf( "Echec lors du chargement de la vid√©o : %s", SDL_GetError() );
      SDL_Quit();
    }
  else
    {
      if ( ecran = SDL_SetVideoMode( 640, 480, 16, SDL_HWSURFACE | SDL_DOUBLEBUF ) )
 
	{  //On marque teris dans la barre de titre	 
	  SDL_WM_SetCaption("Tetris !", NULL);
	  //On cache le curseur de la souris	
	  // SDL_ShowCursor(SDL_DISABLE);
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
  afficher_timer();
 afficher_score();
 afficher_best_score();
 afficher_nbr_ligne();
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
            TTF_CloseFont(police); /* Doit √™tre avant TTF_Quit() */
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
  //On charge  la police √† utiliser
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
  int niveau = 7;

  sprintf (chaine_niveau, "%d",niveau ) ;
  surface_niveau = TTF_RenderText_Solid (police,chaine_niveau , couleur);
  if (surface_niveau)
    {
      rect.x = 215;
      rect.y = 330;
	
      SDL_BlitSurface (surface_niveau, NULL, ecran, &rect);
              
    }
}

void afficher_background (void){
  rect.x = rect.y = 0;
  rect.w = 640;
  rect.h = 480;

  SDL_BlitSurface( Background, &rect, ecran, &rect );
}

void afficher_timer (void) 
   {
  char chaine_timer [5];
  int timer = 350;
  sprintf ( chaine_timer, "%d", timer);
  surface_timer = TTF_RenderText_Solid ( police,chaine_timer , couleur);
  if(surface_timer){
    rect.x = 450;
    rect.y = 420;
    SDL_BlitSurface(surface_timer, NULL, ecran , &rect);
  }
}
 


  
void afficher_score(void){
  char chaine_score [5];
  int score = 350;
  sprintf ( chaine_score, "%d", score);
  surface_score = TTF_RenderText_Solid ( police,chaine_score , couleur);
  if(surface_score){
    rect.x = 450;
    rect.y = 420;
    SDL_BlitSurface(surface_score, NULL, ecran , &rect);
  }
}

void afficher_best_score(void){
  char chaine_best_score [5];
  int best_score = 400;
  sprintf ( chaine_best_score, "%d", best_score);
  surface_best_score = TTF_RenderText_Solid ( police,chaine_best_score , couleur);
  if(surface_best_score){
    rect.x = 445;
    rect.y = 330;
    SDL_BlitSurface(surface_best_score, NULL, ecran , &rect);
  }
}

void afficher_nbr_ligne(void){
  char chaine_nbr_ligne [3];
  int nbr_ligne = 10;
  sprintf ( chaine_nbr_ligne, "%d", nbr_ligne);
  surface_nbr_ligne = TTF_RenderText_Solid ( police,chaine_nbr_ligne , couleur);
  if(surface_nbr_ligne){
    rect.x = 200;
    rect.y = 415;
    SDL_BlitSurface(surface_nbr_ligne, NULL, ecran , &rect);
  }
}

int main( int argc, char* argv[] )
{
  if ( Init() )
    {
      Frame();
    }
    
  return 0;
}


void creer_piece(void)
{
  int tab[4][2];
  int piece;
int I,J;
 /* Pour chaque ligne ... */
 for (I=0; I<4; I++)  
   {
    /* ... considÈrer chaque composante */
/* ... considÈrer chaque composante */
    for (J=0; J<2; J++)  
         
      tab[I][J]=0;
         
   }
switch (piece)
	{
		case 0:
			//PiËce constituÈe de 4 carrÈs placÈs en carrÈ
			//au milieu et en bas de la zone du haut
			tab[1][1] = piece + 1;
			tab[1][2] = piece + 1;
			tab[2][1] = piece + 1;
			tab[2][2] = piece + 1;
			break;


		case 1:
			//PiËce constituÈe de 4 carrÈs placÈs en ligne
	       
			tab[0][1] = piece + 1;
			tab[1][1] = piece + 1;
			tab[2][1] = piece + 1;
			tab[3][1] = piece + 1;
			break;

	case 2:
			//PiËce constituÈe de 4 carrÈs placÈs en T
	       
			tab[1][1] = piece + 1;
			tab[2][1] = piece + 1;
			tab[3][1] = piece + 1;
			tab[2][2] = piece + 1;
			break;

	case 3:
			//PiËce constituÈe de 4 carrÈs placÈs en l
	       
			tab[1][1] = piece + 1;
			tab[2][1] = piece + 1;
			tab[3][1] = piece + 1;
			tab[1][2] = piece + 1;
			break;
	case 4:
			//PiËce constituÈe de 4 carrÈs placÈs en l invere
	       
			tab[1][1] = piece + 1;
			tab[2][1] = piece + 1;
			tab[3][1] = piece + 1;
			tab[2][3] = piece + 1;
			break;


	case 5:
			//PiËce constituÈe de 4 carrÈs placÈs en S
	       
			tab[1][1] = piece + 1;
			tab[2][1] = piece + 1;
			tab[2][2] = piece + 1;
			tab[3][2] = piece + 1;
			break;
	case 6:
			//PiËce constituÈe de 4 carrÈs placÈs en S inverce
	       
			tab[2][1] = piece + 1;
			tab[3][1] = piece + 1;
			tab[1][2] = piece + 1;
			tab[2][2] = piece + 1;
			break;
      
	}
}
