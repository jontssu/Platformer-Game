#include "stdafx.h"
#include "Game.h"

Game::Game()
{
	currentState = GameState::MENU; // Start in menu state
	initWindow();
	initInput();
	initTileSheet();
	initFont();
	// Don't initialize player and tilemap until game starts
	player = nullptr;
	tileMap = nullptr;
	currentLevel = 1;
}

Game::~Game()
{
	if (player != nullptr)
		delete player;
	if (tileMap != nullptr)
		delete tileMap;
	
	// Clean up enemies
	for (auto* enemy : enemies)
	{
		delete enemy;
	}
	enemies.clear();
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
	keyboardMappings["KEY_MOVE_LEFT2"] = sf::Keyboard::Key::A;
	keyboardMappings["KEY_MOVE_RIGHT2"] = sf::Keyboard::Key::D;
	keyboardMappings["KEY_JUMP2"] = sf::Keyboard::Key::W;
	keyboardMappings["KEY_CROUCH2"] = sf::Keyboard::Key::S;
	keyboardMappings["KEY_DELETE"] = sf::Keyboard::Key::X;
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
	tileMap = new Tilemap(25, 19, &tileSheet, 32, currentLevel);
}

void Game::initEnemies()
{
	// Clear existing enemies
	for (auto* enemy : enemies)
	{
		delete enemy;
	}
	enemies.clear();

	enemies.push_back(new Enemy(7 * 32.f, 1 * 32.f, &tileSheet, static_cast<bool>(EnemyMovementType::VERTICAL), 0 * 32.f, 15 * 32.f));
	enemies.push_back(new Enemy(8 * 32.f, 1 * 32.f, &tileSheet, static_cast<bool>(EnemyMovementType::VERTICAL), 0 * 32.f, 15 * 32.f));

	enemies.push_back(new Enemy(3 * 32.f, 2 * 32.f, &tileSheet, static_cast<bool>(EnemyMovementType::HORIZONTAL), 1 * 32.f, 2 * 32.f));
	enemies.push_back(new Enemy(4 * 32.f, 4 * 32.f, &tileSheet, static_cast<bool>(EnemyMovementType::HORIZONTAL), 2 * 32.f, 1 * 32.f));
	enemies.push_back(new Enemy(5 * 32.f, 6 * 32.f, &tileSheet, static_cast<bool>(EnemyMovementType::HORIZONTAL), 3 * 32.f, 0 * 32.f));


	enemies.push_back(new Enemy(17 * 32.f, 2 * 32.f, &tileSheet, static_cast<bool>(EnemyMovementType::VERTICAL), 0 * 32.f, 12 * 32.f));
	enemies.push_back(new Enemy(20 * 32.f, 10 * 32.f, &tileSheet, static_cast<bool>(EnemyMovementType::VERTICAL), 8 * 32.f, 4 * 32.f));
	enemies.push_back(new Enemy(11 * 32.f, 14 * 32.f, &tileSheet, static_cast<bool>(EnemyMovementType::HORIZONTAL), 0 * 32.f, 11 * 32.f));
}

void Game::initFont()
{
	if (!font.openFromFile("/System/Library/Fonts/Helvetica.ttc"))
		std::cout << "ERROR::GAME::INITFONT::FAILED TO LOAD FONT" << "\n";
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
			{
				if (currentState == GameState::PLAYING)
					currentState = GameState::MENU;
				else
					window.close();
			}
			else if (currentState == GameState::MENU || currentState == GameState::FINISHED || currentState == GameState::DIED)
			{
				if (keyPressed->code == sf::Keyboard::Key::Enter || keyPressed->code == sf::Keyboard::Key::Space)
				{
					// Start the game
					currentState = GameState::PLAYING;
					if (player == nullptr)
						initPlayer();
					if (tileMap == nullptr)
						initTileMap();
					initEnemies(); // Always reinitialize enemies
					time = 0.f;
					newHighScore = false;
				}
			}
		}
		
		if (currentState == GameState::PLAYING)
		{
			if (const auto* mousePressed = event->getIf<sf::Event::MouseButtonPressed>())
			{
				const int mouseX = sf::Mouse::getPosition(window).x / int(tileMap->getTileSize());
				const int mouseY = sf::Mouse::getPosition(window).y / int(tileMap->getTileSize());
				
				if (mousePressed->button == sf::Mouse::Button::Left && player->getPlaceableBlocks() > 0)
				{
					sf::FloatRect playerBounds = player->getGlobalBounds();
					sf::FloatRect mouseBounds = sf::FloatRect(
						{float(mouseX * tileMap->getTileSize()), float(mouseY * tileMap->getTileSize())},
						{float(tileMap->getTileSize()), float(tileMap->getTileSize())}
					);
					if (!playerBounds.findIntersection(mouseBounds))
						if (tileMap->addTile(mouseX, mouseY))
							player->addBlock(-1);
				}
				else if (mousePressed->button == sf::Mouse::Button::Right)
				{
					if (tileMap->removeTile(mouseX, mouseY, false, true))
						player->addBlock(1);
				}
			}
		}
	}	

	// Update based on current state
	if (currentState == GameState::MENU)
	{
		updateMenu();
	}
	else if (currentState == GameState::FINISHED)
	{
		// Future finished screen updates
	}
	else if (currentState == GameState::CHOOSING_LEVEL)
	{
		// Future level selection updates
	}
	else if (currentState == GameState::PAUSED)
	{
		// Future paused updates
	}
	else if (currentState == GameState::PLAYING)
	{
		updateInput();
		updatePlayer();
		updateEnemies();
		updateCollision();
		updateTileMap();
	}
	if (currentState == GameState::PLAYING)
		time += 1.f;
}

void Game::updateMenu()
{
	// Menu doesn't need updates, just handles input in main update loop
}

void Game::renderPlayer()
{
	if (player != nullptr)
		player->render(window);
}

void Game::renderEnemies()
{
	for (auto* enemy : enemies)
	{
		if (enemy != nullptr)
			enemy->render(window);
	}
}

void Game::renderTileMap()
{
	if (tileMap != nullptr)
		tileMap->render(window);
}

void Game::renderTime()
{
	sf::Text text(font);
	float totalSeconds = time / 60.f;
	int wholeSeconds = static_cast<int>(totalSeconds);
	int milliseconds = static_cast<int>((totalSeconds - wholeSeconds) * 1000);
	text.setString("Time: " + std::to_string(wholeSeconds) + "." + std::to_string(milliseconds));
	text.setCharacterSize(20);
	text.setFillColor(sf::Color::White);
	text.setPosition({650.f, 10.f});

	window.draw(text);
}

void Game::renderAmountOfPlaceableBlocks()
{
	if (player != nullptr)
	{
		sf::Text text(font);
		text.setString("Placeable Blocks: " + std::to_string(player->getPlaceableBlocks()));
		text.setCharacterSize(20);
		text.setFillColor(sf::Color::White);
		text.setPosition({10.f, 10.f});

		window.draw(text);
	}
}

void Game::updateInput()
{
	if (player == nullptr || tileMap == nullptr)
		return;
		
	//Update mouse positions
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
	if (sf::Keyboard::isKeyPressed(keyboardMappings["KEY_DELETE"]))
	{
		if (tileMap->removeTile(mouseX, mouseY, false, true))
			player->addBlock(1);
	}
	if (sf::Keyboard::isKeyPressed(keyboardMappings["KEY_JUMP"]) && player->getCanJump() == true)
	{
		if (player->getCanStand())
			player->jump();
	}
	else if (sf::Keyboard::isKeyPressed(keyboardMappings["KEY_CROUCH"]))
	{
		player->setCrouching(true);
		player->move(0.f, 1.f);
	}
	else
	{
		if (player->getCanStand() == true)
			player->setCrouching(false);
	}
}

void Game::updatePlayer()
{
	if (player != nullptr)
		player->update();
}

void Game::updateEnemies()
{
	for (auto* enemy : enemies)
	{
		if (enemy != nullptr)
			enemy->update();
	}
}

void Game::updateCollision()
{
	//Collision with tiles
	for (const auto& tileRow : tileMap->getTiles())
	{
		for (const auto& tile : tileRow)
		{
			if (tile != nullptr && tile->getIsSpawned())
			{
				// Manual collision detection without automatic movement
				sf::FloatRect playerBounds = player->getGlobalBounds();
				sf::FloatRect tileBounds = tile->getGlobalBounds();
				
				if (playerBounds.findIntersection(tileBounds))
				{
					if (tile->getType() == TILE_WIN)
					{
						// currentLevel++;
						delete tileMap;
						delete player;
						tileMap = nullptr;
						player = nullptr;
						currentState = GameState::FINISHED;
						if (time < bestTime)
						{
							newHighScore = true;
							std::ofstream highscoreFile("levels/highscore.txt");
							if (highscoreFile.is_open())
							{
								highscoreFile << time;
								highscoreFile.close();
							}
						}
						return; // Exit to avoid further collision processing this frame
					}
					if (tile->getType() == TILE_DEADLY)
					{
						// Reset game when player dies
						delete player;
						delete tileMap;
						player = nullptr;
						tileMap = nullptr;
						currentState = GameState::DIED;
						return; // Exit immediately to avoid accessing deleted objects
					}
					if (tile->getType() == TILE_POWERUP_EXTRA_BLOCK)
					{
						player->addBlock(1);
						tileMap->removeTile(
							unsigned(tile->getSprite().getPosition().x) / tileMap->getTileSize(),
							unsigned(tile->getSprite().getPosition().y) / tileMap->getTileSize(),
							true, false
						);
						continue; // Skip further collision handling for this tile
					}
					else if (tile->getType() == TILE_POWERUP_JUMP_BOOST)
					{
						// Implement jump boost effect
						// For simplicity, just allow an extra jump
						player->setCanJump(true);
						tile->setDespawn();
						continue; // Skip further collision handling for this tile
					}
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

	//Collision with enemies
	for (const auto& enemy : enemies)
	{
		if (enemy != nullptr)
		{
			sf::FloatRect playerBounds = player->getGlobalBounds();
			sf::FloatRect enemyBounds = enemy->getGlobalBounds();
			
			if (playerBounds.findIntersection(enemyBounds))
			{
				// Reset game when player collides with enemy
				delete player;
				delete tileMap;
				player = nullptr;
				tileMap = nullptr;
				currentState = GameState::DIED;
				return; // Exit immediately to avoid accessing deleted objects
			}
		}
	}



	// Check if player can stand (ceiling check for crouching)
	if (player->getIsCrouching())
	{
		// Create a "standing hitbox" to test if player can stand up
		sf::Vector2f playerPos = player->getPosition();
		sf::FloatRect currentBounds = player->getGlobalBounds(); // Current crouched bounds
		
		// Calculate standing bounds: same bottom position, but taller
		float standingHeight = 48.f;
		float bottomY = playerPos.y + currentBounds.size.y; // Bottom of current hitbox
		sf::FloatRect standingBounds = sf::FloatRect(
			{playerPos.x, bottomY - standingHeight}, // Start higher to maintain same bottom
			{32.f, standingHeight}  // Normal standing size
		);
		
		bool canStand = true;
		// Check if standing hitbox would collide with any tiles
		for (const auto& tileRow : tileMap->getTiles())
		{
			for (const auto& tile : tileRow)
			{
				if (tile != nullptr && tile->getSprite().getPosition().y <= playerPos.y -1.f && tile->getSprite().getPosition().y >= playerPos.y -33.f)
				{
					sf::FloatRect tileBounds = tile->getGlobalBounds();
					if (standingBounds.findIntersection(tileBounds))
					{
						canStand = false;
						break;
					}
				}
			}
			if (!canStand) break; // Exit outer loop too
		}
		player->setCanStand(canStand);
	}
	else
	{
		player->setCanStand(true); // Always can stand when not crouching
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
	if (tileMap != nullptr)
		tileMap->update();
}

void Game::renderMenu()
{
	// Render menu background (dark overlay)
	sf::RectangleShape background({800.f, 600.f});
	background.setFillColor(sf::Color(0, 0, 0, 200));
	window.draw(background);
	
	// Title text
	sf::Text titleText(font, "PLATFORMER GAME", 48);
	titleText.setFillColor(sf::Color::White);
	titleText.setPosition({400.f - titleText.getGlobalBounds().size.x / 2.f, 200.f});
	window.draw(titleText);
	
	// Instructions
	sf::Text instructionText(font, "Press ENTER or SPACE to Start", 16);
	instructionText.setFillColor(sf::Color::Yellow);
	instructionText.setPosition({400.f - instructionText.getGlobalBounds().size.x / 2.f, 300.f});
	window.draw(instructionText);
	
	// Controls text
	sf::Text controlsText(font, "ARROW KEYS to move, Left Click to place blocks, Right Click or X to remove", 16);
	controlsText.setFillColor(sf::Color::White);
	controlsText.setPosition({400.f - controlsText.getGlobalBounds().size.x / 2.f, 400.f});
	window.draw(controlsText);
	
	sf::Text escapeText(font, "Press ESC to return to menu during gameplay", 16);
	escapeText.setFillColor(sf::Color::White);
	escapeText.setPosition({400.f - escapeText.getGlobalBounds().size.x / 2.f, 430.f});
	window.draw(escapeText);

	std::ifstream highscoreFile("levels/highscore.txt");
	if (highscoreFile.is_open())
	{
		std::string highscoreStr;
		std::getline(highscoreFile, highscoreStr);
		highscoreFile.close();
		
		// Convert string to float and format with 3 decimal places
		float highscoreFloat = std::stof(highscoreStr) / 60.f;
		bestTime = highscoreFloat * 60.f;
		int wholeSeconds = static_cast<int>(highscoreFloat);
		int milliseconds = static_cast<int>((highscoreFloat - wholeSeconds) * 1000);

		sf::Text highscoreText(font, "Best Time: " + std::to_string(wholeSeconds) + "." + std::to_string(milliseconds), 16);
		highscoreText.setFillColor(sf::Color::White);
		highscoreText.setPosition({400.f - highscoreText.getGlobalBounds().size.x / 2.f, 460.f});
		window.draw(highscoreText);
	}
}

void Game::renderFinishedScreen()
{
	sf::Text finishedText(font, "LEVEL COMPLETE!", 48);
	finishedText.setFillColor(sf::Color::Green);
	finishedText.setPosition({400.f - finishedText.getGlobalBounds().size.x / 2.f, 250.f});

	
	sf::Text escapeText(font, "Press ESC or ENTER to enter the next map", 16);
	escapeText.setFillColor(sf::Color::White);
	escapeText.setPosition({400.f - escapeText.getGlobalBounds().size.x / 2.f, 350.f});

	sf::Text timeText(font);
	std::string topScore = "";
	if (newHighScore)
		topScore = "YOU BEAT THE RECORD!\n        ";
	float totalSeconds = time / 60.f;
	int wholeSeconds = static_cast<int>(totalSeconds);
	int milliseconds = static_cast<int>((totalSeconds - wholeSeconds) * 1000);
	timeText.setString(topScore + "Your Time: " + std::to_string(wholeSeconds) + "." + std::to_string(milliseconds));
	timeText.setCharacterSize(24);
	timeText.setFillColor(sf::Color::Yellow);
	timeText.setPosition({400.f - timeText.getGlobalBounds().size.x / 2.f, 450.f});

	window.draw(finishedText);
	window.draw(escapeText);
	window.draw(timeText);
}

void Game::renderDeathScreen()
{
	sf::Text deathText(font, "YOU DIED!", 48);
	deathText.setFillColor(sf::Color::Red);
	deathText.setPosition({400.f - deathText.getGlobalBounds().size.x / 2.f, 250.f});

	
	sf::Text escapeText(font, "Press ENTER to try again...", 16);
	escapeText.setFillColor(sf::Color::White);
	escapeText.setPosition({400.f - escapeText.getGlobalBounds().size.x / 2.f, 350.f});

	window.draw(deathText);
	window.draw(escapeText);
}

void Game::render()
{
	window.clear();

	if (currentState == GameState::MENU)
	{
		renderMenu();
	}
	else if (currentState == GameState::PLAYING)
	{
		renderTileMap();
		renderEnemies();
		renderPlayer();
		renderAmountOfPlaceableBlocks();
		renderTime();
	}
	else if (currentState == GameState::FINISHED)
	{
		renderFinishedScreen();
	}
	else if (currentState == GameState::DIED)
	{
		renderDeathScreen();
	}

	window.display();
}
