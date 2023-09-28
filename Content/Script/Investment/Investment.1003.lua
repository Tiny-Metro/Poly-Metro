-- Complaint management
-- Investment condition
function InvestmentData()
    local Data = {}
    Data.message = '어떤 역도 불만도 50을 넘지 않도록 하세요.'
    Data.time_require = 7
    Data.award = 800

    return Data
end

local stations
local time
local timestamp

-- Call when investment start
-- Used save info when start
function Start()
    stations = GetStationInfos()
    time = GetTimestamp()
    timestamp = time.Date
end

-- Investment appear condition
function Appearance()
    for i=0, #stations do
        if stations[i].Complain > 50 then
            return false
        end
    end
    return true
end

-- Investment success condition
function Process()
    local curtime = time.Date
    local isFail = false

    for i = 0, #stations do
        if stations[i].Complain > 50 then
            isFail = true
            break
        end
    end

    if curtime - timestamp < InvestmentData().time_require then
        return isFail and "fail" or "continue"
    else
        return isFail and "fail" or "success"
    end
end

-- Investment award
function Award()
    AddMoney(InvestmentData.award)
end