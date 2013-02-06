$(function()
{
    var nrSliders = 7;
    var open = true;
    var dragging = false;
    var mousepos = {x:0,y:0};
    var contentpos = {x:10,y:60};

    for (var i = 0; i <= nrSliders; ++i)
    {
        $('#slider' + i).on('change', function(e)
        {
            $('#' + e.target.id + 'val').html(Math.floor(this.value * 10000) / 10000);
            HE.updateTonemapData(e.target.id.substr(6,1), this.value);
        });
    }

    $("#header").on('click', function(e)
    {
        if (open)
        {
            $('#slidersandlabels').css('display', 'none');
            open = false;
        }
        else
        {
            $('#slidersandlabels').css('display', 'block');
            open = true;
        }
    });

    $("#content").on('mousedown', function(e)
    {
        var isnotdownonsliders = true;

        for (var i = 0; i <= nrSliders; ++i)
        {
            if ('#' + e.target.id === '#slider' + i)
            isnotdownonsliders = false;
        }

        if (isnotdownonsliders)
            dragging = true;
    });

    $(document).on('mouseup', function(e)
    {
        dragging = false;
    });

    $(document).on('mousemove', function(e)
    {
        if (dragging)
        {
            var xt = e.clientX - mousepos.x;
            var yt = e.clientY - mousepos.y;

            contentpos.x += xt;
            contentpos.y += yt;

            mousepos.x = e.clientX;
            mousepos.y = e.clientY;

            updateContentPos();
        }
        else
        {
            mousepos.x = e.clientX;
            mousepos.y = e.clientY;
        }
    });
	
	// disable scrolling by middle mouse
	$(document).on('mousedown', function(e)
	{
		// middlemouse
		if (e.which === 2)
			e.preventDefault();
	});

    function updateContentPos()
    {
        $("#content").css('top', contentpos.y + "px");
        $("#content").css('left', contentpos.x + "px");
    }

    updateContentPos();
	
	$('#slidersandlabels').css('display', 'none');
	open = false;
});

function setSliderValue(id, value)
{
	$('#slider' + id).val(value);
	$('#slider' + id + 'val').html(Math.floor(value * 10000) / 10000);
}