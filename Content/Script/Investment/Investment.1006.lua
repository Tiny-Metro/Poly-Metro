-- don't build
local target_1006 = {
    {name = "다리", award = {Bridge, 2}, message = "다리 2개"},
    {name = "터널", award = {Tunnel, 2}, message = "터널 2개"},
    {name = "노선", award = {Train, 2}, message = "열차 2량"}
}

local selected_target_1006
local time_needed_1006 = 7

-- Investment condition
function InvestmentData()
    local Data = {}
    local idx = math.random(1, #target_1006)
    selected_target_1006 = target_1006[idx]

    Data.message = selected_target_1006.name .. '을/를 건설하지 마세요.'
    Data.time_require = time_needed_1006
    Data.award = selected_target_1006.message

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
    local start_time = GetTimestampAtStart(1006)
    local start_bridge = GetBridgeStatisticsAtStart(1006)
    local start_tunnel = GetTunnelStatisticsAtStart(1006)
    --local start_lane = GetLaneDetailStatisticsAtStart(1006)
    local start_lane_count = GetLaneStatisticsAtStart(1006)

    local cur_time = GetTimestamp()
    local cur_bridge = GetBridgeStatistics()
    local cur_tunnel = GetTunnelStatistics()
    --local cur_lane = GetLaneDetailStatistics()
    local cur_lane_count = GetLaneStatistics()

    local lane_count = cur_lane_count.TotalLaneCount

    if selected_target_1006.name == "다리" then
        if (cur_time.Date - start_time.Date) >= time_needed_1006 then
            if cur_bridge.TotalPlacementCount == start_bridge.TotalPlacementCount then
                return success
            else
                return fail
            end   
        else
            if cur_bridge.TotalPlacementCount ~= start_bridge.TotalPlacementCount then
                return fail
            end
        end
    end

    if selected_target_1006.name == "터널" then
        if (cur_time.Date - start_time.Date) >= time_needed_1006 then
            if cur_tunnel.TotalPlacementCount == start_tunnel.TotalPlacementCount then
                return success
            else
                return fail
            end   
        else
            if cur_tunnel.TotalPlacementCount ~= start_tunnel.TotalPlacementCount then
                return fail
            end
        end
    end

    if selected_target_1006.name == "노선" then
        -- build
        if (cur_time.Date - start_time.Date) >= time_needed_1006 then
            if cur_lane_count.TotalLaneCount == start_lane_count.TotalLaneCount then
                return success
            else
                return fail
            end
        else
            if cur_lane_count.TotalLaneCount ~= start_lane_count.TotalLaneCount then
                return fail
            end
        end
        -- modoify
        --[[
        if (cur_time.Date - start_time.Date) > time_needed_1006 then
            for i = 1, lane_count do
                if cur_lane[i].TotalModifyAndDeleteCount == start_lane[i].TotalModifyAndDeleteCount then
                    return success
                else
                    return fail
                end
            end
        else
            for i = 1, lane_count do
                if cur_lane[i].TotalModifyAndDeleteCount ~= start_lane[i].TotalModifyAndDeleteCount then
                    return fail
                end
            end
        end
        --]]
    end

    return continue
end

-- Investment award
function Award()
    AddItem(selected_target_1006[1], selected_target_1006[2])
end

InvestmentDataStruct= {}
InvestmentDataStruct.InvestmentData = InvestmentData
InvestmentDataStruct.Start = Start
InvestmentDataStruct.Appearance = Appearance
InvestmentDataStruct.Process = Process
InvestmentDataStruct.Award = Award

return InvestmentDataStruct