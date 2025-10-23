#include "stdafx.h"
#include "Tile.h"

Tile::Tile(const unsigned& grid_pos_x, const unsigned& grid_pos_y, const unsigned& tile_size, 
	sf::Texture* texture_sheet, sf::IntRect texture_rect, 
	bool damaging, int tile_type)
	: sprite(*texture_sheet), damaging(damaging), tileType(tile_type)
{
	// Set different texture rectangles based on tile type
	int size = static_cast<int>(tile_size);
	isSpawned = true;
	switch (tileType) {
		case TILE_NORMAL:
			sprite.setTextureRect(sf::IntRect({32, 36}, {size, size})); // Normal block
			break;
		case TILE_POWERUP_EXTRA_BLOCK:
			sprite.setTextureRect(sf::IntRect({160, 36}, {size, size})); // Different texture for powerup block
			timer = 0;
			break;
		case TILE_POWERUP_JUMP_BOOST:
			sprite.setTextureRect(sf::IntRect({96, 36}, {size, size})); // Another texture for jump boost block
			break;
		case TILE_PLACEABLE:
			sprite.setTextureRect(sf::IntRect({224, 36}, {size, size})); 
			break;
		case TILE_DEADLY:
			sprite.setTextureRect(sf::IntRect({160, 100}, {size, size}));  
			break;
		case TILE_WIN:
			sprite.setTextureRect(sf::IntRect({32, 100}, {size, size}));  
			break;
		default:
			sprite.setTextureRect(texture_rect); // Use provided texture rect as fallback
			break;
	}
	
	sprite.setPosition({float(grid_pos_x * tile_size), float(grid_pos_y * tile_size)});

	body.setSize({float(tile_size), float(tile_size)});
	body.setPosition(sprite.getPosition());
}

const sf::FloatRect Tile::getGlobalBounds() const
{
	return sprite.getGlobalBounds();
}

void Tile::setDespawn()
{
	isSpawned = false;
}

void Tile::update()
{
	if (tileType == TILE_POWERUP_JUMP_BOOST)
	{
		timer++;
		if (timer % 240 == 0) {
			isSpawned = true;
		}
	}
}

void Tile::render(sf::RenderTarget& target)
{
	if (isSpawned)
		target.draw(sprite);
}
