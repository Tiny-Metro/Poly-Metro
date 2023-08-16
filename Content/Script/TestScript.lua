print("Hello Lua!")

tmp = GetTimestamp()

function TestFunction()
    stations = GetStationInfos()
    for i, v in pairs(stations) do
        print(v.StationType)
        AddPassengerSpawnProbabilityByType(v.StationType, 50)
    end
end

return 1