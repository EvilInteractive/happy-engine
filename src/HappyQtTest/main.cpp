#include "HeAssert.h"
#define assert ASSERT

#include "boost/shared_ptr.hpp"

#include "HappyEngine.h"
#include "Texture2D.h"

#include "HappyNew.h"

#include <iostream>

#include "HappyQtTest.h"
#pragma warning(disable:4127)
#include <QtGui/QApplication>
#pragma warning(default:4127)

int main(int argc, char** argv)
{
    using namespace he;

    HAPPYENGINE->init(SubEngine_All | SubEngine_Qt);  

    /*qt::*/QApplication a(argc, argv);
    HappyQtTest w;
    w.show();
     
    HAPPYENGINE->start(w.getGameWidget());
    HAPPYENGINE->dispose();

    std::cout << "\nallocated textures: " << gfx::Texture2D::getTextureCount() << "\n";

    #ifdef _DEBUG
    _CrtDumpMemoryLeaks();
    #endif

    std::cout << "\npress enter to quit\n";
    std::cin.get();

    return 0;
}