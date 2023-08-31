-- Investment condition
function InvestmentData()
    Data = {}
    Data.message = 'Mission require text'
    Data.time_require = 0
    Data.award = 'Reward text'

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
    return continue
end

-- Investment award
function Award()
end