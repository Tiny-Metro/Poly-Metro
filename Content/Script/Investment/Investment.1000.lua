-- Lane connectiontimestamp
local additional_needs_1000 = 10
local reward_item_1000 = {Train, 1}
local time_needed_1000 = -1

-- Investment condition
function InvestmentData()
    local Data = {}
    Data.message = additional_needs_1000 .. ' 개의 역을 지나는 노선을 만드세요.'
    Data.time_require = time_needed_1000
    Data.award = '열차 ' .. reward_item_1000[2] .. '량'

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
    local start_lane = GetLaneDetailStatisticsAtStart(1000)
    local start_station = {}

    for i = 1, 8 do
        start_station[i] = start_lane[i].ServiceStationCount
    end

    local cur_lane = GetLaneDetailStatistics()
    for i = 1, 8 do
        if start_station[i] + additional_needs_1000 <= cur_lane[i].ServiceStationCount then
            return success
        end
    end

    return continue
end

-- Investment award
function Award()
    AddItem(reward_item_1000[1], reward_item_1000[2])
end

InvestmentDataStruct= {}
InvestmentDataStruct.InvestmentData = InvestmentData
InvestmentDataStruct.Start = Start
InvestmentDataStruct.Appearance = Appearance
InvestmentDataStruct.Process = Process
InvestmentDataStruct.Award = Award

return InvestmentDataStruct