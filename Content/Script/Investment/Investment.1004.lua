-- carriage connection
-- Investment condition
function InvestmentData()
    local Data = {}
    Data.message = '객차가 두 개 이상 달린 열차를 만드세요.'
    Data.time_require = 7
    Data.award = '열차 1량, 300$'

    return Data
end

local subtrains
local pre_subtrain_count

-- Call when investment start
-- Used save info when start
function Start()
    subtrains = GetSubtrainStatistics()
    pre_subtrain_count = subtrains.TotalPlacementCount
end

-- Investment appear condition
function Appearance()
    time = GetTimestamp()
    return time.Week > 2
end

-- Investment success condition
function Process()
    -- 모든 열차의 객차 확인 후, 추가적으로 객차가 2개 이상 연결되 열차가 있을 시 success 반환
    return "success"
end

-- Investment award
function Award()
    AddItem(Train, 1)
    AddMoney(500)
end