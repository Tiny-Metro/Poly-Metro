-- Circular line connection
-- Investment condition
function InvestmentData()
    local Data = {}
    Data.message = '순환선을 건설하세요.'
    Data.time_require = -1
    Data.award = '객차 1량, 300$'

    return Data
end

local lane
local total_lane
local pre_circular_lane

-- Call when investment start
-- Used save info when start
function Start()
    lane = GetLaneDetailStatistics()
    total_lane = GetLaneStatistics()
    pre_circular_lane = 0

    local lane_count = total_lane.TotalLaneCount
    for i=0, lane_count - 1 do
        if lane[i].IsCircularLane then
            pre_circular_lane = pre_circular_lane + 1
        end
    end
end

-- Investment appear condition
function Appearance()
    return true
end

-- Investment success condition
function Process()
    local cur_circular_lane = 0
    local lane_count = total_lane.TotalLaneCount

    for i=0, lane_count - 1 do
        if lane[i].IsCircularLane then
            cur_circular_lane = cur_circular_lane + 1
        end
    end

    if pre_circular_lane <= cur_circular_lane then
        return "success"
    else
        return "continue"
    end
end

-- Investment award
function Award()
    AddItem(Subtrain, 1)
    AddMoney(300)

    InvestmentDataStruct= {}
    InvestmentDataStruct.InvestmentData = InvestmentData
    InvestmentDataStruct.Start = Start
    InvestmentDataStruct.Appearance = Appearance
    InvestmentDataStruct.Process = Process
    InvestmentDataStruct.Award = Award

    return InvestmentDataStruct
end