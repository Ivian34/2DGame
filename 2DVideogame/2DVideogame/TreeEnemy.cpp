#include "TreeEnemy.h"

#define TREE_WIDTH 24
#define TREE_HEIGHT 32



enum TreeAnimations
{
	MOVE, DIE, NANIMS
};

void TreeEnemy::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram)
{
	spritesheet.loadFromFile("images/Tree.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(TREE_WIDTH, TREE_HEIGHT), glm::vec2(float(TREE_WIDTH) / 96.f, float(TREE_WIDTH) / 96.f), &spritesheet, &shaderProgram);
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

	showHitbox = false;
	prevF1KeyState = false;
}

void TreeEnemy::update(int deltaTime) {
	//Timers

	//Collisions

	//Sprite
	sprite->update(deltaTime);


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

void TreeEnemy::setPosition(const glm::vec2& pos)
{
	posTree = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posTree.x), float(tileMapDispl.y + posTree.y)));
}