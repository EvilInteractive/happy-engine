function LOG(str)
{
    console.log(str);
}


$(function()
{
    // Globals
    var g_Nodes = new Array();
    var g_Graph = $("#graph");
    var g_ZIndex = 0;
    var g_ShiftPressed = false;
    var g_CtrlPressed = false;
    var g_DragHelper;
    var g_LastClickedNode;

    // Init
    Init();

    function Init()
    {
        // Resize graph with document
        $(window).resize(function(ev)
        {
            LOG(ev);
            Resize();
        });
        // Do initial sizing
        Resize();

        InitSelecting();
        
        new Node(64, 64);
        new Node(200, 64);
        new Node(400, 64);
    }

    function Resize()
    {
        var win = $( window );
        var w = win.width();
        var h = win.height();
        g_Graph.css(
        {
            "width" : w,
            "height" : h
        });
    }

    function InitSelecting()
    {
        g_Graph.selectable();
    }

    function SelectNode(node, deselect)
    {        
        // add unselecting class to all elements in the styleboard canvas except the ones to select
        if (deselect)
            $(".ui-selected").not(node).removeClass("ui-selected").addClass("ui-unselecting");

        // add ui-selecting class to the elements to select
        node.not(".ui-selected").addClass("ui-selecting");

        // trigger the mouse stop event (this will select all .ui-selecting elements, and deselect all .ui-unselecting elements)
        g_Graph.selectable('refresh');
        g_Graph.data("ui-selectable")._mouseStop(null);
    }

    // Dragging helper
    function GetContainmentNode()
    {
        var selected = $(".ui-selected");
        
        g_Graph.append("<div id='containmentNode'>Hello!</div>");
        var container = $("#containmentNode");


        var left = 10000000;
        var top = 10000000;
        var right = 0;
        var bottom = 0;

        selected.each(function(index, element)
        {
            var el = $(element);
            left = Math.min(left, el.position().left);
            top = Math.min(top, el.position().top);
            right = Math.max(right, el.width() + el.position().left);
            bottom = Math.max(bottom, el.height() + el.position().top);
        });

        var width = right - left + 6;
        var height = bottom - top + 6;

        container.css(
        {
            "position" : "absolute",
            "left" : left, "top" : top,
            "width" : width, "height" : height,
            "background-color" : "white",
            "padding" : "0px",
            "margin" : "0px",
        });

        // Make sure the offset is correct when multidragging
        var offsetX = container.position().left - g_LastClickedNode.position().left;
        var offsetY = container.position().top - g_LastClickedNode.position().top;
        container.css("left", "+="+offsetX);
        container.css("top", "+="+offsetY);

        // add selected elements to helper
        selected.each(function(index, element)
        {
            var el = $(element);
            el.detach().appendTo(container);
            el.css("left", el.position().left - left);
            el.css("top", el.position().top - top);
        });

        g_DragHelper = container;

        return container;
    }

    function IsSelected(node)
    {
        return $(node).hasClass("ui-selected");
    }

    function Node(x, y)
    {
        this.m_X = x;
        this.m_Y = y;
        this.m_Id = g_Nodes.length;
        ++g_ZIndex;

        g_Graph.append("<div class='node'>Hello!</div>");
        this.m_Element = $(".node").last();
        this.m_Element.css(
        {
            "position" : "absolute",
            "left" : this.m_X, "top" : this.m_Y,
            "width" : "128px", "height" : "64px",
            "border" : "1px solid gray",
            "background-color" : "white"
        });
        this.m_Element.mousedown(function(ev)
        {
            var deselect = IsSelected(this) == false && ev.shiftKey == false;
            SelectNode($(this), deselect);
            g_LastClickedNode = $(this);
            return false;
        });
        this.m_Element.click(function(ev)
        {
            var deselect = ev.shiftKey == false;
            SelectNode($(this), deselect);
        });
        this.m_Element.draggable(
        { 
            stack: ".node", 
            snap: $(".node"),
            snapMode : "both",
            snapTolerance: 10,
            opacity: 0.75,
            containment: g_Graph,
            distance:2,
            helper: function() { return GetContainmentNode(); },
            start: function(event, ui)
            {
                g_DragHelper.css("left", "-=200");
                ui.offset.top = 0;
                ui.offset.left = 0;
            },
            stop: function(event, ui)
            {
                var selected = $(".ui-selected");
                selected.each(function(index, element)
                {
                    var el = $(element);
                    el.detach().appendTo(g_Graph);
                    el.css("left", el.position().left + g_DragHelper.position().left);
                    el.css("top", el.position().top + g_DragHelper.position().top);
                
                });
                g_DragHelper = undefined;
            }
        });

        g_Nodes.push(this);

    }
});
