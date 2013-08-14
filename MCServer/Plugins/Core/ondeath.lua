function OnKilling(Victim, Killer)
	if Victim:IsPlayer() then
		SetBackCoordinates( Victim )
		Server = cRoot:Get():GetServer()
		if Killer == nil then
			if Victim:GetWorld():GetBlock(Victim:GetPosX(), Victim:GetPosY(), Victim:GetPosZ()) == 10 or Victim:GetWorld():GetBlock(Victim:GetPosX(), Victim:GetPosY(), Victim:GetPosZ()) == 11 then
				Server:SendMessage( cChatColor.Red ..  "[FATALITY] " .. cChatColor.White .. Victim:GetName() .. " tried to swim in lava (and failed)" )
				CheckHardcore(Victim)
				return false
			end
			if Victim:IsOnFire() then
				Server:SendMessage( cChatColor.Red .. "[FATALITY] " .. cChatColor.White .. Victim:GetName() .. " was burnt to a cinder" )
				CheckHardcore(Victim)
				return false
			end
		else
			if Killer:IsPlayer() then
				Server:SendMessage( cChatColor.Red .. "[FATALITY] " .. cChatColor.White .. Victim:GetName() .. " was terminated by " .. Killer:GetName() )
				CheckHardcore(Victim)
				return false
			elseif Killer:IsMob() then
				if Killer:IsA("cZombie") then
					Server:SendMessage( cChatColor.Red .. "[FATALITY] " .. cChatColor.White .. Victim:GetName() .. " was eaten by a zombie")
				elseif Killer:IsA("cSkeleton") then
					Server:SendMessage( cChatColor.Red .. "[FATALITY] " .. cChatColor.White .. Victim:GetName() .. " was shot by a skeleton" )
				elseif Killer:IsA("cCreeper") then
					Server:SendMessage( cChatColor.Red .. "[FATALITY] " .. cChatColor.White .. Victim:GetName() .. " was blown up by a creeper")
				elseif Killer:IsA("cSpider") then
					Server:SendMessage( cChatColor.Red .. "[FATALITY] " .. cChatColor.White .. Victim:GetName() .. " was ripped apart by a giant spider")
				elseif Killer:IsA("cCaveSpider") then
					Server:SendMessage( cChatColor.Red .. "[FATALITY] " .. cChatColor.White .. Victim:GetName() .. " was poisoned by a giant cave spider")
				elseif Killer:IsA("cBlaze") then
					Server:SendMessage( cChatColor.Red .. "[FATALITY] " .. cChatColor.White .. Victim:GetName() .. " was flamed by a blaze")
				elseif Killer:IsA("cEnderman") then
					Server:SendMessage( cChatColor.Red .. "[FATALITY] " .. cChatColor.White .. Victim:GetName() .. " was " .. cChatColor.Random .. " by an enderman")
				elseif Killer:IsA("cSilverfish") then
					Server:SendMessage( cChatColor.Red .. "[FATALITY] " .. cChatColor.White .. Victim:GetName() .. " encountered an unexpectedly fatal silverfish attack")
				elseif Killer:IsA("cSlime") then
					Server:SendMessage( cChatColor.Red .. "[FATALITY] " .. cChatColor.White .. Victim:GetName() .. " was absorbed and digested by a slime")
				elseif Killer:IsA("cWitch") then
					Server:SendMessage( cChatColor.Red .. "[FATALITY] " .. cChatColor.White .. Victim:GetName() .. " was enchanted (to death) by a witch")
				elseif Killer:IsA("cZombiepigman") then
					Server:SendMessage( cChatColor.Red .. "[FATALITY] " .. cChatColor.White .. Victim:GetName() .. " was slain by a zombie pigman")
				elseif Killer:IsA("cMagmacube") then
					Server:SendMessage( cChatColor.Red .. "[FATALITY] " .. cChatColor.White .. Victim:GetName() .. " was incinerated by a magmacube")
				elseif Killer:IsA("cWolf") then
					Server:SendMessage( cChatColor.Red .. "[FATALITY] " .. cChatColor.White .. Victim:GetName() .. " was savaged by a wolf")
				end
				CheckHardcore(Victim)
				return false
			end
		end
		Server:SendMessage( cChatColor.Red .. "[FATALITY] " .. cChatColor.White .. Victim:GetName() .. " died of mysterious circumstances")
		CheckHardcore(Victim)
	end
end
