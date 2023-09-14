-- upgrade
-- Investment condition
target = {
    {name = "열차", award = "객차 1량"},
    {name = "객차", award = "300$"},
    {name = "역", award = "500$"}
}

local selectedTarget

function InvestmentData()
    local Data = {}
    local idx = math.random(1, #target)
    selectedTarget = target[idx]

    Data.message = selectedTarget.name .. '을 업그레이드하세요.'
    Data.time_require = 7
    Data.award = selectedTarget.award

    return Data
end

local trains
local subtrains
local stations

local pre_train_upgrade
local pre_subtrain_upgrade
local pre_statiion_upgrade

-- Call when investment start
-- Used save info when start
function Start()
    trains = GetTrainStatistics()
    subtrains = GetSubtrainStatistics()
    stations = GetDefaultStatistics()

    --ToDo: 열차 업그레이드 확인
    -- pre_train_upgrade
    -- pre_subtrain_upgrade
    pre_statiion_upgrade = stations.UpgradeStationCount
end

-- Investment appear condition
function Appearance()
    time = GetTimestamp()
    return time.Week > 2
end

-- Investment success condition
function Process()
    local cur_train_upgrade
    local cur_subtrain_upgrade
    local cur_statiion_upgrade

    if selectedTarget.name == "열차" and cur_train_upgrade > pre_train_upgrade then
        return "success"
    elseif selectedTarget.name == "객차" and cur_subtrain_upgrade > pre_subtrain_upgrade then
        return "success"
    elseif selectedTarget.name == "역" and cur_station_upgrade > pre_station_upgrade then
        return "success"
    else
        return "continue"
    end
end

-- Investment award
function Award()
    if selectedTarget.name == "열차" then
        AddItem("Subtrain", 1)
    elseif selectedTarget.name == "객차" then
        AddMoney(300)
    elseif selectedTarget.name == "역" then
        AddMoney(500)
    end
end