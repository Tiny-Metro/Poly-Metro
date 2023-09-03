-- Increase in tourists data
function EventData()
    Data = {}
    -- 랜드마크 이름
    Data.title = '랜드마크'
    Data.flavor_text = '에 대한 관심 증가... 관광객 크게 늘어'
    Data.message = '랜드마크가 전세계적으로 인기를 끌고 있습니다. 이를 찾는 관광객이 늘었습니다. 승객이 10% 증가합니다.'
    Data.period = 14

    return Data
end

-- Event occur weight
function Weight()
    return 6
end

-- Event start (event effect)
function Start()
    AddPassengerSpawnProbability(0.1)
end