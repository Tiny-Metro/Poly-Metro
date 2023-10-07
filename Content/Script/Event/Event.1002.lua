-- Demonstration data
local stations = GetStationInfos()
local i = math.random(1, #stations)
local station_type = stations[i].StationType

function EventData()
    Data = {}
    Data.title = ''
    Data.flavor_text = station_type .. '에서 서비스 개선을 요구하는 시위 발생... 시민 불만 터졌나'
    Data.message = station_type .. '에서 교통 서비스 개선을 요구하는 시위가 발생했습니다.' .. station_type .. '으로 가는 승객이 30% 증가하고, 불만도가 10% 증가합니다.'
    Data.period = 7

    return Data
end

-- Event occur weight
function Weight()
    return 6
end

-- Event start (event effect)
function Start()
    AddPassengerSpawnProbabilityByType(station_type, 0.3)
    ScaleComplain(1.1)
end

EventDataStruct = {}
EventDataStruct.EventData = EventData
EventDataStruct.Weight = Weight
EventDataStruct.Start = Start

return EventDataStruct