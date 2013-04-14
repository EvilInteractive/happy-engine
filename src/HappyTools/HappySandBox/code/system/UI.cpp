#include "HappySandBoxPCH.h" 
#include "UI.h"

namespace hs {

UI::UI()
    : m_IsVisible(false)
{

}

UI::~UI()
{
}

void UI::show()
{
    HE_ASSERT(m_IsVisible == false, "Showing already visible UI!");
    m_IsVisible = true;
}

void UI::hide()
{
    HE_ASSERT(m_IsVisible == true, "Hiding already hidden UI!");
    m_IsVisible = false;
}

} //end namespace
