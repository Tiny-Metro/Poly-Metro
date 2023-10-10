-- No use in stores
local reward_money_1011 = 500

-- Investment condition
function InvestmentData()
    local Data = {}
    Data.message = '이번 주에는 아이템을 새로 구매하지 마세요.'
    Data.time_require = 7
    Data.award = reward_money_1011 .. "$"

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
    local start_shop = GetShopStatisticsAtStart(1011)
    local start_time = GetTimestampAtStart(1011)

    local cur_shop = GetShopStatistics()
    local cur_time = GetTimestamp()

    if cur_time.Date - start_time.Date < InvestmentData().time_require then
        -- within the deadline
        if cur_shop.TotalPurchaseCount == start_shop.TotalPurchaseCount then
            return continue
        else
            return fail
        end
    else
        -- outside the deadline
        if cur_shop.TotalPurchaseCount == start_shop.TotalPurchaseCount then
            return success
        else
            return fail
        end
    end
end

-- Investment award
function Award()
    AddMoney(reward_money_1011)
end

InvestmentDataStruct= {}
InvestmentDataStruct.InvestmentData = InvestmentData
InvestmentDataStruct.Start = Start
InvestmentDataStruct.Appearance = Appearance
InvestmentDataStruct.Process = Process
InvestmentDataStruct.Award = Award

return InvestmentDataStruct