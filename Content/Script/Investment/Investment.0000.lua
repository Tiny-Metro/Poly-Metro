Data = {}
Data.title = 'Title'
Data.content = 'ContentDescription'
Data.time_require = -1
Data.require_message = 'RequireMessage'
Data.accept_message = 'AcceptEffectMessage'
Data.award_message = 'SuccessEffectMessage'
Data.reward_message = 'FailEffectMessage'

function InvestmentAppear()
    return true
end

--Action when investment start
function InvestmentStart()
end

--Action when investment success
function InvestmentSuccess()
end

--Action when investment fail
function InvestmentFail()
end

--Investment condition check (success or fail)
--Don't need to check for time require
function InvestmentCheck()
end

return Data