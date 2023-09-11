-- Convenience of movement data
function EventData()
    Data = {}
    Data.title = ''
    Data.flavor_text = '"엘리베티어 설치 이후 지하철 이용이 편해졌어요", 승객 만족도 상승'
    Data.message = '엘리베이터 설치 후 교통약자를 포함해 더 많은 승객들이 지하철을 이용하게 되었습니다. 승객이 20%증가하고, 불만도가 20% 감소합니다.'
    Data.period = 7

    return Data
end

-- Event occur weight
function Weight()
    return 3
end

-- Event start (event effect)
function Start()
    AddPassengerSpawnProbability(0.2)
    ScaleComplain(0.8)
end