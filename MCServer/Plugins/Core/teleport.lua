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
		a_Player:SendMessage( cChatColor.Green .. "Usage: /tp [PlayerName] (-h) or /tp [X Y Z]" )
		return true
	end
end

function HandleTPACommand( Split, Player )
    if Split[2] == nil then
        Player:SendMessage( cChatColor.Green .. "Usage: /tpa [Player]" )
        return true
    end
    local loopPlayer = function( OtherPlayer )
        if OtherPlayer:GetName() == Split[2] then
            OtherPlayer:SendMessage( cChatColor.Green .. Player:GetName() .. " send a teleport request" )
            Player:SendMessage( cChatColor.Green .. "You send a teleport request to " .. OtherPlayer:GetName() )
            Destination[OtherPlayer:GetName()] = Player:GetName()
        end
    end
    local loopWorlds = function( World )
        World:ForEachPlayer( loopPlayer )
    end
    cRoot:Get():ForEachWorld( loopWorlds )
    return true
end

function HandleTPAcceptCommand( Split, Player )
    if Destination[Player:GetName()] == nil then
        Player:SendMessage( cChatColor.Green .. "Nobody has send you a teleport request" )
        return true
    end
    local loopPlayer = function( OtherPlayer )
        if Destination[Player:GetName()] == OtherPlayer:GetName() then
            if OtherPlayer:GetWorld():GetName() ~= Player:GetWorld():GetName() then
                OtherPlayer:MoveToWorld( Player:GetWorld():GetName() )
            end
            OtherPlayer:TeleportToEntity( Player )
            Player:SendMessage( cChatColor.Green .. OtherPlayer:GetName() .. " teleported to you" )
            OtherPlayer:SendMessage( cChatColor.Green .. "You teleported to " .. Player:GetName() )
            Destination[Player:GetName()] = nil
        end
    end
    local loopWorlds = function( World )
        World:ForEachPlayer( loopPlayer )
    end
    cRoot:Get():ForEachWorld( loopWorlds )
    return true
end

-- Teleports a_SrcPlayer to a player named a_DstPlayerName; if a_TellDst is true, will send a notice to the destination player
function TeleportToPlayer(a_SrcPlayer, a_DstPlayerName, a_TellDst)
	local teleport = function(OtherPlayer)
		if (OtherPlayer == a_SrcPlayer) then
			-- Asked to teleport to self?
			a_SrcPlayer:SendMessage(cChatColor.Green .. "Y' can't teleport to yerself!");
		else
			SetBackCoordinates(a_SrcPlayer);
			a_SrcPlayer:TeleportToEntity(OtherPlayer);
			a_SrcPlayer:SendMessage(cChatColor.Green .. "You teleported to " .. OtherPlayer:GetName() .. "!");
			if (a_TellDst) then
				OtherPlayer:SendMessage(cChatColor.Green .. Player:GetName().." teleported to you!");
			end
		end
	end
	
    local World = a_SrcPlayer:GetWorld();
    if (not(World:DoWithPlayer(a_DstPlayerName, teleport))) then
	    a_SrcPlayer:SendMessage(cChatColor.Green .. "Can't find player " .. a_DstPlayerName);
	end
end