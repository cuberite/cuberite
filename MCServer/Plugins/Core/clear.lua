function HandleClearCommand( Split, Player )

  if (Split[2] == nil) then
     SendMessage( Player, "Usage: /clear <player>" )
     return true
  end

  local InventoryCleared = false;
  local ClearInventory = function(OtherPlayer)
    if (OtherPlayer:GetName() == Split[2]) then
      OtherPlayer:GetInventory():Clear()
      InventoryCleared = true
    end
  end

  cRoot:Get():FindAndDoWithPlayer(Split[2], ClearInventory);
  if (InventoryCleared) then
    SendMessageSuccess( Player, "You cleared the inventory of " .. Split[2] )
    return true
  else
    SendMessageFailure( Player, "Player not found" )
    return true
  end

end
