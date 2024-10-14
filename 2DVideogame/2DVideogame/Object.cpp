#include "Object.h"


enum TileAnims
{
	BASE, DESTROY, NUM_ANIMS
};

void Object::init(const glm::vec2 &pos, const string &filename, ShaderProgram &shaderProgram, int tileSize, const glm::vec2 &spritesheetSize, const glm::vec2 &spritesheetDispl)
{
	posTile = pos;
	objSize = tileSize;
	spriteSheetSize = spritesheetSize;
	spriteDispl = spritesheetDispl;

	spritesheet.loadFromFile(filename, TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(tileSize), glm::vec2(float(tileSize)/(spritesheetSize.x*tileSize), float(tileSize) / (spritesheetSize.y*tileSize)), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(NUM_ANIMS); // BASE y DESTROY
	sprite->setPosition(glm::ivec2(posTile.x + spritesheetDispl.x, posTile.y + spritesheetDispl.y));
}


void Object::update(int deltaTime)
{
	sprite->update(deltaTime);
}

void Object::render()
{
	sprite->render();
}

/* void SpecialTile::setTileMap(TileMap * tileMap)
{
	map = tileMap;
}*/

void Object::setTexPosition(const glm::vec2 & texturePos)
{
	sprite->addKeyframe(BASE, glm::vec2(texturePos.x/spriteSheetSize.x, texturePos.y/spriteSheetSize.y));
	sprite->changeAnimation(BASE);
}

glm::ivec2 Object::getPosition() const {
	return posTile;
}

int Object::getSize() const {
	return objSize;
}

