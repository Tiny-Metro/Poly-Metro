-- lane change
local reward_money_1008 = 800

-- Investment condition
function InvestmentData()
    local Data = {}
    Data.message = '이번 주에는 노선을 축소, 삭제하지 않고 운영하세요.'
    Data.time_require = 7
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

    local lane_count

    -- current number of lanes
    if start_lane.TotalLaneCount ~= cur_lane.TotalLaneCount then
        return fail
    else
        lane_count = start_lane.TotalLaneCount
    end

    if cur_time - time_stamp <= InvestmentData().time_require then
        -- within the deadline
        for i = 0, lane_count - 1 do
            if cur_lane[i].TotalModifyAndReduceCount ~= start_lane[i].TotalModifyAndReduceCount then
                return fail
            end
        end
        return continue
    else
        -- outside the deadline
        for i = 0, lane_count - 1 do
            if cur_lane[i].TotalModifyAndReduceCount ~= start_lane[i].TotalModifyAndReduceCount then
                return fail
            end
        end
        return success
    end
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