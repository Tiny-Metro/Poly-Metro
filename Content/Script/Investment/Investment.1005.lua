-- upgrade
-- Investment condition
target = {
    {name = "열차", award = "객차 1량"},
    {name = "객차", award = "300$"},
    {name = "역", award = "500$"}
}

local selected_target

function InvestmentData()
    local Data = {}
    local idx = math.random(1, #target)
    selected_target = target[idx]

    Data.message = selected_target.name .. '을 업그레이드하세요.'
    Data.time_require = 7
    Data.award = selected_target.award

    return Data
end

local trains
local subtrains
local stations
local time

local pre_train_upgrade
local pre_subtrain_upgrade
local pre_statiion_upgrade
local time_stamp

-- Call when investment start
-- Used save info when start
function Start()
    trains = GetTrainStatistics()
    subtrains = GetSubtrainStatistics()
    stations = GetDefaultStatistics()
    time = GetTimestamp()

    pre_train_upgrade = trains.TotalUpgradeCount
    pre_subtrain_upgrade = subtrains.TotalUpgradeCount
    pre_statiion_upgrade = stations.UpgradeStationCount
    time_stamp = time.Date
end

-- Investment appear condition
function Appearance()
    time = GetTimestamp()
    return time.Week > 2
end

-- Investment success condition
function Process()
    local cur_train_upgrade = trains.TotalUpgradeCount
    local cur_subtrain_upgrade = subtrains.TotalUpgradeCount
    local cur_statiion_upgrade = stations.UpgradeStationCount

    if selected_target.name == "열차" and cur_train_upgrade > pre_train_upgrade then
        return "success"
    elseif selected_target.name == "객차" and cur_subtrain_upgrade > pre_subtrain_upgrade then
        return "success"
    elseif selected_target.name == "역" and cur_station_upgrade > pre_station_upgrade then
        return "success"
    end

    local cur_time = time.Date
    if cur_time - time_stamp > InvestmentData().time_require then
        return "fail"
    end

    return "continue"
end

-- Investment award
function Award()
    if selected_target.name == "열차" then
        AddItem(Subtrain, 1)
    elseif selected_target.name == "객차" then
        AddMoney(300)
    elseif selected_target.name == "역" then
        AddMoney(500)
    end

    InvestmentDataStruct= {}
    InvestmentDataStruct.InvestmentData = InvestmentData
    InvestmentDataStruct.Start = Start
    InvestmentDataStruct.Appearance = Appearance
    InvestmentDataStruct.Process = Process
    InvestmentDataStruct.Award = Award

    return InvestmentDataStruct
end