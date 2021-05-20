//----------------------------------------------------------
//Name: Coleman Levy
//Date: 8/16/19
//Description: A game about filling in squares that focuses on level
//			   select, level loading, and the ability to change menus
//----------------------------------------------------------

//The cpp file that runs all of the events of the Game
#include "GHeader.h"
#include "TextureManager.h"
#include "Map.h"
#include "Components.h"
#include "Vector2D.h"

//The map used in the game
Map* map;

//The renderer and event handler used in the game
SDL_Renderer* Game::renderer = nullptr;
SDL_Event Game::event;

//The manager used in the game along with all of the entities used in the game. At this point,
//none of the entities have any components
Manager manager;
//Player character
auto& player(manager.addEntity());
//Title screen
auto& screen(manager.addEntity());

//All of the buttons used throughout the game
auto& newButton(manager.addEntity());
auto& nextButton(manager.addEntity());
auto& menuButton(manager.addEntity());
auto& pauseButton(manager.addEntity());
auto& resumeButton(manager.addEntity());
auto& selectButton(manager.addEntity());
auto& resetButton(manager.addEntity());

auto& level1Button(manager.addEntity());
auto& level2Button(manager.addEntity());
auto& level3Button(manager.addEntity());
auto& level4Button(manager.addEntity());
auto& level5Button(manager.addEntity());
auto& level6Button(manager.addEntity());

//The array that holds the design for each level from the files in the asset folders. It is later loaded
//into the map's LoadMap function and used in KeyControl
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
//Ints used for incrementing while reading from the level text files
int i, j = 0;
//Ints used to set the "total" number of dirt blocks that need to be changed into grass blocks
int x, total = 0;
//The int used to determine how many blocks the player has currently turned into grass blocks
int* playerTot;
//Int used to help load the next level when player completes the given level
int level = 1;
int levelsUnl = 1;
//A boolean value that indicates if the game has been started after the title screen
bool start = false;
//The string used to load the file path for each level. Defaults to level 1.
string path = "Assets/level1.txt";
//The stream used to read from the level text files
ifstream inFile;
ofstream outFile;

//A method that adds the components to all of the created entitiesin the game
void initializer()
{
	//Sets all of the components for the title screen and start button to give the
	//player the main menu of the game
	screen.addComponent<TransformComponent>();
	screen.getComponent<TransformComponent>().height = 640;
	screen.getComponent<TransformComponent>().width = 832;
	screen.addComponent<SpriteComponent>("Assets/TitleScreen.png");

	newButton.addComponent<TransformComponent>(420, 400);
	newButton.getComponent<TransformComponent>().height = 100;
	newButton.getComponent<TransformComponent>().width = 160;
	newButton.addComponent<SpriteComponent>("Assets/StartButt(Oth).png");
	newButton.addComponent<ButtonComponent>();
	newButton.getComponent<ButtonComponent>().height = 100;
	newButton.getComponent<ButtonComponent>().width = 160;

	//Creates the components for the player
	player.addComponent<TransformComponent>(-1000, -1000);
	player.addComponent<SpriteComponent>("Assets/GrindBot.png");
	player.addComponent<KeyControl>();
	playerTot = &player.getComponent<KeyControl>().playerTot;
	player.getComponent<KeyControl>().map = map;

	//Creates the components for the Next Button
	nextButton.addComponent<TransformComponent>(-1000, -1000);
	nextButton.getComponent<TransformComponent>().height = 100;
	nextButton.getComponent<TransformComponent>().width = 160;
	nextButton.addComponent<SpriteComponent>("Assets/NextButt(PG).png");
	nextButton.addComponent<ButtonComponent>();
	nextButton.getComponent<ButtonComponent>().height = 100;
	nextButton.getComponent<ButtonComponent>().width = 160;

	//Creates the components for the Main Menu Button
	menuButton.addComponent<TransformComponent>(-1000, -1000);
	menuButton.getComponent<TransformComponent>().height = 100;
	menuButton.getComponent<TransformComponent>().width = 160;
	menuButton.addComponent<SpriteComponent>("Assets/MainButt(PG).png");
	menuButton.addComponent<ButtonComponent>();
	menuButton.getComponent<ButtonComponent>().height = 100;
	menuButton.getComponent<ButtonComponent>().width = 160;

	//Creates the components for the Pause Button
	pauseButton.addComponent<TransformComponent>(-1000, -1000);
	pauseButton.getComponent<TransformComponent>().height = 64;
	pauseButton.getComponent<TransformComponent>().width = 128;
	pauseButton.addComponent<SpriteComponent>("Assets/PauseButt(PG).png");
	pauseButton.addComponent<ButtonComponent>();
	pauseButton.getComponent<ButtonComponent>().height = 64;
	pauseButton.getComponent<ButtonComponent>().width = 128;

	//Creates the components for the Resume Button
	resumeButton.addComponent<TransformComponent>(-1000, -1000);
	resumeButton.getComponent<TransformComponent>().height = 100;
	resumeButton.getComponent<TransformComponent>().width = 160;
	resumeButton.addComponent<SpriteComponent>("Assets/ResumeButt(PG).png");
	resumeButton.addComponent<ButtonComponent>();
	resumeButton.getComponent<ButtonComponent>().height = 100;
	resumeButton.getComponent<ButtonComponent>().width = 160;

	//Creates the components for the Level Select Menu Button
	selectButton.addComponent<TransformComponent>(620, 400);
	selectButton.getComponent<TransformComponent>().height = 100;
	selectButton.getComponent<TransformComponent>().width = 160;
	selectButton.addComponent<SpriteComponent>("Assets/LevelButt(PG).png");
	selectButton.addComponent<ButtonComponent>();
	selectButton.getComponent<ButtonComponent>().height = 100;
	selectButton.getComponent<ButtonComponent>().width = 160;

	//Creates the components for the Reset Button
	resetButton.addComponent<TransformComponent>(560, 550);
	resetButton.getComponent<TransformComponent>().height = 100;
	resetButton.getComponent<TransformComponent>().width = 160;
	resetButton.addComponent<SpriteComponent>("Assets/ResetButt(PG).png");
	resetButton.addComponent<ButtonComponent>();
	resetButton.getComponent<ButtonComponent>().height = 100;
	resetButton.getComponent<ButtonComponent>().width = 160;

	//Creates the components for all of the Level Select Buttons
	level1Button.addComponent<TransformComponent>(-1000, -1000);
	level1Button.getComponent<TransformComponent>().height = 100;
	level1Button.getComponent<TransformComponent>().width = 100;
	level1Button.addComponent<SpriteComponent>("Assets/Level1(PG).png");
	level1Button.addComponent<ButtonComponent>();
	level1Button.getComponent<ButtonComponent>().height = 100;
	level1Button.getComponent<ButtonComponent>().width = 100;

	level2Button.addComponent<TransformComponent>(-1000, -1000);
	level2Button.getComponent<TransformComponent>().height = 100;
	level2Button.getComponent<TransformComponent>().width = 100;
	level2Button.addComponent<SpriteComponent>("Assets/Level2(PG).png");
	level2Button.addComponent<ButtonComponent>();
	level2Button.getComponent<ButtonComponent>().height = 100;
	level2Button.getComponent<ButtonComponent>().width = 100;

	level3Button.addComponent<TransformComponent>(-1000, -1000);
	level3Button.getComponent<TransformComponent>().height = 100;
	level3Button.getComponent<TransformComponent>().width = 100;
	level3Button.addComponent<SpriteComponent>("Assets/Level3(PG).png");
	level3Button.addComponent<ButtonComponent>();
	level3Button.getComponent<ButtonComponent>().height = 100;
	level3Button.getComponent<ButtonComponent>().width = 100;

	level4Button.addComponent<TransformComponent>(-1000, -1000);
	level4Button.getComponent<TransformComponent>().height = 100;
	level4Button.getComponent<TransformComponent>().width = 100;
	level4Button.addComponent<SpriteComponent>("Assets/Level4(PG).png");
	level4Button.addComponent<ButtonComponent>();
	level4Button.getComponent<ButtonComponent>().height = 100;
	level4Button.getComponent<ButtonComponent>().width = 100;

	level5Button.addComponent<TransformComponent>(-1000, -1000);
	level5Button.getComponent<TransformComponent>().height = 100;
	level5Button.getComponent<TransformComponent>().width = 100;
	level5Button.addComponent<SpriteComponent>("Assets/Level5(PG).png");
	level5Button.addComponent<ButtonComponent>();
	level5Button.getComponent<ButtonComponent>().height = 100;
	level5Button.getComponent<ButtonComponent>().width = 100;

	level6Button.addComponent<TransformComponent>(-1000, -1000);
	level6Button.getComponent<TransformComponent>().height = 100;
	level6Button.getComponent<TransformComponent>().width = 100;
	level6Button.addComponent<SpriteComponent>("Assets/Level6(PG).png");
	level6Button.addComponent<ButtonComponent>();
	level6Button.getComponent<ButtonComponent>().height = 100;
	level6Button.getComponent<ButtonComponent>().width = 100;
}

//A small function that sets the position of the passed entity
void setPos(Entity* entity, int x, int y)
{
	entity->getComponent<TransformComponent>().position.x = x;
	entity->getComponent<TransformComponent>().position.y = y;
}

//A small function that sets all of the map to values that will make the map
//invisible. Helps keep map from being seen in certain menus.
void mapOff()
{
	for (int i = 0; i < 10; i++)
		for (int j = 0; j < 13; j++)
			//Goes through the map and changes all the tile values to 0 which doesn't
			//have a tile texture associated with it
			lvl[i][j] = 0;
}

//Function that loads the level design into the lvl array from the text files
void loadLevel(string path)
{
	//Resets increments and the total
	i = 0;
	j = 0;
	total = 0;
	//Opens the file at the given file path
	inFile.open(path);

	//Small if statement used for testing that indicates a level text file hasn't been found
	if (!inFile)
	{
		cout << "File not found" << endl;
	}

	//For each number in the text file, loops through and inserts the given number into the lvl array
	while (inFile >> x)
	{
		if (i == 9 && j == 13)
		{
			//At the end of the file, takes the final number and sets it to the total
			total = x;
		}
		else if (j == 13)
		{
			//Resets the column and enters the next row, then inserts number
			j = 0;
			i++;
			lvl[i][j] = x;
			j++;
		}
		else
		{
			//Inserts number at the given spot in the lvl array
			lvl[i][j] = x;
			j++;
		}
	}
	//Closes the stream upon completion
	inFile.close();
}

//Small function that loads the number of levels completed
void loadLocks()
{
	//Opens the save file
	inFile.open("Assets/Save.txt");
	//Sets the number of levels unlocked to be the one saved
	inFile >> levelsUnl;
	//Closes the save file
	inFile.close();
}

//Small function that saves the number of levels completed
void saveLock()
{
	//Opens the save file
	outFile.open("Assets/Save.txt");
	//Rewrites the number of levels completed
	outFile << levelsUnl;
	//Closes the save file
	outFile.close();
}

//Small function that sets KeyControl's lvl array to equal the lvl array in this cpp file.
void copyCat()
{
	for (int row = 0; row < 10; row++)
	{
		for (int column = 0; column < 13; column++)
		{
			player.getComponent<KeyControl>().lvl[row][column] = lvl[row][column];
		}
	}
	map->LoadMap(lvl);
	map->DrawMap();
}

//Final function that shows the ending screen for the player
void victory()
{
	//Removes the pause button from the screen
	setPos(&pauseButton, -1000, -1000);
	//Sets the position of the end screen buttons
	setPos(&newButton, 100, 100);
	setPos(&menuButton, 300, 100);
	setPos(&selectButton, 500, 100);
	//Removes the player character from the screen
	setPos(&player, -1000, -1000);
	//Makes playerTot to a number that cannot be reached to prevent bugs
	*playerTot = 100;
	//Sets the screen to the end screen texture and moves it into view
	screen.getComponent<SpriteComponent>().setTex("Assets/EndingScreen.png");
	setPos(&screen, 0, 0);
	//Turned off the player's ability to move the character to prevent glitches
	player.getComponent<KeyControl>().move = false;
	start = false;
}

//Function for setting up the pause screen
void pauseGame()
{
	//Changes the screen to be the pause menu
	screen.getComponent<SpriteComponent>().setTex("Assets/PausedScr(PG).png");
	//Prevents player movement to prevent glitches
	player.getComponent<KeyControl>().move = false;
	//Moves the pause button off screen
	setPos(&pauseButton, -1000, -1000);
	//Moves all the pause menu entities into place
	setPos(&resumeButton, 100, 100);
	setPos(&menuButton, 300, 100);
	setPos(&selectButton, 500, 100);
	setPos(&screen, 0, 0);
	//Sets the level to be uncompletable to prevent glitches
	start = false;
}

//Function for returning to the main menu
void mainMenu()
{
	//Sets the screen entity to be that of the title screen
	screen.getComponent<SpriteComponent>().setTex("Assets/TitleScreen.png");
	//Sets player movement to false to prevent glitches
	player.getComponent<KeyControl>().move = false;
	//Turns off map visuals so as to not have the map covering the main menu (just in case)
	mapOff();
	//Removes the player and the in game button from the screen
	setPos(&player, -1000, -1000);
	setPos(&menuButton, -1000, -1000);
	setPos(&resumeButton, -1000, -1000);
	//Moves all of the main menu assets into view
	setPos(&screen, 0, 0);
	setPos(&newButton, 420, 400);
	setPos(&selectButton, 620, 400);
	setPos(&resetButton, 560, 550);
	//Prevents the level from being winnable to prevent glitches
	start = false;
}

//Function used to increment each level and reset variables needed for each level
void nextLevel()
{
	//Removes any and all pause menu, main menu, and ending screen entities from view
	start = true;
	setPos(&screen, -1000, -1000);
	setPos(&newButton, -1000, -1000);
	setPos(&menuButton, -1000, -1000);
	setPos(&selectButton, -1000, -1000);
	setPos(&resetButton, -1000, -1000);
	setPos(&level1Button, -1000, -1000);
	setPos(&level2Button, -1000, -1000);
	setPos(&level3Button, -1000, -1000);
	setPos(&level4Button, -1000, -1000);
	setPos(&level5Button, -1000, -1000);
	setPos(&level6Button, -1000, -1000);
	//Sets path to the latest level text file
	path = "Assets/level" + to_string(level) + ".txt";
	//Loads the latest level into the lvl array
	loadLevel(path);
	//Copy's the array to the player's KeyControl array
	copyCat();
	//If the level that was just entered is higher than the previous highest unlocked level,
	//set the number of levels unlocked to the new highest level and saves the number of levels unlocked
	if (level > levelsUnl)
	{
		levelsUnl = level;
		saveLock();
	}
	//Resets the player's position on screen and in the KeyControl component for each level
	setPos(&player, 64, 64);
	setPos(&pauseButton, 0, 0);
	player.getComponent<KeyControl>().i = 1;
	player.getComponent<KeyControl>().j = 1;
	//Resets the player's number of grass blocks completed
	*playerTot = 0;
	//Resets the player's ability to move and sets mistake to be false
	player.getComponent<KeyControl>().move = true;
	player.getComponent<KeyControl>().mistake = false;
}

//The function that starts off the actual game after the "Start" button on the main menu
//has been pressed
void newGame()
{
	//Sets the start value to true, showing the gameplay has begun
	start = true;
	//Removes any and all pause menu, main menu, and ending screen entities from view
	setPos(&screen, -1000, -1000);
	setPos(&newButton, -1000, -1000);
	setPos(&menuButton, -1000, -1000);
	setPos(&selectButton, -1000, -1000);
	setPos(&resetButton, -1000, -1000);
	setPos(&level1Button, -1000, -1000);
	setPos(&level2Button, -1000, -1000);
	setPos(&level3Button, -1000, -1000);
	setPos(&level4Button, -1000, -1000);
	setPos(&level5Button, -1000, -1000);
	setPos(&level6Button, -1000, -1000);
	//Brings the pause button into view
	setPos(&pauseButton, 0, 0);
	//Sets the current level to be the first level
	level = 1;
	//Loads the first level
	path = "Assets/level" + to_string(1) + ".txt";
	loadLevel(path);
	//Copies it to the KeyControl map
	copyCat();
	//Resets the player's position in KeyControl
	player.getComponent<KeyControl>().i = 1;
	player.getComponent<KeyControl>().j = 1;
	//Moves the player onto the screen
	setPos(&player, 64, 64);
	//Resets the player's number of grass blocks completed
	*playerTot = 0;
	//Copies the lvl array in the file to the KeyControl component for the player
	player.getComponent<KeyControl>().move = true;
}

//The function that continuously checks to see if the player has beaten the level
//or made a mistake
void winCheck()
{
	//If the player is currently playing in a level...
	if (start)
	{
		//...check if the player has filled all tiles with grass...
		if (*playerTot == total)
		{
			//...if they have, increment the level the player is on...
			level++;
			if (level < 7)
				//...and then send the player to the next level...
				nextLevel();
			else
				//..or send the player to the end screen if the final level was completed.
				victory();
		}
		//If the player makes a mistake, resets the level
		if (player.getComponent<KeyControl>().mistake)
		{
			nextLevel();
		}
	}
}

//The function that occurs whenever the resume button is pressed
void resumeGame()
{
	//Removes all pause menu assets from the screen
	setPos(&screen, -1000, -1000);
	setPos(&menuButton, -1000, -1000);
	setPos(&selectButton, -1000, -1000);
	setPos(&resumeButton, -1000, -1000);
	//Puts the pause button back into view
	setPos(&pauseButton, 0, 0);
	//Resumes the game by giving control to the player and reactivating
	//win check
	player.getComponent<KeyControl>().move = true;
	start = true;
}

//Small function that occurs whenver a Level Select button is pressed
void selectLevel(int selected)
{
	//If the level clicked is lower or equal to the number of
	//levels unlocked...
	if (levelsUnl >= selected)
	{
		//Sets the current level to be the selected level and loads up said level
		level = selected;
		nextLevel();
	}
}

//The function that controls the visuals for the level select buttons whenever they
//appear on screen
void lockButtons(Entity* entity, int number)
{
	//If the number of levels unlocked is equal to or greater than the current button's
	//assigned level...
	if (levelsUnl >= number)
	{
		//Set the button's sprite to be it's normal form
		path = "Assets/level" + to_string(number) + "(PG).png";
		entity->getComponent<SpriteComponent>().setTex(&path[0]);
	}
	//Otherwise, set the button's sprite to it being locked
	else
	{
		entity->getComponent<SpriteComponent>().setTex("Assets/LevelLock(PG).png");
	}
}

//Function that occurs whenever the Level Select Menu Button is pressed
void levelSelect()
{
	//Sets the screen to be the pause menu screen
	screen.getComponent<SpriteComponent>().setTex("Assets/PausedScr(PG).png");
	//Removes any of the pause menu and main menu buttons from view
	setPos(&menuButton, -1000, -1000);
	setPos(&selectButton, -1000, -1000);
	setPos(&resumeButton, -1000, -1000);
	setPos(&newButton, -1000, -1000);
	setPos(&resetButton, -1000, -1000);
	//Brings all of the Level Select Buttons into view and sets their
	//sprites based on the number of levels the player currently has unlocked
	setPos(&level1Button, 150, 50);
	lockButtons(&level2Button, 2);
	setPos(&level2Button, 350, 50);
	lockButtons(&level3Button, 3);
	setPos(&level3Button, 550, 50);
	lockButtons(&level4Button, 4);
	setPos(&level4Button, 150, 200);
	lockButtons(&level5Button, 5);
	setPos(&level5Button, 350, 200);
	lockButtons(&level6Button, 6);
	setPos(&level6Button, 550, 200);
}

void buttonControl()
{
	//Wait until the player clicks the start button...
	if (newButton.getComponent<ButtonComponent>().press)
	{
		//...and then start the game.
		newGame();
		newButton.getComponent<ButtonComponent>().press = false;
	}

	if (menuButton.getComponent<ButtonComponent>().press)
	{
		//If the main menu button is pressed, the main menu is loaded up
		mainMenu();
		menuButton.getComponent<ButtonComponent>().press = false;
	}

	if (resumeButton.getComponent<ButtonComponent>().press)
	{
		//If the resume button is pressed, resumes the level being played
		resumeGame();
		resumeButton.getComponent<ButtonComponent>().press = false;
	}

	if (resetButton.getComponent<ButtonComponent>().press)
	{
		//If the reset button is pressed, the number of levels unlocked is reset to 1
		//and this is saved to the save file
		levelsUnl = 1;
		saveLock();
		resetButton.getComponent<ButtonComponent>().press = false;
	}

	if (pauseButton.getComponent<ButtonComponent>().press)
	{
		//If the pause button is pressed, set up the pause menu and pause the game
		pauseGame();
		pauseButton.getComponent<ButtonComponent>().press = false;
	}

	if (selectButton.getComponent<ButtonComponent>().press)
	{
		//If the level select menu button is clicked, open the level select menu
		levelSelect();
		selectButton.getComponent<ButtonComponent>().press = false;
	}

	if (level1Button.getComponent<ButtonComponent>().press)
	{
		//If the level 1 button is pressed and level 1 is unlocked, load level 1
		selectLevel(1);
		level1Button.getComponent<ButtonComponent>().press = false;
	}

	if (level2Button.getComponent<ButtonComponent>().press)
	{
		//If the level 2 button is pressed and level 2 is unlocked, load level 2
		selectLevel(2);
		level2Button.getComponent<ButtonComponent>().press = false;
	}

	if (level3Button.getComponent<ButtonComponent>().press)
	{
		//If the level 3 button is pressed and level 3 is unlocked, load level 3
		selectLevel(3);
		level3Button.getComponent<ButtonComponent>().press = false;
	}

	if (level4Button.getComponent<ButtonComponent>().press)
	{
		//If the level 4 button is pressed and level 4 is unlocked, load level 4
		selectLevel(4);
		level4Button.getComponent<ButtonComponent>().press = false;
	}

	if (level5Button.getComponent<ButtonComponent>().press)
	{
		//If the level 5 button is pressed and level 5 is unlocked, load level 5
		selectLevel(5);
		level5Button.getComponent<ButtonComponent>().press = false;
	}

	if (level6Button.getComponent<ButtonComponent>().press)
	{
		//If the level 6 button is pressed and level 6 is unlocked, load level 6
		selectLevel(6);
		level6Button.getComponent<ButtonComponent>().press = false;
	}
}

//Function that initializes the game to its earliest form
void Game::init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen)
{
	//Value used for the game's fullscreen mode. Currently unused.
	int flags = 0;
	//if (fullscreen)
	//	flags = SDL_WINDOW_FULLSCREEN;

	//Small function that initializes the window and draws the background onto it. Comes with
	//small messages to indicate the progress of the window's creation
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
	{
		cout << "Initialized..." << endl;
		//Creates the actual window
		window = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
		if (window)
			cout << "Window created." << endl;
		//Renders what is in the windown
		renderer = SDL_CreateRenderer(window, -1, 0);
		if (renderer)
		{
			//Sets the background of the windown to white
			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
			cout << "Renderer created." << endl;
		}
		//Finally, shows the game is operational by setting "isRunning" to true
		isRunning = true;
	}
	else
	{
		//If initialization fails, sets "isRunning" to false and instantly closes window
		isRunning = false;
	}
	
	//Creates a new map based on the lvl array
	map = new Map(lvl);
	//Initializes all the components for the entities
	initializer();
	//Auto-loads the number of levels unlocked at the start of the game
	loadLocks();
}

//Handles the events in the game
void Game::events()
{
	SDL_PollEvent(&event);

	switch (event.type)
	{
		//If the X button is clicked...
		case SDL_QUIT:
			//...close the game
			isRunning = false;
			break;

		default:
			break;
	}
}

//Function that runs every frame
void Game::update()
{
	//Refreshes the manager and performs any updates the manager has to perform (mostly
	//just for entity deletion)
	manager.refresh();
	manager.update();
	buttonControl();
	winCheck();
}

//Renders everything in the window every frame
void Game::render()
{
	//Deletes all graphics shown in the window
	SDL_RenderClear(renderer);
	//Renders the map if the gameplay has started
	if (start)
		map->DrawMap();
	//Renders the entities at their current position
	manager.draw();
	//Shows the rendered graphics in the window
	SDL_RenderPresent(renderer);
}

//Function that occurs when the game is closed (when "isRunning" = false)
void Game::clean()
{
	//Deletes window
	SDL_DestroyWindow(window);
	//Deletes the render for the window
	SDL_DestroyRenderer(renderer);
	//Quits the program
	SDL_Quit();
}