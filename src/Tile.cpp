#include "stdafx.h"
#include "Tile.h"

Tile::Tile(const unsigned& grid_pos_x, const unsigned& grid_pos_y, const unsigned& tile_size, 
	sf::Texture* texture_sheet, sf::IntRect texture_rect, 
	bool damaging)
	: sprite(*texture_sheet), damaging(damaging)
{
	sprite.setTextureRect(texture_rect);
	sprite.setPosition({float(grid_pos_x * tile_size), float(grid_pos_y * tile_size)});
}

const sf::FloatRect Tile::getGlobalBounds() const
{
	return sprite.getGlobalBounds();
}

void Tile::update()
{

}

void Tile::render(sf::RenderTarget& target)
{
	target.draw(sprite);
}
