#include "SpecialTile.h"


enum TileAnims
{
	BASE, DESTROY, NUM_ANIMS
};

SpecialTile::SpecialTile(const glm::vec2 &pos, const string &filename, ShaderProgram &shaderProgram, const glm::vec2 &tileSize, const glm::vec2 &sizeInSpritesheet)
{
	spritesheet.loadFromFile(filename, TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(tileSize, sizeInSpritesheet, &spritesheet, &shaderProgram);
	posTile = pos;
	sprite->setNumberAnimations(NUM_ANIMS); // BASE y DESTROY

	sprite->setPosition(posTile);
}


void SpecialTile::update(int deltaTime)
{
	sprite->update(deltaTime);
}

void SpecialTile::render()
{
	sprite->render();
}

void SpecialTile::setTileMap(TileMap * tileMap)
{
	map = tileMap;
}

void SpecialTile::setTexPosition(const glm::vec2 & texturePos)
{
	sprite->addKeyframe(BASE, glm::vec2(texturePos));
	sprite->changeAnimation(BASE);
}

SpecialTile::~SpecialTile()
{
}
