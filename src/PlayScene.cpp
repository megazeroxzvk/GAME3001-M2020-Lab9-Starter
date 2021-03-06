#include "PlayScene.h"
#include "Game.h"
#include "EventManager.h"
#include "Util.h"

PlayScene::PlayScene()
{
	PlayScene::start();
}

PlayScene::~PlayScene()
= default;

void PlayScene::draw()
{
	drawDisplayList();
	
	//Lab 9---------------------------
	if (m_debugModeToggle)
	{
		Util::DrawLine(m_pPlayer->getTransform()->position, m_pPlaneSprite->getTransform()->position);

		Util::DrawRect(m_pPlayer->getTransform()->position - glm::vec2(m_pPlayer->getWidth() * 0.5, m_pPlayer->getHeight() * 0.5),
			m_pPlayer->getWidth(), m_pPlayer->getHeight());

		Util::DrawRect(m_pObstacle->getTransform()->position - glm::vec2(m_pObstacle->getWidth() * 0.5, m_pObstacle->getHeight() * 0.5),
			m_pObstacle->getWidth(), m_pObstacle->getHeight());

		
		Util::DrawRect(m_pPlaneSprite->getTransform()->position - glm::vec2(m_pPlaneSprite->getWidth() * 0.5, m_pPlaneSprite->getHeight() * 0.5),
			m_pPlaneSprite->getWidth(), m_pPlaneSprite->getHeight());
		m_displayGrid();			//display our nice grid of tilemap set and tile nodes
		m_displayGridLOS();			//display the grids and their lines IF NO LOS !!!!
	}
	
}

void PlayScene::update()
{
	updateDisplayList();

	CollisionManager::LOSCheck(m_pPlayer, m_pPlaneSprite, m_pObstacle);

	//Lab 9---------------------------
	CollisionManager::AABBCheck(m_pPlayer, m_pPlaneSprite);
	CollisionManager::AABBCheck(m_pPlayer, m_pObstacle);
	m_setGridLOS();
}

void PlayScene::clean()
{
	removeAllChildren();
}

void PlayScene::handleEvents()
{
	EventManager::Instance().update();

	// handle player movement with GameController
	if (SDL_NumJoysticks() > 0)
	{
		if (EventManager::Instance().getGameController(0) != nullptr)
		{
			const auto deadZone = 10000;
			if (EventManager::Instance().getGameController(0)->LEFT_STICK_X > deadZone)
			{
				m_pPlayer->setAnimationState(PLAYER_RUN_RIGHT);
				m_playerFacingRight = true;

				m_pPlayer->getRigidBody()->velocity = glm::vec2(5.0f, 0.0f);
				m_pPlayer->getTransform()->position += m_pPlayer->getRigidBody()->velocity;
				m_pPlayer->getRigidBody()->velocity *= m_pPlayer->getRigidBody()->velocity * 0.9f;
			}
			else if (EventManager::Instance().getGameController(0)->LEFT_STICK_X < -deadZone)
			{
				m_pPlayer->setAnimationState(PLAYER_RUN_LEFT);
				m_playerFacingRight = false;

				m_pPlayer->getRigidBody()->velocity = glm::vec2(-5.0f, 0.0f);
				m_pPlayer->getTransform()->position += m_pPlayer->getRigidBody()->velocity;
				m_pPlayer->getRigidBody()->velocity *= m_pPlayer->getRigidBody()->velocity * 0.9f;
			}
			else
			{
				if (m_playerFacingRight)
				{
					m_pPlayer->setAnimationState(PLAYER_IDLE_RIGHT);
				}
				else
				{
					m_pPlayer->setAnimationState(PLAYER_IDLE_LEFT);
				}
			}
		}
	}


	// handle player movement if no Game Controllers found
	if (SDL_NumJoysticks() < 1)
	{
		if (EventManager::Instance().isKeyDown(SDL_SCANCODE_A))
		{
			m_pPlayer->setAnimationState(PLAYER_RUN_LEFT);
			m_playerFacingRight = false;

			m_pPlayer->getRigidBody()->velocity = glm::vec2(-5.0f, 0.0f);
			m_pPlayer->getTransform()->position += m_pPlayer->getRigidBody()->velocity;
			m_pPlayer->getRigidBody()->velocity *= m_pPlayer->getRigidBody()->velocity * 0.9f;
		}
		else if (EventManager::Instance().isKeyDown(SDL_SCANCODE_D))
		{
			m_pPlayer->setAnimationState(PLAYER_RUN_RIGHT);
			m_playerFacingRight = true;

			m_pPlayer->getRigidBody()->velocity = glm::vec2(5.0f, 0.0f);
			m_pPlayer->getTransform()->position += m_pPlayer->getRigidBody()->velocity;
			m_pPlayer->getRigidBody()->velocity *= m_pPlayer->getRigidBody()->velocity * 0.9f;
		}
		else
		{
			if (m_playerFacingRight)
			{
				m_pPlayer->setAnimationState(PLAYER_IDLE_RIGHT);
			}
			else
			{
				m_pPlayer->setAnimationState(PLAYER_IDLE_LEFT);
			}
		}
	}
	

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_ESCAPE))
	{
		TheGame::Instance()->quit();
	}

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_1))
	{
		TheGame::Instance()->changeSceneState(START_SCENE);
	}

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_2))
	{
		TheGame::Instance()->changeSceneState(END_SCENE);
	}


	//Lab 9---------------------------
	//if(EventManager::Instance().isKeyUp(SDL_SCANCODE_H))		//faulty
	//Comments - not so great, but acceptable.
	//DEBUG - H KEY
	if(!m_debugKeys[H_KEY])
	{
		if (EventManager::Instance().isKeyDown(SDL_SCANCODE_H))
		{
			m_debugModeToggle = !m_debugModeToggle;
			m_debugKeys[H_KEY] = true;

			m_debugModeToggle ?
				std::cout << "DEBUG: Debug Mode Started" << std::endl
				:
				std::cout << "DEBUG: Debug Mode Stopped" << std::endl;
		}
	}
	if(EventManager::Instance().isKeyUp(SDL_SCANCODE_H))
	{
		m_debugKeys[H_KEY] = false;
	}

	//K KEY
	if(!m_debugKeys[K_KEY])
	{
		if (EventManager::Instance().isKeyDown(SDL_SCANCODE_K))
		{
			m_debugKeys[K_KEY] = true;
			std::cout << "DEBUG: Enemy Damaged" << std::endl;
		}
	}
	if (EventManager::Instance().isKeyUp(SDL_SCANCODE_K))
	{
		m_debugKeys[K_KEY] = false;
	}

	//P KEY
	if (!m_debugKeys[P_KEY])
	{
		if (EventManager::Instance().isKeyDown(SDL_SCANCODE_P))
		{
			m_patrolModeToggle = !m_patrolModeToggle;
			m_debugKeys[P_KEY] = true;

			m_patrolModeToggle ?
				std::cout << "DEBUG: Patrol Mode Started" << std::endl
				:
				std::cout << "DEBUG: Patrol Mode Stopped" << std::endl;
		}
	}
	if (EventManager::Instance().isKeyUp(SDL_SCANCODE_P))
	{
		m_debugKeys[P_KEY] = false;
	}
	

	
	
}


//Lab 9---------------------------
void PlayScene::m_buildGrid()
{
	//Logic to add the pathnodes to overall scene/s
	for (int row = 0; row < Config::ROW_NUM; ++row)
	{
		for (int col = 0; col < Config::COL_NUM; ++col)
		{
			auto pathNode = new PathNode();
			pathNode->getTransform()->position = glm::vec2(pathNode->getWidth() * col + Config::TILE_SIZE*0.5,pathNode->getHeight() * row + Config::TILE_SIZE * 0.5);
			m_pGrid.push_back(pathNode);
		}
	}
}

//Lab 9---------------------------
void PlayScene::m_displayGrid()
{
	//Logic to add the pathnodes to overall scene/s
	for (int row = 0; row < Config::ROW_NUM; ++row)
	{
		for (int col = 0; col < Config::COL_NUM; ++col)
		{
			//Actual TileMap grid
			Util::DrawRect(m_pGrid[row * Config::COL_NUM + col]->getTransform()->position - glm::vec2(m_pGrid[row * Config::COL_NUM + col]->getWidth() * 0.5f, 
				m_pGrid[row * Config::COL_NUM + col]->getHeight() * 0.5f),
				40, 40);

			//CenterPoint for those TileMap grid for doing various AI functions, PF
			Util::DrawRect(m_pGrid[row * Config::COL_NUM + col]->getTransform()->position, 5, 5);
		}
	}
}

//Lab 9---------------------------
void PlayScene::m_displayGridLOS()
{
	for (auto* node : m_pGrid)
	{
		if(!node->getLineOfSight())
		{
			Util::DrawLine(node->getTransform()->position, m_pPlayer->getTransform()->position, glm::vec4(1, 0, 0, 1.0f));
		}
		
	}
}

//Lab 9---------------------------
void PlayScene::m_setGridLOS()
{
	//for each node set a line
	for (auto* node : m_pGrid)
	{
		node->setLineOfSight(CollisionManager::LOSCheck(node, m_pPlayer, m_pObstacle));
	}
}

void PlayScene::start()
{
	//Lab 9---------------------------
	m_buildGrid();
	m_debugModeToggle = false;
	//m_HPressedCheck = false;
	
	
	// Plane Sprite
	m_pPlaneSprite = new Plane();
	addChild(m_pPlaneSprite);

	// Player Sprite
	m_pPlayer = new Player();
	addChild(m_pPlayer);
	m_playerFacingRight = true;

	// Obstacle Texture
	m_pObstacle = new Obstacle();
	addChild(m_pObstacle);
	
}


