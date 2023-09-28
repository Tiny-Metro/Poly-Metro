-- remaining loan amount
-- Investment condition
function InvestmentData()
    local Data = {}
    Data.message = '180명을 수송하세요'
    Data.time_require = 28
    Data.award = 500

    return Data
end

local statistics
local pre_passenger
local time
local timestamp

-- Call when investment start
-- Used save info when start
function Start()
    statistics = GetDefaultStatistics()
    time = GetTimestamp()
    timestamp = time.Date
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
    end

    local curtime = time.Date
    if curtime - timestamp >= InvestmentData().time_require then
        return "fail"
    end

    return "continue"
end

-- Investment award
function Award()
    AddMoney(InvestmentData.award)
end