-- Crime arrest data
function EventData()
    Data = {}
    Data.title = ''
    Data.flavor_text = '지하철 범죄율 크게 감소... "CCTV 효과"봤나'
    Data.message = 'CCTV 설치 이후 범죄가 눈에 띄게 감소했습니다. 불만도가 20% 감소합니다.'
    Data.period = 7

    return Data
end

-- Event occur weight
function Weight()
    policy = GetPolicyData()
    if policy.IsCCTV
    {
        return 3
    }
    return 0
end

-- Event start (event effect)
function Start()
    ScaleComplain(0.8)
end