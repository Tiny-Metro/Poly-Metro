-- Festival data
function EventData()
    local Data = {}
    local stations = GetStationInfos()
    local i = math.random(1, #stations)

    local festival = {
        {title = '애완 돌맹이 경주', flavor = '누구의 애완 돌맹이가 가장 빠를까요?!'},
        {title = '비밀 조직 홍보회', flavor ='일루미나티, 프리메이슨,... 여기서 가입하세요!'},
        {title ='어린이를 위한 맥주 박람회', flavor = '아버지를 위한 영양 간식도 준비되어 있습니다!'},
        {title ='거울과 가위바위보 대회', flavor = '가장 어려운 싸움이 자기 자신과의 싸움이죠!'},
        {title ='그림자 따돌리기 대회', flavor = '내 발밑에 누구야!'},
        {title ='멍때리기 대회', flavor = '아무것도 안 하는 사람이 이기는 대회입니다.'},
        {title ='코골이 음악회', flavor = '반드시 잠든 후에 연주해야 합니다.'},
        {title ='유사 과학 박람회', flavor = '물은 답을 알고 있습니다.'},
        {title ='교수님 정강이 차기 대회', flavor = '"아이고 종강이야!"라는 말이 나오면 성공! 너무 세면 "으윽..개강해"라고 할 수도 있으니 주의해야 합니다.'},
        {title ='옹알이 토론 대회', flavor = '다다따따다다다다다다다다.따다다?힣!'},
        {title ='평화주의자를 위한 격투기 선수권 대회', flavor = '한 대도 안 때리고 우승할 수 있을까요?'},
        {title ='무색 무지개 퍼레이드', flavor = '무(지개)색 퍼레이드입니다!'},
        {title ='투명 예술 전시회', flavor = '아무것도 안 보이지만 훌륭하군요!'},
        {title ='모기 밥 주기 대회', flavor = '야생동물에게 먹이를 주는 것은 고귀한 일이죠!'},
        {title ='대회 아이디어 공모 대회', flavor = '절대 아이디어 고갈이 아닙니다.'}
    }

    local idx = math.random(1, #festival)

    local title = festival[idx].title
    local flavor_text = festival[idx].flavor
    
    Data.title = title
    Data.flavor_text = flavor_text
    Data.message = stations[i].StationType .. '에서' .. title .. '가 열렸습니다! 역 후보지가 즉시 2개 생성되고 승객이 30% 증가합니다.'
    Data.period = 7

    return Data
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

EventDataStruct = {}
EventDataStruct.EventData = EventData
EventDataStruct.Weight = Weight
EventDataStruct.Start = Start

return EventDataStruct