-- lane change
-- Investment condition
function InvestmentData()
    local Data = {}
    Data.message = '이번 주에는 노선을 축소, 삭제하지 않고 운영하세요.'
    Data.time_require = 7
    Data.award = '800$'

    return Data
end

local lane
local pre_lane_change

-- Call when investment start
-- Used save info when start
function Start()
    lane = GetLaneDetailStatistics()
    pre_lane_change = lane.TotalModifyAndReduceCount
end

-- Investment appear condition
function Appearance()
    return true
end

-- Investment success condition
function Process()
    for i = 0, #lane do
        if lnae.TotalModifyAndReduceCount == pre_lane_change then
            return "success"
        end
    end
    return "continue"
end

-- Investment award
function Award()
    AddMoney(800)
end