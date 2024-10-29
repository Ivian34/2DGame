#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Game.h"
#include <iostream>

void Game::init()
{
    bPlay = true;
    glClearColor(0.4f, 0.7f, 7.0f, 1.0f);

    currentState = GameState::MENU;

    menuScene.init();
    scene.init();
}

bool Game::update(int deltaTime)
{
    if (currentState == GameState::MENU)
        menuScene.update(deltaTime);
    else if (currentState == GameState::PLAYING)
        scene.update(deltaTime);

    return bPlay;
}

void Game::render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (currentState == GameState::MENU) {
        menuScene.render();
    }
       
    else if (currentState == GameState::PLAYING)
        scene.render();
}


void Game::keyPressed(int key)
{
    keys[key] = true;

    if (key == GLFW_KEY_ESCAPE)
        bPlay = false;

    if (currentState == GameState::MENU)
    {
        // Maneja la navegación del menú
        if (key == GLFW_KEY_ENTER)
            currentState = GameState::PLAYING;
        // Agrega más controles del menú según sea necesario
    }
    else if (currentState == GameState::PLAYING)
    {
        // Maneja la entrada durante el juego
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



