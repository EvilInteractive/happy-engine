
addFilter(HAPPYSANDBOX_CODE_SRCS src/forms code/forms/
        GameWidget.cpp              GameWidget.h
        MainWindow.cpp              MainWindow.h                MainWindow.ui
        Resources.qrc
    )

addFilter(HAPPYSANDBOX_CODE_SRCS src/forms/entity code/forms/
        EntityPropertiesForm.cpp        EntityPropertiesForm.h          EntityPropertiesForm.ui
        EntityPropertyList.cpp          EntityPropertyList.h            EntityPropertyList.ui
        EntityPropertyListItem.cpp      EntityPropertyListItem.h
    )
    
addFilter(HAPPYSANDBOX_CODE_SRCS src/forms/entity/feels code/forms/
        EntityPropertyFeel.cpp              EntityPropertyFeel.h
        EntityPropertyColorFeel.cpp         EntityPropertyColorFeel.h           EntityPropertyColorFeel.ui
        EntityPropertyDefaultFeel.cpp       EntityPropertyDefaultFeel.h         EntityPropertyDefaultFeel.ui
        EntityPropertyDropDownFeel.cpp      EntityPropertyDropDownFeel.h        EntityPropertyDropDownFeel.ui
        EntityPropertySliderFeel.cpp        EntityPropertySliderFeel.h          EntityPropertySliderFeel.ui
    )