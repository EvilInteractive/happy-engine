#ifndef _HS_UI_H_
#define _HS_UI_H_
#pragma once

namespace hs {

class UI
{
public:
    UI();
    virtual ~UI();
    
    virtual void load() = 0;
    virtual void unload() = 0;

    virtual void show();
    virtual void hide();

    bool isVisible() const { return m_IsVisible; }

private:
    bool m_IsVisible;
    
    //Disable default copy constructor and default assignment operator
    UI(const UI&);
    UI& operator=(const UI&);
};

} //end namespace

#endif
