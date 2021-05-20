//One of the Component header files that transforms an entity into a button that can be clicked
#pragma once

#include "Components.h"
#include "Vector2D.h"

class ButtonComponent : public Component
{
public:
	//Variable that take's the entity's position variable from its TransformComponent
	//for use in this component
	Vector2D* position;
	//The height and width of the button (specified in this case due to the fact there is only
	//one button in the entire game.)
	int height = 64;
	int width = 64;
	//The variables used as the x and y positions of the mouse
	int x, y;
	//The variable that indicates if the button was pressed
	bool press = false;
	bool mouseDown = false;


	//Sets the position of the clickable rectangle for the button to the entity's position
	void init() override
	{
		position = &entity->getComponent<TransformComponent>().position;
	}

	//This update override waits for the left mouse button to be clicked and then 
	//checks to see if the mouse is hovering over the entity
	void update() override
	{
		//This override continuously keeps the button component at the same position as the entity
		position = &entity->getComponent<TransformComponent>().position;

		//This checks if the mouse button has been clicked
		if (Game::event.type == SDL_MOUSEBUTTONDOWN)
		{
			//Gets the mouse's x and y positions
			SDL_GetMouseState(&x, &y);
			//Checks if the mouse is over the entity
			if ((x > position->x) && (x < position->x + width))
				if ((y > position->y) && (y < position->y + height))
				{
					//And if it is, indicates that the mouse has been click on the button
					mouseDown = true;
				}
		}
		//Wait for the button to be unlicked before acitivating it
		if (Game::event.type == SDL_MOUSEBUTTONUP)
		{
			//Only works if the mouse was clicked on the button
			if (mouseDown)
			{
				//Gets the mouse's x and y positions
				SDL_GetMouseState(&x, &y);
				//Checks if the mouse is over the entity
				if ((x > position->x) && (x < position->x + width))
					if ((y > position->y) && (y < position->y + height))
					{
						//Changes the bool value to indicate the button has been pressed
						press = true;
					}
			}
			//Sets mouseDown to false to prevent constant button clicking
			mouseDown = false;
		}
	}
};