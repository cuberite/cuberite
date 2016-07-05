
let g_Canvas = null;
let g_Context = null;
let g_ValueMap = null;




function init() {
	g_Canvas = document.getElementById("stage");
	g_Context = g_Canvas.getContext("2d");
	g_ValueMap = new ValueMap();
	g_ValueMap.visualize(g_Context, g_Canvas);
}



function btnZoom(btn) {
	g_ValueMap.zoom();
	g_ValueMap.visualize(g_Context, g_Canvas);
	if (
		(g_ValueMap.sizeX * 2 - 1 > 600) ||
		(g_ValueMap.sizeZ * 2 - 1 > 600)
	) {
		btn.disabled = true;
	}
}



function btnSmooth() {
	g_ValueMap.smooth();
	g_ValueMap.visualize(g_Context, g_Canvas);
}


function btnReset() {
	g_ValueMap.reset();
	g_ValueMap.visualize(g_Context, g_Canvas);
	
	document.getElementById("grownZoomButton").disabled = false;
}


function btnAutomatic(target) {
	target.disabled = true;
	document.getElementById("grownZoomButton").disabled = true;

	// Reset the valuemap. We don't want to continue on a 500x500 map.
	g_ValueMap.reset();
	g_ValueMap.visualize(g_Context, g_Canvas);

	const animationTimeBetween = 350;
	let zoom =   () => { g_ValueMap.zoom() };
	let smooth = () => { g_ValueMap.smooth() };
	let actions = [];
	
	for (let i = 0; i < 6; i++) actions.push(zoom);   // First zoom 6 times
	for (let i = 0; i < 3; i++) actions.push(smooth); // Then smooth 3 times
	for (let i = 0; i < 2; i++) actions.push(zoom);   // Zoom 2 times
	for (let i = 0; i < 2; i++) actions.push(smooth); // And finally smooth 2 more times.

	let update = () => {
		if (actions[0] == null) {
			target.disabled = false;
			return;
		}

		actions[0].call();
		g_ValueMap.visualize(g_Context, g_Canvas);

		actions.splice(0, 1);
		setTimeout(update, animationTimeBetween);
	};
	setTimeout(update, animationTimeBetween + 500);
}


window.onload = init;
