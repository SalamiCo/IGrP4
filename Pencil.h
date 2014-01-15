//Samuel Mendez Galán
//Pedro Morgado Alarcón

#ifndef PencilH
#define PencilH

#include "PointCoord.h"
#include <math.h>

class Pencil{
	private:
		PointCoord pos;
		GLdouble dir;
	public:
		Pencil();
		Pencil(PointCoord p, GLdouble dir);
		void SetPos(PointCoord p);
		void SetDir(GLdouble d);
		GLdouble GetDir();
		PointCoord GetPos();
        void Turn(GLdouble d);
        void Forward(GLdouble dist);
};

#endif
 