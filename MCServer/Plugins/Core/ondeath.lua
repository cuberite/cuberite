function OnKilling(Victim, Killer)
	if Victim:IsPlayer() then
		SetBackCoordinates( Victim )
		Server = cRoot:Get():GetServer()
		if Killer == nil then
			if Victim:IsOnFire() then
				Server:SendMessage( cChatColor.Red .. Victim:GetName() .. " was burnt to a cinder" )
				CheckHardcore(Victim)
				return false
			end
			if Victim:GetWorld():GetBlock(Victim:GetPosX(), Victim:GetPosY(), Victim:GetPosZ()) == 10 or Victim:GetWorld():GetBlock(Victim:GetPosX(), Victim:GetPosY(), Victim:GetPosZ()) == 11 then
				Server:SendMessage( cChatColor.Red ..  Victim:GetName() .. " tried to swim in lava (and failed)" )
				CheckHardcore(Victim)
				return false
			end
		else
			if Killer:IsPlayer() then
				Server:SendMessage( cChatColor.Red .. Victim:GetName() .. " was terminated by " .. Killer:GetName() )
				CheckHardcore(Victim)
				return false
			elseif Killer:IsMob() then
				if Killer:IsA("cZombie") then
					Server:SendMessage( cChatColor.Red .. Victim:GetName() .. " was eaten by a Zombie")
				elseif Killer:IsA("cSkeleton") then
					Server:SendMessage( cChatColor.Red .. Victim:GetName() .. " was shot by a Skeleton" )
				elseif Killer:IsA("cCreeper") then
					Server:SendMessage( cChatColor.Red .. Victim:GetName() .. " was blown up by a Creeper")
				elseif Killer:IsA("cSpider") then
					Server:SendMessage( cChatColor.Red .. Victim:GetName() .. " was ripped apart by a giant Spider")
				elseif Killer:IsA("cCaveSpider") then
					Server:SendMessage( cChatColor.Red .. Victim:GetName() .. " was poisoned by a giant Cave Spider")
				elseif Killer:IsA("cBlaze") then
					Server:SendMessage( cChatColor.Red .. Victim:GetName() .. " was flamed by a Blaze")
				elseif Killer:IsA("cEnderman") then
					Server:SendMessage( cChatColor.Red .. Victim:GetName() .. " was " .. cChatColor.Random .. " by an Enderman")
				elseif Killer:IsA("cSilverfish") then
					Server:SendMessage( cChatColor.Red .. Victim:GetName() .. " was... DERPED by a Silverfish!")
				elseif Killer:IsA("cSlime") then
					Server:SendMessage( cChatColor.Red .. Victim:GetName() .. " was stuck fast and killed by a Slime")
				elseif Killer:IsA("cWitch") then
					Server:SendMessage( cChatColor.Red .. Victim:GetName() .. " was enchanted (to death) by a Witch")
				elseif Killer:IsA("cZombiepigman") then
					Server:SendMessage( cChatColor.Red .. Victim:GetName() .. " was slain by a Zombie Pigman")
				elseif Killer:IsA("cMagmacube") then
					Server:SendMessage( cChatColor.Red .. Victim:GetName() .. " was incinerated by a Magmacube")
				elseif Killer:IsA("cWolf") then
					Server:SendMessage( cChatColor.Red .. Victim:GetName() .. " was savaged by a Wolf")					
				end
				CheckHardcore(Victim)
				return false
			end
		end
		Server:SendMessage( cChatColor.Red .. Victim:GetName() .. " died of mysterious circumstances")
		CheckHardcore(Victim)
	end
end

function CheckHardcore(Victim)
	if HardCore == "true" then
		if Victim:IsPlayer() == true then
			local KilledPlayer = tolua.cast(Victim, "cPlayer")
			BanPlayer(KilledPlayer:GetName(), "You died, haha. Good game, bro.")
		end
	end
end