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

$(function() {

    // Test
    /*addNodeSubdivision("Math with space");
    addNode("Math with space", "Test1");
    addNode("Math with space", "Test2");

    addNodeSubdivision("Math2");
    addNode("Math2", "Test2");*/

    $( "#nodeSelectPanel" )
        .resizable({
            maxWidth: 440,
            minWidth: 64,
            handles: "e"
        })
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
        $( this )
            //HE.nodeDropped();
        }
    });

    init();
});