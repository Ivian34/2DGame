#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h"
#include "Game.h"


#define SCREEN_X 64
#define SCREEN_Y 0

#define INIT_PLAYER_X_TILES 4
#define INIT_PLAYER_Y_TILES 6

#define SCENE_WIDTH 256
#define SCENE_HEIGHT 192

Scene::Scene()
{
	map = NULL;
	player = NULL;
	pendingCamUpdate = false;
}

Scene::~Scene()
{
	if(map != NULL)
		delete map;
	if(player != NULL)
		delete player;
}


void Scene::init()
{
	initShaders();
	map = TileMap::createTileMap("levels/levelPractice.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	player = new Player();
	player->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize(), INIT_PLAYER_Y_TILES * map->getTileSize()));
	player->setTileMap(map);
	camPosition = glm::vec2(0.0f, 0.0f);
	projection = glm::ortho(camPosition.x, float(SCENE_WIDTH) + camPosition.x, float(SCENE_HEIGHT) + camPosition.y, camPosition.y);
	currentTime = 0.0f;
}

void Scene::update(int deltaTime)
{
	currentTime += deltaTime;
	player->update(deltaTime);
	map->update(deltaTime);
	updateCamera();
}

void Scene::render()
{
	glm::mat4 modelview;

	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
	map->render();
	player->render();
}

void Scene::initShaders()
{
	Shader vShader, fShader;

	vShader.initFromFile(VERTEX_SHADER, "shaders/texture.vert");
	if(!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/texture.frag");
	if(!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	texProgram.init();
	texProgram.addShader(vShader);
	texProgram.addShader(fShader);
	texProgram.link();
	if(!texProgram.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << texProgram.log() << endl << endl;
	}
	texProgram.bindFragmentOutput("outColor");
	vShader.free();
	fShader.free();
}

void Scene::updateCamera()
{
	glm::ivec2 pos = player->getPosition();

	if ((pos.x - camPosition.x) < (SCENE_WIDTH / 3)) {
		camPosition.x = pos.x - SCENE_WIDTH / 3;
	}
	if ((pos.x - camPosition.x) > 2*(SCENE_WIDTH / 3)) {
		camPosition.x = pos.x - 2*(SCENE_WIDTH / 3);
	}
	projection = glm::ortho(camPosition.x, float(SCENE_WIDTH) + camPosition.x, float(SCENE_HEIGHT) + camPosition.y, camPosition.y);
}



