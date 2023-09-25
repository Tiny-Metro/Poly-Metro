-- remaining loan amount
-- Investment condition
function InvestmentData()
    local Data = {}
    Data.message = '180명을 수송하세요'
    Data.time_require = 28
    Data.award = '500$'

    return Data
end

local statistics
local pre_passenger

-- Call when investment start
-- Used save info when start
function Start()
    statistics = GetDefaultStatistics()
    pre_passenger = statistics.TotalArrivePassenger
end

-- Investment appear condition
function Appearance()
    time = GetTimestamp()
    return time.Week < 8
end

-- Investment success condition
function Process()
    if statistics.TotalArrivePassenger >= pre_passenger + 180 then
        return "success"
    else
        return "continue"
    end
end

-- Investment award
function Award()
    AddMoney(500)
end