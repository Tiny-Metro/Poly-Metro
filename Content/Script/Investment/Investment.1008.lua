-- lane change
local reward_money_1008 = 800
local time_needed_1008 = 7

-- Investment condition
function InvestmentData()
    local Data = {}
    Data.message = '이번 주에는 노선을 축소, 삭제하지 않고 운영하세요.'
    Data.time_require = time_needed_1008
    Data.award = reward_money_1008 .. '$'

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
    local start_lane = GetLaneDetailStatisticsAtStart(1008)
    local start_total_lane = GetLaneDetailStatisticsAtStart(1008)
    local start_time = GetTimestampAtStart(1008)

    local cur_lane = GetLaneDetailStatistics()
    local cur_total_lane = GetLaneStatistics()
    local cur_time = GetTimestamp()

    local lane_count = cur_total_lane.TotalLaneCount

    -- build
    if cur_total_lane.TotalLaneCount < start_total_lane.TotalLaneCount then
        return fail
    end

    -- modoify
    if (cur_time.Date - start_time.Date) > time_needed_1006 then
        for i = 1, lane_count do
            if cur_lane[i].TotalModifyAndReduceCount > start_lane[i].TotalModifyAndReduceCount then
                return fail
            else
                return success
            end
        end
    else
        for i = 1, lane_count do
            if cur_lane[i].TotalModifyAndReduceCount > start_lane[i].TotalModifyAndReduceCount then
                return fail
            end
        end
    end

    return continue
end

-- Investment award
function Award()
    AddMoney(reward_money_1008)
end

InvestmentDataStruct= {}
InvestmentDataStruct.InvestmentData = InvestmentData
InvestmentDataStruct.Start = Start
InvestmentDataStruct.Appearance = Appearance
InvestmentDataStruct.Process = Process
InvestmentDataStruct.Award = Award

return InvestmentDataStruct