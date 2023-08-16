Data = {}
Data.title = '테스트'
Data.message = '신규 역이 1개 추가되고, 7일동안 불만도 상승량이 10% 증가합니다.'
Data.period = 7

-- Event occur weight
function Weight()
    return 3
end

-- Event start (event effect)
function Start()
    AddStation(1)
    AddComplainIncreaseRate(0.1)
end

return Data