function OnKilling(Victim, Killer)
	if Victim:IsPlayer() then
		Server = cRoot:Get():GetServer()
		if Killer == nil then
			if Victim:IsOnFire() then
				Server:SendMessage( cChatColor.Red .. "Player " .. Victim:GetName() .. " burned away." )
				CheckHardcore(Victim)
				return false
			end
			if Victim:GetWorld():GetBlock(Victim:GetPosX(), Victim:GetPosY(), Victim:GetPosZ()) == 10 or Victim:GetWorld():GetBlock(Victim:GetPosX(), Victim:GetPosY(), Victim:GetPosZ()) == 11 then
				Server:SendMessage( cChatColor.Red ..  Victim:GetName() .. " tried to swim in lava" )
				CheckHardcore(Victim)
				return false
			end
		else
			if Killer:IsPlayer() then
				Server:SendMessage( cChatColor.Red .. "Player " .. Victim:GetName() .. " is killed by " .. Killer:GetName() )
				CheckHardcore(Victim)
				return false
			elseif Killer:IsMob() then
				if Killer:IsA("cZombie") then
					Server:SendMessage( cChatColor.Red .. "Player " .. Victim:GetName() .. " is eaten by a zombie")
				elseif Killer:IsA("cSkeleton") then
					Server:SendMessage( cChatColor.Red .. "Player " .. Victim:GetName() .. " is killed by a skeleton" )
				elseif Killer:IsA("cCreeper") then
					Server:SendMessage( cChatColor.Red .. "Player " .. Victim:GetName() .. " is blown up by a creeper")
				elseif Killer:IsA("cSpider") then
					Server:SendMessage( cChatColor.Red .. "Player " .. Victim:GetName() .. " is killed by a spider")
				elseif Killer:IsA("cCaveSpider") then
					Server:SendMessage( cChatColor.Red .. "Player " .. Victim:GetName() .. " is killed by a cavespider")
				elseif Killer:IsA("cBlaze") then
					Server:SendMessage( cChatColor.Red .. "Player " .. Victim:GetName() .. " is killed by a blaze")
				elseif Killer:IsA("cEnderman") then
					Server:SendMessage( cChatColor.Red .. "Player " .. Victim:GetName() .. " is slain by a enderman")
				elseif Killer:IsA("cSilverfish") then
					Server:SendMessage( cChatColor.Red .. "Player " .. Victim:GetName() .. " is killed by a silverfish")
				elseif Killer:IsA("cSlime") then
					Server:SendMessage( cChatColor.Red .. "Player " .. Victim:GetName() .. " is killed by a slime")
				elseif Killer:IsA("cWitch") then
					Server:SendMessage( cChatColor.Red .. "Player " .. Victim:GetName() .. " is killed by a witch")
				elseif Killer:IsA("cZombiepigman") then
					Server:SendMessage( cChatColor.Red .. "Player " .. Victim:GetName() .. " is slain by a zombiepigman")
				elseif Killer:IsA("cMagmacube") then
					Server:SendMessage( cChatColor.Red .. "Player " .. Victim:GetName() .. " is killed by a magmacube")
				elseif Killer:IsA("cWolf") then
					Server:SendMessage( cChatColor.Red .. "Player " .. Victim:GetName() .. " is killed by a wolf")					
				end
				CheckHardcore(Victim)
				return false
			end
		end
		Server:SendMessage( cChatColor.Red .. Victim:GetName() .. " Died")
		CheckHardcore(Victim)
	end
end

function CheckHardcore(Victim)
	if HardCore == "true" then
		if Victim:IsPlayer() == true then
			local KilledPlayer = tolua.cast(Victim, "cPlayer")
			BanPlayer(KilledPlayer:GetName(), "You Died")
		end
	end
end