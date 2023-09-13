-- Citizen interview positive data
function EventData()
    Data = {}
    Map = GetMapName()
    Data.title = ''
    Data.flavor_text = '"시간도 안 지키고 냄새 나는 지하철"...' .. Map .. ' 지하철에 쏟아진 혹평'
    Data.message = '승객들의 민원이 폭주하고 있습니다. 서비스 비용에 투자를 할 필요가 있어보입니다... 불만도가 10% 증가합니다.'
    Data.period = 7

    return Data
end

-- Event occur weight
function Weight()
    policy = GetPolicyData()
    level = policy.ServiceLevel

    if level == 1 then
        return 4
    elseif level == 2 then
        return 3
    elseif level == 3 then
        return 2
    elseif level == 4 then
        return 1
    elseif level == 5 then
        return 0
    end
end

-- Event start (event effect)
function Start()
    ScaleComplain(1.1)
end