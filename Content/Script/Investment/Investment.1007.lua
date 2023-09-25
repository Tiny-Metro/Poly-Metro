-- transfer station
-- Investment condition
function InvestmentData()
    local Data = {}
    Data.message = '3개 이상의 노선이 지나는 환승역을 만드세요.'
    Data.time_require = 7
    Data.award = '500$'

    return Data
end

local lane
local pre_transfer_station

-- Call when investment start
-- Used save info when start
function Start()
    lane = GetLaneDetailStatistics()
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
    for i = 0, #lane do
        if lane[i].TransferStationCount - pre_transfer_station[i] >= 3 then
            return "success"
        end
    end
    return "continue"
end

-- Investment award
function Award()
    AddMoney(500)
end