#pragma once

#include "stdafx.h"
#include "Collider.h"
#include <optional>

enum PLAYER_ANIMATION_STATES {
	IDLE = 0,
	MOVING_LEFT,
	MOVING_RIGHT,
	JUMPING,
	FALLING
};

class Player {
	public:
		Player();
		virtual ~Player() {}

		//Getters
		const bool getAnimSwitch();
		inline const bool getCanJump() const { return canJump; };
		const sf::Vector2f getPosition() const;
		const sf::FloatRect getGlobalBounds() const;
		const sf::Vector2f getVelocity() const;
		Collider getCollider();

		//Modifiers
		inline void setCanJump(bool can_jump) {canJump = can_jump;};
		void setPosition(const float x, const float y);
		void resetVelocityY();
	
		//Functions
		void resetAnimationTimer();
		void move(const float dirX, const float dirY);
		void jump();
		void updateMovement();
		void updateAnimations();
		void updatePhysics();
		void update();
		void render(sf::RenderTarget& target);

	private:
		sf::Texture textureSheet;
		sf::Sprite sprite;
		sf::RectangleShape colliderBody;
		bool isMoving;
		bool canJump;
		sf::Clock animationTimer;

		void initVariables();
		void initTexture();
		void initSprite();
		void initAnimations();
		void initPhysics();

		//Animation
		short animState;
		sf::IntRect currentFrame;
		bool animationSwitch;

		//Physics
		sf::Vector2f velocity;
		float velocityMax;
		float velocityMin;
		float acceleration;
		float drag;
		float gravity;
		float velocityMaxY;
};