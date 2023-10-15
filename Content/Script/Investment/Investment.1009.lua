-- remaining loan amount
local reward_money_1009 = 2000
local time_needed_1009 = -1

-- Investment condition
function InvestmentData()
    local Data = {}
    Data.message = '잔여 대출금이 없도록 하세요.'
    Data.time_require = time_needed_1009
    Data.award = reward_money_1009 .. "$"

    return Data
end

-- Call when investment start
-- Used save info when start
function Start()
end

-- Investment appear condition
function Appearance()
    local loan = GetBankStatistics()
    return loan.TotalUsingLoanCount ~= 0
end

-- Investment success condition
function Process()
    local loan = GetBankStatistics()

    if (loan.TotalLoanMoney - loan.TotalRepayMoney) <= 0 then
        return success
    else
        return continue
    end
end

-- Investment award
function Award()
    AddMoney(reward_money_1009)
end

InvestmentDataStruct= {}
InvestmentDataStruct.InvestmentData = InvestmentData
InvestmentDataStruct.Start = Start
InvestmentDataStruct.Appearance = Appearance
InvestmentDataStruct.Process = Process
InvestmentDataStruct.Award = Award

return InvestmentDataStruct