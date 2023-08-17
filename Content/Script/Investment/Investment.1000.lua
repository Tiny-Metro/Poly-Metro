-- Investment condition
function InvestmentData()
    Data = {}
    Data.message = 'Mission require text'
    Data.time_require = 7
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
    missionState = GetTimestamp()
    print(missionState.Date)
    if missionState.Date > 5 then
        return success
    else
        return continue
    end
end

-- Investment award
function Award()
    print('success!')
    AddIncome(5000)
end