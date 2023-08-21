-- Investment condition
function InvestmentData()
    Data = {}
    Data.message = '테스트 미션 : 3일 후 완료됩니다.'
    Data.time_require = 7
    Data.award = '5000$'

    return Data
end

saveDate = {}

-- Call when investment start
-- Used save info when start
function Start()
    saveDate = GetTimestamp()
end

-- Investment appear condition
function Appearance()
    return true
end

-- Investment success condition
function Process()
    print(saveDate.Date)
    missionState = GetTimestamp()
    if saveDate.Date + 2 < missionState.Date then
        return success
    end
end

-- Investment award
function Award()
    AddIncome(5000)
end