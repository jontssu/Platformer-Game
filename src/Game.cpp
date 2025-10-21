#include "stdafx.h"
#include "Game.h"

Game::Game()
{
	initWindow();
	initInput();
	initTileSheet();
	initPlayer();
	initTileMap();
}

Game::~Game()
{
	delete player;
	delete tileMap;
}

const sf::RenderWindow &Game::getWindow() const
{
	return window;
}

void Game::initWindow()
{
	window.create(sf::VideoMode({800, 600}), "Platformer", sf::Style::Close | sf::Style::Titlebar);
	window.setFramerateLimit(60);
}

void Game::initInput()
{
	//Mouse bindigs
	mouseMappings["BTN_ADD_TILE"] = sf::Mouse::Button::Left;
	mouseMappings["BTN_REMOVE_TILE"] = sf::Mouse::Button::Right;

	//Keyboard bindings
	keyboardMappings["KEY_MOVE_LEFT"] = sf::Keyboard::Key::Left;
	keyboardMappings["KEY_MOVE_RIGHT"] = sf::Keyboard::Key::Right;
	keyboardMappings["KEY_JUMP"] = sf::Keyboard::Key::Up;
	keyboardMappings["KEY_CROUCH"] = sf::Keyboard::Key::Down;
}

void Game::initTileSheet()
{
	if (!tileSheet.loadFromFile("textures/blocksheet.png"))
	{
		std::cout << "ERROR::GAME::INITTILESHEET::FAILED TO LOAD TILESHEET" << "\n";
	}
}

void Game::initPlayer()
{
	player = new Player();
}
void Game::initTileMap()
{
	tileMap = new Tilemap(25, 19, &tileSheet, 32);
}

void Game::update()
{
	//Polling window events
	while (const std::optional event = window.pollEvent())
	{
		if (event->is<sf::Event::Closed>())
			window.close();
		else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
		{
			if (keyPressed->code == sf::Keyboard::Key::Escape)
				window.close();
		}
	}	

	updateInput();

	updatePlayer();

	updateCollision();

	updateTileMap();
}

void Game::renderPlayer()
{
	player->render(window);
}

void Game::renderTileMap()
{
	tileMap->render(window);
}

void Game::updateInput()
{
	//Update mouse positions
	std::cout << int(sf::Mouse::getPosition(window).x) / int(tileMap->getTileSize()) << ", " << int(sf::Mouse::getPosition(window).y) / int(tileMap->getTileSize()) << "\n";
	const int mouseX = sf::Mouse::getPosition(window).x / int(tileMap->getTileSize());
	const int mouseY = sf::Mouse::getPosition(window).y / int(tileMap->getTileSize());

	//Player movement
	if (sf::Keyboard::isKeyPressed(keyboardMappings["KEY_MOVE_LEFT"]))
	{
		player->move(-1.f, 0.f);
	}
	else if (sf::Keyboard::isKeyPressed(keyboardMappings["KEY_MOVE_RIGHT"]))
	{
		player->move(1.f, 0.f);
	}
	if (sf::Keyboard::isKeyPressed(keyboardMappings["KEY_JUMP"]) && player->getCanJump() == true)
	{
		player->jump();
	}
	else if (sf::Keyboard::isKeyPressed(keyboardMappings["KEY_CROUCH"]))
	{
		player->move(0.f, 1.f);
	}

	//Tile functions
	if (sf::Mouse::isButtonPressed(mouseMappings["BTN_ADD_TILE"]))
	{
		tileMap->addTile(mouseX, mouseY);
	}
	else if (sf::Mouse::isButtonPressed(mouseMappings["BTN_REMOVE_TILE"]))
	{
		tileMap->removeTile(mouseX, mouseY);
	}
}

void Game::updatePlayer()
{
	player->update();
}

void Game::updateCollision()
{
	//Collision with tiles
	for (const auto& tileRow : tileMap->getTiles())
	{
		for (const auto& tile : tileRow)
		{
			if (tile != nullptr)
			{
				// Manual collision detection without automatic movement
				sf::FloatRect playerBounds = player->getGlobalBounds();
				sf::FloatRect tileBounds = tile->getGlobalBounds();
				
				if (playerBounds.findIntersection(tileBounds))
				{
					//Handle collision
					sf::Vector2f playerPos = player->getPosition();
					sf::Vector2f playerSize = playerBounds.size;
					sf::Vector2f tilePos = tileBounds.position;
					sf::Vector2f tileSize = tileBounds.size;
					sf::Vector2f velocity = player->getVelocity();
					
					// Calculate overlap amounts
					float overlapX = std::min((playerPos.x + playerSize.x) - tilePos.x, 
					                         (tilePos.x + tileSize.x) - playerPos.x);
					float overlapY = std::min((playerPos.y + playerSize.y) - tilePos.y, 
					                         (tilePos.y + tileSize.y) - playerPos.y);
					
					// Only resolve if velocity is moving towards the tile to prevent jittering
					if (overlapX < overlapY)
					{
						// Horizontal collision
						if (velocity.x > 0 && playerPos.x < tilePos.x)
						{
							// Moving right, hitting left side of tile
							player->setPosition(tilePos.x - playerSize.x, playerPos.y);
						}
						else if (velocity.x < 0 && playerPos.x > tilePos.x)
						{
							// Moving left, hitting right side of tile
							player->setPosition(tilePos.x + tileSize.x, playerPos.y);
						}
					}
					else
					{
						// Vertical collision
						if (velocity.y > 0 && playerPos.y < tilePos.y)
						{
							// Falling down, landing on top of tile
							player->setCanJump(true);
							player->resetVelocityY();
							player->setPosition(playerPos.x, tilePos.y - playerSize.y);
						}
						else if (velocity.y < 0 && playerPos.y > tilePos.y)
						{
							// Moving up, hitting bottom of tile
							player->resetVelocityY();
							player->setPosition(playerPos.x, tilePos.y + tileSize.y);
						}
					}
				}
			}
		}
	}

	//Collision bottom of screen
	if (player->getPosition().y + player->getGlobalBounds().size.y >= window.getSize().y)
	{
		player->setCanJump(true);
		player->resetVelocityY();
		player->setPosition(
			player->getPosition().x,
			window.getSize().y - player->getGlobalBounds().size.y);
	}
	
	//Collision top of screen
	if (player->getPosition().y <= 0.f)
	{
		player->resetVelocityY();
		player->setPosition(
			player->getPosition().x,
			0.f);
	}

	//Collision left of screen
	if (player->getPosition().x <= 0.f)
	{
		player->setPosition(
			0.f,
			player->getPosition().y);
	}

	//Collision right of screen
	if (player->getPosition().x + player->getGlobalBounds().size.x >= window.getSize().x)
	{
		player->setPosition(
			window.getSize().x - player->getGlobalBounds().size.x,
			player->getPosition().y);
	}	
}

void Game::updateTileMap()
{
	tileMap->update();
}

void Game::render()
{
	window.clear();

	renderTileMap();
	renderPlayer();

	window.display();
}
