//---------------------------------------------------------------------------

#ifndef UBmpRGBAH
#define UBmpRGBAH
//---------------------------------------------------------------------------
#include <gl\gl.h>
#include <gl\glu.h>
#include <string>
using namespace std;
//---------------------------------------------------------------------------
typedef GLubyte colorRGBA[3];

class BMPRGBA{
        private:

                GLuint nCols;
                GLuint nRows;
                colorRGBA* pixmap;

                void recoloca(Graphics::TBitmap* AuxBMP);

        public:
                BMPRGBA(){pixmap=NULL; nCols=0; nRows=0;};
                BMPRGBA(const string& arch);
                ~BMPRGBA(){};
                GLuint getCols(){return nCols;};
                GLuint getRows(){return nRows;};
                colorRGBA* getBMP(){return pixmap;};
                bool cargaBMP(const string& arch);
};


#endif
 