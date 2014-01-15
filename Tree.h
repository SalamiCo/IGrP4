//Samuel Mendez Galán
//Pedro Morgado Alarcón

#ifndef TreeH
#define TreeH

#define DOUBLE_MAX 1.7976931348623158e+308 /* max value */

#include <vector>
#include <iostream>
#include <math.h>
#include <algorithm>
#include "Square.h"
#include "Pencil.h"

class Tree{
	private:
		std::vector<Square> next, archive;
	public:
		Tree();
		Tree(int X,int Y);
		void DrawTree(Square selectedSquare);
		void AddLevel();
		void UndoLevel();
		Square SelectSquare(int x, int y);
		void TreeColors(int lvl);
};

#endif
 