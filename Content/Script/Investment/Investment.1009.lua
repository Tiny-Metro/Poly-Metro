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
    total_loan_money = loan.TotalUsingLoanCount
    return total_loan_money ~= 0
end

-- Investment success condition
function Process()
    local repay_money = loan.TotalRepayMoney
    local repay_interest_money = loan.TotalRepayInterestMoney

    if repay_money - repay_interest_money <= 0 then
        return "success"
    else
        return "continue"
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