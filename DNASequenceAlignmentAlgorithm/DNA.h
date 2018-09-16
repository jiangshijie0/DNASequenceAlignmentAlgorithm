#pragma once

#include <list>

class DNA
{
public:
	DNA(char *sequence, int length);
	~DNA();
	DNA& operator =(const DNA& a);

	int length;
	char* sequence;
};