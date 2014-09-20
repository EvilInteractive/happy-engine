
addFilter(HAPPYSANDBOX_CODE_SRCS src/forms code/forms/
        GameWidget.cpp              GameWidget.h
        MainWindow.cpp              MainWindow.h                MainWindow.ui
        RenderWidget.cpp            RenderWidget.h        
        Resources.qrc
    )

addFilter(HAPPYSANDBOX_CODE_SRCS src/forms/nodeGraph code/forms/
        NodeGraph.cpp                   NodeGraph.h
        NodeGraphNode.cpp               NodeGraphNode.h
        NodeGraphEnums.h
        NodeGraphNodeConnector.cpp      NodeGraphNodeConnector.h
        NodeGraphNodeInput.cpp          NodeGraphNodeInput.h
        NodeGraphNodeOutput.cpp         NodeGraphNodeOutput.h
        NodeGraphNodeDecoration.cpp     NodeGraphNodeDecoration.h
        INodeGraphNodeAttachment.h
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