#pragma once
#include "DNA.h"

static class Function
{
public:
	static DNA** getDNASequence(DNA** data);
	static int opt(DNA* x, DNA* y, int i, int j, int m, int n);
	static int dynamicProgrammingAlgorithm(DNA* x, DNA* y);
	Function();
	~Function();

private:

};