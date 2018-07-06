// odplot productions is a trademarked name. Project Yugh is a copyrighted property. This code, however, is free to be copy and extended as you see fit.

#pragma once

#include "dictionary.h"
#include "object.h"
#include "reference.h"
#include "scene/main/node.h"

// TODO: Not integrating boost libraries right now to keep it simple, but should later
//#include "boost/date_time/gregorian/gregorian_types.hpp"

// Attach to an actor to let it keep track of time.
class Damageable : public Node {
	GDCLASS(Damageable, Node);

public:
	Damageable() {
	};

	~Damageable(){ };
};
