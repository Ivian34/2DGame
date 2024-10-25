#ifndef _TREE_ENEMY_INCLUDE
#define _TREE_ENEMY_INCLUDE


#include "Sprite.h"
#include "TileMap.h"
#include "Hitbox.h"
#include <vector>





enum class EnemyStates
{
	ATTACK, DIE, SPAWN, NSTATES
};

class TreeEnemy
{
public:
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram);
	void update(int deltaTime);
	void render();

	void setTileMap(TileMap* tileMap);
	void setPosition(const glm::vec2& pos);
private:
	glm::ivec2 tileMapDispl, posTree;
	Texture spritesheet;
	Sprite* sprite;
	TileMap* map;

	//Hitbox
	bool showHitbox;
	bool prevF1KeyState;
};

#endif // _TREE_ENEMY_INCLUD