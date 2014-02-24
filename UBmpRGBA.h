//---------------------------------------------------------------------------

#ifndef UBmpRGBAH
#define UBmpRGBAH
//---------------------------------------------------------------------------
#include <gl\gl.h>
#include <gl\glu.h>
#include <string>
#include <cmath>
#include <algorithm>
#include <iostream>
using namespace std;
//---------------------------------------------------------------------------
typedef GLubyte colorRGBA[3];

class BMPRGBA{
        private:

                GLuint nCols;
                GLuint nRows;
                colorRGBA* pixmap;
                colorRGBA* pixmap2Imagen;
                int queImagen; // Si 1, imagen por defecto. Si 2, imagen para ops aritméticas

                void recoloca(Graphics::TBitmap* AuxBMP, int queImagen);
                void bilinearInterpolation(double newPositionX, double newPositionY);

        public:
                BMPRGBA(){pixmap=NULL; pixmap2Imagen=NULL; nCols=0; nRows=0;};
                BMPRGBA(const string& arch);
                ~BMPRGBA(){delete[] pixmap; delete[] pixmap2Imagen;};
                GLuint getCols(){return nCols;};
                GLuint getRows(){return nRows;};
                colorRGBA* getBMP(){return pixmap;};
                int getSize(){return nCols*nRows;};
                bool cargaBMP(const string& arch, int queImagen);
                void drawPixmap(GLfloat x, GLfloat y, int queImagen);
                void bufferToPixmap(int width, int height, GLfloat x, GLfloat y);
                void mediaPonderada();
                void diferencia();
                void rotate(GLfloat angle);
                void rotate2(GLfloat angle, int x, int y);
                int bilinearInterpolation(double newPositionX, double newPositionY, int rgb);
                void negative();
};


#endif
 