#ifndef ENTITYPROPERTYFEEL_H
#define ENTITYPROPERTYFEEL_H

namespace hs {

class PropertyFeel
{
public:
    PropertyFeel();
    virtual ~PropertyFeel() {}

    virtual void setValue(const he::String& value) = 0;
    virtual void setValueMixed() = 0;
    he::event1<void, const he::String&> ValueChanged;

    void setDirty(const bool dirty);
    bool isDirty() const { return m_Dirty; }
    virtual void onDirtyChanged(const bool newDirty) = 0;

private:
    bool m_Dirty;
};

}
#endif // ENTITYPROPERTYFEEL_H
