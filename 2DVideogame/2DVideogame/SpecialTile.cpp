#include "SpecialTile.h"


enum TileAnims
{
	BASE, DESTROY, NUM_ANIMS
};

void SpecialTile::init(const glm::vec2 &pos, const string &filename, ShaderProgram &shaderProgram, int tileSize, const glm::vec2 &spritesheetSize)
{
	spritesheet.loadFromFile(filename, TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(tileSize), glm::vec2(float(tileSize)/(spritesheetSize.x*tileSize), float(tileSize) / (spritesheetSize.y*tileSize)), &spritesheet, &shaderProgram);
	posTile = pos;
	sprite->setNumberAnimations(NUM_ANIMS); // BASE y DESTROY
	spriteSheetSize = spritesheetSize;
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

/* void SpecialTile::setTileMap(TileMap * tileMap)
{
	map = tileMap;
}*/

void SpecialTile::setTexPosition(const glm::vec2 & texturePos)
{
	sprite->addKeyframe(BASE, glm::vec2(texturePos.x/spriteSheetSize.x, texturePos.y/spriteSheetSize.y));
	sprite->changeAnimation(BASE);
}

