#ifndef _PLAYER_INCLUDE
#define _PLAYER_INCLUDE


#include "Sprite.h"
#include "TileMap.h"
#include "Hitbox.h"
#include <vector>


// Player is basically a Sprite that represents the player. As such it has
// all properties it needs to track its movement, jumping, and collisions.


class Player
{

public:
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, ShaderProgram& hitboxShaderProgram);
	void update(int deltaTime);
	void render();

	void setTileMap(TileMap* tileMap);
	void setPosition(const glm::vec2& pos);
	void translatePosition(const glm::vec2& t);
	void updateHitbox();
	glm::ivec2 getPosition();

private:
	bool bJumping;
	glm::ivec2 tileMapDispl, posPlayer, posHitbox;
	int jumpAngle, startY, xhitboxPadding = 6, yhitboxPadding = 0, hitboxWidth = 20, hitboxHeight = 40;
	float mov_acceleration_left, mov_acceleration_right;
	float horizontalSpeed;
	bool smashing;
	Texture spritesheet;
	Sprite* sprite;
	TileMap* map;
	Hitbox* hitbox;

	//Player states
	bool facingLeft;
	bool carryObj = false;
	bool showHitbox;
	bool prevF1KeyState;

	//Collisions
	bool collisions[2];
	Object* lastInteractableObj;
	Object* currentCarryObj;

	// Timers
	float throwCooldown = 0;
};


#endif // _PLAYER_INCLUDE


