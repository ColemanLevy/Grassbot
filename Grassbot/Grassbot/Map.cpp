//The cpp file that uses the Map.h header to create the map on the screen
#include "Map.h"
#include "TextureManager.h"

//Variable used by the draw program to determine which texture will be drawn on each tile
int type;

//The function that constructs the map class 
Map::Map(int lvl[10][13])
{
	//Sets the textures for each of the three tiles used in the map
	dirt = TextureManager::loadTexture("Assets/Dirt.png");
	grass = TextureManager::loadTexture("Assets/Grass.png");
	water = TextureManager::loadTexture("Assets/Water.png");
	//Loads a default map for the map to use
	LoadMap(lvl);
	//Defines the orientation and how much of the png files is used for each of the textures
	src.x = 0;
	src.y = 0;
	src.h = 64;
	src.w = 64;
	//Defines the orientation and size of each texture used on each tile
	dest.x = 0;
	dest.y = 0;
	dest.w = 64;
	dest.h = 64;
}

//Function that destroys all tile textures on screen, leaving each tile blank
Map::~Map()
{
	SDL_DestroyTexture(grass);
	SDL_DestroyTexture(water);
	SDL_DestroyTexture(dirt);
}

//Updates the multidimensional array used by the map to draw the map on screen with its new values
void Map::LoadMap(int arr[10][13])
{
	//Increments down each row
	for (int row = 0; row < 10; row++)
	{
		//Increments down each column
		for (int column = 0; column < 13; column++)
		{
			//Updates the section of the arra with the value of the inputted array
			map[row][column] = arr[row][column];
		}
	}
}

//Draws the map to the screen using the values from the array
void Map::DrawMap()
{
	//Initializes the type value to 0
	type = 0;
	//Increments each row
	for (int row = 0; row < 10; row++)
	{
		//Increments each column
		for (int column = 0; column < 13; column++)
		{
			//Sets the tile type to the recorded value 
			type = map[row][column];
			//Sets the x axis position of the tile
			dest.x = column * 64;
			//Sets the y axis position of the tile
			dest.y = row * 64;
			//Draws each tile as a water, grass, or dirt tile based on their type number 
			//(Uses TextureManager's draw function as the tool to draw each tile)
			switch (type)
			{
			case 1:
				//Type 1 tiles are water
				TextureManager::Draw(water, src, dest);
				break;

			case 2:
				//Type 2 tiles are grass
				TextureManager::Draw(grass, src, dest);
				break;

			case 3:
				//Type 3 tiles are dirt
				TextureManager::Draw(dirt, src, dest);
				break;

			default:
				break;
			}
		}
	}
}