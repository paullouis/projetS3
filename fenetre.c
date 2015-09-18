
#include <stdlib.h>

#include <stdio.h>

#include <SDL/SDL.h>

 

void pause();

 int main(int argc, char *argv[])
{
  SDL_Surface *ecran = NULL; 
    SDL_Init(SDL_INIT_VIDEO);
    ecran = SDL_SetVideoMode(640, 480, 32, SDL_HWSURFACE);
    if (ecran == NULL)
    {
        fprintf(stderr, "Impossible de charger le mode vidéo : %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    SDL_WM_SetCaption("Tetris !", NULL);
    pause();
    SDL_Quit();
    return EXIT_SUCCESS;
}
 
void pause()

{

  int continuer = 1;
  SDL_Event event;
  while (continuer)

    {
      SDL_WaitEvent(&event);
      switch(event.type)
	{
	case SDL_QUIT:
	  continuer = 0;

        }

    }

}
