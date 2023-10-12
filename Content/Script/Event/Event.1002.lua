-- Demonstration data
local stations_1002 = GetStationInfos()
local i_1002 = math.random(1, #stations_1002)
local station_type_1002 = stations_1002[i_1002].StationType

function EventData()
    local Data = {}
    Data.title = ''
    Data.flavor_text = station_type_1002 .. '에서 서비스 개선을 요구하는 시위 발생... 시민 불만 터졌나'
    Data.message = station_type_1002 .. '에서 교통 서비스 개선을 요구하는 시위가 발생했습니다.' .. station_type_1002 .. '으로 가는 승객이 30% 증가하고, 불만도가 10% 증가합니다.'
    Data.period = 7

    return Data
end

-- Event occur weight
function Weight()
    return 6
end

-- Event start (event effect)
function Start()
    AddPassengerSpawnProbabilityByType(station_type_1002, 0.3)
    ScaleComplain(1.1)
end

EventDataStruct = {}
EventDataStruct.EventData = EventData
EventDataStruct.Weight = Weight
EventDataStruct.Start = Start

return EventDataStruct