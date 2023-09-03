-- Festival data
function EventData()
    Data = {}

    Data[0] = {}
    Data[0].title = '애완 돌맹이 경주 대회가 열려... 관광객들 북적북적'
    Data[0].flavor_text = '누구의 애완 돌맹이가 가장 빠를가요?!'
    Data[0].message = '에서' .. Data[0].title .. '가 열렸습니다! 역 후보지가 즉시 2개 생성되고 승객이 30% 증가합니다.'
    Data[0].period = 7
    
    Data[1] = {}
    Data[1].title = '비밀 조직 홍보회'
    Data[1].flavor_text = '일루미나티, 프리메이슨,... 여기서 가입하세요!'
    Data[1].message = '에서' .. Data[1].title .. '가 열렸습니다! 역 후보지가 즉시 2개 생성되고 승객이 30% 증가합니다.'
    Data[1].period = 7

    Data[2] = {}
    Data[2].title = '어린이를 위한 맥주 박람회'
    Data[2].flavor_text = '아버지를 위한 영양 간식도 준비되어 있습니다!'
    Data[2].message = '에서' .. Data[2].title .. '가 열렸습니다! 역 후보지가 즉시 2개 생성되고 승객이 30% 증가합니다.'
    Data[2].period = 7

    Data[3] = {}
    Data[3].title = '거울과 가위바위보 대회'
    Data[3].flavor_text = '가장 어려운 싸움이 자기 자신과의 싸움이죠!'
    Data[3].message = '에서' .. Data[3].title .. '가 열렸습니다! 역 후보지가 즉시 2개 생성되고 승객이 30% 증가합니다.'
    Data[3].period = 7

    Data[4] = {}
    Data[4].title = '그림자 따돌리기 대회'
    Data[4].flavor_text = '내 발밑에 누구야!'
    Data[4].message = '에서' .. Data[4].title .. '가 열렸습니다! 역 후보지가 즉시 2개 생성되고 승객이 30% 증가합니다.'
    Data[4].period = 7

    Data[5] = {}
    Data[5].title = '멍때리기 대회'
    Data[5].flavor_text = '아무것도 안 하는 사람이 이기는 대회입니다.'
    Data[5].message = '에서' .. Data[5].title .. '가 열렸습니다! 역 후보지가 즉시 2개 생성되고 승객이 30% 증가합니다.'
    Data[5].period = 7

    Data[6] = {}
    Data[6].title = '코골이 음악회'
    Data[6].flavor_text = '반드시 잠든 후에 연주해야 합니다.'
    Data[6].message = '에서' .. Data[6].title .. '가 열렸습니다! 역 후보지가 즉시 2개 생성되고 승객이 30% 증가합니다.'
    Data[6].period = 7

    Data[7] = {}
    Data[7].title = '유사 과학 박람회'
    Data[7].flavor_text = '물은 답을 알고 있습니다.'
    Data[7].message = '에서' .. Data[7].title .. '가 열렸습니다! 역 후보지가 즉시 2개 생성되고 승객이 30% 증가합니다.'
    Data[7].period = 7

    Data[8] = {}
    Data[8].title = '교수님 정강이 차기 대회'
    Data[8].flavor_text = '"아이고 종강이야!"라는 말이 나오면 성공! 너무 세면 "으윽..개강해"라고 할 수도 있으니 주의해야 합니다.'
    Data[8].message = '에서' .. Data[8].title .. '가 열렸습니다! 역 후보지가 즉시 2개 생성되고 승객이 30% 증가합니다.'
    Data[8].period = 7

    Data[9] = {}
    Data[9].title = '옹알이 토론 대회'
    Data[9].flavor_text = '다다따따다다다다다다다.따다다?힣!'
    Data[9].message = '에서' .. Data[9].title .. '가 열렸습니다! 역 후보지가 즉시 2개 생성되고 승객이 30% 증가합니다.'
    Data[9].period = 7

    Data[10] = {}
    Data[10].title = '평화주의자를 위한 격투기 선수권 대회'
    Data[10].flavor_text = '한 대도 안 때리고 우승할 수 있을까요?'
    Data[10].message = '에서' .. Data[10].title .. '가 열렸습니다! 역 후보지가 즉시 2개 생성되고 승객이 30% 증가합니다.'
    Data[10].period = 7

    Data[11] = {}
    Data[11].title = '무색 무지개 퍼레이드'
    Data[11].flavor_text = '무(지개)색 퍼레이드입니다!'
    Data[11].message = '에서' .. Data[11].title .. '가 열렸습니다! 역 후보지가 즉시 2개 생성되고 승객이 30% 증가합니다.'
    Data[11].period = 7

    Data[12] = {}
    Data[12].title = '투명 예술 전시회'
    Data[12].flavor_text = '아무것도 안 보이지만 훌륭하군요!'
    Data[12].message = '에서' .. Data[12].title .. '가 열렸습니다! 역 후보지가 즉시 2개 생성되고 승객이 30% 증가합니다.'
    Data[12].period = 7

    Data[13] = {}
    Data[13].title = '모기 밥 주기 대회'
    Data[13].flavor_text = '야생동물에게 먹이를 주는 것은 고귀한 일이죠!'
    Data[13].message = '에서' .. Data[13].title .. '가 열렸습니다! 역 후보지가 즉시 2개 생성되고 승객이 30% 증가합니다.'
    Data[13].period = 7

    Data[14] = {}
    Data[14].title = '대회 아이디어 공모 대회'
    Data[14].flavor_text = '절대 아이디어 고갈이 아닙니다.'
    Data[14].message = '에서' .. Data[14].title .. '가 열렸습니다! 역 후보지가 즉시 2개 생성되고 승객이 30% 증가합니다.'
    Data[14].period = 7

    math.randomseed(os.time())
    idx = math.random(0, 14)
    return Data[idx]
end

-- Event occur weight
function Weight()
    return 4
end

-- Event start (event effect)
function Start()
    AddStation(2)
    AddPassengerSpawnProbability(0.3)
end