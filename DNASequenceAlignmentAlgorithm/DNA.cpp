#include "DNA.h"
#include <iostream>
DNA::DNA(char* newsequence, int length) {
	this->length = length;
	sequence = (char*)malloc(sizeof(char)*length);
	for (int i = 0; i < length; i++) {
		sequence[i] = newsequence[i];
	}
}

DNA::~DNA()
{
	free(sequence);
}

DNA & DNA::operator=(const DNA & a)
{
	// TODO: 在此处插入 return 语句
	this->length = a.length;
	//std::cout << a.length << std::endl;
	//std::cout << a.sequence[0] << std::endl;
	sequence = (char*)malloc(sizeof(char)*length);
	for (int i = 0; i < length; i++) {
		sequence[i] = a.sequence[i];
	}
	return *this;
}
