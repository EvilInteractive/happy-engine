#include "HappySandBoxPCH.h"
#include "EntityComponentFrame.h"
#include "ui_EntityComponentFrame.h"

namespace hs {

EntityComponentFrame::EntityComponentFrame(QWidget *parent)
    : QWidget(parent)
    , m_UI(HENew(Ui::EntityComponentFrame))
    , m_Header(nullptr)
    , m_Content(nullptr)
{
    m_UI->setupUi(this);

    connect(m_UI->m_BtnExpand, SIGNAL(clicked(bool)), this, SLOT(expandPressed(bool)));
}

EntityComponentFrame::~EntityComponentFrame()
{
    HEDelete(m_UI);
    HEDelete(m_Header);
    HEDelete(m_Content);
}

void EntityComponentFrame::setHeader( QWidget* header )
{
    m_Header = header;
    m_UI->m_HeaderFrame->layout()->addWidget(header);
}

void EntityComponentFrame::setContent( QWidget* content )
{
    m_Content = content;
    m_UI->m_ContentFrame->layout()->addWidget(content);
}

void EntityComponentFrame::setExpanded( const bool expand )
{
    m_UI->m_ContentFrame->setVisible(expand);
}

void EntityComponentFrame::expandPressed(const bool val)
{
    setExpanded(val);
    m_UI->m_BtnExpand->setText(val ? "|" : "+");
    resizeToFit();
}

void EntityComponentFrame::resizeToFit()
{
    int ctH(m_Content->minimumHeight());
    ctH += m_UI->m_ContentFrame->contentsMargins().top();
    ctH += m_UI->m_ContentFrame->contentsMargins().bottom();
    m_UI->m_ContentFrame->setMinimumHeight(ctH);
    m_UI->m_ContentFrame->setMaximumHeight(ctH);

    int fullHeight(m_UI->m_ContentFrame->isVisible() ? ctH : 0);
    fullHeight += contentsMargins().top();
    fullHeight += contentsMargins().bottom();
    fullHeight += m_UI->m_Header->minimumHeight() + 2;
    setMinimumHeight(fullHeight);
    setMaximumHeight(fullHeight);
}

}
