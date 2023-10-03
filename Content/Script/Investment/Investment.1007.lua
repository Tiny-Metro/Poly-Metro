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
local pre_transfer_station
local time
local timestamp

-- Call when investment start
-- Used save info when start
function Start()
    lane = GetLaneDetailStatistics()
    time = GetTimestamp()
    timestamp = time.Date
    pre_transfer_station = {}

    for i = 0, #lane do
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
    local curtime = time.Date

    for i = 0, #lane do
        if lane[i].TransferStationCount - pre_transfer_station[i] >= 3 then
            return "success"
        end
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