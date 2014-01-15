//Samuel Mendez Galán
//Pedro Morgado Alarcón
//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
USERES("glSkeleton.res");
USEFORM("UFP.cpp", GLForm2D);
USEUNIT("Tree.cpp");
USEUNIT("Square.cpp");
USEUNIT("PointCoord.cpp");
USEUNIT("Pencil.cpp");
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
        try
        {
                 Application->Initialize();
                 Application->CreateForm(__classid(TGLForm2D), &GLForm2D);
                 Application->Run();
        }
        catch (Exception &exception)
        {
                 Application->ShowException(&exception);
        }
        return 0;
}
//---------------------------------------------------------------------------
