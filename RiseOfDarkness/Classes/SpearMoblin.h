#ifndef SPEAR_MOBLIN_H
#define SPEAR_MOBLIN_H	

#include "Enemy.h"

class SpearMoblin : public Enemy
{
private:
public:
	SpearMoblin();
	SpearMoblin(Layer* layer);

	void Update(float deltaTime);
};

#endif
