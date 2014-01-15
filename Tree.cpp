//Samuel Mendez Galán
//Pedro Morgado Alarcón

#include "Tree.h"

Tree::Tree(){
	Square firstSquare = Square(PointCoord(50,-100), PointCoord(-50,-100), PointCoord(-50,-200), PointCoord(50,-200), 0);
    this->next.push_back(firstSquare);
}

Tree::Tree(int X, int Y){
    Square newSquare = Square(PointCoord(X+50,Y+50), PointCoord(X-50,Y+50), PointCoord(X-50,Y-50), PointCoord(X+50,Y-50), 0);
    this->next.push_back(newSquare);
}

void Tree::DrawTree(Square selectedSquare){
    std::vector<Square>::iterator i;
    int lvl;

    for(i=this->archive.begin(); i!=this->archive.end(); ++i){
        if((*i).SquareEqual(selectedSquare.GetP1(), selectedSquare.GetP2(), selectedSquare.GetP3(), selectedSquare.GetP4())){
            glColor3f(1.0,1.0,1.0); //White  
            i->DrawSquare(i->GetP1(), i->GetP2(), i->GetP3(), i->GetP4());
        } else {
            lvl = i->GetCurrLevel();
            TreeColors(lvl);
            i->DrawSquare(i->GetP1(), i->GetP2(), i->GetP3(), i->GetP4());
        }
    }

    for(i=this->next.begin(); i!=this->next.end(); ++i){
        if((*i).SquareEqual(selectedSquare.GetP1(), selectedSquare.GetP2(), selectedSquare.GetP3(), selectedSquare.GetP4())){
            glColor3f(1.0,1.0,1.0); //White
            i->DrawSquare(i->GetP1(), i->GetP2(), i->GetP3(), i->GetP4());
        } else {
            lvl = i->GetCurrLevel();
            TreeColors(lvl);
            i->DrawSquare(i->GetP1(), i->GetP2(), i->GetP3(), i->GetP4());
        }   
    }
}

void Tree::AddLevel(){
    Pencil pen;
    std::vector<Square>::iterator level;
    std::vector<Square>::iterator stop;
    GLdouble distance;
    int currLevel;

    this->next.reserve(this->next.size()*3);

    // Last Square of the current level
    stop = this->next.end();

    for(level=this->next.begin(); level!=stop; ++level){
        //Calculate the distance for the pencil to move
        distance = level->GetEdge()*cos(M_PI_4);

        //Get next level of the tree
        currLevel = level->GetCurrLevel() + 1;

        //First square
        PointCoord p1 (level->GetP1().GetX(), level->GetP1().GetY());
        pen.SetPos(p1);
        pen.SetDir(level->GetAngle() + M_PI_4);
        pen.Forward(distance);

        PointCoord p2 = pen.GetPos();
        pen.Turn(M_PI_2);
        pen.Forward(distance);

        PointCoord p3 = pen.GetPos();
        pen.Turn(M_PI_2);
        pen.Forward(distance);

        PointCoord p4 = pen.GetPos();
        pen.Turn(M_PI_2);
        pen.Forward(distance);

        Square s1 = Square(p2,p3,p4,p1,currLevel);
        this->next.push_back(s1);

        //Second Square
        pen.Turn(M_PI_4+M_PI);
        pen.Forward(level->GetEdge());

        PointCoord p5 = pen.GetPos();
        pen.Turn(-M_PI_2 - M_PI_4);
        pen.Forward(distance);

        PointCoord p6 = pen.GetPos();
        pen.Turn(M_PI_2);
        pen.Forward(distance);

        PointCoord p7 = pen.GetPos();
        pen.Turn(M_PI_2);
        pen.Forward(distance);

        PointCoord p8 = pen.GetPos();
        pen.Turn(M_PI_2);
        pen.Forward(distance);

        Square s2 = Square(p7,p8,p5,p6,currLevel);
        this->next.push_back(s2);

        // Save the current level in list 'archive'
        this->archive.push_back(*level);
    }

    // Delete the current level from list 'next'
    this->next.erase(this->next.begin(), stop);
}

void Tree::UndoLevel(){
    // Iterate backwards
    std::vector<Square>::reverse_iterator level;
    int currLevel, squareLevel;

    // Delete the last level from list 'next'
    this->next.erase(this->next.begin(), this->next.end());

    // Add to list 'next' last level from list 'archive'
    level = this->archive.rbegin();
    currLevel = level->GetCurrLevel();
    squareLevel = pow(2,currLevel);
    
    for(int i=1; i<=squareLevel; ++level, ++i){
        this->next.push_back(*level);
        this->archive.pop_back();
    }
}

Square Tree::SelectSquare(int x, int y){
    GLdouble x1, x2, x3, x4, y1, y2, y3, y4;
    GLdouble minX, maxX, minY, maxY;
    Square square;
    std::vector<Square>::iterator it;
    for(it=this->archive.begin(); it!=this->archive.end(); ++it){
        x1 = it->GetP1().GetX();
        x2 = it->GetP2().GetX();
        x3 = it->GetP3().GetX();
        x4 = it->GetP4().GetX();
        y1 = it->GetP1().GetY();
        y2 = it->GetP2().GetY();
        y3 = it->GetP3().GetY();
        y4 = it->GetP4().GetY();
        minX = std::min(std::min(x1, x2), std::min(x3, x4));
        maxX = std::max(std::max(x1, x2), std::max(x3, x4));
        minY = std::min(std::min(y1, y2), std::min(y3, y4));
        maxY = std::max(std::max(y1, y2), std::max(y3, y4));
        if(x>minX && x<maxX && y>minY && y<maxY){
            square = (*it);
        }
    }

    for(it=this->next.begin(); it!=this->next.end(); ++it){
        x1 = it->GetP1().GetX();
        x2 = it->GetP2().GetX();
        x3 = it->GetP3().GetX();
        x4 = it->GetP4().GetX();
        y1 = it->GetP1().GetY();
        y2 = it->GetP2().GetY();
        y3 = it->GetP3().GetY();
        y4 = it->GetP4().GetY();
        minX = std::min(std::min(x1, x2), std::min(x3, x4));
        maxX = std::max(std::max(x1, x2), std::max(x3, x4));
        minY = std::min(std::min(y1, y2), std::min(y3, y4));
        maxY = std::max(std::max(y1, y2), std::max(y3, y4));
        if(x>minX && x<maxX && y>minY && y<maxY){
            square = (*it);
        }
    }
    return square;
}

void Tree::TreeColors(int lvl){
    if(lvl == 0){
        glColor3f(0.38, 0.19, 0.0); // Brown
    } else if(lvl <= 3){
        glColor3f(0.61, 0.31, 0.0); // Light brown
    } else if(lvl <= 5){
        glColor3f(0.0, 0.25, 0.0); // Dark Green
    } else if(lvl > 5 ){
        glColor3f(0.0, 0.73, 0.0); // Light green
    }
}
 