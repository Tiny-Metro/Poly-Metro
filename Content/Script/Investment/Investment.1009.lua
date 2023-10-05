-- remaining loan amount
-- Investment condition
function InvestmentData()
    local Data = {}
    Data.message = '잔여 대출금이 없도록 하세요.'
    Data.time_require = -1
    Data.award = 2000

    return Data
end

local loan

-- Call when investment start
-- Used save info when start
function Start()
    loan = GetBankStatistics()
end

-- Investment appear condition
function Appearance()
    totalloanmoney = loan.TotalUsingLoanCount
    return totalloanmoney ~= 0
end

-- Investment success condition
function Process()
    local repaymoney = loan.TotalRepayMoney
    local repayinterestmoney = loan.TotalRepayInterestMoney

    if repaymoney - repayinterestmoney <= 0 then
        return "success"
    else
        return "continue"
    end
end

-- Investment award
function Award()
    AddMoney(InvestmentData.award)
end