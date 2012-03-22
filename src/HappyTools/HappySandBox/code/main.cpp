#include "HeAssert.h"
#define assert ASSERT

#include "boost/shared_ptr.hpp"

#include "HappyEngine.h"
#include "Texture2D.h"

#include "HappyNew.h"

#include <iostream>

#include "HappySandBox.h"
#pragma warning(disable:4127)
#include <QtGui/QApplication>
#include <QPlastiqueStyle>
#pragma warning(default:4127)

int main(int argc, char** argv)
{
    #ifdef _DEBUG
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    #endif
    using namespace he;
    {
        HAPPYENGINE->init(SubEngine_All | SubEngine_Qt);  
		QApplication::setStyle(new QPlastiqueStyle);

        QApplication pApp(argc, argv);
        pApp.setQuitOnLastWindowClosed(true);
        HappySandBox pWindow;
        pWindow.setAttribute(Qt::WA_QuitOnClose);
        pWindow.show();
     
        HAPPYENGINE->start(pWindow.getGameWidget());
    }
    HAPPYENGINE->dispose();
    std::cout << "\nallocated textures: " << gfx::Texture2D::getTextureCount() << "\n";
    /*#ifdef _DEBUG
    _CrtDumpMemoryLeaks();
    #endif*/

    std::cout << "\npress enter to quit\n";
    std::cin.get();

    return 0;
}
