-- carriage connection
-- Investment condition
function InvestmentData()
    local Data = {}
    Data.message = '객차가 두 개 이상 달린 열차를 만드세요.'
    Data.time_require = 7
    Data.award = '열차 1량, 300$'

    return Data
end

local trains
local pre_subtrain_count
local time
local time_stamp

-- Call when investment start
-- Used save info when start
function Start()
    trains = GetTrainInfos()
    time = GetTimestamp()
    time_stamp = time.Date

    pre_subtrain_count = {}
    for i=0, #trains do
        if trains[i].SubtrainCount ~= -1 then
            pre_subtrain_count[i] = trains[i].SubtrainCount
        end
    end
end

-- Investment appear condition
function Appearance()
    time = GetTimestamp()
    return time.Week > 2
end

-- Investment success condition
function Process()
    local additional_subtrains_needed = 2
    local cur_time = time.Date

    for i = 0, #trains do
        if trains[i].SubtrainCount ~= -1 then
            local additionalSubtrains = trains[i].SubtrainCount - (pre_subtrain_count[i] or 0)
            if additionalSubtrains >= additional_subtrains_needed then
                return "success"
            end
        end
    end

    if cur_time - time_stamp > InvestmentData().time_require then
        return "fail"
    end

    return "continue"
end

-- Investment award
function Award()
    AddItem(Train, 1)
    AddMoney(300)

    InvestmentDataStruct= {}
    InvestmentDataStruct.InvestmentData = InvestmentData
    InvestmentDataStruct.Start = Start
    InvestmentDataStruct.Appearance = Appearance
    InvestmentDataStruct.Process = Process
    InvestmentDataStruct.Award = Award

    return InvestmentDataStruct
end