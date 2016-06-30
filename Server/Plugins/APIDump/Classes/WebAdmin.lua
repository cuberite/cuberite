return
{
	cWebAdmin =
	{
		Desc = "",
		Functions =
		{
			AddWebTab = { Params = "Title, UrlPath, HandlerFn", Return = "", Notes = "(STATIC) Adds a new web tab to webadmin. The tab uses \"Title\" as its display string and is identified in the URL using the UrlPath (https://server.domain.com/webadmin/{PluginName}/{UrlPath}). The HandlerFn is the callback function that is called when the admin accesses the page, it has the following signature:<br/><pre class=\"prettyprint lang-lua\">function ({{a_Request|HTTPRequest}}, a_UrlPath)<br/>  return Content, ContentType<br/>end</pre> URLPath must not contain a '/', the recommendation is to use only 7-bit-clean ASCII character set." },
			GetAllWebTabs = { Params = "", Return = "array-table", Notes = "(STATIC) Returns an array-table with each item describing a web tab, for all web tabs registered in the WebAdmin, for all plugins. The returned table has the following format:<br/><pre class=\"prettyprint lang-lua\">{<br/>  {<br/>    PluginName = \"Plugin's API name\",<br/>    UrlPath = \"UrlPath given to AddWebTab\",<br/>    Title = \"Title given to AddWebTab\",<br/>  },<br/>  ...<br/>}"},
			GetBaseURL = { Params = "URL", Return = "string", Notes = "(STATIC) Returns the string that is the path of the base webadmin (\"../../../webadmin\") relative to the given URL." },
			GetContentTypeFromFileExt = { Params = "FileExt", Return = "string", Notes = "(STATIC) Returns the content-type that should be used for files with the specified extension (without the dot), such as \"text/plain\" for the \"txt\" extension. If the extension is not known, returns an empty string." },
			GetHTMLEscapedString = { Params = "string", Return = "string", Notes = "(STATIC) Gets the HTML-escaped representation of a requested string. This is useful for user input and game data that is not guaranteed to be escaped already." },
			GetPage = { Params = "{{Request|HTTPRequest}}", Return = "table", Notes = "(STATIC) Returns the (inner HTML) page contents for the specified request. Calls the appropriate WebTab handler registered via AddWebTab() and returns the information from that plugin wrapped in a table with the following structure:<br/><pre class=\"prettyprint lang-lua\">{<br/>  Content = \"\",      -- Content returned by the plugin<br/>  ContentType = \"\",  -- Content type returned by the plugin, or \"text/html\" if none returned<br/>  UrlPath = \"\",      -- UrlPath decoded from the request<br/>  TabTitle = \"\",     -- Title of the tab that handled the request, as given to AddWebTab()<br/>  PluginName = \"\",   -- API name of the plugin that handled the request<br/>  PluginFolder = \"\", -- Folder name (= display name) of the plugin that handled the request<br/>}</pre>This function is mainly used in the webadmin template file." },
			GetPorts = { Params = "", Return = "string", Notes = "Returns a comma-separated list of ports on which the webadmin is configured to listen. Note that this list does include ports that may currently be unavailable (another server was already listening on them prior to launching Cuberite)." },
			GetURLEncodedString = { Params = "string", Return = "string", Notes = "(STATIC) Returns the string given to it escaped by URL encoding, which makes the string suitable for transmission in an URL. Invalid characters are turned into \"%xy\" values." },
			Reload = { Params = "", Return = "", Notes = "Reloads the webadmin's config - the allowed logins, the template script and the login page. Note that reloading will not change the \"enabled\" state of the server, and it will not update listening ports. Existing WebTabs will be kept registered even after the reload." },
		},
	},  -- cWebAdmin


	HTTPFormData =
	{
		Desc = "This class stores data for one form element for a {{HTTPRequest|HTTP request}}.",
		Variables =
		{
			Name = { Type = "string", Notes = "Name of the form element" },
			Type = { Type = "string", Notes = "Type of the data (usually empty)" },
			Value = { Type = "string", Notes = "Value of the form element. Contains the raw data as sent by the browser." },
		},
	},  -- HTTPFormData


	HTTPRequest =
	{
		Desc = [[
			This class encapsulates all the data that is sent to the WebAdmin through one HTTP request. Plugins
			receive this class as a parameter to the function handling the web requests, as registered in the
			{{cPluginLua}}:AddWebPage().
		]],
		Constants =
		{
			FormData = { Notes = "Array-table of {{HTTPFormData}}, contains the values of individual form elements submitted by the client" },
			Params = { Notes = "Map-table of parameters given to the request in the URL (?param=value); if a form uses GET method, this is the same as FormData. For each parameter given as \"param=value\", there is an entry in the table with \"param\" as its key and \"value\" as its value." },
			PostParams = { Notes = "Map-table of data posted through a FORM - either a GET or POST method. Logically the same as FormData, but in a map-table format (for each parameter given as \"param=value\", there is an entry in the table with \"param\" as its key and \"value\" as its value)." },
		},

		Variables =
		{
			Method = { Type = "string", Notes = "The HTTP method used to make the request. Usually GET or POST." },
			Path = { Type = "string", Notes = "The Path part of the URL (excluding the parameters)" },
			URL = { Type = "string", Notes = "The entire URL used for the request." },
			Username = { Type = "string", Notes = "Name of the logged-in user." },
		},
	},  -- HTTPRequest
}




