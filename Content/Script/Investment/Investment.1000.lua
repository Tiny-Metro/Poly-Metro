Data = {}
Data.title = '환승 시스템 요청'
Data.content = '대중교통 간 환승 시스템을 도입해야 한다는 목소리가 커지고 있습니다. 정부에서는 환승 시스템을 20일간 시범 도입하는 방안을 고려하고 있습니다.'
Data.time_require = 20
Data.require_message = '환승 시스템 20일 동안 활성화'
Data.accept_message = '+300$'
Data.award_message = '+300$, 7일간 승객 20% 증가'
Data.reward_message = '-300$, 7일간 승객 20% 감소'

function InvestmentStart()
    AddMoney(1111)
    SetTransfer(true)
    print("Code 1000 : StationCount = "..GetStationCount())
end

function InvestmentSuccess()
end

function InvestmentFail()
end

function InvestmentCheck()
end

return Data