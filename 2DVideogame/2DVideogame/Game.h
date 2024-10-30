#ifndef _GAME_INCLUDE
#define _GAME_INCLUDE


#include <GLFW/glfw3.h>
#include "Scene.h"
#include "MenuScene.h"
#include "TextRenderer.h"
#include "InstructionsMenu.h"

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720


// Game is a singleton (a class with a single instance) that represents our whole application

enum class GameState { MENU, LEVEL1, LEVEL2, INSTRUCTIONS, GAMEOVER };

class Game
{

private:
	Game() {}
	
public:
	static Game& instance()
	{
		static Game G;

		return G;
	}
	
	void init();
	bool update(int deltaTime);
	void render();
	
	// Input callback methods
	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMove(int x, int y);
	void mousePress(int button);
	void mouseRelease(int button);
	void setState(GameState state);

	bool getKey(int key) const;

	void setMenu();

private:
	bool bPlay; // Continue to play game?
	bool keys[GLFW_KEY_LAST+1]; // Store key states so that 
							    // we can have access at any time
	GameState currentState;
	GameState beforeState;
	MenuScene menuScene;
	InstructionsMenu instructionsScene;
	Scene scene, scene2;
	TextRenderer* textRenderer, * textRenderer2;

};


#endif // _GAME_INCLUDE


