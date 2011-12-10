$(function()
{
	$(".expand").click(function(e)
	{
		$($(this).next().get(0)).toggle();
		e.preventDefault();
	});
    $('#contactFloatTab').tabSlideOut(
    {
        tabHandle: 		'.handle',	//class of the element that will become your tab
        tabLocation: 	'right',	//side of screen where tab lives, top, right, bottom, or left
        speed: 			300,		//speed of animation
        action:			'click',	//options: 'click' or 'hover', action to trigger animation
        topPos:			'300px',	//position from the top/ use if tabLocation is left or right
        leftPos:		'20px',		//position from left/ use if tabLocation is bottom or top
        fixedPosition:	true		//options: true makes it stick(fixed position) on scroll
    });
    $('.user_search_autocomplete').autocomplete("?a=jsonQueryUsers&c=Users",
    {
    	delay:		50
    }).stopSearch(function()
    {
    	$('.user_search_autocomplete_loader').css('visibility','hidden');
    }).startSearch(function()
    {
    	$('.user_search_autocomplete_loader').css('visibility','visible');
    });

});
