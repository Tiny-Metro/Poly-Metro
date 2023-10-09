-- remaining loan amount
local reward_amount_1009 = 2000

-- Investment condition
function InvestmentData()
    local Data = {}
    Data.message = '잔여 대출금이 없도록 하세요.'
    Data.time_require = -1
    Data.award = reward_amount_1009 .. "$"

    return Data
end

-- Call when investment start
-- Used save info when start
function Start()
end

-- Investment appear condition
function Appearance()
    local loan = GetBankStatistics()
    total_loan_money = loan.TotalUsingLoanCount
    return total_loan_money ~= 0
end

-- Investment success condition
function Process()
    local loan = GetBankStatistics()

    if (loan.TotalRepayMoney - loan.TotalRepayInterestMoney) <= 0 then
        return success
    else
        return continue
    end
end

-- Investment award
function Award()
    AddMoney(InvestmentData.award)
end

InvestmentDataStruct= {}
InvestmentDataStruct.InvestmentData = InvestmentData
InvestmentDataStruct.Start = Start
InvestmentDataStruct.Appearance = Appearance
InvestmentDataStruct.Process = Process
InvestmentDataStruct.Award = Award

return InvestmentDataStruct