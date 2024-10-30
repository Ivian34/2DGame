#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Game.h"
#include <iostream>

#define FONT_SIZE 14
#define FONT_SIZE2 12

void Game::init()
{
    bPlay = true;
    glClearColor(0.4f, 0.7f, 7.0f, 1.0f);

    currentState = GameState::MENU;

    textRenderer = new TextRenderer("fonts/8bitBold.ttf", SCENE_WIDTH, SCENE_HEIGHT, FONT_SIZE);
    textRenderer2 = new TextRenderer("fonts/8bitBold.ttf", SCENE_WIDTH, SCENE_HEIGHT, FONT_SIZE2);
	//puedo agregar m�s fuentes si es necesario

    /* glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);*/

    menuScene.init(*textRenderer, *textRenderer2);
    scene.init(*textRenderer, "levels/levelPractice.txt"); //LEVEL1
	scene2.init(*textRenderer, "levels/levelFull.txt"); //LEVEL2
	instructionsScene.init(*textRenderer, menuScene.getShaderProgram());

}

bool Game::update(int deltaTime)
{
    if (currentState == GameState::MENU)
        menuScene.update(deltaTime);
    else if (currentState == GameState::LEVEL1)
        scene.update(deltaTime);

	else if (currentState == GameState::LEVEL2)
		scene2.update(deltaTime);

	else if (currentState == GameState::INSTRUCTIONS) {
		instructionsScene.update(deltaTime);
	}

    return bPlay;
}

void Game::render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (currentState == GameState::MENU)
        menuScene.render();

    else if (currentState == GameState::LEVEL1)
        scene.render();

    else if (currentState == GameState::LEVEL2)
        scene2.render();

    else if (currentState == GameState::INSTRUCTIONS)
		instructionsScene.render();

}


void Game::keyPressed(int key)
{
    keys[key] = true;

    // Verifica si se presion� ESC para salir del juego
    if (key == GLFW_KEY_ESCAPE)
        bPlay = false;

    if (currentState == GameState::MENU)
    {
        // Maneja la navegaci�n en el men�
        /*if (key == GLFW_KEY_ENTER)
        {
            beforeState = currentState;  // Guarda el estado actual antes de cambiar
            currentState = GameState::LEVEL1;
        }*/
        // Agrega m�s controles para el men� si es necesario
        if (key == GLFW_KEY_I)
        {
            beforeState = currentState;  // Guarda el estado actual antes de cambiar
            currentState = GameState::INSTRUCTIONS;
        }
    }
    else if (currentState == GameState::LEVEL2)
    {
        if (key == GLFW_KEY_I)
        {
            beforeState = currentState;  // Guarda el estado actual antes de cambiar
            currentState = GameState::INSTRUCTIONS;
        }
	}
    else if (currentState == GameState::LEVEL1) 
    {
        if (key == GLFW_KEY_I)
        {
            beforeState = currentState;  // Guarda el estado actual antes de cambiar
            currentState = GameState::INSTRUCTIONS;
        }
    }

    else if (currentState == GameState::INSTRUCTIONS)
    {
        if (key == GLFW_KEY_I)
        {
            // Regresa al estado anterior guardado en `beforeState`
            currentState = beforeState;
        }
        // Maneja otras entradas durante el men� de instrucciones
    }
    else if (currentState == GameState::GAMEOVER)
    {
        // Maneja la entrada durante la pantalla de Game Over
    }
}


void Game::keyReleased(int key)
{
	keys[key] = false;
}

void Game::mouseMove(int x, int y)
{
}

void Game::mousePress(int button)
{
}

void Game::mouseRelease(int button)
{
}

bool Game::getKey(int key) const
{
	return keys[key];
}

void Game::setState(GameState state)
{
	currentState = state;
}



