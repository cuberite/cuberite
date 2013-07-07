function HandleTPCommand(a_Split, a_Player)
	if ((#a_Split == 2) or (#a_Split == 3)) then
		-- Teleport to player specified in a_Split[2], tell them unless a_Split[3] equals "-h":
		TeleportToPlayer(a_Player, a_Split[2], (a_Split[3] ~= "-h"));
		return true;
	elseif (#a_Split == 4) then
		-- Teleport to XYZ coords specified in a_Split[2, 3, 4]:
		SetBackCoordinates(a_Player);
		a_Player:TeleportToCoords(a_Split[2], a_Split[3], a_Split[4]);
		a_Player:SendMessage(cChatColor.Green .. "You teleported to {" .. a_Split[2] .. ", " .. a_Split[3] .. ", " .. a_Split[4] .. "}");
		return true;
	else
		Player:SendMessage( cChatColor.Green .. "Usage: /tp [PlayerName] (-h)" )
		return true
	end
end





-- Teleports a_SrcPlayer to a player named a_DstPlayerName; if a_TellDst is true, will send a notice to the destination player
function TeleportToPlayer(a_SrcPlayer, a_DstPlayerName, a_TellDst)
	local teleport = function(OtherPlayer)
		if (OtherPlayer == a_SrcPlayer) then
			-- Asked to teleport to self?
			a_SrcPlayer:SendMessage(cChatColor.Green .. "Already there :)");
		else
			SetBackCoordinates(a_SrcPlayer);
			a_SrcPlayer:TeleportToEntity(OtherPlayer);
			a_SrcPlayer:SendMessage(cChatColor.Green .. "You teleported to " .. OtherPlayer:GetName() .. "!");
			if (a_TellDst) then
				OtherPlayer:SendMessage(cChatColor.Green .. Player:GetName().." teleported to you!");
			end
		end
	end
	
	local World = Player:GetWorld();
	if (not(World:DoWithPlayer(s_DstPlayerName, teleport))) then
		a_SrcPlayer:SendMessage(cChatColor.Green .. "Can't find player " .. a_DstPlayerName);
	end
end




