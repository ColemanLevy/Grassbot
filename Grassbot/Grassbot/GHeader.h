//The main header file that contains the Game class
#ifndef GHeader_h
#define GHeader_h

#include <iostream>
#include <fstream>
#include <stdio.h>
#include "SDL.h"
#include "SDL_image.h"
#undef main
using namespace std;

class Game
{
public:
	//The function that initializes the game
	void init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen);
	
	//The functions that repeat every frame
	void events();
	void update();
	void render();
	//The function meant for closing the game
	void clean();

	//The function that is constantly checked by the game to see if the game is still running
	bool running() { return isRunning; }

	//Variables that handle game events and rendering
	static SDL_Renderer* renderer;
	static SDL_Event event;

private:
	//The bool value that indicates the game is still running
	bool isRunning;
	//The window that shows the game being played
	SDL_Window *window;
};

#endif // !GHeader_h
