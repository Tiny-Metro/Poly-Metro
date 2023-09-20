-- Complaint management
-- Investment condition
function InvestmentData()
    local Data = {}
    Data.message = '어떤 역도 불만도 50을 넘지 않도록 하세요.'
    Data.time_require = 7
    Data.award = '800$'

    return Data
end

local stations

-- Call when investment start
-- Used save info when start
function Start()
    stations = GetStationInfos()
end

-- Investment appear condition
function Appearance()
    for i=0, #stations do
        if stations[i].Complain > 50 then
            return false
        end
    end
    return true
end

-- Investment success condition
function Process()
    for i = 0, #stations do
        if stations[i].Complain > 50 then
            return "fail"
        else
            return "continue"
        end
    end
    return "success"
end

-- Investment award
function Award()
    AddMoney(500)
end