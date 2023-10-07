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
local total_lane
local pre_lane_change
local time
local time_stamp

-- Call when investment start
-- Used save info when start
function Start()
    lane = GetLaneDetailStatistics()
    total_lane = GetLaneStatistics()
    time = GetTimestamp()
    time_stamp = time.Date
    for i = 0, total_lane.TotalLaneCount - 1 do
        pre_lane_change[i] = lane[i].TotalModifyAndReduceCount
    end
end

-- Investment appear condition
function Appearance()
    return true
end

-- Investment success condition
function Process()
    local cur_time = time.Date

    if cur_time - time_stamp < InvestmentData().time_require then
        return "continue"
    else
        for i = 0, total_lane.TotalLaneCount - 1 do
            if lane[i].TotalModifyAndReduceCount ~= pre_lane_change[i] then
                return "fail"
            end
        end
        return "success"
    end
end

-- Investment award
function Award()
    AddMoney(InvestmentData.award)

    InvestmentDataStruct= {}
    InvestmentDataStruct.InvestmentData = InvestmentData
    InvestmentDataStruct.Start = Start
    InvestmentDataStruct.Appearance = Appearance
    InvestmentDataStruct.Process = Process
    InvestmentDataStruct.Award = Award

    return InvestmentDataStruct
end