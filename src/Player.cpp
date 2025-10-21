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
	canJump = true;
}

void Player::initPhysics()
{
	velocityMax = 5.f;
	velocityMin = 1.f;
	acceleration = 2.5f;
	drag = 0.80f;
	gravity = 2.5f;
	velocityMaxY = 40.f;
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
	sprite.setScale({1.5f, 1.5f});
	currentFrame = sf::IntRect({0, 0}, {64, 64});
	sprite.setTextureRect(currentFrame);
	
}

void Player::initAnimations()
{
	animationTimer.restart();
	animationSwitch = true;
}

void Player::updateMovement()
{
	animState = IDLE;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
	{
		move(1.f, 0.f);
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
	{
		move(-1.f, 0.f);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up) && canJump)
	{
		velocity.y = -70.f;
		canJump = false;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
	{
		move(0.f, 1.f);
		animState = FALLING;
	}
	if (velocity.x > 0.f)
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
			currentFrame.position.y = 1536;  // Third row for idle
			currentFrame.position.x += 64;
			if (currentFrame.position.x >= 128)  // 2 frames of animation
				currentFrame.position.x = 0;
			animationTimer.restart();
			sprite.setTextureRect(currentFrame);
		}

	}
	else if (animState == MOVING_RIGHT) // MOVING RIGHT Animation
	{
		if (animationTimer.getElapsedTime().asSeconds() >= 0.1f || getAnimSwitch())  // 0.15 seconds per frame
		{
			currentFrame.position.y = 704;  // Fourth row
			currentFrame.position.x += 64;
			if (currentFrame.position.x >= 512)  // 6 frames of animation
				currentFrame.position.x = 0;
			animationTimer.restart();
			sprite.setTextureRect(currentFrame);
			sprite.setScale({1.5f, 1.5f});
			sprite.setOrigin({0.f, 0.f});
		}
	}
	else if (animState == MOVING_LEFT) // MOVING LEFT Animation
	{
		if (animationTimer.getElapsedTime().asSeconds() >= 0.1f || getAnimSwitch())  // 0.15 seconds per frame
		{
			currentFrame.position.y = 704;  // Fourth row
			currentFrame.position.x += 64;
			if (currentFrame.position.x >= 512)  // 6 frames of animation
				currentFrame.position.x = 0;
			animationTimer.restart();
			sprite.setTextureRect(currentFrame);
			sprite.setScale({-1.5f, 1.5f});
			sprite.setOrigin({sprite.getGlobalBounds().size.x / 1.5f, 0.f});
		}
	}
		// else if (animState == JUMPING) // JUMPING Animation
		// {
		// 	currentFrame.position.y = 0;  // First row
		// 	currentFrame.position.x += 64;
		// 	if (currentFrame.position.x >= 384)  // 6 frames of animation
		// 		currentFrame.position.x = 0;
		// }
		// else if (animState == FALLING) // FALLING Animation
		// {
		// 	currentFrame.position.y = 128;  // Third row
		// 	currentFrame.position.x += 64;
		// 	if (currentFrame.position.x >= 384)  // 6 frames of animation
		// 		currentFrame.position.x = 0;
		// }

		// Reset timer only after advancing the frame
}

const bool& Player::getAnimSwitch()
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

void Player::setPosition(const float x, const float y)
{
	sprite.setPosition({x, y});
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

	//Limit velocitycty
	if (std::abs(velocity.x) > velocityMax)
		velocity.x = (velocity.x > 0) ? velocityMax : -velocityMax;
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
}

void Player::update()
{
	updateMovement();
	updateAnimations();
	updatePhysics();
	
	// Apply the current frame to the sprite
	sprite.setTextureRect(currentFrame);
}

void Player::render(sf::RenderTarget& target)
{
	target.draw(sprite);

	sf::CircleShape circ;

	circ.setRadius(5.f);
	circ.setFillColor(sf::Color::Red);
	circ.setPosition(getPosition());
	target.draw(circ);
}