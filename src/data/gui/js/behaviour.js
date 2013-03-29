// namespace
var MenuBehaviour = (function()
{
	// our public container
	var pub = {};
	
	// private vars
	var menuButtons = [];
	var dropdownMenus = [];

	// change to add menuButtons
	var nrButtons = 4;
	var menuButtonClicked = false;

	var selectedMenu = "";

	// main
	pub.init = function()
	{
		// get all buttons and menus of top bar
		for (var i = 0; i < nrButtons; ++i)
		{
			menuButtons['#menuButton' + i] = $('#menuButton' + i);
			dropdownMenus['#menuButton' + i] = $('#dropdownMenu' + i);
			
			// on click
			$('#menuButton' + i).on('click', function(e)
			{
				menuButtonClicked = true;
				
				// select new
				selectedMenu = '#' + e.target.id;
				
				// display the menu of that button
				dropdownMenus[selectedMenu].css('display', 'block');
				menuButtons[selectedMenu].addClass("selected");
			});
			
			// on hover
			$('#menuButton' + i).on('mouseover', function(e)
			{
				// this is only important if we have a menu open
				if (menuButtonClicked === false)
					return;
				
				// close previous
				if (selectedMenu !== "")
				{
					dropdownMenus[selectedMenu].css('display', 'none');
					menuButtons[selectedMenu].removeClass("selected");                
				}
				
				// select new
				selectedMenu = '#' + e.target.id;
				
				// display the menu of that button
				dropdownMenus[selectedMenu].css('display', 'block');
				menuButtons[selectedMenu].addClass("selected");
			});
		}
		
		// on menu leave
		$(document).on('click', function(e)
		{
			var targetId = '#' + e.target.id;
			var buttonClicked = false;
			
			// check if clicked on button
			for (var i = 0; i < nrButtons; ++i)
			{
				if (targetId === '#menuButton' + i ||
					targetId === '#dropdownMenu' + i)
				{
					buttonClicked = true;
					break;
				}
			}
			
			// if not clicked on button, close
			if (buttonClicked === false)
			{
				menuButtonClicked = false;
				
				// close previous
				if (selectedMenu !== "")
				{
					dropdownMenus[selectedMenu].css('display', 'none');
					menuButtons[selectedMenu].removeClass("selected");
				}
			}
		});

        // disable scrolling by middle mouse
        $(document).on('mousedown', function(e)
        {
            // middlemouse
            if (e.which === 2)
                e.preventDefault();
        });

        // disable selection by dragging
        //$('#propertyPanel').draggable = false;

        pub.makeElementsUnselectable();
	}

    pub.makeElementsUnselectable = function()
    {
        $("*:not(input)").addClass("unselectable");
    }
	
	return pub;
})();
// run script on load
window.addEventListener('load', MenuBehaviour.init);