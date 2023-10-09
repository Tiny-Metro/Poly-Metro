-- upgrade
target_1005 = {
    {name = "열차", award = {Subtrain, 1}, message = "객차 1량"},
    {name = "객차", award = 300, message = "300$"},
    {name = "역", award = 500, message = "500$"}
}

local selected_target_1005

-- Investment condition
function InvestmentData()
    local Data = {}
    local idx = math.random(1, #target_1005)
    selected_target_1005 = target_1005[idx]

    Data.message = selected_target_1005.name .. '을 업그레이드하세요.'
    Data.time_require = 7
    Data.award = selected_target_1005.message

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
    local pre_train_upgrade = GetTrainStatisticsAtStart(1005)
    local pre_subtrain_upgrade = GetSubtrainStatisticsAtStart(1005)
    local pre_statiion_upgrade = GetDefaultStatisticsAtStart(1005)

    local cur_train_upgrade = GetTrainStatistics()
    local cur_subtrain_upgrade = GetSubtrainStatistics()
    local cur_statiion_upgrade = GetDefaultStatistics()

    if selected_target_1005.name == "열차" and (cur_train_upgrade.TotalUpgradeCount > pre_train_upgrade.TotalUpgradeCount) then
        return success
    elseif selected_target_1005.name == "객차" and (cur_subtrain_upgrade.TotalUpgradeCount > pre_subtrain_upgrade.TotalUpgradeCount) then
        return success
    elseif selected_target_1005.name == "역" and (cur_station_upgrade.UpgradeStationCount > pre_station_upgrade.UpgradeStationCount) then
        return success
    end

    return continue
end

-- Investment award
function Award()
    if selected_target_1005.name == "열차" then
        AddItem(selected_target_1005.award[1], selected_target_1005.award[2])
    elseif selected_target_1005.name == "객차" then
        AddMoney(selected_target_1005.award)
    elseif selected_target_1005.name == "역" then
        AddMoney(selected_target_1005.award)
    end
end

InvestmentDataStruct= {}
InvestmentDataStruct.InvestmentData = InvestmentData
InvestmentDataStruct.Start = Start
InvestmentDataStruct.Appearance = Appearance
InvestmentDataStruct.Process = Process
InvestmentDataStruct.Award = Award

return InvestmentDataStruct