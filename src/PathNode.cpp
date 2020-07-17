#include "PathNode.h"

#include "CollisionManager.h"

PathNode::PathNode()
	:m_lineOfSight(false)
{
	setWidth(Config::TILE_SIZE);
	setHeight(Config::TILE_SIZE);
	setType(PATH_NODE);
}

PathNode::~PathNode()
= default;

void PathNode::draw()
{
}

void PathNode::update()
{
	//CollisionManager::LOSCheck();
}

void PathNode::clean()
{
}

void PathNode::setLineOfSight(const bool LOS)
{
	m_lineOfSight = LOS;
}

bool PathNode::getLineOfSight() const
{
	return m_lineOfSight;
}
