function addNodeSubdivision(name)
{
    var container = document.getElementById("nodeSelectPanel");
    var li = document.createElement("li");
    var div = document.createElement("div");
    var h3 = document.createElement("h3");
    var ul = document.createElement("ul");

    li.classList.add("nodeSelectPanelSubDContainer");
    div.classList.add("nodeSelectPanelSubdivision");
    h3.innerHTML = name;
    ul.id = name;

    container.appendChild(li);
    li.appendChild(div);
    div.appendChild(h3);
    div.appendChild(ul);
}

function addNode(sub, itemName)
{
    var ul = document.getElementById(sub);
    var li = document.createElement("li");
    li.innerHTML=itemName;
    li.id=itemName;
    li.classList.add("node");
    ul.appendChild(li);
}

function init()
{
    $( ".nodeSelectPanelSubDContainer" )
        .accordion({
            header: "> div > h3",
            collapsible: true,
            heightStyle: "content"
        });

    $( ".node" ).draggable({
        containment: "window",
        scroll: false,
        helper: "clone"
    });
}

function resizeBottomPanel()
{
    var bottomPanel = $("#bottomPanel");
    bottomPanel.position({
        "my": "left bottom",
        "at": "right bottom",
        "of": "#sidePanel"
    });
    bottomPanel.width($(document).width() - $("#sidePanel").width() - 4);
}

function resizePropertyColumns()
{
    var column = $("#propertyValueColumn");
    var firtColumn = $("#propertyNameColumn");
    column.position({
        "my": "left top",
        "at": "right top",
        "of": "#propertyNameColumn"
    });
    column.width(firtColumn.right)
}
function clearProperties()
{

}

function addProperty(name, node)
{
    var container = document.getElementById("properties");
    var li = document.createElement("li");
    var div = document.createElement("div");
    var h3 = document.createElement("h3");
    var ul = document.createElement("ul");
}

function addFloatProperty(name, defaultValue)
{
    var input = document.createElement("input");
    input.type="spinner";
    input.id=name;
    input.value=defaultValue;
    addProperty(input);

    $( "#"+name ).spinner({
        step: 0.01,
        numberFormat: "n"
    })
}
function addComboBoxProperty(name, array)
{
   /* <input type=file list=browsers >
    <datalist id=browsers >
        <option> Google
        <option> IE9
    </datalist>*/
}
function addBoolProperty(name, defaultValue)
{
    var input = document.createElement("input");
    input.type="checkbox";
    input.id=name;
    input.value=defaultValue;
    addProperty(name, input);
}

function nodeDropped(node)
{
    //console.log(node);
    HME.nodeDropped(node);
}

$(function() {

    // Test
    /*addNodeSubdivision("Math with space");
    addNode("Math with space", "Test1");
    addNode("Math with space", "Test2");

    addNodeSubdivision("Math2");
    addNode("Math2", "Test2");*/

    $("#sidePanel")
        .resizable({
            maxWidth: 440,
            minWidth: 64,
            handles: "e",
            resize: function(event, ui) { resizeBottomPanel(); }
        });

    $("#bottomPanel")
        .resizable({
            maxHeight: 440,
            minHeight: 64,
            handles: "n"
        })
        .position({
            "my": "left bottom",
            "at": "right bottom",
            "of": "#sidePanel"
        })
        .tabs({
    });
    $("#propertyNameColumn")
        .resizable({
            minWidth: 64,
            handles: "e",
            resize: function(event, ui) { resizePropertyColumns(); }
        });

    $( "#nodeSelectPanel" )
        .droppable({
            tolerance: "pointer",
            greedy: true
        })
        .disableSelection(
    );

    $("#scene").droppable({
        tolerance: "pointer",
        greedy: true,
        drop: function( event, ui ) {
            nodeDropped($(ui.draggable).attr("id"));
        }
    });

    init();
    resizeBottomPanel();
});