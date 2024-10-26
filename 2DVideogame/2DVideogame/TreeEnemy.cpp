#include "TreeEnemy.h"
#include "Player.h"
#include <iostream>

#define TREE_WIDTH 24
#define TREE_HEIGHT 32

#define SPEED 1
#define FALL_STEP 4


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

	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posTree.x), float(tileMapDispl.y + posTree.y)));

}

void TreeEnemy::render()
{
	sprite->render();
}

void TreeEnemy::setTileMap(TileMap* tileMap)
{
	map = tileMap;
}

void TreeEnemy::setPlayer(Player * playerPtr)
{
	player = player;
}

void TreeEnemy::setPosition(const glm::vec2& pos)
{
	posTree = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posTree.x), float(tileMapDispl.y + posTree.y)));
}

void TreeEnemy::setFacingLeft(bool faceLeft)
{
	facingLeft = faceLeft;
	if (faceLeft) sprite->setScale(glm::vec2(-1.f, 1.f));
}

void TreeEnemy::translatePosition(const glm::vec2& t) {
	posTree += t;
}