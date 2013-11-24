return
{
	HOOK_WEATHER_CHANGING =
	{
		CalledWhen = "The weather is about to change",
		DefaultFnName = "OnWeatherChanging",  -- also used as pagename
		Desc = [[
			This hook is called when the current weather has expired and a new weather is selected. Plugins may
			override the new weather setting.</p>
			<p>
			The new weather setting is sent to the clients only after this hook has been processed.</p>
			<p>
			See also the {{OnWeatherChanged|HOOK_WEATHER_CHANGED}} hook for a similar hook called after the
			change.
		]],
		Params =
		{
			{ Name = "World", Type = "{{cWorld}}", Notes = "World for which the weather is changing" },
			{ Name = "Weather", Type = "number", Notes = "The newly selected weather. One of wSunny, wRain, wStorm" },
		},
		Returns = [[
			If the function returns false or no value, the server calls other plugins' callbacks and finally
			sets the weather. If the function returns true, the server takes the second returned value (wSunny
			by default) and sets it as the new weather. No other plugins' callbacks are called in this case.
		]],
	},  -- HOOK_WEATHER_CHANGING
}





