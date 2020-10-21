#include <assert.h>
#include "Common/Components/Component.h"

Component::Component(Entity* entity)
{
	assert(entity != nullptr);
	this->entity = entity;
}

void update()
{
	return;
}