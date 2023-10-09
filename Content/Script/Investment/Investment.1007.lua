-- transfer station
local additional_needs_1007 = 3
local reward_money_amount_1007 = 500

-- Investment condition
function InvestmentData()
    local Data = {}
    Data.message = additional_needs_1007 .. '개 이상의 노선이 지나는 환승역을 만드세요.'
    Data.time_require = 7
    Data.award = reward_money_amount_1007 .. '$'

    return Data
end

-- Call when investment start
-- Used save info when start
function Start()
end

-- Investment appear condition
function Appearance()
    local time = GetTimestamp()
    return time.Week > 2
end

-- Investment success condition
function Process()
    local start_lane = GetLaneDetailStatisticsAtStart(1007)
    local start_total_lane = GetLaneStatisticsAtStart(1007)

    local cur_lane = GetLaneDetailStatistics()
    local cur_total_lane = GetLaneStatistics()
    
    local lane_count
    local over_flag = false

    -- current number of lanes
    if start_lane.TotalLaneCount < cur_lane.TotalLaneCount then
        lane_count = cur_lane.TotalLaneCount
        over_flag = true
    else
        lane_count = start_lane.TotalLaneCount
    end

    for i = 0, lane_count - 1 do
        if over_flag and i > (cur_lane.TotalLaneCount - start_lane.TotalLaneCount) then
            -- new lane
            if cur_lane[i].TransferStationCount >= additional_needs_1004 then
                return success
            end
        else
            -- existing lane
            if (cur_lane[i].TransferStationCount - start_lane[i].TransferStationCount) >= additional_needs_1004 then
                return success
            end
        end
    end

    return continue
end

-- Investment award
function Award()
    AddMoney(reward_money_amount_1007)
end

InvestmentDataStruct= {}
InvestmentDataStruct.InvestmentData = InvestmentData
InvestmentDataStruct.Start = Start
InvestmentDataStruct.Appearance = Appearance
InvestmentDataStruct.Process = Process
InvestmentDataStruct.Award = Award

return InvestmentDataStruct