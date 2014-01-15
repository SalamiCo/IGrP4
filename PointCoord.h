//Samuel Mendez Galán
//Pedro Morgado Alarcón

#ifndef PointCoordH
#define PointCoordH

#include <windows.h>
#include <gl\gl.h>
#include <gl\glu.h>

class PointCoord{
    private:
        GLdouble x;
        GLdouble y;
    public:
        PointCoord();
        PointCoord(GLdouble x, GLdouble y);
        GLdouble GetX();
        GLdouble GetY();
        void SetX(GLdouble x);
        void SetY(GLdouble y);
        bool PointCoordEqual(GLdouble x, GLdouble y);
};

#endif
 