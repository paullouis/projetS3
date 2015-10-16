all: tetris


%: %.c
	gcc $< -o $@ `sdl-config --cflags --libs` -lm -lSDL_ttf 

