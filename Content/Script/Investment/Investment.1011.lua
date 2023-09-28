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
local timestamp

-- Call when investment start
-- Used save info when start
function Start()
    shop = GetShopStatistics()
    time = GetTimestamp()
    timestamp = time.Date
    pre_shop = shop.TotalPurchaseCount
end

-- Investment appear condition
function Appearance()
    return true
end

-- Investment success condition
function Process()
    local curtime = time.Date

    if curtime - timestamp < InvestmentData().time_require then
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
end