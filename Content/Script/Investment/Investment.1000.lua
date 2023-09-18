-- Lane connection
-- Investment condition
function InvestmentData()
    local Data = {}
    Data.message = '10개의 역을 지나는 노선을 만드세요.'
    Data.time_require = -1
    Data.award = '열차 1량'

    return Data
end

local lane
local target_station_count
local pre_sum

-- Call when investment start
-- Used save info when start
function Start()
    lane = GetLaneDetailStatistics()
    target_station_count = 10
    pre_sum = 0

    local lane_count = lane.TotalLaneCount
    for i=0, lane_count - 1 do
        pre_sum = pre_sum + lane[i].TransferStationCount
    end
end

-- Investment appear condition
function Appearance()
    return true
end

-- Investment success condition
function Process()
    local cur_sum = 0
    local lane_count = lane.TotalLaneCount

    for i=0, lane_count - 1 do
        cur_sum = cur_sum + lane[i].TransferStationCount
    end

    if pre_sum + target_station_count <= cur_sum then
        return "success"
    else
        return "continue"
    end
end

-- Investment award
function Award()
    AddItem(Train, 1)
end