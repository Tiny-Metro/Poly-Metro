-- remaining loan amount
-- Investment condition
function InvestmentData()
    local Data = {}
    Data.message = '잔여 대출금이 없도록 하세요.'
    Data.time_require = 0
    Data.award = '2000$'

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
    -- 잔여 대출금 확인
    return true
end

-- Investment success condition
function Process()
--ToDo: 모든 대출이 상납되었다면 success 그렇지 않다면 continue
end

-- Investment award
function Award()
    AddMoney(2000)
end