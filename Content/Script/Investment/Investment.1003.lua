-- Complaint management
local additional_needs_1003 = 50
local reward_money_1003 = 800

-- Investment condition
function InvestmentData()
    local Data = {}
    Data.message = '어떤 역도 불만도 ' .. additional_needs_1003 .. '을 넘지 않도록 하세요.'
    Data.time_require = 7
    Data.award = reward_money_1003 .. "$"

    return Data
end

-- Call when investment start
-- Used save info when start
function Start()
end

-- Investment appear condition
function Appearance()
    local stations = GetStationInfos()

    for i, v in pairs(stations) do
        if v.Complain > additional_needs_1003 then
            return false
        end
    end
    return true
end

-- Investment success condition
function Process()
    local start_time = GetTimestampAtStart(1003)
    local cur_time = GetTimestamp()
    local stations = GetStationInfos()

    local all_stations_limit = true

    for i, v in pairs(stations) do
        if v.Complain > additional_needs_1003 then
            -- If the deadline is exceeded, it immediately fails.
            all_stations_limit = false
            break
        end
    end

    if all_stations_limit then
        if (start_time.Date - cur_time.Date) <= InvestmentData().time_require then
            return continue
        else
            return success
        end
    else
        return fail
    end
end

-- Investment award
function Award()
    AddMoney(reward_money_1003)
end

InvestmentDataStruct= {}
InvestmentDataStruct.InvestmentData = InvestmentData
InvestmentDataStruct.Start = Start
InvestmentDataStruct.Appearance = Appearance
InvestmentDataStruct.Process = Process
InvestmentDataStruct.Award = Award

return InvestmentDataStruct