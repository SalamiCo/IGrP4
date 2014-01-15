//Samuel Mendez Galán
//Pedro Morgado Alarcón

#include "PointCoord.h"

PointCoord::PointCoord(){
	this->x = 0;
	this->y = 0;
}

PointCoord::PointCoord(GLdouble x, GLdouble y){
	this->x = x;
	this->y = y;
}

GLdouble PointCoord::GetX(){
	return this->x;
}

GLdouble PointCoord::GetY(){
	return this->y;
}

void PointCoord::SetX(GLdouble x){
	this->x = x;
}

void PointCoord::SetY(GLdouble y){
	this->y = y;
}

bool PointCoord::PointCoordEqual(GLdouble x, GLdouble y){
	return (this->x == x && this->y == y);
}