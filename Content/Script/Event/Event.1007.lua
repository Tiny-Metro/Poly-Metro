-- Crime arrest data
function EventData()
    local Data = {}
    Data.title = ''
    Data.flavor_text = '지하철 범죄율 크게 감소... "CCTV 효과"봤나'
    Data.message = 'CCTV 설치 이후 범죄가 눈에 띄게 감소했습니다. 불만도가 20% 감소합니다.'
    Data.period = 7

    return Data
end

-- Event occur weight
function Weight()
    local policy = GetPolicyData()
    if policy.IsCCTV then
        return 3
    else
        return 0
    end
end

-- Event start (event effect)
function Start()
    ScaleComplain(0.8)
end

EventDataStruct = {}
EventDataStruct.EventData = EventData
EventDataStruct.Weight = Weight
EventDataStruct.Start = Start

return EventDataStruct