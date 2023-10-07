-- No use in stores
-- Investment condition
function InvestmentData()
    local Data = {}
    Data.message = '이번 주에는 아이템을 새로 구매하지 마세요.'
    Data.time_require = 7
    Data.award = 500

    return Data
end

local shop
local pre_shop
local time
local time_stamp

-- Call when investment start
-- Used save info when start
function Start()
    shop = GetShopStatistics()
    time = GetTimestamp()
    time_stamp = time.Date
    pre_shop = shop.TotalPurchaseCount
end

-- Investment appear condition
function Appearance()
    return true
end

-- Investment success condition
function Process()
    local cur_time = time.Date

    if cur_time - time_stamp < InvestmentData().time_require then
        return "continue"
    elseif shop.TotalPurchaseCount == pre_shop then
        return "success"
    else
        return "fail"
    end
end

-- Investment award
function Award()
    AddMoney(InvestmentData.award)

    InvestmentDataStruct= {}
    InvestmentDataStruct.InvestmentData = InvestmentData
    InvestmentDataStruct.Start = Start
    InvestmentDataStruct.Appearance = Appearance
    InvestmentDataStruct.Process = Process
    InvestmentDataStruct.Award = Award

    return InvestmentDataStruct
end