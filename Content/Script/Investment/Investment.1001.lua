-- Circular line connection
local reward_item_amount_1001 = {Subtrain, 1}
local reward_money_1001 = 300

-- Investment condition
function InvestmentData()
    local Data = {}
    Data.message = '순환선을 건설하세요.'
    Data.time_require = -1
    Data.award = '객차 '.. reward_item_amount_1001[2] ..'량, '.. reward_money_amount_1001 ..'$'

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
    local start_lane = GetLaneStatisticsAtStart(1001)
    local start_circle_lane = GetLaneDetailStatisticsAtStart(1001)

    local cur_lane = GetLaneStatistics()
    local cur_circle_lane = GetLaneDetailStatistics()

    local lane_count
    local over_flag = false

    -- current number of lanes
    if start_lane.TotalLaneCount < cur_lane.TotalLaneCount then
        lane_count = cur_lane.TotalLaneCount
        over_flag = true
    else
        lane_count = start_lane.TotalLaneCount
    end

    for i=0, lane_count - 1 do
        if over_flag and i > (cur_lane.TotalLaneCount - start_lane.TotalLaneCount) then
            -- new lane
            if cur_circle_lane[i].IsCircularLane then
                return success
            end
        end
        else
            -- existing lane
            if start_circle_lane[i].IsCircularLane == false and cur_circle_lane[i].IsCircularLane == true then
                return success
            end
        end
    end
    return continue
end

-- Investment award
function Award()
    AddItem(reward_amount_1001[1], reward_amount_1001[2])
    AddMoney(reward_money_amount_1001)
end

InvestmentDataStruct= {}
InvestmentDataStruct.InvestmentData = InvestmentData
InvestmentDataStruct.Start = Start
InvestmentDataStruct.Appearance = Appearance
InvestmentDataStruct.Process = Process
InvestmentDataStruct.Award = Award

return InvestmentDataStruct