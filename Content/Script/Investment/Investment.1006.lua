-- don't build
-- Investment condition
target = {
    {name = "다리", award = "다리 2개"},
    {name = "터널", award = "터널 2개"},
    {name = "노선", award = "열차 2량"}
}

local selected_target

function InvestmentData()
    local Data = {}
    local idx = math.random(1, #target)
    selected_target = target[idx]

    Data.message = selected_target.name .. '을 건설하지 마세요.'
    Data.time_require = 7
    Data.award = selected_target.award

    return Data
end

local bridge
local tunnel
local lane
local time

local pre_bridge
local pre_tunnel
local pre_lane
local time_stamp

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
    time_stamp = time.Date
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

    if selected_target.name == "다리" and cur_bridge == pre_bridge then
        return "success"
    elseif selected_target.name == "터널" and cur_tunnel == pre_tunnel then
        return "success"
    elseif selected_target.name == "노선" and cur_lane == pre_lane then
        return "success"
    end

    local cur_time = time.Date
    if cur_time - time_stamp >= InvestmentData().time_require then
        return "fail"
    end

    return "continue"
end

-- Investment award
function Award()
    if selected_target.name == "다리" then
        AddItem(Bridge, 2)
    elseif selected_target.name == "터널" then
        AddItem(Tunnel, 2)
    elseif selected_target.name == "노선" then
        AddItem(Train, 2)
    end

    InvestmentDataStruct= {}
    InvestmentDataStruct.InvestmentData = InvestmentData
    InvestmentDataStruct.Start = Start
    InvestmentDataStruct.Appearance = Appearance
    InvestmentDataStruct.Process = Process
    InvestmentDataStruct.Award = Award

    return InvestmentDataStruct
end