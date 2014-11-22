
addFilter(HAPPYSANDBOX_CODE_SRCS src/forms code/forms/
        GameWidget.cpp              GameWidget.h
        MainWindow.cpp              MainWindow.h                MainWindow.ui
        RenderWidget.cpp            RenderWidget.h        
        Resources.qrc
    )

addFilter(HAPPYSANDBOX_CODE_SRCS src/forms/nodeGraph code/forms/nodegraph/
        NodeGraph.cpp                   NodeGraph.h
        NodeGraphNode.cpp               NodeGraphNode.h
        NodeGraphEnums.h
        
        NodeGraphNodeAttachment.cpp     NodeGraphNodeAttachment.h
        NodeGraphNodeAttachments.cpp    NodeGraphNodeAttachments.h
        
        NodeGraphNodeConnector.cpp      NodeGraphNodeConnector.h
    )
addFilter(HAPPYSANDBOX_CODE_SRCS src/forms/entity code/forms/
        EntityPropertiesForm.cpp        EntityPropertiesForm.h          EntityPropertiesForm.ui
    )
    

addFilter(HAPPYSANDBOX_CODE_SRCS src/forms/property code/forms/
        PropertyList.cpp          PropertyList.h            PropertyList.ui
        PropertyListItem.cpp      PropertyListItem.h
        EntityPropertyList.cpp	  EntityPropertyList.h
)

addFilter(HAPPYSANDBOX_CODE_SRCS src/forms/property/feels code/forms/
        PropertyFeel.cpp              PropertyFeel.h
        PropertyColorFeel.cpp         PropertyColorFeel.h           PropertyColorFeel.ui
        PropertyDefaultFeel.cpp       PropertyDefaultFeel.h         PropertyDefaultFeel.ui
        PropertyDropDownFeel.cpp      PropertyDropDownFeel.h        PropertyDropDownFeel.ui
        PropertySliderFeel.cpp        PropertySliderFeel.h          PropertySliderFeel.ui
        PropertyCheckBoxFeel.cpp      PropertyCheckBoxFeel.h        PropertyCheckBoxFeel.ui
    )