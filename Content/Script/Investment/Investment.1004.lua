-- carriage connection
local additional_needs_1004 = 2
local reward_item_1004 = {Train, 1}
local reward_money_1004 = 300
local time_needed_1004 = 7

-- Investment condition
function InvestmentData()
    local Data = {}
    Data.message = '객차가 '.. additional_needs_1004 ..' 개 이상 달린 열차를 만드세요.'
    Data.time_require = time_needed_1004
    Data.award = '열차 '..  reward_item_1004[2]..'량, '.. reward_money_1004 ..'$'

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
    local start_train = GetTrainInfosAtStart(1004)
    local cur_train = GetTrainInfos()

    for i = 0, #cur_train do
        if i <= #start_train then
            -- existing train
            local additional_subtrains = (cur_train[i].SubtrainCount - (start_train[i].SubtrainCount or 0))
            if additional_subtrains >= additional_needs_1004 then
                return success
            end
        else
            -- new train
            if cur_train[i].SubtrainCount >= additional_needs_1004 then
                return success
            end
        end
    end
    return continue
end

-- Investment award
function Award()
    AddItem(reward_item_1004[1], reward_item_1004[2])
    AddMoney(reward_money_1004)
end

InvestmentDataStruct= {}
InvestmentDataStruct.InvestmentData = InvestmentData
InvestmentDataStruct.Start = Start
InvestmentDataStruct.Appearance = Appearance
InvestmentDataStruct.Process = Process
InvestmentDataStruct.Award = Award

return InvestmentDataStruct