-- transfer station
local additional_needs_1007 = 3
local reward_money_1007 = 500
local time_needed_1007 = 7

-- Investment condition
function InvestmentData()
    local Data = {}
    Data.message = additional_needs_1007 .. '개 이상의 노선이 지나는 환승역을 만드세요.'
    Data.time_require = time_needed_1007
    Data.award = reward_money_1007 .. '$'

    return Data
end

-- Call when investment start
-- Used save info when start
function Start()
end

-- Investment appear condition
function Appearance()
    local time = GetTimestamp()
    return time.Week >= 2
end

-- Investment success condition
function Process()
    local start_station = GetStationInfosAtStart(1007)
    local cur_station = GetStationInfos()
    
    for i = 0, #cur_station do
        -- new station
        if i > #start_station then
            if cur_station[i].ServiceLaneCount >= 3 then
                return success
            end
        -- existing station
        else
            if (cur_station[i].ServiceLaneCount - start_station.ServiceLaneCount) >= 3 then
                return success
            end
        end
    end

    return continue
end

-- Investment award
function Award()
    AddMoney(reward_money_1007)
end

InvestmentDataStruct= {}
InvestmentDataStruct.InvestmentData = InvestmentData
InvestmentDataStruct.Start = Start
InvestmentDataStruct.Appearance = Appearance
InvestmentDataStruct.Process = Process
InvestmentDataStruct.Award = Award

return InvestmentDataStruct