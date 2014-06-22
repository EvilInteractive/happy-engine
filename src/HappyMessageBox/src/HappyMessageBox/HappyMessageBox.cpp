//HappyEngine Copyright (C) 2011 - 2014  Evil Interactive
//
//This file is part of HappyEngine.
//
//    HappyEngine is free software: you can redistribute it and/or modify
//    it under the terms of the GNU Lesser General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    HappyEngine is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU Lesser General Public License for more details.
//
//    You should have received a copy of the GNU Lesser General Public License
//    along with HappyEngine.  If not, see <http://www.gnu.org/licenses/>.
//
//Author:  Bastian Damman
//Created: 2013/03/16
#include "HappyMessageBox.h"
#include "MessageBoxWindow.h"
#ifdef USE_WEB
#include "Awesomium/STLHelpers.h"
#endif
#ifdef _WIN32
#include "stackwalk/StackWalker.h"
#endif

#include <sstream>
#include <assert.h>

namespace he {

std::string HappyMessageBox::s_HtmlPage("");
int HappyMessageBox::s_DefaultHeigth(512);
int HappyMessageBox::s_DefaultWidth(256);

HappyMessageBox::HappyMessageBox()
{
}


HappyMessageBox::~HappyMessageBox()
{
}

void HappyMessageBox::init( const char* htmlPage, const int width, const int height )
{
#ifdef USE_WEB
    assert(Awesomium::WebCore::instance() != nullptr); // Please initialize Webcore before calling this method
#endif
    s_HtmlPage = htmlPage;
    s_DefaultWidth = width;
    s_DefaultHeigth = height;
}

void HappyMessageBox::destroy()
{
}

HappyMessageBox::Button HappyMessageBox::showExt(const char* caption, const char* message, const Icon icon,
                    const char* button1, const char* button2, const char* button3)
{
    if (button1 == NULL)
        button1 = "";
    hmb::MessageBoxWindow window;
    window.init(s_HtmlPage.c_str(), s_DefaultWidth, s_DefaultHeigth, caption);
    window.addText("Message", message);
    window.addButton(button1);
    window.setIcon(icon);
    if (button2 != NULL) window.addButton(button2);
    if (button3 != NULL) window.addButton(button3);
    window.show(); // Blocking

    return window.getResult();
}
    
#ifdef _WIN32
class HappyStackWalker : public StackWalker
{
public:
    HappyStackWalker() {}
    void OnCallstackEntry(CallstackEntryType eType, CallstackEntry &entry)
    {
        CHAR buffer[STACKWALK_MAX_NAMELEN];
        if ( (eType != lastEntry) && (entry.offset != 0) )
        {
            if (entry.name[0] == 0)
                strcpy_s(entry.name, "(function-name not available)");
            if (entry.undName[0] != 0)
                strcpy_s(entry.name, entry.undName);
            if (entry.undFullName[0] != 0)
                strcpy_s(entry.name, entry.undFullName);
            if (entry.lineFileName[0] == 0)
            {
                strcpy_s(entry.lineFileName, "(filename not available)");
                if (entry.moduleName[0] == 0)
                    strcpy_s(entry.moduleName, "(module-name not available)");
                _snprintf_s(buffer, STACKWALK_MAX_NAMELEN, "%p (%s): %s: %s\n", (LPVOID) entry.offset, entry.moduleName, entry.lineFileName, entry.name);
            }
            else
            {
                // limit file depth
                int len(strlen(entry.lineFileName));
                size_t counter(0);
                size_t offset(0);
                for (int i(len - 1); i >= 0; --i)
                {
                    if (entry.lineFileName[i] == '\\' || entry.lineFileName[i] == '/')
                    {
                        ++counter;
                        if (counter == 3)
                        {
                            offset = i;
                            break;
                        }
                    }
                }
                _snprintf_s(buffer, STACKWALK_MAX_NAMELEN, "...%s (%d): %s\n", entry.lineFileName + offset, entry.lineNumber, entry.name);
            }
            m_Buffer << buffer;
        }
    }

    const std::stringstream& getBuffer() { return m_Buffer; }

private:
    std::stringstream m_Buffer;
};
#endif
    

HappyMessageBox::Button HappyMessageBox::showAssert( const char* caption, const char* message, const Icon icon /*= Icon_ProgrammerAssert*/, const char* button1/*="Debug"*/, const char* button2/*="Skip"*/, const char* button3/*="Ignore"*/ )
{
    std::string callstack("Unable to trace stack!");
#ifdef _WIN32
    HappyStackWalker w;
    w.ShowCallstack();
    callstack = w.getBuffer().str();
#endif

    if (button1 == NULL)
        button1 = "";

    hmb::MessageBoxWindow window;
    window.init(s_HtmlPage.c_str(), s_DefaultWidth, s_DefaultHeigth, caption);
    window.addText("Message", message);
    window.addText("StackTrace", callstack.c_str());
    window.addButton(button1);
    window.setIcon(icon);
    if (button2 != NULL) window.addButton(button2);
    if (button3 != NULL) window.addButton(button3);
    window.show(); // Blocking

    return window.getResult();
}




} //end namespace
