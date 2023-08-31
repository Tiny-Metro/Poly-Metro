-- Event data
function EventData()
    Data = {}
    Data[0] = {}
    Data[0].title = '테스트1'
    Data[0].flavor_text = '플레이버 텍스트 1'
    Data[0].message = '첫번째 테스트입니다. 사양이 갑자기 바뀌어서 곤란해졌습니다.'
    Data[0].period = 1
    
    Data[1] = {}
    Data[1].title = '테스트2'
    Data[1].flavor_text = '플레이버 텍스트 2'
    Data[1].message = '두번째 테스트입니다. 메시지가 많아지면 어찌될까 걱정됩니다.'
    Data[1].period = 2

    Data[2] = {}
    Data[2].title = '테스트3'
    Data[2].flavor_text = '플레이버 텍스트 3'
    Data[2].message = '세번째 테스트라고 쓰면 뭔가 형식이 정해진 느낌이 듭니다.'
    Data[2].period = 3

    Data[3] = {}
    Data[3].title = '테스트4'
    Data[3].flavor_text = '플레이버 텍스트 4'
    Data[3].message = '네번째밖에 안 썼는데 벌써부터 노가다 느낌이 심해졌습니다.'
    Data[3].period = 4

    Data[4] = {}
    Data[4].title = '테스트5'
    Data[4].flavor_text = '플레이버 텍스트 5'
    Data[4].message = '다섯번째만 하면 테스트는 어느정도 할 만 할 것 같습니다.'
    Data[4].period = 5

    math.randomseed(os.time())
    idx = math.random(0, 4)
    return Data[idx]
end

-- Event occur weight
function Weight()
    return 3
end

-- Event start (event effect)
function Start()
    AddStation(1)
    AddComplainIncreaseRate(0.1)
end