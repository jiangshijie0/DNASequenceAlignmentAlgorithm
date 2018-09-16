#include "StaticFunctions.h"
#include "DNA.h"
#include <iostream>

int main() {
	DNA** data;
	data = (DNA**)malloc(sizeof(DNA*) * 100);
	data = Function::getDNASequence(data);
	//int min = Function::opt(data[2], data[3], 0, 0, data[2]->length, data[3]->length);
	////puts(data[0]->sequence);
	////puts(data[1]->sequence);
	//std::cout << "**********\n";
	//std::cout << min << std::endl;

	int min = Function::dynamicProgrammingAlgorithm(data[6], data[7]);
	puts(data[6]->sequence);
	puts(data[7]->sequence);
	std::cout << "**********\n";
	std::cout << min << std::endl;
	free(data);
	return 0;
}