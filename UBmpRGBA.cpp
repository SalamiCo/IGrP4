#include <vcl.h>
#pragma hdrstop

#include "UBmpRGBA.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

BMPRGBA::BMPRGBA(const string& arch){
    pixmap=NULL; nCols=0; nRows=0;
    pixmap2Imagen=NULL; queImagen=1;
    for(int i=0; i < 9; i++){
        mascara[i] = 0;
    }
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

void BMPRGBA::mediaPonderada(double k){
    //c(i,j) = ka(i,j) + (1-k)b(i,j), k [0,1]
    int count;
    int n = 0;

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

void BMPRGBA::rotate(GLfloat angle, int x, int y){
    int count;
    int newPosition;
    colorRGBA* auxiliar = new colorRGBA[nCols * nRows];
    //Coordenates where the image is rotating
    GLfloat xCenter = (nCols / 2) + x;
    GLfloat yCenter = (nRows / 2) + y;
    //ShowMessage("X: "+ IntToStr((int)xCenter) + "Y: " + IntToStr((int)yCenter));

    for(int i=0; i < nRows; i++){
        for(int j=0; j < nCols; j++){
            count=i*nCols + j;
            //distance
            GLfloat xDist = j - xCenter;
            GLfloat yDist = i - yCenter;
            if(xDist != 0 && yDist != 0){
                //Angle
                GLfloat ang = atan2(yDist, xDist) - angle;
                //Length
                GLfloat length = sqrt(xDist * xDist + yDist * yDist);

                double newPositionX = xCenter + length * cos(ang);
                double newPositionY = yCenter + length * sin(ang);
                    
                if(newPositionX < nRows && newPositionY < nCols && newPositionX >= 0 && newPositionY >= 0){
                    //newPosition = newPositionY*nCols + newPositionX;
                    //auxiliar[count][0] = pixmap[newPosition][0];
                    //auxiliar[count][1] = pixmap[newPosition][1];
                    //auxiliar[count][2] = pixmap[newPosition][2];
                    auxiliar[count][0] = bilinearInterpolation(newPositionX, newPositionY, 0);
                    auxiliar[count][1] = bilinearInterpolation(newPositionX, newPositionY, 1);
                    auxiliar[count][2] = bilinearInterpolation(newPositionX, newPositionY, 2);
                } else {
                    auxiliar[count][0] = 0;
                    auxiliar[count][1] = 0;
                    auxiliar[count][2] = 0;
                }
            }
             
        }
    }

    delete[] pixmap;
    pixmap = auxiliar;

}

int BMPRGBA::bilinearInterpolation(double newPositionX, double newPositionY, int rgb){
    int pos, pos2, pos3, pos4;
    double decimalX, decimalY;
    int newPosition = (int)newPositionY*nCols + (int)newPositionX;

    //Parte_decimal = numero-parte_entera
    decimalX = newPositionX - (int)newPositionX;
    decimalY = newPositionY - (int)newPositionY;

    pos = ((1 - decimalX)*(1 - decimalY)*pixmap[newPosition][rgb]); //pixmap(i,j)
    if(newPosition+1 >= getSize()){
        pos2 = 0;
    } else {
        pos2 = ((1 - decimalX) * decimalY * pixmap[newPosition + 1][rgb]); //pixmap(i, j+1)
    }
    if(newPosition+nCols+1 >= getSize()){
        pos3 = 0;
    } else {
        pos3 = (decimalX * decimalY * pixmap[newPosition + nCols + 1][rgb]); //pixmap(i+1, j+1)
    }
    if (newPosition+nCols >= getSize()){
        pos4 = 0;
    } else {
        pos4 = (decimalX * (1 - decimalY) * pixmap[newPosition + nCols][rgb]); //pixmap(i+1, j)
    }

    return pos + pos2 + pos3 + pos4;
    
}

void BMPRGBA::gaussianBlur(){
    getGaussianMascara();
    for(int i=0; i < nRows; i++){
        for(int j=0; j < nCols/2; j++){
            colorBlur(i,j);
        }
    }
}

void BMPRGBA::getGaussianMascara(){
    int m = 1;
    double sigma = 1.5;
    int count = 0;
    double num, den = 2*(sigma*sigma);
    double sum = 0.0; 

    for(int j=-m; j <= m; j++){
        for (int i=m; i >=- m; i--){
            num = i*i + j*j;
            mascara[count] = exp(-num / den) / (den*M_PI);
            sum += mascara[count];
            count++;
        }
    }

    for(int k=0; k < 9; k++){
        mascara[k] /= sum;
    }
}

void BMPRGBA::colorBlur(int x, int y){
    int count = x*nCols + y;

    int w0 = count + nCols - 1;
    int w1 = count + nCols;
    int w2 = count + nCols + 1;
    int w3 = count - 1;
    int w4 = count;
    int w5 = count + 1;
    int w6 = count - nCols - 1;
    int w7 = count - nCols;
    int w8 = count - nCols + 1;

    double r = mascara[4] * pixmap[w4][0];
    double g = mascara[4] * pixmap[w4][1];
    double b = mascara[4] * pixmap[w4][2];

    if(x < nRows-1 && y > 0){
        r += mascara[0] * pixmap[w0][0];
        g += mascara[0] * pixmap[w0][1];
        b += mascara[0] * pixmap[w0][2];
    }

    if(y > 0 && x < nRows-1){
        r += mascara[1] * pixmap[w1][0];
        g += mascara[1] * pixmap[w1][1];
        b += mascara[1] * pixmap[w1][2];
    }
    if(x < nRows-1 && y < nCols-1){
        r += mascara[2] * pixmap[w2][0];
        g += mascara[2] * pixmap[w2][1];
        b += mascara[2] * pixmap[w2][2];
    }
    if(y > 0){
        r += mascara[3] * pixmap[w3][0];
        g += mascara[3] * pixmap[w3][1];
        b += mascara[3] * pixmap[w3][2];
    }

    if(y < nCols-1){
        r += mascara[5] * pixmap[w5][0];
        g += mascara[5] * pixmap[w5][1];
        b += mascara[5] * pixmap[w5][2];
    }
    if(x > 0 && y > 0){
        r += mascara[6] * pixmap[w6][0];
        g += mascara[6] * pixmap[w6][1];
        b += mascara[6] * pixmap[w6][2];
    }
    if(x < nRows-1 && x > 0){
        r += mascara[7] * pixmap[w7][0];
        g += mascara[7] * pixmap[w7][1];
        b += mascara[7] * pixmap[w7][2];
    }
    if(x > 0 && y < nCols-1){
        r += mascara[8] * pixmap[w8][0];
        g += mascara[8] * pixmap[w8][1];
        b += mascara[8] * pixmap[w8][2];
    }

    pixmap[count][0] = r;
    pixmap[count][1] = g;
    pixmap[count][2] = b;
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