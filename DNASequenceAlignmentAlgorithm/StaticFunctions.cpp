#include <vector>
#include <fstream>
#include <iostream>
#include <string>

#include "StaticFunctions.h"
#include "DNA.h"

DNA** Function::getDNASequence(DNA** data)
{
	std::ifstream inf;
	inf.open("data.dat");
	char s[2048];
	int i = 0;
	while (!inf.eof()) {
		inf.getline(s, 2048);

		char* temp_1 = strtok(s, " ");
		std::cout << temp_1 << std::endl;
		char* temp_2 = strtok(NULL, " ");
		std::cout << temp_2 << std::endl;

		int length = atoi(temp_1);
		data[i] = new DNA(temp_2, length);
		i++;
	}
	inf.close();
	return data;
}

Function::Function()
{
}

Function::~Function()
{
}

int min(int a, int b, int c) {
	int min = a;
	if (min > b) min = b;
	if (min > c) min = c;
	return min;
}

int Function::opt(DNA* x, DNA* y, int i, int j, int m, int n) {
	int penalty;
	if (i == m) {
		return 2 * (n - j);
	}
	else if (j == n) {
		return 2 * (m - i);
	}
	else {
		if (*(x->sequence + i) == *(y->sequence + j)) {
			penalty = 0;
		}
		else if (*(x->sequence + i) != *(y->sequence + j)) {
			penalty = 1;
		}
		else penalty = 2;
	}
	return min(opt(x, y, i + 1, j + 1, m, n) + penalty, opt(x, y, i + 1, j, m, n) + penalty, opt(x, y, i, j + 1, m, n) + penalty);
}

typedef struct element
	{
		int opt;
		int x, y;		//location of elements in matrix

		std::vector<element> predecessor;
		element* successor;

	};

element* findMinElement(DNA* x, DNA* y, element* currentElement, element* right, element* down, element* diagonal) {
	int rightPenalty = right->opt + 2, downPenalty = down->opt + 2, diagonalPenalty;
	if (*(x->sequence + currentElement->x) == *(y->sequence + currentElement->y)) {
		//std::cout << *(x->sequence + currentElement->x) << "***" << *(y->sequence + currentElement->y) << std::endl;
		diagonalPenalty = diagonal->opt + 0;
	}
	else
	{
		diagonalPenalty = diagonal->opt + 1;
	}

	if (rightPenalty <= downPenalty && rightPenalty <= diagonalPenalty) {
		currentElement->opt = rightPenalty;
		//std::cout << "rightPenalty:" << rightPenalty << std::endl;
		return right;
	}
	if (downPenalty <= rightPenalty && downPenalty <= diagonalPenalty) {
		currentElement->opt = downPenalty;
		//std::cout << "downPenalty:" << downPenalty << std::endl;
		return down;
	}
	if (diagonalPenalty <= rightPenalty && diagonalPenalty <= downPenalty) {
		currentElement->opt = diagonalPenalty;
		//std::cout << "diagonalPenalty:" << diagonalPenalty << std::endl;
		return diagonal;
	}
}


int Function::dynamicProgrammingAlgorithm(DNA* x, DNA* y) {

	//分配矩阵空间
	element** matrix = (element**)malloc(sizeof(element) * ((x->length) + 1));
	for (int i = 0; i <= x->length; i++) {
		matrix[i] = (element*)malloc(sizeof(element) * ((y->length) + 1));
	}

	//初始化坐标
	for (int i = 0; i <= x->length; i++) {
		for (int j = 0; j <= y->length; j++) {
			matrix[i][j].x = i;
			matrix[i][j].y = j;
		}
	}

	//初始化最后一行GAP行
	for (int i = x->length; i >= 0; i--) matrix[i][y->length].opt = 2 * (x->length - i);

	//初始化最后一列GAP列
	for (int j = y->length; j >= 0; j--) matrix[x->length][j].opt = 2 * (y->length - j);

	//计算矩阵
	for (int i = x->length - 1; i >= 0; i--) {
		for (int j = y->length - 1; j >= 0; j--) {
			element* temp = findMinElement(x, y, &matrix[i][j], &matrix[i + 1][j], &matrix[i][j + 1], &matrix[i + 1][j + 1]);

			matrix[i][j].successor = temp;
			//element* b = &matrix[i][j];
			//matrix[i][j].successor.push_back(*temp);
			//std::cout << "matrix[i][j].successor.push_back(temp);\n";
			//b = &matrix[i][j];
			//temp->predecessor.push_back(matrix[i][j]);
			//std::cout << "temp->predecessor.push_back(b);\n";
		}
	}

	//重组DNA序列
	DNA temp_x(x->sequence, x->length);
	DNA temp_y(y->sequence, y->length);
	std::cout << temp_x.length << "***" << temp_y.length << std::endl;
	element* temp = &matrix[0][0];
	int process_x = 0;	//记录当前DNA序列重组到第几个
	int process_y = 0;
	int process = 0;

	//std::cout << "x->length:" << x->length << std::endl;
	while (process_x < x->length || process_y < y->length) {
		//判断到头补齐DNA sequence
		std::cout << "temp->x:" << temp->x << std::endl;
		std::cout << "temp->y:" << temp->y << std::endl;
		if (process_x >= x->length || process_y >= y->length) {
			std::cout << "in if " << std::endl;
			if (process_y < y->length) {
				int i;
				for (i = process; process_y < y->length; i++) {
					x->sequence[i] = '-';
					y->sequence[i] = temp_y.sequence[process_y];
					process_y++;
				}
				x->sequence[i] = '\0';
				y->sequence[i] = '\0';
				break;
			}
			else if (process_x < x->length) {
				int i;
				for (i = process; process_x < x->length; i++) {
					y->sequence[i] = '-';
					x->sequence[i] = temp_x.sequence[process_x];
					process_x++;
				}
				x->sequence[i] = '\0';
				y->sequence[i] = '\0';
				break;
			}
		}

		else if (temp->successor->x - temp->x == 1 && temp->successor->y - temp->y == 1) {
			//diagonal
			std::cout << "diagonal" << std::endl;
			x->sequence[process] = temp_x.sequence[process_x];
			y->sequence[process] = temp_y.sequence[process_y];

			x->sequence[process+1] = '\0';
			y->sequence[process+1] = '\0';
			puts(x->sequence);
			puts(y->sequence);
			process_x++; process_y++;
		}
		else if (temp->successor->x - temp->x == 1 && temp->successor->y - temp->y == 0) {
			//right
			std::cout << "right" << std::endl;
			x->sequence[process] = '-';
			y->sequence[process] = temp_y.sequence[process_y];
			x->sequence[process + 1] = '\0';
			y->sequence[process + 1] = '\0';
			puts(x->sequence);
			puts(y->sequence);
			process_y++;
		}
		else if (temp->successor->x - temp->x == 0 && temp->successor->y - temp->y == 1) {
			//down
			std::cout << "down" << std::endl;
			x->sequence[process] = temp_x.sequence[process_x];
			y->sequence[process] = '-';
			x->sequence[process + 1] = '\0';
			y->sequence[process + 1] = '\0';
			puts(x->sequence);
			puts(y->sequence);
			process_x++;
		}
		process++;
		temp = temp->successor;
		std::cout << "process_x:" << process_x << std::endl;
		std::cout << "process_y:" << process_y << std::endl;
	}
	return matrix[0][0].opt;
}