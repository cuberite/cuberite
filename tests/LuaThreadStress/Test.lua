-- Test.lua

-- Implements the test support functions
-- This file is loaded into the cLuaState used for stress-testing





--- Returns a function that the C++ code can call
-- The callback takes a single number as param and returns the sum of the param and the seed, given to this factory function (for verification)
function getCallback(a_Seed)
	return function (a_Param)
		-- print("Callback " .. a_Seed .. " called with param " .. a_Param)
		return a_Param + a_Seed
	end
end
