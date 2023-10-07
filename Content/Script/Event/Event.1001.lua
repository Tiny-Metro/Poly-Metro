-- Calamity data
local calamities = {
    { title = '사라', message = '을 덮치다! 역이 파괴될 수 있습니다.' },
    { title = '셀마', message = '을 덮치다! 역이 파괴될 수 있습니다.' },
    { title = '볼라벤', message = '을 덮치다! 역이 파괴될 수 있습니다.' },
    { title = '매미', message = '을 덮치다! 역이 파괴될 수 있습니다.' },
    { title = '메기', message = '을 덮치다! 역이 파괴될 수 있습니다.' },
    { title = '나비', message = '을 덮치다! 역이 파괴될 수 있습니다.' },
    { title = '곤파스', message = '을 덮치다! 역이 파괴될 수 있습니다.' },
    { title = '무이파', message = '을 덮치다! 역이 파괴될 수 있습니다.' },
    { title = '카눈', message = '을 덮치다! 역이 파괴될 수 있습니다.' },
    { title = '링링', message = '을 덮치다! 역이 파괴될 수 있습니다.' },
    { title = '아이유', message = '을 덮치다! 역이 파괴될 수 있습니다.' },
    { title = '마이삭', message = '을 덮치다! 역이 파괴될 수 있습니다.' },
    { title = '힌남노', message = '을 덮치다! 역이 파괴될 수 있습니다.' },
    { title = '전태풍', message = '을 덮치다! 역이 파괴될 수 있습니다.' }
}

local n = 0

function EventData()
    local Data = {}
    local idx = math.random(1, #calamities)
    n = n + 1

    local calamity = calamities[idx]

    Data.title = calamity.title
    Data.flavor_text = n .. '호 태풍 ' .. calamity.title
    Data.message = '태풍 ' .. calamity.title .. '이 ' .. calamity.message
    Data.period = 3

    return Data
end

-- Event occur weight
function Weight()
    return 2
end

-- Event start (event effect)
function Start()
    local i = math.random(1, 3)
    DestroyStation(i)
    AddPassengerSpawnProbability(-0.3)
end

EventDataStruct = {}
EventDataStruct.EventData = EventData
EventDataStruct.Weight = Weight
EventDataStruct.Start = Start

return EventDataStruct