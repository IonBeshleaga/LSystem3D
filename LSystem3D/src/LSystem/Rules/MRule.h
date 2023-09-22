#ifndef MRULE_CLASS_H
#define MRULE_CLASS_H

#include <glm/glm.hpp>

struct mrule_data {
	int type;
	glm::vec3 data;
};

class MRule
{
public:
	MRule();
	~MRule();

};

#endif