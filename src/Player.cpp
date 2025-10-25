#include "stdafx.h"
#include "Player.h"

Player::Player() : sprite(textureSheet)
{
	initVariables();
	initTexture();
	initSprite();
	initAnimations();
	initPhysics();
}

void Player::initVariables()
{
	animState = IDLE;
	isMoving = false;
	canJump = false;
	isCrouching = false;
	canStand = true;
	placeableBlocks = 50;
}

void Player::initPhysics()
{
	velocityMax = 5.f;
	velocityMin = 1.f;
	acceleration = 2.5f;
	drag = 0.80f;
	gravity = 2.5f;
	velocityMaxY = 30.f;
}

void Player::initTexture()
{
	if (!textureSheet.loadFromFile("textures/texturesheet.png"))
	{
		std::cout << "ERROR::PLAYER::INITTEXTURE::FAILED TO LOAD TEXTURE" << "\n";
	}
}

void Player::initSprite()
{
	sprite.setPosition({0.f, 0.f});
	sprite.setScale({PlayerScale, PlayerScale});
	currentFrame = sf::IntRect({0, 0}, {32, 48});
	sprite.setTextureRect(currentFrame);
	
	// Simple hitbox with fixed size
	colliderBody.setSize({PlayerHitboxWidth, PlayerHitboxHeight});
	colliderBody.setPosition(sprite.getPosition());
}

void Player::initAnimations()
{
	animationTimer.restart();
	animationSwitch = true;
}

void Player::updateMovement()
{
	if (isCrouching && velocity.x == 0.f)
		animState = CROUCHING;
	else if (isCrouching && velocity.x > 0.f)
		animState = CROUCHING_RIGHT;
	else if (isCrouching && velocity.x < 0.f)
		animState = CROUCHING_LEFT;
	else if (velocity.x > 0.f)
		animState = MOVING_RIGHT;
	else if (velocity.x < 0.f)
		animState = MOVING_LEFT;
	else
		animState = IDLE;
}

void Player::updateAnimations()
{
	if (animState == IDLE)
	{
		if (animationTimer.getElapsedTime().asSeconds() >= 0.2f || getAnimSwitch())  // 0.15 seconds per frame
		{
			currentFrame.position.y = 1552;  // Third row for idle
			currentFrame.position.x += 64;
			if (currentFrame.position.x >= 128)  // 2 frames of animation
				currentFrame.position.x = 16;
			currentFrame.size = {32, 48};    // Normal hitbox size
			animationTimer.restart();
			sprite.setTextureRect(currentFrame);
		}

	}
	else if (animState == MOVING_RIGHT) // MOVING RIGHT Animation
	{
		if (animationTimer.getElapsedTime().asSeconds() >= 0.1f || getAnimSwitch())  // 0.15 seconds per frame
		{
			currentFrame.position.y = 720;  // Fourth row
			currentFrame.position.x += 64;
			if (currentFrame.position.x >= 512)  // 6 frames of animation
				currentFrame.position.x = 16;
			currentFrame.size = {32, 48};    // Normal hitbox size
			animationTimer.restart();
			sprite.setTextureRect(currentFrame);
			sprite.setScale({PlayerScale, PlayerScale});
			sprite.setOrigin({0.f, 0.f});
		}
	}
	else if (animState == MOVING_LEFT) // MOVING LEFT Animation
	{
		if (animationTimer.getElapsedTime().asSeconds() >= 0.1f || getAnimSwitch())  // 0.15 seconds per frame
		{
			currentFrame.position.y = 720;  // Fourth row
			currentFrame.position.x += 64;
			if (currentFrame.position.x >= 512)  // 6 frames of animation
				currentFrame.position.x = 16;
			currentFrame.size = {32, 48};    // Normal hitbox size
			animationTimer.restart();
			sprite.setTextureRect(currentFrame);
			sprite.setScale({-PlayerScale, PlayerScale});
			sprite.setOrigin({sprite.getGlobalBounds().size.x / PlayerScale, 0.f});
		}
	}
	else if (animState == CROUCHING) // CROUCHING Animation (stationary)
	{
		if (animationTimer.getElapsedTime().asSeconds() >= 0.1f || getAnimSwitch())
		{
			currentFrame.position.y = 2080;  // Crouch row
			currentFrame.position.x = 80;    // Single frame for stationary crouch
			currentFrame.size = {32, 32};    // Smaller hitbox for crouching
			animationTimer.restart();
			sprite.setTextureRect(currentFrame);
			sprite.setScale({PlayerScale, PlayerScale});  // Normal scale
			sprite.setOrigin({0.f, 0.f});                 // Normal origin
		}
	}
	else if (animState == CROUCHING_RIGHT) // CROUCHING RIGHT Animation
	{
		if (animationTimer.getElapsedTime().asSeconds() >= 0.1f || getAnimSwitch())
		{
			currentFrame.position.y = 2016;  // Crouch movement row
			currentFrame.position.x += 64;
			if (currentFrame.position.x >= 144)  // Animation frames
				currentFrame.position.x = 16;
			currentFrame.size = {32, 32};    // Smaller hitbox for crouching
			animationTimer.restart();
			sprite.setTextureRect(currentFrame);
			sprite.setScale({-PlayerScale, PlayerScale}); // Flipped scale (facing right)
			sprite.setOrigin({sprite.getGlobalBounds().size.x / PlayerScale, 0.f}); // Adjusted origin for flip
		}
	}
	else if (animState == CROUCHING_LEFT) // CROUCHING LEFT Animation
	{
		if (animationTimer.getElapsedTime().asSeconds() >= 0.1f || getAnimSwitch())
		{
			currentFrame.position.y = 2016;  // Crouch movement row
			currentFrame.position.x += 64;
			if (currentFrame.position.x >= 144)  // Animation frames
				currentFrame.position.x = 16;
			currentFrame.size = {32, 32};    // Smaller hitbox for crouching
			animationTimer.restart();
			sprite.setTextureRect(currentFrame);
			sprite.setScale({PlayerScale, PlayerScale});  // Normal scale (facing left)
			sprite.setOrigin({0.f, 0.f});                 // Normal origin
		}
	}
}

const bool Player::getAnimSwitch()
{
	bool anim_switch = animationSwitch;

	if (animationSwitch)
		animationSwitch = false;

	return anim_switch;
}

const sf::Vector2f Player::getPosition() const
{
	return sprite.getPosition();
}

const sf::FloatRect Player::getGlobalBounds() const
{
	return sprite.getGlobalBounds();
}

const sf::Vector2f Player::getVelocity() const
{
	return velocity;
}

Collider Player::getCollider()
{
	return Collider(colliderBody);
}

void Player::setPosition(const float x, const float y)
{
	sprite.setPosition({x, y});
	colliderBody.setPosition({x, y});
}

void Player::resetVelocityY()
{
	velocity.y = 0.f;
}

void Player::resetAnimationTimer()
{
	animationTimer.restart();
	animationSwitch = true;
}

void Player::move(const float dirX, const float dirY)
{
	//Acceleration
	velocity.x += dirX * acceleration;

	//Limit velocity
	if (std::abs(velocity.x) > velocityMax)
		velocity.x = (velocity.x > 0) ? velocityMax : -velocityMax;
}

void Player::jump()
{
	velocity.y = -50.f;
	canJump = false;
}

void Player::updatePhysics()
{
	//Gravity
	velocity.y += 1.0 * gravity;
	if (std::abs(velocity.y) > velocityMaxY)
		velocity.y = (velocity.y > 0) ? velocityMaxY : -velocityMaxY;

	//Deceleration
	velocity *= drag;

	//Limit deceleration
	if (std::abs(velocity.x) < velocityMin)
		velocity.x = 0.f;
	if (std::abs(velocity.y) < velocityMin)
		velocity.y = 0.f;

	if (std::abs(velocity.x) <= 1.f)
		velocity.x = 0.f;
	sprite.move(velocity);
	colliderBody.move(velocity); // Keep collider in sync with sprite
}

void Player::update()
{
	updateMovement();
	updateAnimations();
	updatePhysics();
	
	// Apply the current frame to the sprite
	sprite.setTextureRect(currentFrame);
	
	// Update hitbox size in case constants changed
	colliderBody.setSize({PlayerHitboxWidth, PlayerHitboxHeight});
}

void Player::render(sf::RenderTarget& target)
{
	target.draw(sprite);

	sf::RectangleShape hitbox;
	hitbox.setPosition(this->getPosition());  // Player's world position
	hitbox.setSize({(float)currentFrame.size.x * PlayerScale, (float)currentFrame.size.y * PlayerScale});  // Frame size with scale
	hitbox.setFillColor(sf::Color::Transparent);
	hitbox.setOutlineThickness(1.f);
	hitbox.setOutlineColor(sf::Color::Red);
	target.draw(hitbox);
}