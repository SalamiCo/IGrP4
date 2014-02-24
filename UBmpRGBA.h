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
#include <windows.h>
using namespace std;
//---------------------------------------------------------------------------
typedef GLubyte colorRGBA[3];
typedef struct
{
        unsigned char iD[2];      // the magic number used to identify the BMP file: 0x42 0x4D (ASCII code points for B and M)
        unsigned int filesize;   // the size of the BMP file in bytes
        unsigned short reserved1;  // reserved; actual value depends on the application that creates the image
        unsigned short reserved2;  // reserved; actual value depends on the application that creates the image
        unsigned int dataOffset;  // the offset, i.e. starting address, of the byte where the bitmap data can be found.
} myBitmapHeader;
 
typedef struct
{
        long size;              //the size of this header (40 bytes)
        long bm_width;          //the bitmap width in pixels (signed integer).
        long bm_height;         //the bitmap height in pixels (signed integer).
        short  colPlanes;         //the number of color planes being used. Must be set to 1.
        short  bitsPerPixel;      //the number of bits per pixel, which is the color depth of the image. Typical values are 1, 4, 8, 16, 24 and 32.
        long compressionType;   //the compression method being used. See the next table for a list of possible values.
        long imageSize;         //the image size. This is the size of the raw bitmap data (see below), and should not be confused with the file size.
        long horRes;            //the horizontal resolution of the image. (pixel per meter, signed integer)
        long verRes;            //the vertical resolution of the image. (pixel per meter, signed integer)
        long paletteCols;       //the number of colors in the color palette, or 0 to default to 2n.
        long importantCols;     //the number of important colors used, or 0 when every color is important; generally ignore
} dibHeader;

class BMPRGBA{
        private:

                GLuint nCols;
                GLuint nRows;
                colorRGBA* pixmap;
                colorRGBA* pixmap2Imagen;
                int queImagen; // Si 1, imagen por defecto. Si 2, imagen para ops aritméticas
                double mascara[9];

                void recoloca(Graphics::TBitmap* AuxBMP, int queImagen);
                void bilinearInterpolation(double newPositionX, double newPositionY);
                void getGaussianMascara();
                void colorBlur(int x, int y);

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
                void mediaPonderada(double k);
                void diferencia();
                void rotate(GLfloat angle, int x, int y);
                int bilinearInterpolation(double newPositionX, double newPositionY, int rgb);
                void gaussianBlur();
                void negative();
};


#endif
 