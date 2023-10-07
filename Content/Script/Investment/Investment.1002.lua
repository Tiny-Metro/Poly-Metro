-- additional operations
-- Investment condition
function InvestmentData()
    local Data = {}
    Data.message = '10개의 역을 추가로 운임하세요.'
    Data.time_require = 14
    Data.award = 500

    return Data
end

local stations
local target_station_count
local pre_active_station
local time
local time_stamp

-- Call when investment start
-- Used save info when start
function Start()
    stations = GetStationInfos()
    time = GetTimestamp()
    time_stamp = time.Date
    target_station_count = 10
    pre_active_station = 0
    
    for i=0, #stations do
        if stations[i].IsActive then
            pre_active_station = pre_active_station + 1
        end
    end
end

-- Investment appear condition
function Appearance()
    return true
end

-- Investment success condition
function Process()
    local cur_active_station = 0
    local cur_time = time.Date

    for i=0, #stations do
        if stations[i].IsActive then
            cur_active_station = cur_active_station + 1
        end
    end

    if pre_active_station + target_station_count <= cur_active_station then
        return "success"
    elseif cur_time - time_stamp > InvestmentData.time_require then
        return "fail"
    else
        return "continue"
    end
end

-- Investment award
function Award()
    AddMoney(InvestmentData.award)
end