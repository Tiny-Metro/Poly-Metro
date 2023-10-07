-- Decline in tourists data
function EventData()
    Data = {}
    Map = GetMapName()
    Data.title = ''
    Data.flavor_text = '"생각보다 별거 없네"...' .. Map .. '을 찾는 관광객 크게 줄어'
    Data.message = Map .. '을 찾는 관광객이 늘었습니다. 승객이 10% 감소합니다.'
    Data.period = 14

    return Data
end

-- Event occur weight
function Weight()
    return 6
end

-- Event start (event effect)
function Start()
    AddPassengerSpawnProbability(-0.1)
end

EventDataStruct = {}
EventDataStruct.EventData = EventData
EventDataStruct.Weight = Weight
EventDataStruct.Start = Start

return EventDataStruct