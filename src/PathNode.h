#pragma once
#ifndef __PATH_NODE__
#define __PATH_NODE__

#include "DisplayObject.h"
#include "GameObject.h"

class PathNode : public DisplayObject
{
public:
	PathNode();
	~PathNode();
	
	virtual void draw() override;
	virtual void update() override;
	virtual void clean() override;

	void setLineOfSight(bool LOS);
	bool getLineOfSight() const;

private:
	bool m_lineOfSight;
};


#endif	/*defined __PATH_NODE__*/