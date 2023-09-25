-- Citizen interview positive data
function EventData()
    Data = {}
    Map = GetMapName()
    Data.title = ''
    Data.flavor_text = Map..'"지하철 서비스에 대만족" 시민 호평 잇따라'
    Data.message = '지하철 서비스에 대한 칭찬이 자자합니다. 서비스 비용에 투자한 보람이 있군요! 불만도가 10% 감소합니다.'
    Data.period = 7

    return Data
end

-- Event occur weight
function Weight()
    policy = GetPolicyData()
    level = policy.ServiceLevel

    if level == 1 then
        return 0
    elseif level == 2 then
        return 1
    elseif level == 3 then
        return 2
    elseif level == 4 then
        return 3
    elseif level == 5 then
        return 4
    end
end

-- Event start (event effect)
function Start()
    ScaleComplain(0.9)
end