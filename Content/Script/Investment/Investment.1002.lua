-- additional operations
local additional_needs_1002 = 10
local reward_money_1002 = 500
local time_needed_1002 = 14

-- Investment condition
function InvestmentData()
    local Data = {}
    Data.message = additional_needs_1002 .. '개의 역을 추가로 운임하세요.'
    Data.time_require = time_needed_1002
    Data.award = reward_money_1002 .. "$"

    return Data
end

-- Call when investment start
-- Used save info when start
function Start()
end

-- Investment appear condition
function Appearance()
    return true
end

-- Investment success condition
function Process()
    local start_stats = GetDefaultStatisticsAtStart(1002)
    local cur_stats = GetDefaultStatistics()

    if (start_stats.ServiceStationCount + additional_needs_1002) <= cur_stats.ServiceStationCount then
        return success
    end
    return continue
end

-- Investment award
function Award()
    AddMoney(reward_money_1002)
end

InvestmentDataStruct= {}
InvestmentDataStruct.InvestmentData = InvestmentData
InvestmentDataStruct.Start = Start
InvestmentDataStruct.Appearance = Appearance
InvestmentDataStruct.Process = Process
InvestmentDataStruct.Award = Award

return InvestmentDataStruct