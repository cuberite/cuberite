#!/usr/bin/lua

-- InfoDump.lua

-- Goes through all subfolders, loads Info.lua and dumps its g_PluginInfo into various text formats
-- This is used for generating plugin documentation for the forum and for GitHub's INFO.md files

-- This script requires LuaRocks with LFS installed, instructions are printed when this is not present.





-- Check Lua version. We use 5.1-specific construct when loading the plugin info, 5.2 is not compatible!
if (_VERSION ~= "Lua 5.1") then
	print("Unsupported Lua version. This script requires Lua version 5.1, this Lua is version " .. (_VERSION or "<nil>"));
	return;
end

-- Try to load lfs, do not abort if not found
local lfs, err = pcall(
	function()
		return require("lfs")
	end
);

-- Rather, print a nice message with instructions:
if not(lfs) then
	print([[
Cannot load LuaFileSystem
Install it through luarocks by executing the following command:
  sudo luarocks install luafilesystem

If you don't have luarocks installed, you need to install them using your OS's package manager, usually:
  sudo apt-get install luarocks
On windows, a binary distribution can be downloaded from the LuaRocks homepage, http://luarocks.org/en/Download
]]);
  
	print("Original error text: ", err);
	return;
end

-- We now know that LFS is present, get it normally:
lfs = require("lfs");






--- Replaces generic formatting with forum-specific formatting
-- Also removes the single line-ends
local function ForumizeString(a_Str)
	assert(type(a_Str) == "string");
	
	-- Replace line ends with {%p} when multiple and space when single, so that manual word-wrap in the Info.lua file is fixed
	a_Str = a_Str:gsub("\n\n", "{%%p}");
	a_Str = a_Str:gsub("\n", " ");
	
	-- Replace the generic formatting:
	a_Str = a_Str:gsub("{%%p}", "\n\n");
	-- TODO: Other formatting
	
	return a_Str;
end





--- Returns an array-table of all commands that are in the specified category
-- Each item is a table {Command = "/command string", Info = {<command info in PluginInfo>}}
local function GetCategoryCommands(a_PluginInfo, a_CategoryName)
	local res = {};
	local function AppendCategoryCommand(a_Prefix, a_Commands)
		for cmd, info in pairs(a_Commands) do
			info.Category = info.Category or {};
			if (type(info.Category) == "string") then
				info.Category = {info.Category};
			end
			for idx, cat in ipairs(info.Category) do
				if (cat == a_CategoryName) then
					table.insert(res, {Command = a_Prefix .. cmd, Info = info});
				end
			end
			if (info.Subcommands ~= nil) then
				AppendCategoryCommand(a_Prefix .. cmd .. " ", info.Subcommands);
			end
		end
	end
	AppendCategoryCommand("", a_PluginInfo.Commands);
	return res;
end





--- Builds an array of categories, each containing all the commands belonging to the category,
-- and the category description, if available.
-- Returns the array table, each item has the following format:
-- { Name = "CategoryName", Description = "CategoryDescription", Commands = {{CommandString = "/cmd verb", Info = {...}}, ...}}
local function BuildCategories(a_PluginInfo)
	-- The returned result
	-- This will contain both an array and a dict of the categories, to allow fast search
	local res = {};
	
	-- For each command add a reference to it into all of its categories:
	local function AddCommands(a_CmdPrefix, a_Commands)
		for cmd, info in pairs(a_Commands) do
			local NewCmd =
			{
				CommandString = a_CmdPrefix .. cmd,
				Info = info,
			}
			
			if ((info.HelpString ~= nil) and (info.HelpString ~= "")) then
				-- Add to each specified category:
				local Category = info.Category;
				if (type(Category) == "string") then
					Category = {Category};
				end
				for idx, cat in ipairs(Category or {""}) do
					local CatEntry = res[cat];
					if (CatEntry == nil) then
						-- First time we came across this category, create it:
						local NewCat = {Name = cat, Description = "", Commands = {NewCmd}};
						table.insert(res, NewCat);
						res[cat] = NewCat;
					else
						-- We already have this category, just add the command to its list of commands:
						table.insert(CatEntry.Commands, NewCmd);
					end
				end  -- for idx, cat - Category[]
			end  -- if (HelpString valid)
			
			-- Recurse all subcommands:
			if (info.Subcommands ~= nil) then
				AddCommands(a_CmdPrefix .. cmd .. " ", info.Subcommands);
			end
		end  -- for cmd, info - a_Commands[]
	end  -- AddCommands()
	
	AddCommands("", a_PluginInfo.Commands);
	
	-- Assign descriptions to categories:
	for name, desc in pairs(a_PluginInfo.Categories or {}) do
		local CatEntry = res[name];
		if (CatEntry ~= nil) then
			-- The result has this category, add the description:
			CatEntry.Description = desc.Description;
		end
	end
	
	-- Alpha-sort each category's command list:
	for idx, cat in ipairs(res) do
		table.sort(cat.Commands,
			function (cmd1, cmd2)
				return (string.lower(cmd1.CommandString) < string.lower(cmd2.CommandString));
			end
		);
	end
	
	return res;
end





local function WriteCommandsCategoryForum(a_Category, f)
	-- Write category name:
	local CategoryName = a_Category.Name;
	if (CategoryName == "") then
		CategoryName = "General";
	end
	f:write("\n[size=Large]", a_Category.DisplayName or CategoryName, "[/size]\n");
	
	-- Write description:
	if (a_Category.Description ~= "") then
		f:write(a_Category.Description, "\n");
	end
	
	-- Write commands:
	f:write("\n[list]");
	for idx2, cmd in ipairs(a_Category.Commands) do
		f:write("\nCommand: [b]", cmd.CommandString, "[/b] - ", (cmd.Info.HelpString or "UNDOCUMENTED"), "\n");
		if (cmd.Info.Permission ~= nil) then
			f:write("Permission required: ", cmd.Info.Permission, "\n");
		end
		if (cmd.Info.DetailedDescription ~= nil) then
			f:write(cmd.Info.DetailedDescription);
		end
	end
	f:write("[/list]\n\n")
end





local function DumpCommandsForum(a_PluginInfo, f)
	-- Copy all Categories from a dictionary into an array:
	local Categories = BuildCategories(a_PluginInfo);
	
	-- Sort the categories by name:
	table.sort(Categories,
		function(cat1, cat2)
			return (string.lower(cat1.Name) < string.lower(cat2.Name));
		end
	);
	
	if (#Categories == 0) then
		return;
	end
	
	f:write("\n[size=X-Large]Commands[/size]\n");

	-- Dump per-category commands:
	for idx, cat in ipairs(Categories) do
		WriteCommandsCategoryForum(cat, f);
	end
end





local function DumpAdditionalInfoForum(a_PluginInfo, f)
	local AInfo = a_PluginInfo.AdditionalInfo;
	if ((AInfo == nil) or (type(AInfo) ~= "table")) then
		-- There is no AdditionalInfo in a_PluginInfo
		return;
	end
	
	for idx, info in ipairs(a_PluginInfo.AdditionalInfo) do
		if ((info.Title ~= nil) and (info.Contents ~= nil)) then
			f:write("\n[size=X-Large]", ForumizeString(info.Title), "[/size]\n");
			f:write(ForumizeString(info.Contents), "\n");
		end
	end
end





local function DumpPluginInfoForum(a_PluginFolder, a_PluginInfo)
	-- Open the output file:
	local f, msg = io.open(a_PluginInfo.Name .. "_forum.txt", "w");
	if (f == nil) then
		print("\tCannot dump forum info for plugin " .. a_PluginFolder .. ": " .. msg);
		return;
	end

	-- Write the description:
	f:write(a_PluginInfo.Description);
	DumpAdditionalInfoForum(a_PluginInfo, f);
	DumpCommandsForum(a_PluginInfo, f);

	f:close();
end





local function DumpPluginInfoGitHub()
	-- TODO
end





--- Tries to load the g_PluginInfo from the plugin's Info.lua file
-- Returns the g_PluginInfo table on success, or nil and error message on failure
local function LoadPluginInfo(a_FolderName)
	-- Check if the Info file is present at all:
	local Attribs = lfs.attributes(a_FolderName .. "/Info.lua");
	if ((Attribs == nil) or (Attribs.mode ~= "file")) then
		return nil;
	end
	
	-- Load and compile the Info file:
	local cfg, err = loadfile(a_FolderName .. "/Info.lua");
	if (cfg == nil) then
		return nil, "Cannot open 'Info.lua': " .. err;
	end
	
	-- Execute the loaded file in a sandbox:
	-- This is Lua-5.1-specific and won't work in Lua 5.2!
	local Sandbox = {};
	setfenv(cfg, Sandbox);
	cfg();
	if (Sandbox.g_PluginInfo == nil) then
		return nil, "Info.lua doesn't contain the g_PluginInfo declaration";
	end
	return Sandbox.g_PluginInfo;
end





local function ProcessPluginFolder(a_FolderName)
	local PluginInfo, Msg = LoadPluginInfo(a_FolderName);
	if (PluginInfo == nil) then
		if (Msg ~= nil) then
			print("\tCannot load Info.lua: " .. Msg);
		end
		return;
	end
	DumpPluginInfoForum(a_FolderName, PluginInfo);
end





print("Processing plugin subfolders:");
for fnam in lfs.dir(".") do
	if ((fnam ~= ".") and (fnam ~= "..")) then
		local Attributes = lfs.attributes(fnam);
		if (Attributes ~= nil) then
			if (Attributes.mode == "directory") then
				print(fnam);
				ProcessPluginFolder(fnam);
			end
		end
	end
end




