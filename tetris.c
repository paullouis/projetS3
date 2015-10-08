//Librairie standard
#include <stdio.h>
#include <stdlib.h>
//Pour utiliser la SDL
#include <SDL.h>
#include <SDL_ttf.h>

//dÃ©finition des fonctions utilisÃ©es par la suite
int Init( void );
int Draw( void );
void Frame( void );
void charger_images (void);
void afficher_niveau (void);
int Init_ttf( void );
void afficher_background (void);
void afficher_tableau (void);
void free_surface(void);
void creer_tetramino(void);
void afficher_timer(void);
void afficher_score(void);
void afficher_best_score(void);
void afficher_nbr_ligne(void);
void creer_tetramino(void);
int hazard(void);
void verif_tableau (int n ,int m ,int tab[n][m]);
void creer_matrice_jeu(void);
void afficher_futur_tetramino(void);
void afficher_matrice_jeu (void);
void chute_tetramino(void);

//La mÃ©moire vidÃ©o contenant ce qui s'affiche
SDL_Surface *ecran;
//Les Ã©vÃ¨nements du programme
SDL_Event evenements;
//La mÃ©moire vidÃ©o contenant le fond du jeu
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
//Les couleur des score;
SDL_Color GREEN = {0,255,0};
SDL_Color WHITE = {255,255,255};
SDL_Color BLUE = {0,0,255}; 
SDL_Color RED = {255,0,0}; 

SDL_Surface *ico;
//Le tetramino en cours de chute
int jeu_tetramino;
//Le tetramino suivant
int future_tetramino;
//Le tableau de la matrice de jeu

int matrice_jeu[12][23];

int l,h ;
int largeur=12;
int hauteur = 23 ;
 //Les 7 différents carrés
SDL_Surface *tetramino[7];


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
      printf( "Echec lors du chargement de la vidÃ©o : %s", SDL_GetError() );
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
	  future_tetramino=hazard();
   
	  
	  return 1;}
      else
	SDL_Quit();
    }
  return 0;
}
void charger_images(void)
{
  //image du fond
  Background = SDL_LoadBMP ("images/bg.bmp");
  if (Background == NULL)
    {
      printf("Probleme de chargement du fond : %s\n", SDL_GetError());
      exit(-1);
    }
  //icone du jeu
  ico = SDL_LoadBMP("images/ico.png");

  //image du carré rouge
  tetramino[0] = SDL_LoadBMP ("images/rouge.bmp");
  if (tetramino[0] == NULL)
    {
      printf("Probleme de chargement du fichier : %s\n", SDL_GetError());
      exit(-1);
    }
  //image du carré bleuclaire
  tetramino[1] = SDL_LoadBMP ("images/bleuc.bmp");
  if (tetramino[1] == NULL)
    {
      printf("Probleme de chargement du fichier : %s\n", SDL_GetError());
      exit(-2);
    }
  //image du carré vert
  tetramino[2] = SDL_LoadBMP ("images/vert.bmp");
  if (tetramino[2] == NULL)
    {
      printf("Probleme de chargement du fichier : %s\n", SDL_GetError());
      exit(-3);
    }
  //image du carré violet
  tetramino[3] = SDL_LoadBMP ("images/violet.bmp");
  if (tetramino[3] == NULL)
    {
      printf("Probleme de chargement du fichier : %s\n", SDL_GetError());
      exit(-4);
    }
  //image du carré bleu
  tetramino[4] = SDL_LoadBMP ("images/bleu.bmp");
  if (tetramino[4] == NULL)
    {
      printf("Probleme de chargement du fichier : %s\n", SDL_GetError());
      exit(-5);
    }
  //image du carré brun
  tetramino[5] = SDL_LoadBMP ("images/brun.bmp");
  if (tetramino[5] == NULL)
    {
      printf("Probleme de chargement du fichier : %s\n", SDL_GetError());
      exit(-6);
    }
  //image du carré jaune
  tetramino[6] = SDL_LoadBMP ("images/jaune.bmp");
  if (tetramino[6] == NULL)
    {
      printf("Probleme de chargement du fichier : %s\n", SDL_GetError());
      exit(-7);
    }
}	


int Draw( void )
{
  afficher_background ();
  afficher_niveau ();
  afficher_timer();
  afficher_score();
  afficher_best_score();
  afficher_nbr_ligne();

  creer_matrice_jeu();
  creer_tetramino();
  afficher_futur_tetramino();
  afficher_matrice_jeu ();

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

      SDL_PollEvent(&event);

      switch (event.type)
        {
	case SDL_VIDEOEXPOSE:



	  Draw();
         
	  break;
	case SDL_QUIT:
	  TTF_CloseFont(police); /* Doit Ãªtre avant TTF_Quit() */
	  TTF_Quit(); 
	  free_surface();
	  SDL_Quit();
	  return;
	}
    }
}

int Init_ttf( void )
{

  if (TTF_Init()<0)
    {
      printf("Impossible d'initialiser la librairie SDL_ttf : %s\n", TTF_GetError());
      exit(-1);
    }
}
int charger_police( void ){
  //On charge  la police Ã  utiliser
  police = TTF_OpenFont ("ttf/DIGIT-LCD.ttf", 32);
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
  surface_niveau = TTF_RenderText_Solid (police,chaine_niveau , GREEN);
  if (surface_niveau)
    {
      rect.x = 170;
      rect.y = 350;
	
      SDL_BlitSurface (surface_niveau, NULL, ecran, &rect);
              
    }
}

void afficher_background (void)
{
  rect.x = rect.y = 0;
  rect.w = 640;
  rect.h = 480;

  SDL_BlitSurface( Background, &rect, ecran, &rect );
}

void afficher_timer (void) 
{
  char chaine_timer [5];
  int timer2 = 350;
  sprintf ( chaine_timer, "%d", timer2);
  surface_timer = TTF_RenderText_Solid ( police,chaine_timer , GREEN);
  if(surface_timer)
    {
      rect.x = 550;
      rect.y = 400;
      SDL_BlitSurface(surface_timer, NULL, ecran , &rect);
    }
}
 


  
void afficher_score(void){
  char chaine_score [5];
  int score = 350;
  sprintf ( chaine_score, "%d", score);
  surface_score = TTF_RenderText_Solid ( police,chaine_score , GREEN);
  if(surface_score){
    rect.x = 420;
    rect.y = 400;
    SDL_BlitSurface(surface_score, NULL, ecran , &rect);
  }
}

void afficher_best_score(void)
{ // a faire lire le melleur score dans un fichier  afficher  si score actuel sup mettre a jour l'enregistrement et l affichage
  char chaine_best_score [5];
  int best_score = 400;
  sprintf ( chaine_best_score, "%d", best_score);
  surface_best_score = TTF_RenderText_Solid ( police,chaine_best_score ,GREEN);
  if(surface_best_score){
    rect.x = 420;
    rect.y = 330;
    SDL_BlitSurface(surface_best_score, NULL, ecran , &rect);
  }
}

void afficher_nbr_ligne(void)
{
  char chaine_nbr_ligne [3];
  int nbr_ligne = 10;
  sprintf ( chaine_nbr_ligne, "%d", nbr_ligne);
  surface_nbr_ligne = TTF_RenderText_Solid ( police,chaine_nbr_ligne , GREEN);
  if(surface_nbr_ligne){
    rect.x = 170;
    rect.y = 400;
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

int hazard(void)
{
  return SDL_GetTicks()%7;
}

void creer_tetramino(void) 
{ 

  jeu_tetramino= future_tetramino;

  future_tetramino = (SDL_GetTicks()%7);



  switch (jeu_tetramino) 
    {
    case 0: 
      //Pièce constituée de 4 carrés placés en carré 
      //au milieu et en bas de la zone du haut 
      matrice_jeu[5][0] = jeu_tetramino + 1 ;
      matrice_jeu[5][1] = jeu_tetramino + 1 ;
      matrice_jeu[6][0] = jeu_tetramino + 1 ;
      matrice_jeu[6][1] = jeu_tetramino + 1;
      break; 

    case 1:
      //Pièce constituée de 4 carrés placés en ligne
	       
      matrice_jeu[4][0] = jeu_tetramino + 1;
      matrice_jeu[5][0] = jeu_tetramino + 1;
      matrice_jeu[6][0] = jeu_tetramino + 1 ;
      matrice_jeu[7][0] = jeu_tetramino + 1 ;
      break;

    case 2:
      //Pièce constituée de 4 carrés placés en T
	       
      matrice_jeu[5][0] = jeu_tetramino + 1 ;
      matrice_jeu[5][1] = jeu_tetramino + 1 ;
      matrice_jeu[5][2] = jeu_tetramino + 1 ;
      matrice_jeu[6][1] = jeu_tetramino + 1 ;
      break;

    case 3:
      //Pièce constituée de 4 carrés placés en l
	       
      matrice_jeu[5][0] = jeu_tetramino + 1 ;
      matrice_jeu[5][1] = jeu_tetramino + 1 ;
      matrice_jeu[5][2] = jeu_tetramino + 1 ;
      matrice_jeu[6][2] = jeu_tetramino + 1 ;
      break;
    case 4:
      //Pièce constituée de 4 carrés placés en l invere
	       
      matrice_jeu[6][0] = jeu_tetramino + 1 ;
      matrice_jeu[6][1] = jeu_tetramino + 1 ;
      matrice_jeu[6][2] = jeu_tetramino + 1 ;
      matrice_jeu[5][2] = jeu_tetramino + 1 ;
      break;


    case 5:
      //Pièce constituée de 4 carrés placés en S
	       
      matrice_jeu[5][1] = jeu_tetramino + 1 ;
      matrice_jeu[6][1] = jeu_tetramino + 1 ;
      matrice_jeu[6][0] = jeu_tetramino + 1 ;
      matrice_jeu[7][0] = jeu_tetramino + 1 ;
      break;
    case 6:
      //Pièce constituée de 4 carrés placés en S inverce
	       
      matrice_jeu[6][0] = jeu_tetramino + 1 ;
      matrice_jeu[7][0] = jeu_tetramino + 1 ;
      matrice_jeu[5][1] = jeu_tetramino + 1 ;
      matrice_jeu[6][1] = jeu_tetramino + 1 ;
      break;
      
    } 

  verif_tableau (12 ,23 ,matrice_jeu);

} 

void verif_tableau (int l ,int h ,int  matrice_jeu[l][h])
{



  int i = 0;
  int j = 0;

  for (i = 0; i < h; i++) {
    for(j = 0; j < l; j++) {
      printf("Tab[%d][%d] = %d\n", j, i, matrice_jeu[j][i]);
    }
  }



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

void creer_matrice_jeu(void)
{  
 


  //On initialise le tableau de jeu
  //Pour chaque ligne
  for (h=0;h<hauteur;h++)
    {
      //Pour chaque colonne
      for (l=0;l<largeur;l++)
	{
	  matrice_jeu[l][h] = 0;
	}
    }
  //On initialise la ligne du bas
  //Pour chaque colonne de la première ligne
  for (l=0;l<largeur;l++)
    {
      matrice_jeu[l][hauteur-1] = 255; 
    }
  //On initialise les deux colonnes sur les côtés
  for (h=0;h<hauteur;h++)
    {
      matrice_jeu[0][h] = 255;
      matrice_jeu[11][h] = 255;
    }
  // verif_tableau (12 ,23 ,matrice_jeu);
}


void afficher_futur_tetramino()
{
  int d_x = 400;
  int d_y = 50;
  int px = 16;
	
  switch (future_tetramino)
    {
    case 0:
      //Pièce constituée de 4 carrés placés en carré
      rect.x = d_x + (px*1);
      rect.y = d_y + (px*2);
      SDL_BlitSurface (tetramino[future_tetramino], NULL, ecran, &rect);
      rect.x = d_x + (px*2);
      rect.y = d_y + (px*2);
      SDL_BlitSurface (tetramino[future_tetramino], NULL, ecran, &rect);
      rect.x = d_x + (px*1);
      rect.y = d_y + (px*1);
      SDL_BlitSurface (tetramino[future_tetramino], NULL, ecran, &rect);
      rect.x = d_x + (px*2);
      rect.y = d_y + (px*1);
      SDL_BlitSurface (tetramino[future_tetramino], NULL, ecran, &rect);
      break;
    case 1:
      //Pièce constituée de 4 carrés placés en ligne droite
      rect.x = d_x + (px*1);
      rect.y = d_y + (px*2);
      SDL_BlitSurface (tetramino[future_tetramino], NULL, ecran, &rect);
      rect.x = d_x + (px*2);
      rect.y = d_y + (px*2);
      SDL_BlitSurface (tetramino[future_tetramino], NULL, ecran, &rect);
      rect.x = d_x + (px*3);
      rect.y = d_y + (px*2);
      SDL_BlitSurface (tetramino[future_tetramino], NULL, ecran, &rect);
      rect.x = d_x + (px*4);
      rect.y = d_y + (px*2);
      SDL_BlitSurface (tetramino[future_tetramino], NULL, ecran, &rect);
      break;
    case 2:
      //Pièce constituée de 4 carrés placés en L sur la gauche
      rect.x = d_x + (px*1);
      rect.y = d_y + (px*1);
      SDL_BlitSurface (tetramino[future_tetramino], NULL, ecran, &rect);
      rect.x = d_x + (px*1);
      rect.y = d_y + (px*2);
      SDL_BlitSurface (tetramino[future_tetramino], NULL, ecran, &rect);
      rect.x = d_x + (px*2);
      rect.y = d_y + (px*2);
      SDL_BlitSurface (tetramino[future_tetramino], NULL, ecran, &rect);
      rect.x = d_x + (px*3);
      rect.y = d_y + (px*2);
      SDL_BlitSurface (tetramino[future_tetramino], NULL, ecran, &rect);
      break;
    case 3:
      //Pièce constituée de 4 carrés placés en L sur la droite
      rect.x = d_x + (px*1);
      rect.y = d_y + (px*2);
      SDL_BlitSurface (tetramino[future_tetramino], NULL, ecran, &rect);
      rect.x = d_x + (px*2);
      rect.y = d_y + (px*2);
      SDL_BlitSurface (tetramino[future_tetramino], NULL, ecran, &rect);
      rect.x = d_x + (px*3);
      rect.y = d_y + (px*2);
      SDL_BlitSurface (tetramino[future_tetramino], NULL, ecran, &rect);
      rect.x = d_x + (px*3);
      rect.y = d_y + (px*1);
      SDL_BlitSurface (tetramino[future_tetramino], NULL, ecran, &rect);
      break;
    case 4:
      //Pièce constituée de 4 carrés placés en T
      rect.x = d_x + (px*1);
      rect.y = d_y + (px*2);
      SDL_BlitSurface (tetramino[future_tetramino], NULL, ecran, &rect);
      rect.x = d_x + (px*2);
      rect.y = d_y + (px*2);
      SDL_BlitSurface (tetramino[future_tetramino], NULL, ecran, &rect);
      rect.x = d_x + (px*2);
      rect.y = d_y + (px*1);
      SDL_BlitSurface (tetramino[future_tetramino], NULL, ecran, &rect);
      rect.x = d_x + (px*3);
      rect.y = d_y + (px*2);
      SDL_BlitSurface (tetramino[future_tetramino], NULL, ecran, &rect);
      break;
    case 5:
      //Pièce constituée de 4 carrés placés en S à plat
      rect.x = d_x + (px*1);
      rect.y = d_y + (px*2);
      SDL_BlitSurface (tetramino[future_tetramino], NULL, ecran, &rect);
      rect.x = d_x + (px*2);
      rect.y = d_y + (px*2);
      SDL_BlitSurface (tetramino[future_tetramino], NULL, ecran, &rect);
      rect.x = d_x + (px*2);
      rect.y = d_y + (px*1);
      SDL_BlitSurface (tetramino[future_tetramino], NULL, ecran, &rect);
      rect.x = d_x + (px*3);
      rect.y = d_y + (px*1);
      SDL_BlitSurface (tetramino[future_tetramino], NULL, ecran, &rect);
      break;
    case 6:
      //Pièce constituée de 4 carrés placés en S à plat dans l'autre sens
      rect.x = d_x + (px*1);
      rect.y = d_y + (px*1);
      SDL_BlitSurface (tetramino[future_tetramino], NULL, ecran, &rect);
      rect.x = d_x + (px*2);
      rect.y = d_y + (px*1);
      SDL_BlitSurface (tetramino[future_tetramino], NULL, ecran, &rect);
      rect.x = d_x + (px*2);
      rect.y = d_y + (px*2);
      SDL_BlitSurface (tetramino[future_tetramino], NULL, ecran, &rect);
      rect.x = d_x + (px*3);
      rect.y = d_y + (px*2);
      SDL_BlitSurface (tetramino[future_tetramino], NULL, ecran, &rect);
      break;
    default:
      break;
    }
}


void afficher_matrice_jeu ()
{
  int i;
  int j;
  int decalage_x = (640-(12*16))/2;
  int decalage_y = 10;
  int px = 16;
  rect.x = 0;
  rect.y = 0;

    for (i=0;i<hauteur-1;i++)
    {
         for (j=1;j<(largeur-1);j++)
	{
	 	  if (matrice_jeu[j][i] > 0)
	    {
	      rect.x = decalage_x + (px*j);
	      rect.y = decalage_y + (px*i);
						
	     
	      SDL_BlitSurface (tetramino[matrice_jeu[j][i]-1], NULL, ecran, &rect);
	    }
	}
    }
}

void chute_tetramino(void)
{ int i,j;
			for (i=-hauteur+1;i<0;i++)
		{
			for (j=1;j<(largeur-1);j++)
			{
							if (matrice_jeu[j][i] < 10 && matrice_jeu[j][i]>0)
				{
										matrice_jeu[j][i-1] = matrice_jeu[j][i];
								matrice_jeu[j][i] = 0;
				}
			}
		}}
