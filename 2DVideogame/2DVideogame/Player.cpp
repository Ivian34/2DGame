#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include "Player.h"
#include "Game.h"
#include "Object.h"


#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 96
#define FALL_STEP 4


#define PLAYER_WIDTH 32
#define PLAYER_HEIGHT 40

//Trowing
#define THROW_COOLDOWN 1
#define THROW_VELOCITY 8

enum PlayerAnims
{
	STAND, MOVE, CROUCH, JUMP, FALL
};

enum PlayerCollisions
{
	OBJH, OBJD, ENEMY, NCOLLISIONS
};

void Player::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, ShaderProgram& hitboxShaderProgram)
{
	bJumping = false;
	spritesheet.loadFromFile("images/MM.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(PLAYER_WIDTH, PLAYER_HEIGHT), glm::vec2(float(PLAYER_WIDTH) / 488.f, float(PLAYER_HEIGHT) / 488.f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(5); // Solo STAND y MOVE

	// Animacin STAND
	sprite->setAnimationSpeed(STAND, 2);
	float ini = 2.f / 488.f;
	sprite->addKeyframe(STAND, glm::vec2(0.f  + ini, 0.f));
	sprite->addKeyframe(STAND, glm::vec2(32.f / 488.f + ini, 0.f));

	// Animacion MOVE
	sprite->setAnimationSpeed(MOVE, 6);
	sprite->addKeyframe(MOVE, glm::vec2(64.f / 488.f + ini, 0.f));
	sprite->addKeyframe(MOVE, glm::vec2(96.f / 488.f + ini, 0.f));
	sprite->addKeyframe(MOVE, glm::vec2(128.f / 488.f + ini, 0.f));
	sprite->addKeyframe(MOVE, glm::vec2(160.f / 488.f + ini, 0.f));
	sprite->addKeyframe(MOVE, glm::vec2(192.f / 488.f + ini, 0.f));
	sprite->addKeyframe(MOVE, glm::vec2(224.f / 488.f + ini, 0.f));

	// Animacion CROUCH
	sprite->setAnimationSpeed(CROUCH, 2);
	sprite->addKeyframe(CROUCH, glm::vec2(0.f + ini, 48.f / 488.f));
	sprite->addKeyframe(CROUCH, glm::vec2(33.f / 488.f + ini, 48.f / 488.f));

	//Jump animation
	sprite->setAnimationSpeed(JUMP, 6);
	sprite->addKeyframe(JUMP, glm::vec2(64.f / 488.f + ini, 48.f / 488.f));

	sprite->setAnimationSpeed(FALL, 6);
	sprite->addKeyframe(FALL, glm::vec2(96.f / 488.f + ini, 48.f / 488.f));



	sprite->changeAnimation(STAND);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));

	showHitbox = false;
	prevF1KeyState = false;

	// Inicializar la hitbox
	xhitboxPadding = 6;
	yhitboxPadding = 8;
	hitboxWidth = 20;
	hitboxHeight = 32;
	updateHitbox();

	// Crear la hitbox
	hitbox = new Hitbox(glm::vec2(posHitbox.x, posHitbox.y), glm::vec2(hitboxWidth, hitboxHeight), &hitboxShaderProgram);
}


void Player::update(int deltaTime)
{
	//Timers
	throwCooldown -= deltaTime / 1000.f;

	for (int i = 0; i < NCOLLISIONS; ++i) collisions[i] = false;
	lastInteractableObj = nullptr;

	sprite->update(deltaTime);
	if (Game::instance().getKey(GLFW_KEY_LEFT))
	{
		facingLeft = true;
		sprite->setScale(glm::vec2(-1.f, 1.f)); // Voltear horizontalmente
		if (sprite->animation() != MOVE)
		{
			xhitboxPadding = 6, yhitboxPadding = 8, hitboxWidth = 20, hitboxHeight = 32;
			updateHitbox();
			sprite->changeAnimation(MOVE);
		}
		translatePosition(glm::ivec2(-3, 0));
		if (map->collisionMoveLeft(posHitbox, glm::ivec2(hitboxWidth, hitboxHeight), &collisions[OBJH], lastInteractableObj))
		{
			translatePosition(glm::ivec2(3, 0));
			xhitboxPadding = 6, yhitboxPadding = 8, hitboxWidth = 20, hitboxHeight = 32;
			updateHitbox();
			sprite->changeAnimation(STAND);
		}
	}
	else if (Game::instance().getKey(GLFW_KEY_RIGHT))
	{
		facingLeft = false;
		sprite->setScale(glm::vec2(1.f, 1.f)); // Escala normal
		if (sprite->animation() != MOVE)
		{
			xhitboxPadding = 6, yhitboxPadding = 8, hitboxWidth = 20, hitboxHeight = 32;
			updateHitbox();
			sprite->changeAnimation(MOVE);
		}
		translatePosition(glm::ivec2(3, 0));
		if (map->collisionMoveRight(posHitbox, glm::ivec2(hitboxWidth, hitboxHeight), &collisions[OBJH], lastInteractableObj))
		{
			translatePosition(glm::ivec2(-3, 0));
			xhitboxPadding = 6, yhitboxPadding = 8, hitboxWidth = 20, hitboxHeight = 32;
			updateHitbox();
			sprite->changeAnimation(STAND);
		}
	}
	else if (Game::instance().getKey(GLFW_KEY_DOWN))
	{
		if (sprite->animation() != CROUCH) {
			
			xhitboxPadding = 6, yhitboxPadding = 20; hitboxWidth = 20, hitboxHeight = 20;
			updateHitbox();
			sprite->changeAnimation(CROUCH);
		}
	}
	else
	{
		if (sprite->animation() != STAND) {
			
			xhitboxPadding = 6, yhitboxPadding = 8, hitboxWidth = 20, hitboxHeight = 32;
			updateHitbox();
			sprite->changeAnimation(STAND);
		}


	}


	if (bJumping)
	{
		jumpAngle += JUMP_ANGLE_STEP;
		if (jumpAngle == 180)
		{
			bJumping = false;
			posPlayer.y = startY;
			updateHitbox();
		}
		else
		{
			posPlayer.y = int(startY - 96 * sin(3.14159f * jumpAngle / 180.f));
			updateHitbox();
			if (jumpAngle > 90)
			{ // Falling
				if (sprite->animation() != FALL) {
					xhitboxPadding = 6, yhitboxPadding = 8, hitboxWidth = 20, hitboxHeight = 32;
					updateHitbox();
					sprite->changeAnimation(FALL);
				}
					
				bJumping = !map->collisionMoveDown(posHitbox, glm::ivec2(hitboxWidth, hitboxHeight),PLAYER_HEIGHT, &posPlayer.y, &collisions[OBJD], lastInteractableObj);
				updateHitbox();
			}
			else
			{
				if (sprite->animation() != JUMP) {
					xhitboxPadding = 6, yhitboxPadding = 8, hitboxWidth = 20, hitboxHeight = 32;
					updateHitbox();
					sprite->changeAnimation(JUMP);
				}
					
			}
		}
	}
	else
	{
		translatePosition(glm::ivec2(0, FALL_STEP));
		bool condition = map->collisionMoveDown(posHitbox, glm::ivec2(hitboxWidth, hitboxHeight), PLAYER_HEIGHT ,&posPlayer.y, &collisions[OBJD], lastInteractableObj);
		updateHitbox();
		if (condition)
		{
			if (Game::instance().getKey(GLFW_KEY_UP))
			{
				bJumping = true;
				jumpAngle = 0;
				startY = posPlayer.y;
			}
			else if (Game::instance().getKey(GLFW_KEY_Z) && !carryObj) {
				if (lastInteractableObj != nullptr && collisions[OBJH] && lastInteractableObj->canBeMoved(posPlayer.y + PLAYER_HEIGHT)) {
					carryObj = true;
					currentCarryObj = lastInteractableObj;
					currentCarryObj->setPos(glm::vec2(posPlayer.x, posPlayer.y - currentCarryObj->getSize()));
					currentCarryObj->setHeld();
					throwCooldown = float(THROW_COOLDOWN);
				}
			}
		}
	}

	if (carryObj) {
		if (Game::instance().getKey(GLFW_KEY_Z) && throwCooldown < 0) {
			carryObj = false;
			currentCarryObj->setMoving();
			if (facingLeft) currentCarryObj->throwObject(glm::vec2(float(-THROW_VELOCITY), 0.f));
			else currentCarryObj->throwObject(glm::vec2(float(THROW_VELOCITY), 0.f));
			currentCarryObj = nullptr;
		}
		else currentCarryObj->setPos(glm::vec2(posPlayer.x, posPlayer.y - currentCarryObj->getSize()));
	}

	bool currentF1KeyState = Game::instance().getKey(GLFW_KEY_F1);
	if (currentF1KeyState && !prevF1KeyState) {
		showHitbox = !showHitbox; // Alternar la visualización de la hitbox
	}
	prevF1KeyState = currentF1KeyState;

	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
	updateHitbox();
}

void Player::render()
{
	sprite->render();
	if (showHitbox && hitbox != nullptr) {
		hitbox->render();
	}
}

void Player::setTileMap(TileMap* tileMap)
{
	map = tileMap;
}

void Player::setPosition(const glm::vec2& pos)
{
	posPlayer = pos;
	posHitbox = glm::ivec2( float(posPlayer.x + xhitboxPadding),float(posPlayer.y + yhitboxPadding));
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}

void Player::updateHitbox() {
	posHitbox = glm::ivec2(posPlayer.x + xhitboxPadding, posPlayer.y + yhitboxPadding);
	if (hitbox != nullptr) {
		hitbox->setPosition(glm::vec2(float(tileMapDispl.x + posHitbox.x), float(tileMapDispl.y + posHitbox.y)));
		hitbox->setSize(glm::vec2(hitboxWidth, hitboxHeight));
	}
}

void Player::translatePosition(const glm::vec2& t) {
	posPlayer += t;
	posHitbox += t;
}


glm::ivec2 Player::getPosition() {
	return posPlayer;
}


