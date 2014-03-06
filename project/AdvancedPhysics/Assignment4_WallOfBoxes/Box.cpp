#include "Box.h"


Box::Box(void)
{
	body = new cyclone::RigidBody;
}


Box::~Box(void)
{
	delete body;
}
