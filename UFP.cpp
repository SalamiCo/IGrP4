//Samuel Mendez Galán
//Pedro Morgado Alarcón

//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "UFP.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TGLForm2D *GLForm2D;
//---------------------------------------------------------------------------
__fastcall TGLForm2D::TGLForm2D(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TGLForm2D::FormCreate(TObject *Sender)
{
    hdc = GetDC(Handle);
    SetPixelFormatDescriptor();
    hrc = wglCreateContext(hdc);
    if(hrc == NULL)
        ShowMessage(":-)~ hrc == NULL");
    if(wglMakeCurrent(hdc, hrc) == false)
        ShowMessage("Could not MakeCurrent");
    //Cor de fondo de la ventana
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    //inicialización del volumen de vista
    xRight=200.0; xLeft=-xRight;
    yTop=xRight; yBot=-yTop;
    //Radio del volumen de vista == 1

    //inicialización del puerto de vista
    //ClientWidth=400;
    //ClientHeight=400;
    RatioViewPort=1.0;

    //Scene inicialization
    this->tree = Tree();
    lvl = 0;
    baldosas = false;

    // inicialización de las variables del programa
    displacementeIncrease = 10;
    acumulateZoom = 1;
}
//---------------------------------------------------------------------------
void __fastcall TGLForm2D::SetPixelFormatDescriptor()
{
    PIXELFORMATDESCRIPTOR pfd = {
        sizeof(PIXELFORMATDESCRIPTOR),
        1,
        PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
        PFD_TYPE_RGBA,
        32,
        0,0,0,0,0,0,
        0,0,
        0,0,0,0,0,
        32,
        0,
        0,
        PFD_MAIN_PLANE,
        0,
        0,0,0
    };
    int PixelFormat = ChoosePixelFormat(hdc, &pfd);
    SetPixelFormat(hdc, PixelFormat, &pfd);
}
//---------------------------------------------------------------------
void __fastcall TGLForm2D::FormResize(TObject *Sender)
{
    if ((ClientWidth<=1)||(ClientHeight<=1)){
        ClientWidth=400;
        ClientHeight=400;
    }

    glViewport(0,0,ClientWidth,ClientHeight);

    centerX = (xLeft + xRight) / 2.0;
    xLeft = centerX - ((ClientWidth*acumulateZoom) / 2.0);
    xRight = centerX + ((ClientWidth*acumulateZoom) / 2.0);

    centerY = (yBot + yTop) / 2.0;
    yBot = centerY - ((ClientHeight*acumulateZoom) / 2.0);
    yTop = centerY + ((ClientHeight*acumulateZoom) / 2.0);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(xLeft,xRight,yBot,yTop);


    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    GLScene();
}
//---------------------------------------------------------------------------
void __fastcall TGLForm2D::GLScene()
{
    glClear(GL_COLOR_BUFFER_BIT);

    if(baldosas){
        Embaldosar(atoi(input.c_str()));
    } else {
        // Draw the scene
        tree.DrawTree(selectedSquare);
    }

    glFlush();
    SwapBuffers(hdc);
}

//---------------------------------------------------------------------------
void __fastcall TGLForm2D::FormPaint(TObject *Sender)
{
    GLScene();
}
//---------------------------------------------------------------------------
void __fastcall TGLForm2D::FormDestroy(TObject *Sender)
{
    ReleaseDC(Handle,hdc);
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(hrc);
}
//---------------------------------------------------------------------------

void __fastcall TGLForm2D::FormKeyPress(TObject *Sender, char &Key)
{
    GLdouble f = 0.9;
    GLdouble width, height, centerX, centerY;

    switch(Key){
    //Left
    case 'a':
        xLeft += displacementeIncrease;
        xRight += displacementeIncrease;
        break;
    //Up
    case 'w':
        yBot -= displacementeIncrease;
        yTop -= displacementeIncrease;
        break;
    //Down
    case 's':
        yBot += displacementeIncrease;
        yTop += displacementeIncrease;
        break;
    //Right
    case 'd':
        xLeft -= displacementeIncrease;
        xRight -= displacementeIncrease;
        break;

    //Zoom ++
    case '+':
        width = xRight - xLeft;
        height = yTop - yBot;
        centerX = (xRight + xLeft) / 2;
        centerY = (yTop + yBot) / 2;
        acumulateZoom *= f;

        xLeft = (centerX-width*f/2);
        xRight = (centerX+width*f/2);
        yBot = (centerY-height*f/2);
        yTop = (centerY+height*f/2);
        break;
    //Zoom --
    case '-':
        width = xRight - xLeft;
        height = yTop - yBot;
        centerX = (xRight + xLeft) / 2;
        centerY = (yTop + yBot) / 2;
        acumulateZoom *= f;

        xLeft = (centerX-width/f/2);
        xRight = (centerX+width/f/2);
        yBot = (centerY-height/f/2);
        yTop = (centerY+height/f/2);
        break;

    //Tree ++
    case 'z':
        lvl++;
        tree.AddLevel();
        break;

    // Tree--
    case 'x':
        if(lvl!=0){
            lvl--;
            tree.UndoLevel();
        }
        break;

    // Baldosas
    case 'b':
        baldosas = true;
        input = InputBox("Numero de columnas para embaldosado", "Introduzca el numero de columnas deseadas (>1): ", "3"); //3 por defecto
        if(input < 2) input = 3;
        break;

    case 'n':
        baldosas = false;
        Desembaldosar();
        break;
    };

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(xLeft,xRight,yBot,yTop);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    GLScene();
}
//---------------------------------------------------------------------------

void __fastcall TGLForm2D::FormMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    switch(Button){
        //Left button
        case 0:
            // (0,0) is in up-left corner. X grows to the right, Y to the bottom
            int newX, newY;

            newX = (int) (((xRight - xLeft)*X)/ClientWidth) + xLeft;
            newY = (int) yTop - (((yTop - yBot)*Y)/ClientHeight);

            //ShowMessage("Clicked at (" + IntToStr(newX) + ", " + IntToStr(newY) + ")");
            this->tree = Tree(newX, newY);
            break;

        //Right button
        case 1:
            GLdouble w = (ClientWidth / (xRight - xLeft));
            GLdouble h = (ClientHeight / (yTop - yBot));
            GLdouble xp = xLeft + (X / w);
            GLdouble yp = yTop - (Y / h);
            selectedSquare = tree.SelectSquare(xp,yp);
            break;
    };

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(xLeft,xRight,yBot,yTop);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    GLScene();
}
//---------------------------------------------------------------------------

void TGLForm2D::Embaldosar(int nCol){
    GLdouble SAVratio = (xRight - xLeft) / (yTop - yBot);
    GLdouble w = (GLdouble) ClientWidth / (GLdouble) nCol;
    GLdouble h = w / SAVratio;

    for(GLint c=0; c<nCol; c++){
        GLdouble currentH = 0;
        while((currentH + h) < ClientHeight){
            glViewport((GLint)(c*w), (GLint)currentH, (GLint)w, (GLint)h);
            // Draw the scene
            tree.DrawTree(selectedSquare);
            currentH += h;
        }
    }
}

void TGLForm2D::Desembaldosar(){
    glViewport(0,0,ClientWidth,ClientHeight);
}
