//Samuel Mendez Galán
//Pedro Morgado Alarcón

#include "Square.h"

Square::Square(){
	this->p1 = PointCoord();
	this->p2 = PointCoord();
	this->p3 = PointCoord();
	this->p4 = PointCoord();
	this->currLevel = 0;
}

Square::Square(PointCoord p1, PointCoord p2, PointCoord p3, PointCoord p4, int currLevel){
	this->p1 = p1;
	this->p2 = p2;
	this->p3 = p3;
	this->p4 = p4;
	this->currLevel = currLevel;
}

PointCoord Square::GetP1(){
    return this->p1;
}

PointCoord Square::GetP2(){
    return this->p2;
}

PointCoord Square::GetP3(){
    return this->p3;
}

PointCoord Square::GetP4(){
    return this->p4;
}

GLdouble Square::GetEdge(){
	GLdouble dx = this->p1.GetX()-this->p2.GetX();
	GLdouble dy = this->p1.GetY()-this->p2.GetY();
	return sqrt(dx*dx + dy*dy);
}

GLdouble Square::GetAngle(){
	return atan2(this->p1.GetY() - this->p2.GetY(), this->p1.GetX() - this->p2.GetX());
}

int Square::GetCurrLevel(){
	return this->currLevel;
}

void Square::DrawSquare(PointCoord p1, PointCoord p2, PointCoord p3, PointCoord p4){
	glBegin(GL_QUADS);
        glVertex2f( p1.GetX(), p1.GetY());
        glVertex2f( p2.GetX(), p2.GetY());
        glVertex2f( p3.GetX(), p3.GetY());
        glVertex2f( p4.GetX(), p4.GetY());
    glEnd();
}

bool Square::SquareEqual(PointCoord p1, PointCoord p2, PointCoord p3, PointCoord p4){
	return (this->p1.PointCoordEqual(p1.GetX(), p1.GetY()) && this->p2.PointCoordEqual(p2.GetX(), p2.GetY()) && this->p3.PointCoordEqual(p3.GetX(), p3.GetY()) && this->p4.PointCoordEqual(p4.GetX(), p4.GetY()));
}
 