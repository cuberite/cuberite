function OnKilling(Victim, Killer)
	if Victim:IsPlayer() == true then
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
			if Killer:IsPlayer() == true then
				Server:SendMessage( cChatColor.Red .. "Player " .. Victim:GetName() .. " is killed by " .. Killer:GetName() )
				CheckHardcore(Victim)
				return false
			elseif Killer:IsMob() == true then
				if Killer:IsA("cZombie") then
					Server:SendMessage( cChatColor.Red .. "Player " .. Victim:GetName() .. " is eaten by a zombie")
					CheckHardcore(Victim)
					return false
				elseif Killer:IsA("cSkeleton") then
					Server:SendMessage( cChatColor.Red .. "Player " .. Victim:GetName() .. " is killed by a skeleton" )
					CheckHardcore(Victim)
					return false
				elseif Killer:IsA("cCreeper") then
					Server:SendMessage( cChatColor.Red .. "Player " .. Victim:GetName() .. " is killed by a creeper")
					CheckHardcore(Victim)
					return false
				elseif Killer:IsA("cSpider") then
					Server:SendMessage( cChatColor.Red .. "Player " .. Victim:GetName() .. " is killed by a spider")
					CheckHardcore(Victim)
					return false
				elseif Killer:IsA("cCaveSpider") then
					Server:SendMessage( cChatColor.Red .. "Player " .. Victim:GetName() .. " is killed by a cavespider")
					CheckHardcore(Victim)
					return false
				elseif Killer:IsA("cBlaze") then
					Server:SendMessage( cChatColor.Red .. "Player " .. Victim:GetName() .. " is killed by a blaze")
					CheckHardcore(Victim)
					return false
				elseif Killer:IsA("cEnderman") then
					Server:SendMessage( cChatColor.Red .. "Player " .. Victim:GetName() .. " is slain by a enderman")
					CheckHardcore(Victim)
					return false
				elseif Killer:IsA("cSilverfish") then
					Server:SendMessage( cChatColor.Red .. "Player " .. Victim:GetName() .. " is killed by a silverfish")
					CheckHardcore(Victim)
					return false
				elseif Killer:IsA("cSlime") then
					Server:SendMessage( cChatColor.Red .. "Player " .. Victim:GetName() .. " is killed by a slime")
					CheckHardcore(Victim)
					return false
				elseif Killer:IsA("cWitch") then
					Server:SendMessage( cChatColor.Red .. "Player " .. Victim:GetName() .. " is killed by a witch")
					CheckHardcore(Victim)
					return false
				elseif Killer:IsA("cZombiepigman") then
					Server:SendMessage( cChatColor.Red .. "Player " .. Victim:GetName() .. " is slain by a zombiepigman")
					CheckHardcore(Victim)
					return false
				elseif Killer:IsA("cMagmacube") then
					Server:SendMessage( cChatColor.Red .. "Player " .. Victim:GetName() .. " is killed by a magmacube")
					CheckHardcore(Victim)
					return false
				elseif Killer:IsA("cWolf") then
					Server:SendMessage( cChatColor.Red .. "Player " .. Victim:GetName() .. " is killed by a wolf")
					CheckHardcore(Victim)
					return false
				end
			end
		end
		Server:SendMessage( cChatColor.Red .. Victim:GetName() .. " Died")
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