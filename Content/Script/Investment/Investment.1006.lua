-- don't build
target_1006 = {
    {name = "다리", award = {Bridge, 2}, message = "다리 2개"},
    {name = "터널", award = {Tunnel, 2}, message = "터널 2개"},
    {name = "노선", award = {Train, 2}, message = "열차 2량"}
}

local selected_target_1006

-- Investment condition
function InvestmentData()
    local Data = {}
    local idx = math.random(1, #target_1006)
    selected_target_1006 = target_1006[idx]

    Data.message = selected_target_1006.name .. '을 건설하지 마세요.'
    Data.time_require = 7
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
    return time.Week > 2
end

-- Investment success condition
function Process()
    local pre_bridge = GetBridgeStatisticsAtStart(1006)
    local pre_tunnel = GetTunnelStatisticsAtStart(1006)
    local pre_lane = GetLaneDetailStatisticsAtStart(1006)

    local cur_bridge = GetBridgeStatistics()
    local cur_tunnel = GetTunnelStatistics()
    local cur_lane = GetLaneDetailStatistics()

    if selected_target_1006.name == "다리" and (cur_bridge.TotalPlacementCount == pre_bridge.TotalPlacementCount) then
        return success
    elseif selected_target_1006.name == "터널" and (cur_tunnel.TotalPlacementCount == pre_tunnel.TotalPlacementCount) then
        return success
    elseif selected_target_1006.name == "노선" and (cur_lane.TotalModifyAndDeleteCount == pre_lane.TotalModifyAndDeleteCount) then
        return success
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