-- Lane connectiontimestamp
local additional_needs_1000 = 10
local reward_item_1000 = {Train, 1}
local time_needed_1000 = -1

-- Investment condition
function InvestmentData()
    local Data = {}
    Data.message = additional_needs_1000 .. ' 개의 역을 지나는 노선을 만드세요.'
    Data.time_require = time_needed_1000
    Data.award = '열차 ' .. reward_item_1000[2] .. '량'

    return Data
end

-- Call when investment start
-- Used save info when start
function Start()
end

-- Investment appear condition
function Appearance()
    return true
end

-- Investment success condition
function Process()
    local start_lane = GetLaneStatisticsAtStart(1000)
    local start_station = GetLaneDetailStatisticsAtStart(1000)

    local cur_lane = GetLaneStatistics()
    local cur_station = GetLaneDetailStatistics()

    local lane_count = cur_lane.TotalLaneCount

    if lane_count ~= 0 then
        for i = 1,  lane_count do
            if i > start_lane.TotalLaneCount then
                -- new lane
                if cur_station[i] == nil then
                    break
                elseif cur_station[i].TransferStationCount >= additional_needs_1000 then
                    return success
                end
            else
                -- existing lane
                if (cur_lane[i] == nil) or (start_lane[i] == i) then
                    break
                elseif cur_station[i].TransferStationCount >= (start_station[i].TransferStationCount + additional_needs_1000) then
                    return success
                end
            end
        end
    end

    return continue
end

-- Investment award
function Award()
    AddItem(reward_item_1000[1], reward_item_1000[2])
end

InvestmentDataStruct= {}
InvestmentDataStruct.InvestmentData = InvestmentData
InvestmentDataStruct.Start = Start
InvestmentDataStruct.Appearance = Appearance
InvestmentDataStruct.Process = Process
InvestmentDataStruct.Award = Award

return InvestmentDataStruct