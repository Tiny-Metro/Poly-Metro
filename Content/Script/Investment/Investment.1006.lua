-- don't build
-- Investment condition
target = {
    {name = "다리", award = "다리 2개"},
    {name = "터널", award = "터널 2개"},
    {name = "노선", award = "열차 2량"}
}

local selectedTarget

function InvestmentData()
    local Data = {}
    local idx = math.random(1, #target)
    selectedTarget = target[idx]

    Data.message = selectedTarget.name .. '을 건설하지 마세요.'
    Data.time_require = 7
    Data.award = selectedTarget.award

    return Data
end

local bridge
local tunnel
local lane
local time

local pre_bridge
local pre_tunnel
local pre_lane
local timestamp

-- Call when investment start
-- Used save info when start
function Start()
    bridge = GetBridgeStatistics()
    tunnel = GetTunnelStatistics()
    lane = GetLaneDetailStatistics()
    time = GetTimestamp()

    pre_bridge = bridge.TotalPlacementCount
    pre_tunnel = tunnel.TotalPlacementCount
    pre_lane = lane.TotalModifyAndDeleteCount
    timestamp = time.Date
end

-- Investment appear condition
function Appearance()
    time = GetTimestamp()
    return time.Week > 2
end

-- Investment success condition
function Process()
    local cur_bridge = bridge.TotalPlacementCount
    local cur_tunnel = tunnel.TotalPlacementCount
    local cur_lane = lane.TotalModifyAndDeleteCount

    if selectedTarget.name == "다리" and cur_bridge == pre_bridge then
        return "success"
    elseif selectedTarget.name == "터널" and cur_tunnel == pre_tunnel then
        return "success"
    elseif selectedTarget.name == "노선" and cur_lane == pre_lane then
        return "success"
    end

    local curtime = time.Date
    if curtime - timestamp >= InvestmentData().time_require then
        return "fail"
    end

    return "continue"
end

-- Investment award
function Award()
    if selectedTarget.name == "다리" then
        AddItem("Bridge", 2)
    elseif selectedTarget.name == "터널" then
        AddItem("Tunnel", 2)
    elseif selectedTarget.name == "노선" then
        AddItem("Train", 2)
    end
end