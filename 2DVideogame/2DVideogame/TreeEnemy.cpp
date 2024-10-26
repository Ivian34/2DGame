#include "TreeEnemy.h"
#include "Player.h"
#include <iostream>

#define TREE_WIDTH 24
#define TREE_HEIGHT 32

#define SPEED 1
#define FALL_STEP 4

//spawn
#define SPAWN_BORDER 128
#define BORDER_SIZE 16
#define SPAWN_SIZE 128
#define DESPAWN_BORDER 384


enum TreeAnimations
{
	MOVE, DIE, NANIMS
};

void TreeEnemy::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram)
{
	spritesheet.loadFromFile("images/Tree.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(TREE_WIDTH, TREE_HEIGHT), glm::vec2(float(TREE_WIDTH) / 96.f, float(TREE_HEIGHT)/TREE_HEIGHT), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(NANIMS);

	sprite->setAnimationSpeed(MOVE, 5);
	sprite->addKeyframe(MOVE, glm::vec2(0.f, 0.f));
	sprite->addKeyframe(MOVE, glm::vec2(24.f / 96.f, 0.f));
	sprite->addKeyframe(MOVE, glm::vec2(48.f / 96.f, 0.f));

	// Animacion MOVE
	sprite->setAnimationSpeed(DIE, 0);
	sprite->addKeyframe(DIE, glm::vec2(72.f / 96.f, 0.f));

	sprite->changeAnimation(MOVE);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posTree.x), float(tileMapDispl.y + posTree.y)));

	facingLeft = false;

	showHitbox = false;
	prevF1KeyState = false;
}

void TreeEnemy::update(int deltaTime) {
	//Timers

	//Collisions

	//Sprite
	sprite->update(deltaTime);

	//Movement

	switch (enemyState) {
	case EnemyStates::SPAWN: updateSpawn(deltaTime); break;
	case EnemyStates::ATTACK: updateAttack(deltaTime); break;
	case EnemyStates::DIE: updateDie(deltaTime); break;
	}

	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posTree.x), float(tileMapDispl.y + posTree.y)));

}

void TreeEnemy::updateAttack(int deltaTime)
{
	Object* obj = nullptr;
	translatePosition(glm::ivec2(0, FALL_STEP));
	if (map->collisionMoveDown(posTree, glm::ivec2(TREE_WIDTH, TREE_HEIGHT), TREE_HEIGHT, &posTree.y, &collisions[0], obj)) {
		if (facingLeft) {
			translatePosition(glm::ivec2(-SPEED, 0));
			if (map->collisionMoveLeft(posTree, glm::ivec2(TREE_WIDTH, TREE_HEIGHT), &collisions[0], &posTree.x, obj))
			{
				facingLeft = false;
				sprite->setScale(glm::vec2(1.f, 1.f));
			}
		}
		else {
			translatePosition(glm::ivec2(SPEED, 0));
			if (map->collisionMoveRight(posTree, glm::ivec2(TREE_WIDTH, TREE_HEIGHT), &collisions[0], &posTree.x, obj))
			{
				facingLeft = true;
				sprite->setScale(glm::vec2(-1.f, 1.f));
			}
		}
	}
	obj = nullptr;

	glm::vec2 playerPos = player->getPosition();
	glm::vec2 distance = glm::vec2(abs(playerPos.x - posTree.x), abs(playerPos.y - posTree.y));
	if (distance.x >= DESPAWN_BORDER) {
		enemyState = EnemyStates::SPAWN;
		posTree = initPosTree;
	}
}

void TreeEnemy::updateDie(int deltaTime)
{
}

void TreeEnemy::updateSpawn(int deltaTime)
{
	glm::vec2 playerPos = player->getPosition();
	glm::vec2 distance = glm::vec2(abs(playerPos.x - posTree.x), abs(playerPos.y - posTree.y));
	if (distance.x <= SPAWN_BORDER - BORDER_SIZE && distance.y <= SPAWN_SIZE) playerInRange = true;
	else if ((distance.x <= SPAWN_BORDER) && (distance.y <= SPAWN_SIZE) && !playerInRange) {
		enemyState = EnemyStates::ATTACK;
		posTree = initPosTree;
		playerInRange = true;
	}
	else if (distance.x > SPAWN_BORDER) playerInRange = false;
}

void TreeEnemy::render()
{
	if (!isSpawn()) sprite->render();
}

void TreeEnemy::setTileMap(TileMap* tileMap)
{
	map = tileMap;
}

void TreeEnemy::setPlayer(Player * playerPtr)
{
	player = playerPtr;
}

void TreeEnemy::setPosition(const glm::vec2& pos)
{
	posTree = pos;
	initPosTree = posTree;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posTree.x), float(tileMapDispl.y + posTree.y)));
}

void TreeEnemy::setFacingLeft(bool faceLeft)
{
	facingLeft = faceLeft;
	if (faceLeft) sprite->setScale(glm::vec2(-1.f, 1.f));
}

bool TreeEnemy::isSpawn()
{
	return (enemyState == EnemyStates::SPAWN);
}

void TreeEnemy::translatePosition(const glm::vec2& t) {
	posTree += t;
}