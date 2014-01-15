//Samuel Mendez Galán
//Pedro Morgado Alarcón

#include "Pencil.h"

Pencil::Pencil(){
	this->pos = PointCoord();
	this->dir = 0;
}

Pencil::Pencil(PointCoord p, GLdouble dir){
	this->pos = p;
	this->dir = dir;
}

void Pencil::SetPos(PointCoord p){
	this->pos = p;
}

void Pencil::SetDir(GLdouble d){
	this->dir = d;
}

GLdouble Pencil::GetDir(){
	return this->dir;
}

PointCoord Pencil::GetPos(){
	return this->pos;
}

void Pencil::Turn(GLdouble d){
	//d>0 -> left, d<0 -> right
	this->dir += d;
} 

void Pencil::Forward(GLdouble dist){
	GLdouble x = dist*cos(this->dir);
	GLdouble y = dist*sin(this->dir);
    this->pos.SetX(pos.GetX() + x);
    this->pos.SetY(pos.GetY() + y);     
}

