//Librairie standard
#include <stdio.h>
#include <stdlib.h>
//Pour utiliser la SDL
#include <SDL.h>
#include <SDL_ttf.h>
//#include <SDL_mixer.h>

//dÃ©finition des fonctions utilisÃ©es par la suite
int Init( void );
int Draw( void );
int Frame( void );
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
Uint32 chute_tetramino(Uint32 intervalle, void *param);
void chute_du_tetramino(void);
void nouveau_score (int ok);
void deplacementgauche();
void deplacementdroite();
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
//Le temps entre 2 mouvement du tetramino
int intervalle=1000; 

//Le timer pour la chute
SDL_TimerID timer_chute;

//The music that will be played
//Mix_Music *music = NULL;

/*//The sound effects that will be used
Mix_Chunk *scratch = NULL;
Mix_Chunk *high = NULL;
Mix_Chunk *med = NULL;
Mix_Chunk *low = NULL;*/
int fin_partie = 0;

//Le niveau de jeu
int niveau=1;
//Le score lui-même
int score=0;
int best_score = 400;


int main( int argc, char* argv[] )
{
  if ( Init() )
    {
      Frame();
    }
    
  return 0;
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
	  //Initialize SDL_mixer

	  /*-----------------------MUSIQUE--------------------*/

	  /* if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024) == -1) //Initialisation de l'API Mixer

	    {

	      printf("%s", Mix_GetError());

	    }

	   music = Mix_LoadMUS("musiques/tetris.ogx");
	   Mix_PlayMusic(music, -1);*/
          // une icone pour windows
	  SDL_WM_SetIcon(ico,NULL);
	  //On initialise la police du jeu
	  Init_ttf();
          //On charge la police du jeu
          charger_police();

            

	  future_tetramino=hazard();
   
	  return 1;}
      else
	//Mix_FreeMusic(music); 
	// Mix_CloseAudio(); //Fermeture de l'API
      SDL_Quit();
    }
  return 0;
}


int Frame( void )
{
  int continuer = 1; 
  

  Draw();
  SDL_EnableKeyRepeat(500,50);
  timer_chute = SDL_AddTimer(1000, chute_tetramino, NULL);	
  while(continuer)

    {


      SDL_WaitEvent(&evenements);


      switch(evenements.type) 


        {

	case SDL_QUIT:

	  continuer = 0;

	  break;


	  // En appuyant sur une touche, on quitte le programme

	case SDL_KEYDOWN :

	  switch (evenements.key.keysym.sym)
	    {
	    case SDLK_ESCAPE:
	      continuer = 0 ;
	      break;
	      
	    case SDLK_LEFT:
	      deplacementgauche();
	      break;

	    case SDLK_RIGHT:
	      deplacementdroite();
	      break;
	    case SDLK_DOWN:
	     chute_du_tetramino();
	     break;

        }
	}
      SDL_Flip(ecran);
    }

  return EXIT_SUCCESS;
}

void free_surface(void)
{
  SDL_FreeSurface(ico);
  SDL_FreeSurface(surface_niveau);

  SDL_FreeSurface(Background);
  SDL_FreeSurface(ecran);
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
      // en carré 
      matrice_jeu[5][0] = jeu_tetramino + 1 ;
      matrice_jeu[5][1] = jeu_tetramino + 1 ;
      matrice_jeu[6][0] = jeu_tetramino + 1 ;
      matrice_jeu[6][1] = jeu_tetramino + 1;
      break; 

    case 1:
      // en ligne
      matrice_jeu[4][0] = jeu_tetramino + 1;
      matrice_jeu[5][0] = jeu_tetramino + 1;
      matrice_jeu[6][0] = jeu_tetramino + 1 ;
      matrice_jeu[7][0] = jeu_tetramino + 1 ;
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
	       
      matrice_jeu[4][2] = jeu_tetramino + 1 ;
      matrice_jeu[5][2] = jeu_tetramino + 1 ;
      matrice_jeu[6][2] = jeu_tetramino + 1 ;
      matrice_jeu[6][1] = jeu_tetramino + 1 ;
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
  int px = 20;
	
  switch (future_tetramino)
    {
    case 0:
      //en carré
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
      //en ligne droite
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
      // en L
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
      //en _|
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
      //en T
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
      rect.y = d_y + (px*1);
      SDL_BlitSurface (tetramino[future_tetramino], NULL, ecran, &rect);
      break;
    case 5:
      // en _|-
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
      //en -|_
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
  int decalage_y = 20;
  int px = 20;
  rect.x = 0;
  rect.y = 0;

  // on recharge l image de fond
  SDL_BlitSurface (Background, NULL, ecran, &rect);

  for (i=0;i<hauteur-1;i++)
    {
      for (j=1;j<(largeur-1);j++)
	{
	  if (matrice_jeu[j][i] > 0)
	    {
	      rect.x = decalage_x + (px*j);
	      rect.y = decalage_y + (px*i);
	      if (matrice_jeu[j][i]>10)
		{
		  //Affichage d'un carré fixe
		  SDL_BlitSurface (tetramino[matrice_jeu[j][i]-51], NULL, ecran, &rect);
		}
	      else{					
	     
		SDL_BlitSurface (tetramino[matrice_jeu[j][i]-1], NULL, ecran, &rect);
	      }
	    }
	}
    }
  afficher_niveau ();
  afficher_timer();
  afficher_score();
  afficher_best_score();
  afficher_nbr_ligne();
  afficher_futur_tetramino();


  SDL_Flip (ecran);
}

void chute_du_tetramino(void)
{ int i,j;
  int chute_possible;
  chute_possible = 1;
  // tester si chute possible
  for (i=21;i>=0 && chute_possible;i--)
    {
	
      for (j=1;j<11 && chute_possible;j++)
	{
	  if (matrice_jeu[j][i] < 10 && matrice_jeu[j][i]>0 )
	    {
	      if (matrice_jeu[j][i] < matrice_jeu[j][i+1])
		{   
		  chute_possible = 0;
		}
	    }
	}
    }
  if (chute_possible){

    for (i=21;i>=0 && chute_possible ;i--)
      {
	for (j=1;j<11&& chute_possible;j++)
	  {if (matrice_jeu[j][i] < 10 && matrice_jeu[j][i]>0)
	      {
		matrice_jeu[j][i+1] = matrice_jeu[j][i];
		matrice_jeu[j][i] = 0;

	      }
	  }
      }
  }

  else
    {

      for (i=23;i>=0;i--)
	{
		
	  for (j=1;j<11;j++)
	    {
			
	      if (matrice_jeu[j][i] < 10 && matrice_jeu[j][i]>0)
		{
					
		  matrice_jeu[j][i] = matrice_jeu[j][i] + 50;
				
		  if (i<=2)
		    {
		      // a revoir
		      for (i=0;i<22;i++)
			for (j=1;j<11;j++)
			  matrice_jeu[j][i] = 0;
		      fin_partie = 0;
		    }
		}
	    }
	}
      creer_tetramino() ;
      nouveau_score (1);
    }
  verif_tableau (12 ,23 ,matrice_jeu);

  afficher_matrice_jeu();
}




Uint32 chute_tetramino (Uint32 intervalle, void *param)
{
  chute_du_tetramino();

  return intervalle;
}
void nouveau_score (int ok)
{
  switch (ok)
    {
    case 1 :
		
      score += niveau;
      break;

    default :
	
      break;
    }

}

void deplacementgauche(){

  int i,j;
  int gauche_possible;
  gauche_possible = 1;
  // tester si deplacement gauche est possible
 for (i=21;i>=0 && gauche_possible;i--)
    {
  for (j=1;j<11 && gauche_possible;j++)
    {
      if (matrice_jeu[j][i] < 10 && matrice_jeu[j][i]>0 )
	{
	  if (matrice_jeu[j][i] < matrice_jeu[j-1][i])
	    {  
	      // gauche_possible = 0;
	    }
	}
    }
    }

  if (gauche_possible){

    for (i=21;i>=0 && gauche_possible ;i--)
      {
	for (j=1;j<11&& gauche_possible;j++)
	  {
	    if (matrice_jeu[j][i] < 10 && matrice_jeu[j][i]>0)
	      {
		matrice_jeu[j-1][i] = matrice_jeu[j][i];
		matrice_jeu[j][i] = 0;

	      }
	  }
      }
  }

  verif_tableau (12 ,23 ,matrice_jeu);

  afficher_matrice_jeu();
}


void deplacementdroite(){

  int i,j;
  int droite_possible;
  droite_possible = 1;
  // tester si deplacement droite est possible
 for (i=21;i>=0 && droite_possible;i--)
    {
  for (j=1;j<11 && droite_possible;j++)
    {
      if (matrice_jeu[j][i] < 10 && matrice_jeu[j][i]>0 )
	{
	  if (matrice_jeu[j][i] < matrice_jeu[j][i])
	    {  
	      //  droite_possible = 0;
	    }
	}
    }
    }

  if (droite_possible){

    for (i=21;i>=0 && droite_possible ;i--)
      {
	for (j=11;j>=1 && droite_possible;j--)
	  {
	    if (matrice_jeu[j][i] < 10 && matrice_jeu[j][i]>0)
	      {
		matrice_jeu[j+1][i] = matrice_jeu[j][i];
		matrice_jeu[j][i] = 0;

	      }
	  }
      }
  }

  verif_tableau (12 ,23 ,matrice_jeu);

  afficher_matrice_jeu();
}


