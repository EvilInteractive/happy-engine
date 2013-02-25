
function onButtonClicked(name)
{
    HE.onButtonClicked(name);
}

function setMessageText(text)
{
    var area = document.getElementById("textBoxMessage");
    area.innerHTML = text;
}
function setInfoText(text)
{
    var area = document.getElementById("textBoxInfo");
    area.innerHTML = text;
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

    //setMessageText("Blah\ntest\ntest\n\n\n\n\nsdnfjfbvdvbdffgfgdhfhuvshbvdsabgvydfgvbyadgvbyuagvadhgvuaghaufdahvfaudi");
});