-- Citizen interview positive data
function EventData()
    Data = {}
    Data.title = ''
    -- 서울 -> 맵 이름
    Data.flavor_text = '"시간도 안 지키고 냄새 나는 지하철"...' .. "서울" .. ' 지하철에 쏟아진 혹평'
    Data.message = '승객들의 민원이 폭주하고 있습니다. 서비스 비용에 투자를 할 필요가 있어보입니다... 불만도가 10% 증가합니다.'
    Data.period = 7

    return Data
end

-- Event occur weight
function Weight()
    policy = GetPolicyData()

    if policy == 1 then
        return 4
    elseif policy == 2 then
        return 3
    elseif policy == 3 then
        return 2
    elseif policy == 4 then
        return 1
    elseif policy == 5 then
        return 0
    endend

-- Event start (event effect)
function Start()
    ScaleComplain(1.1)
end