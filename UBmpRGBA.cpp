#include <vcl.h>
#pragma hdrstop

#include "UBmpRGBA.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

BMPRGBA::BMPRGBA(const string& arch){
    pixmap=NULL; nCols=0; nRows=0;
    pixmap2Imagen=NULL; queImagen=1;
    cargaBMP(arch, queImagen);
}

bool BMPRGBA::cargaBMP(const string& arch, int queImagen){
    Graphics::TBitmap* AuxBMP = new Graphics::TBitmap();
    AuxBMP->LoadFromFile(arch.c_str());
    if (AuxBMP->Empty) {
            delete AuxBMP;
            return false;
    }
    else {
            recoloca(AuxBMP, queImagen);
            delete AuxBMP;
            return true;
    }
}

void BMPRGBA::recoloca(Graphics::TBitmap* AuxBMP, int queImagen){

    nCols=AuxBMP->Width;
    nRows=AuxBMP->Height;
    int count;
    unsigned int i, j;

    if(queImagen == 1){
        if(pixmap){
            delete [] pixmap;
        }
        pixmap = new colorRGBA[AuxBMP->Width * AuxBMP->Height];
        
        for (i=0; i < nRows; i++){
                for (j=0; j < nCols; j++) {
                        count=i*nCols + j;
                        pixmap[count][0]=GetRValue(AuxBMP->Canvas->Pixels[j][nRows-i-1]);
                        pixmap[count][1]=GetGValue(AuxBMP->Canvas->Pixels[j][nRows-i-1]);
                        pixmap[count][2]=GetBValue(AuxBMP->Canvas->Pixels[j][nRows-i-1]);
                }
        }    
    } else { //queImagen = 2
        if(pixmap2Imagen){
            delete [] pixmap2Imagen;
        }
        pixmap2Imagen = new colorRGBA[AuxBMP->Width * AuxBMP->Height];
        for (i=0; i < nRows; i++){
                for (j=0; j < nCols; j++) {
                        count=i*nCols + j;
                        pixmap2Imagen[count][0]=GetRValue(AuxBMP->Canvas->Pixels[j][nRows-i-1]);
                        pixmap2Imagen[count][1]=GetGValue(AuxBMP->Canvas->Pixels[j][nRows-i-1]);
                        pixmap2Imagen[count][2]=GetBValue(AuxBMP->Canvas->Pixels[j][nRows-i-1]);
          
                }
        }
    }
    
}

void BMPRGBA::drawPixmap(GLfloat x, GLfloat y, int queImagen){
    glPixelStorei(GL_UNPACK_ALIGNMENT, //como escribir los pixeles 
                    1); //sin padding entre filas
    glRasterPos2i(x, y); //esquina inferior-izquerda donde escribiremos el bloque, usando coordenadas OpenGL de la ventana
    if(queImagen == 1){
        glDrawPixels(nCols, nRows, //tamaño del bloque
                    GL_RGB, //datos a escribir: buffer de color, de profundidad, componente alpha...
                    GL_UNSIGNED_BYTE, //tipo de los datos
                    pixmap); //origen
    } else { //queImagen = 2
        glDrawPixels(nCols, nRows, //tamaño del bloque
                    GL_RGB, //datos a escribir: buffer de color, de profundidad, componente alpha...
                    GL_UNSIGNED_BYTE, //tipo de los datos
                    pixmap2Imagen); //origen
    }
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

void BMPRGBA::mediaPonderada(){
    //c(i,j) = ka(i,j) + (1-k)b(i,j), k [0,1]
    int count;
    int n = 0;
    double k = 0.5;

    for(int i=0; i < nRows; i++){
        //k = n/nRows;
        for(int j=0; j < nCols; j++){
            count=i*nCols + j;
            pixmap[count][0] = k * pixmap[count][0] + (1-k) * pixmap2Imagen[count][0];
            pixmap[count][1] = k * pixmap[count][1] + (1-k) * pixmap2Imagen[count][1];
            pixmap[count][2] = k * pixmap[count][2] + (1-k) * pixmap2Imagen[count][2];
        }
        n++;
    }
}

void BMPRGBA::diferencia(){
    //(0.299 * px.red + 0.586 * px.green + 0.114 * px.blue)
    int count, dif;
    double luma;

    //Bucle para pasar a grises
    for(int i=0; i < nRows; i++){
        for(int j=0; j < nCols; j++){
            count=i*nCols + j;
            luma = std::min(255.0, (pixmap[count][0] * 0.299 + pixmap[count][1] * 0.586 + pixmap[count][2] * 0.144));
            pixmap[count][0] = luma;
            pixmap[count][1] = luma;
            pixmap[count][2] = luma;

            luma = std::min(255.0, (pixmap2Imagen[count][0] * 0.299 + pixmap2Imagen[count][1] * 0.586 + pixmap2Imagen[count][2] * 0.144));
            pixmap2Imagen[count][0] = luma;
            pixmap2Imagen[count][1] = luma;
            pixmap2Imagen[count][2] = luma;
        }
    }

    //Bucle para la diferencia
    for(int i=0; i < nRows; i++){
        for(int j=0; j < nCols; j++){
            count=i*nCols + j;
            dif = abs(pixmap[count][0] - pixmap2Imagen[count][0]);
            pixmap[count][0] = dif;
            pixmap[count][1] = dif;
            pixmap[count][2] = dif;
        }
    }
}

void BMPRGBA::rotate(GLfloat xLeft, GLfloat yBot){
    int count;
    int newPosition;
    //Calculate the center of the image. It will be the center of the rotation
    GLfloat xCenter = (xLeft + nCols) / 2;
    GLfloat yCenter = (yBot + nRows) / 2;

    for(int i=0; i < nRows; i++){
        for(int j=0; j < nCols; j++){
            count=i*nCols + j;
            //distance
            GLfloat xDist = j - xCenter;
            GLfloat yDist = i - yCenter;
            //Angle
            GLfloat angle;
            if(xDist == 0 && yDist == 0)
                angle = 0;
            else
                angle = atan2(xDist, yDist) - M_PI_2;
            //Length
            GLfloat length = sqrt(xDist * xDist + yDist * yDist);

            newPosition = (yCenter + length * sin(angle))*nCols + (xCenter + length * cos(angle));
            //ShowMessage(IntToStr(count) + " " + IntToStr(newPosition));
            //if(count == 45299)
            //    ShowMessage(IntToStr(newPosition));
            //pixmap[count][0] = pixmap[newPosition][0];
            //pixmap[count][1] = pixmap[newPosition][1];
            //pixmap[count][2] = pixmap[newPosition][2];
        }
    }
    //La var count al final del bucle es 89999. La cosa se jode a la mitad de la imagen (45299) al final de la fila
    ShowMessage(count + " " + IntToStr(newPosition));

}

void BMPRGBA::negative(){
    int count;
    for(int i=0; i < nRows; i++){
        for(int j=0; j < nCols; j++){
            count=i*nCols + j;
            pixmap[count][0] = 255 - pixmap[count][0];
            pixmap[count][1] = 255 - pixmap[count][1];
            pixmap[count][2] = 255 - pixmap[count][2];
        }
    }
}



