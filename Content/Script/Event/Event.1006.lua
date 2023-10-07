-- Crime increase data
function EventData()
    Data = {}
    Data.title = ''
    Data.flavor_text = '지하철 범죄 극성... 특단의 조치 필요하다.'
    Data.message = '지하철 내 범죄가 기승을 부리고 있습니다. CCTV를 확충하고 서비스 비용을 늘리는 것을 검토해야 합니다. 승객이 10% 감소하고, 불만도가 20% 더 증가합니다.'
    Data.period = 14

    return Data
end

-- Event occur weight
function Weight()
    policy = GetPolicyData()
    if policy.IsCCTV then
        return 3
    else
        return 6
    end
end

-- Event start (event effect)
function Start()
    AddPassengerSpawnProbability(-0.1)
    ScaleComplain(1.2)
end

EventDataStruct = {}
EventDataStruct.EventData = EventData
EventDataStruct.Weight = Weight
EventDataStruct.Start = Start

return EventDataStruct