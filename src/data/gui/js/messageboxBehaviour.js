
function onButtonClicked(name)
{
    HE.onButtonClicked(name);
}

function addTab(name, text)
{
    var bodyContainer = document.getElementById("tabBodyContainer");
    var newDiv = document.createElement("div");
    newDiv.id = "tab-" + name;
    newDiv.classList.add("tab");
    var newText = document.createElement("textarea");
    newText.innerHTML = text;
    newText.classList.add("textBoxMessage");
    newDiv.appendChild(newText);
    bodyContainer.appendChild(newDiv);

    $("#tabContainer").tabs("add", "#tab-" + name, name);
    $("#tabContainer").tabs( "option", "active", 0 );
}

function addButton(name)
{
    var table = document.getElementById("buttonTable");
    var td = document.createElement("td");
    td.classList.add("buttonCell");
    var button = document.createElement("button");
    button.type = "button";
    button.classList.add("button");
    button.id = "btn_" + name;
    button.innerHTML = name;

    table.appendChild(td);
    td.appendChild(button);

    $( "#"+ "btn_" + name )
        .button()
        .click(function( event ) {
            event.preventDefault();
            onButtonClicked(name);
        });
}

$(function()
{
    $("#tabContainer").tabs({
        active: 0
    });

    //addButton("Test1");
    //addButton("Test2");
    //addButton("Test3");

    //addTab("Test", "Test\nTextje")
    //addTab("Test3", "Test\nTextje")
    //addTab("Test2", "Test\nTextje")


    //setMessageText("Blah\ntest\ntest\n\n\n\n\nsdnfjfbvdvbdffgfgdhfhuvshbvdsabgvydfgvbyadgvbyuagvadhgvuaghaufdahvfaudi");
});