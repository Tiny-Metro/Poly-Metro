-- transfer station
-- Investment condition
function InvestmentData()
    local Data = {}
    Data.message = '3개 이상의 노선이 지나는 환승역을 만드세요.'
    Data.time_require = 7
    Data.award = 500

    return Data
end

local lane
local total_lane
local pre_transfer_station
local time
local time_stamp

-- Call when investment start
-- Used save info when start
function Start()
    lane = GetLaneDetailStatistics()
    total_lane = GetLaneStatistics()
    time = GetTimestamp()
    time_stamp = time.Date
    pre_transfer_station = {}

    for i = 0, total_lane.TotalLaneCount - 1 do
        pre_transfer_station[i] = lane[i].TransferStationCount
    end
end

-- Investment appear condition
function Appearance()
    time = GetTimestamp()
    return time.Week > 2
end

-- Investment success condition
function Process()
    local cur_time = time.Date

    for i = 0, #lane do
        if lane[i].TransferStationCount - pre_transfer_station[i] >= 3 then
            return "success"
        end
    end

    if cur_time - time_stamp >= InvestmentData().time_require then
        return "fail"
    end
    
    return "continue"
end

-- Investment award
function Award()
    AddMoney(InvestmentData.award)
end