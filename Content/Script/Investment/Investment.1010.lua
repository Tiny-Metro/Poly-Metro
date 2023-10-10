-- remaining loan amount
local additional_needs_1010 = 180
local reward_money_1010 = 500

-- Investment condition
function InvestmentData()
    local Data = {}
    Data.message = additional_needs_1010 .. '명을 수송하세요'
    Data.time_require = 28
    Data.award = reward_money_1010 .. '$'

    return Data
end

-- Call when investment start
-- Used save info when start
function Start()
end

-- Investment appear condition
function Appearance()
    time = GetTimestamp()
    return time.Week < 8
end

-- Investment success condition
function Process()
    local start_stats = GetDefaultStatisticsAtStart(1010)
    local cur_stats = GetDefaultStatistics()

    if cur_stats.TotalArrivePassenger >= (start_stats.TotalArrivePassenger + additional_needs_1010) then
        return success
    end

    return continue
end

-- Investment award
function Award()
    AddMoney(reward_money_1010)
end

InvestmentDataStruct= {}
InvestmentDataStruct.InvestmentData = InvestmentData
InvestmentDataStruct.Start = Start
InvestmentDataStruct.Appearance = Appearance
InvestmentDataStruct.Process = Process
InvestmentDataStruct.Award = Award

return InvestmentDataStruct