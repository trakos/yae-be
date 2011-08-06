$(function()
{
	$("#groupby").change(function()
	{
		window.location.search = jQuery.query.set("group_by", $("#groupby").val()).set("page",1);
	});
});