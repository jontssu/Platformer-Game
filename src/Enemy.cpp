#include "stdafx.h"
#include "Enemy.h"

Enemy::Enemy(float x, float y, sf::Texture* texture_sheet, bool vertical_movement, int minBound, int maxBound)
	: textureSheet(texture_sheet), sprite(*texture_sheet), verticalMovement(vertical_movement)
{
	alive = true;
	speed = 1.0f;
	direction = 1; // Start moving right
	velocity = {0.f, 0.f};
	
	if (verticalMovement)
	{
		topBound = y - minBound;    // Move minBound units up
		bottomBound = y + maxBound; // Move maxBound units down
	}
	else
	{
		leftBound = x - minBound;   // Move minBound units to the left
		rightBound = x + maxBound;  // Move maxBound units to the right
	}
	
	initSprite(x, y);
}

void Enemy::initSprite(float x, float y)
{
	// Set enemy texture (using a different part of the tile sheet)
	sprite.setTextureRect(sf::IntRect({64, 100}, {32, 32})); // Adjust coordinates as needed
	sprite.setPosition({x, y});
	sprite.setScale({1.f, 1.f});
}

const sf::Vector2f Enemy::getPosition() const
{
	return sprite.getPosition();
}

const sf::FloatRect Enemy::getGlobalBounds() const
{
	return sprite.getGlobalBounds();
}

void Enemy::updateMovement()
{
	if (!alive) return;

	if (verticalMovement)
	{
		velocity.y = speed * direction;
		// Check bounds and reverse direction
		sf::Vector2f currentPos = getPosition();
		if (currentPos.y <= topBound && direction == -1)
		{
			direction = 1; // Change to moving down
		}
		else if (currentPos.y >= bottomBound && direction == 1)
		{
			direction = -1; // Change to moving up
		}
	}
	else 
	{
		velocity.x = speed * direction;
		// Check bounds and reverse direction
		sf::Vector2f currentPos = getPosition();
		if (currentPos.x <= leftBound && direction == -1)
		{
			direction = 1; // Change to moving right
		}
		else if (currentPos.x >= rightBound && direction == 1)
		{
			direction = -1; // Change to moving left
		}
	}
	
	// Apply movement
	sprite.move(velocity);
}

void Enemy::update()
{
	if (alive)
		updateMovement();
}

void Enemy::takeDamage()
{
	alive = false;
}

void Enemy::render(sf::RenderTarget& target)
{
	if (alive)
	{
		target.draw(sprite);
		
		// Debug: draw collision box
		if (DEBUG_MODE)
		{
			sf::RectangleShape debugBox;
			sf::FloatRect bounds = sprite.getGlobalBounds();
			debugBox.setPosition(bounds.position);
			debugBox.setSize(bounds.size);
			debugBox.setFillColor(sf::Color::Transparent);
			debugBox.setOutlineThickness(1.f);
			debugBox.setOutlineColor(sf::Color::Blue);
			target.draw(debugBox);
		}
	}
}
