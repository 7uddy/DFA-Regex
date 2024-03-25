#include "GNFA.h"

GNFA::GNFA(const std::vector<char>& possibleStates, const std::vector<char>& alphabet, const char& initialState, 
	const char& finalState, const std::map<char, std::vector<Transition>>& transitions) noexcept
	: m_possibleStates(possibleStates), m_alphabet(alphabet), m_initialState(initialState), m_finalState(finalState), 
	m_transitions(transitions)
{
	//empty
}

std::string GNFA::GNFAToRegex() noexcept
{

	std::vector<char> statesWithoutInitialAndFinalStates=m_possibleStates;
	statesWithoutInitialAndFinalStates.erase(statesWithoutInitialAndFinalStates.end() - 2,statesWithoutInitialAndFinalStates.end());
	TransitionsToRegex();
	while (!statesWithoutInitialAndFinalStates.empty())
	{
		DeleteState(statesWithoutInitialAndFinalStates.back());
		statesWithoutInitialAndFinalStates.pop_back();
	}
	return RemoveEpsilonFromTransition();
}

std::string GNFA::SimplifyTransitionToRegex( const char& inputState, const char& state, const char& outputState) noexcept
{
	for (const auto& inputToStateTransition : m_transitions.at(inputState))
	{
		if(inputToStateTransition.GetCurrentState()==inputState&& inputToStateTransition.GetNextState()==state)
		{
			for (const auto& stateToOutputTransition : m_transitions.at(state))
			{
				if(stateToOutputTransition.GetCurrentState()==state && stateToOutputTransition.GetNextState()==outputState)
				{
					for(const auto& stateToStateTransition : m_transitions.at(state))
					{
						if (stateToStateTransition.GetCurrentState() == state && stateToStateTransition.GetNextState() == state)
						{
							for (const auto& inputToOutputTransition : m_transitions.at(inputState))
							{
								if (inputToOutputTransition.GetCurrentState() == inputState && inputToOutputTransition.GetNextState() == outputState)
								{
									if (stateToOutputTransition.GetInput() != "e" && inputToOutputTransition.GetInput() != "e")
										return inputToStateTransition.GetInput() + "(" + stateToStateTransition.GetInput() + ")*" +
										stateToOutputTransition.GetInput() + "U" + inputToOutputTransition.GetInput();
									else if (stateToOutputTransition.GetInput() != "e")
										return inputToStateTransition.GetInput() + "(" + stateToStateTransition.GetInput() + ")*" +
										stateToOutputTransition.GetInput();
									else return inputToStateTransition.GetInput() + "(" + stateToStateTransition.GetInput() + ")*";
								}
							}
						}
					}
				}
			}
		}
	}
}

void GNFA::DeleteState(const char& state) noexcept
{
	std::vector<char> inputStates;
	std::vector<char> outputStates;
	for(const auto& transitions: m_transitions)
	for (const auto& transition : transitions.second)
	{
		if (transition.GetNextState() == transition.GetCurrentState()) continue;
		if (transition.GetNextState() == state && std::find(inputStates.begin(),inputStates.end(), transition.GetCurrentState())==inputStates.end())
		{
			inputStates.push_back(transition.GetCurrentState());
		}
		else if (transition.GetCurrentState() == state && std::find(outputStates.begin(), outputStates.end(), transition.GetNextState()) == outputStates.end())
		{
			outputStates.push_back(transition.GetNextState());
		}
	}
	for (auto& inputState : inputStates)
	{
		for (auto& outputState : outputStates)
		{
			if (inputState != outputState) {
				Transition newTransition = Transition(inputState, SimplifyTransitionToRegex(inputState, state, outputState), outputState);
				m_transitions.at(inputState).push_back(newTransition);
				for (const auto& transition : m_transitions.at(inputState))
				{
					if (transition.GetNextState() == outputState && transition.GetInput()[0] == k_epsilon)
					{
						m_transitions.at(inputState).erase(std::find(m_transitions.at(inputState).begin(), m_transitions.at(inputState).end(), transition));
						break;
					}
				}
				DeleteStateFromTransitions(state);
			}
		}
	}
}

void GNFA::DeleteStateFromTransitions(const char& state) noexcept
{
	m_transitions.erase(state);
	std::vector<Transition> newTransitions;
	for(auto& transitions: m_transitions)
	{
		newTransitions.clear();
		for(auto& transition: transitions.second)
		{
			if(transition.GetNextState()!=state)
			{
				newTransitions.push_back(transition);
			}
		}
		transitions.second=newTransitions;
	}
}

void GNFA::TransitionsToRegex() noexcept
{
	Transition firstTransition;
	bool firstFound=false;
	Transition secondTransition;
	for(const auto& transitions: m_transitions)
	{
		firstFound = false;
		for (const auto& transition : transitions.second)
		{
			if (transition.GetCurrentState() == transition.GetNextState() && firstFound == false)
			{
				firstTransition = transition;
				firstFound = true;
			}
			else if (transition.GetCurrentState() == transition.GetNextState() && firstFound == true && firstTransition!=transition)
			{
				secondTransition = transition;
				Transition newTransition{ transitions.first, firstTransition.GetInput() + k_union + secondTransition.GetInput(), transitions.first};
				m_transitions.at(transitions.first).erase(std::find(transitions.second.begin(), transitions.second.end(), firstTransition));
				m_transitions.at(transitions.first).erase(std::find(transitions.second.begin(), transitions.second.end(), secondTransition));
				m_transitions[transitions.first].push_back(newTransition);
				firstTransition = newTransition;
			}
		}
	}
}

std::string GNFA::RemoveEpsilonFromTransition() noexcept
{
	std::string regex = m_transitions[m_initialState].back().GetInput();
	/*for (char character : regex)
	{
		if (character == k_epsilon)
		{
			regex.erase(std::find(regex.begin(), regex.end(), character));
		}
	}*/
	return regex;

}