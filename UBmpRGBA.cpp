#include <vcl.h>
#pragma hdrstop

#include "UBmpRGBA.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

BMPRGBA::BMPRGBA(const string& arch){
        pixmap=NULL; nCols=0; nRows=0;
        cargaBMP(arch);
}

bool BMPRGBA::cargaBMP(const string& arch){
        Graphics::TBitmap* AuxBMP = new Graphics::TBitmap();
        AuxBMP->LoadFromFile(arch.c_str());
        if (AuxBMP->Empty) {
                delete AuxBMP;
                return false;
        }
        else {
                recoloca(AuxBMP);
                delete AuxBMP;
                return true;
        }
}

void BMPRGBA::recoloca(Graphics::TBitmap* AuxBMP){
        pixmap = new colorRGBA[AuxBMP->Width * AuxBMP->Height];
        nCols=AuxBMP->Width;
        nRows=AuxBMP->Height;
        int count;
        unsigned int i, j;
        for (i=0; i < nRows; i++){
                for (j=0; j < nCols; j++) {
                        count=i*nCols + j;
                        pixmap[count][0]=GetRValue(AuxBMP->Canvas->Pixels[j][nRows-i-1]);
                        pixmap[count][1]=GetGValue(AuxBMP->Canvas->Pixels[j][nRows-i-1]);
                        pixmap[count][2]=GetBValue(AuxBMP->Canvas->Pixels[j][nRows-i-1]);
                        //pixmap[count][3]=255;
                }
        }
}

void BMPRGBA::drawPixmap(GLfloat x, GLfloat y){
        glPixelStorei(GL_UNPACK_ALIGNMENT, //como escribir los pixeles 
                        1); //sin padding entre filas
        glRasterPos2i(x, y); //esquina inferior-izquerda donde escribiremos el bloque, usando coordenadas OpenGL de la ventana
        glDrawPixels(nCols, nRows, //tamaño del bloque
                        GL_RGB, //datos a escribir: buffer de color, de profundidad, componente alpha...
                        GL_UNSIGNED_BYTE, //tipo de los datos
                        pixmap); //origen

        //Si una de las operaciones devuelve en escala de grises, pixmap es más pequeño. Pasarle a drawPixels GL_LUMINANCE
}

void BMPRGBA::bufferToPixmap(int width, int height, GLfloat x, GLfloat y){
        delete[] pixmap;
        pixmap = new colorRGBA[width * height];
        nCols = width;
        nRows = height;
        glPixelStorei(GL_PACK_ALIGNMENT, //Cómo se leen los pixeles
                        1); //sin padding entre filas
        glReadPixels(x, y, //esquina inferior-izquierda del bloque,
                width, height, // tamaño del bloque
                GL_RGB, //datos a leer
                GL_UNSIGNED_BYTE, //tipo de los datos
                pixmap); //destino
}



