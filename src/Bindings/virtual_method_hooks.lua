
-- virtual_method_hooks.lua

-- Implements additional processing that is done while generating the Lua bindings





local access = {public = 0, protected = 1, private = 2}





function parser_hook(s)
	local container = classContainer.curr -- get the current container

	-- process access-specifying labels (public, private, etc)
	do
		local b, e, label = string.find(s, "^%s*(%w*)%s*:[^:]") -- we need to check for [^:], otherwise it would match 'namespace::type'
		if b then

			-- found a label, get the new access value from the global 'access' table
			if access[label] then
				container.curr_member_access = access[label]
			end -- else ?

			return strsub(s, e) -- normally we would use 'e+1', but we need to preserve the [^:]
		end
	end
end





function post_output_hook()
	print("Bindings have been generated.")
end




