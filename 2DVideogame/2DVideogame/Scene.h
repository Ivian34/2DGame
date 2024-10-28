#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "TileMap.h"
#include "Player.h"



// Scene contains all the entities of our game.
// It is responsible for updating and render them.


class Scene
{

public:
	Scene();
	~Scene();

	void init();
	void update(int deltaTime);
	void render();

private:
	void initShaders();
	void updateCamera();
	int getCameraLevel(const glm::vec2 &pos);

private:
	TileMap *map;
	Player *player;
	ShaderProgram texProgram, hitboxProgram;
	float currentTime;
	glm::mat4 projection;
	glm::vec2 camPosition;

	bool pendingCamUpdate;

	int currentCamLevel;
};


#endif // _SCENE_INCLUDE

