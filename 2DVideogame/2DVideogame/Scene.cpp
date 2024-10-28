#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h"
#include "Game.h"


#define SCREEN_X 0
#define SCREEN_Y 0

#define INIT_PLAYER_X_TILES 8
#define INIT_PLAYER_Y_TILES 82

#define SCENE_WIDTH 352
#define SCENE_HEIGHT 198

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
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	initShaders();
	player = new Player();
	map = TileMap::createTileMap("levels/levelFull.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram, player);
	player->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, hitboxProgram);
	player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize(), INIT_PLAYER_Y_TILES * map->getTileSize()));
	player->setCheckpoint(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize(), INIT_PLAYER_Y_TILES * map->getTileSize()));
	player->setTileMap(map);
	camPosition = glm::vec2(0.0f, 0.0f);
	projection = glm::ortho(camPosition.x, float(SCENE_WIDTH) + camPosition.x, float(SCENE_HEIGHT) + camPosition.y, camPosition.y);
	currentTime = 0.0f;
	currentCamLevel = 0;
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
    glm::mat4 modelview2;

    texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);

	hitboxProgram.use();
    hitboxProgram.setUniformMatrix4f("projection", projection);

    texProgram.use();
    map->render();

	player->render();
}

void Scene::initShaders()
{
    Shader vShader, fShader;

    // Shader para texturas (el existente)
    vShader.initFromFile(VERTEX_SHADER, "shaders/texture.vert");
    if (!vShader.isCompiled())
    {
        cout << "Vertex Shader Error" << endl;
        cout << "" << vShader.log() << endl << endl;
    }
    fShader.initFromFile(FRAGMENT_SHADER, "shaders/texture.frag");
    if (!fShader.isCompiled())
    {
        cout << "Fragment Shader Error" << endl;
        cout << "" << fShader.log() << endl << endl;
    }
    texProgram.init();
    texProgram.addShader(vShader);
    texProgram.addShader(fShader);
    texProgram.link();
    if (!texProgram.isLinked())
    {
        cout << "Shader Linking Error" << endl;
        cout << "" << texProgram.log() << endl << endl;
    }
    texProgram.bindFragmentOutput("outColor");
    vShader.free();
    fShader.free();

    // Shader para la hitbox (nuevo)
    Shader hitboxVShader, hitboxFShader;

    hitboxVShader.initFromFile(VERTEX_SHADER, "shaders/simple.vert");
    if (!hitboxVShader.isCompiled())
    {
        cout << "Hitbox Vertex Shader Error" << endl;
        cout << "" << hitboxVShader.log() << endl << endl;
    }
    hitboxFShader.initFromFile(FRAGMENT_SHADER, "shaders/simple.frag");
    if (!hitboxFShader.isCompiled())
    {
        cout << "Hitbox Fragment Shader Error" << endl;
        cout << "" << hitboxFShader.log() << endl << endl;
    }
    hitboxProgram.init();
    hitboxProgram.addShader(hitboxVShader);
    hitboxProgram.addShader(hitboxFShader);
    hitboxProgram.link();
    if (!hitboxProgram.isLinked())
    {
        cout << "Hitbox Shader Linking Error" << endl;
        cout << "" << hitboxProgram.log() << endl << endl;
    }
    hitboxProgram.bindFragmentOutput("outColor");
    hitboxVShader.free();
    hitboxFShader.free();
}


void Scene::updateCamera()
{
	glm::ivec2 pos = player->getPosition();
	glm::vec2 mapSize = map->getSize();

	int level = getCameraLevel(pos);

	if (level != currentCamLevel && (pos.y + 64) < mapSize.y) {
		camPosition.y = level * 224 + 96;
		currentCamLevel = level;
	}

	if ((pos.x - camPosition.x) < (SCENE_WIDTH / 3) && camPosition.x > 0) {
		camPosition.x = pos.x - SCENE_WIDTH / 3;
		if (camPosition.x < 0) camPosition.x = 0;
	}
	if ((pos.x - camPosition.x) > 2*(SCENE_WIDTH / 3) && camPosition.x < (mapSize.x - SCENE_WIDTH)) {
		camPosition.x = pos.x - 2*(SCENE_WIDTH / 3);
		if (camPosition.x > (mapSize.x - SCENE_WIDTH)) camPosition.x = mapSize.x - SCENE_WIDTH;
	}
	projection = glm::ortho(camPosition.x, float(SCENE_WIDTH) + camPosition.x, float(SCENE_HEIGHT) + camPosition.y, camPosition.y);
}

int Scene::getCameraLevel(const glm::vec2 &pos)
{
	return (pos.y - 64) / 224;
}



