//Header file that contains the component that controls all of the player movement along with the changes
//to the map and recording the number of blocks changed
#pragma once

#include "Gheader.h"
#include "ECS.h"
#include "Components.h"
#include "Map.h"

class KeyControl : public Component
{
public:
	//The position of the entity that has the KeyControl component
	TransformComponent *transform;
	//The map of the game
	Map* map;
	//The number of squares the player has changed to grass
	int playerTot = 1;
	//Incremental integers used to show which tile on the map the player is standing on
	int i, j;
	bool move = false;
	bool mistake = false;
	//The array used by the component to control where the player can move,
	//and what each block is changed to
	int lvl[10][13] = {
		{ 0,0,0,0,0,0,0,0,0,0,0,0,0 },
		{ 0,0,0,0,0,0,0,0,0,0,0,0,0 },
		{ 0,0,0,0,0,0,0,0,0,0,0,0,0 },
		{ 0,0,0,0,0,0,0,0,0,0,0,0,0 },
		{ 0,0,0,0,0,0,0,0,0,0,0,0,0 },
		{ 0,0,0,0,0,0,0,0,0,0,0,0,0 },
		{ 0,0,0,0,0,0,0,0,0,0,0,0,0 },
		{ 0,0,0,0,0,0,0,0,0,0,0,0,0 },
		{ 0,0,0,0,0,0,0,0,0,0,0,0,0 },
		{ 0,0,0,0,0,0,0,0,0,0,0,0,0 }
	};

	//The function that initializes the component
	void init() override
	{
		//Sets the position to be equal to the entity's position
		transform = &entity->getComponent<TransformComponent>();
		//Sets the incremental integers
		i = 1;
		j = 1;
	}

	//Function that gives the player their movement and changes the map
	void update() override
	{
		if (move)
		{
			//Whenever a key is pressed...
			if (Game::event.type == SDL_KEYDOWN)
			{
				//...check which key is pressed.
				switch (Game::event.key.keysym.sym)
				{
				//If the w key was pressed...
				case SDLK_w:
					//...check if the tile above the player isn't water...
					if (lvl[i - 1][j] > 1)
					{
						//...then move the player up up one tile...
						transform->position.y -= 64;
						//...update where the player is in the array...
						i -= 1;
						//...then check if the player stepped on dirt or grass. 
						//If dirt, change it to grass and add to the number of grass tiles. 
						if (lvl[i][j] == 3)
						{
							//Change to grass
							lvl[i][j] = 2;
							//Add to number of grass blocks
							playerTot += 1;
						}
						//If grass, turn on the mistake boolean, which restarts the level
						else
						{
							mistake = true;
						}
						//Finally, reload and redraw the map
						map->LoadMap(lvl);
						map->DrawMap();
					}
					break;
					//If the key was a, perform the same actions as w, but move the character left one tile
				case SDLK_a:
					//Checking if the tile left of the player is water
					if (lvl[i][j - 1] > 1)
					{
						transform->position.x -= 64;
						j -= 1;
						//...then check if the player stepped on dirt or grass. 
						//If dirt, change it to grass and add to the number of grass tiles. 
						if (lvl[i][j] == 3)
						{
							//Change to grass
							lvl[i][j] = 2;
							//Add to number of grass blocks
							playerTot += 1;
						}
						//If grass, turn on the mistake boolean, which restarts the level
						else
						{
							mistake = true;
						}
						//Finally, reload and redraw the map
						map->LoadMap(lvl);
						map->DrawMap();
					}
					break;
					//If the key was d, perform the same actions as w, but move the character right one tile
				case SDLK_d:
					//Checking if the tile right of the player is water
					if (lvl[i][j + 1] > 1)
					{
						transform->position.x += 64;
						j += 1;
						//...then check if the player stepped on dirt or grass. 
						//If dirt, change it to grass and add to the number of grass tiles. 
						if (lvl[i][j] == 3)
						{
							//Change to grass
							lvl[i][j] = 2;
							//Add to number of grass blocks
							playerTot += 1;
						}
						//If grass, turn on the mistake boolean, which restarts the level
						else
						{
							mistake = true;
						}
						//Finally, reload and redraw the map
						map->LoadMap(lvl);
						map->DrawMap();
					}
					break;
					//If the key was s, perform the same actions as w, but move the character dowm one tile
				case SDLK_s:
					//Checking if the tile under the player is water
					if (lvl[i + 1][j] > 1)
					{
						transform->position.y += 64;
						i += 1;
						//...then check if the player stepped on dirt or grass. 
						//If dirt, change it to grass and add to the number of grass tiles. 
						if (lvl[i][j] == 3)
						{
							//Change to grass
							lvl[i][j] = 2;
							//Add to number of grass blocks
							playerTot += 1;
						}
						//If grass, turn on the mistake boolean, which restarts the level
						else
						{
							mistake = true;
						}
						//Finally, reload and redraw the map
						map->LoadMap(lvl);
						map->DrawMap();
					}
					break;

				//If none of these keys were pressed, does nothing
				default:
					break;
				}
			}
		}
	}
};