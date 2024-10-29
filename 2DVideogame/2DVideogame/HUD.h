#ifndef _HUD_INCLUDE
#define _HUD_INCLUDE

#include "Sprite.h"
#include <vector>

class HUD
{
public:
	void init(ShaderProgram& shaderProgram, const glm::vec2 &pos);
	void render();

	void setPos(const glm::vec2 &pos);
	void setLife(int l);
	void setTries(int t);

private:
	glm::ivec2 posHUD;
	Texture spritesheet, hudSpritesheet;
	Sprite* sprite;
	Sprite* lifeSprite;
	Sprite* triesSprite;
};

#endif //_HUD_iNCLUDE