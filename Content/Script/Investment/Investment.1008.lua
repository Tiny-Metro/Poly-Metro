-- lane change
-- Investment condition
function InvestmentData()
    local Data = {}
    Data.message = '이번 주에는 노선을 축소, 삭제하지 않고 운영하세요.'
    Data.time_require = 7
    Data.award = 800

    return Data
end

local lane
local pre_lane_change
local time
local timestamp

-- Call when investment start
-- Used save info when start
function Start()
    lane = GetLaneDetailStatistics()
    time = GetTimestamp()
    timestamp = time.Date
    pre_lane_change = lane.TotalModifyAndReduceCount
end

-- Investment appear condition
function Appearance()
    return true
end

-- Investment success condition
function Process()
    local curtime = time.Date

    if lane.TotalModifyAndReduceCount == pre_lane_change then
        return "success"
    end

    if curtime - timestamp >= InvestmentData().time_require then
        return "fail"
    end

    return "continue"
end

-- Investment award
function Award()
    AddMoney(InvestmentData.award)
end