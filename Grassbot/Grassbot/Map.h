//Header file that holds the Map class and the constructors for its functions and variables
#pragma once
#include "GHeader.h"

class Map
{
public:
	//The constructor function for the map
	Map(int lvl[10][13]);
	//The deconstuctor function for the map
	~Map();

	//The function that updates the map's array
	void LoadMap(int arr[10][13]);
	//The function that draws the map based on the values in the array
	void DrawMap();

private:
	//The SDL rectangles used by the map
	SDL_Rect src, dest;
	//The textures used by the map
	SDL_Texture* dirt;
	SDL_Texture* grass;
	SDL_Texture* water;
	//The array used to draw the map on screen
	int map[10][13];
};

