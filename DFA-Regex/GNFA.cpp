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
									return inputToStateTransition.GetInput() + "(" + stateToStateTransition.GetInput() + ")*" +
										stateToOutputTransition.GetInput() + "|" + inputToOutputTransition.GetInput();
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
			Transition newTransition = Transition(inputState, SimplifyTransitionToRegex(inputState, state, outputState), outputState);
			if (newTransition.GetInput() == "") break;
//			m_transitions.at(inputState).push_back(newTransition);
			for (const auto& transition : m_transitions.at(inputState))
			{
				if (transition.GetNextState() == outputState)
				{
					m_transitions.at(inputState).erase(std::find(m_transitions.at(inputState).begin(), m_transitions.at(inputState).end(), transition));
					break;
				}
			}
			m_transitions.at(inputState).push_back(newTransition);
		}
	}
	DeleteStateFromTransitions(state);
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

//void GNFA::TransitionsToRegex() noexcept
//{
//	std::map<char, std::vector<Transition>> newTransitions;
//	for (const auto& transitions : m_transitions)
//	{
//		for (const auto& transition : transitions.second)
//		{
//			if (newTransitions.find(transition.GetCurrentState()) == newTransitions.end())
//			{
//				newTransitions[transition.GetCurrentState()] = std::vector<Transition>();
//			newTransitions[transition.GetCurrentState()].push_back(transition);
//			}
//			else {
//				//cautam daca exista tranzitia
//				Transition* auxTransition = nullptr;
//				for (auto& trans : newTransitions[transition.GetCurrentState()])
//				{
//					if (trans.GetCurrentState() == transition.GetCurrentState() && trans.GetNextState() == transition.GetNextState())
//					{
//						auxTransition = &trans;
//						break;
//					}
//				}
//				if (auxTransition)
//				{
//					for (auto& tran : newTransitions[transition.GetNextState()])
//					{
//						if (tran.GetCurrentState() == auxTransition->GetNextState() && tran.GetNextState() == auxTransition->GetNextState())
//						{
//							newTransitions[transition.GetNextState()].erase(std::find(newTransitions[transition.GetNextState()].begin(), newTransitions[transition.GetNextState()].end(), tran));
//							break;
//						}
//					}
//
//					std::string newInput{ auxTransition->GetInput() + '|' + transition.GetInput() };
//					auxTransition->SetInput(newInput);
//
//					newTransitions[transition.GetCurrentState()].push_back(*auxTransition);
//
//				}
//				else
//				{
//					newTransitions[transition.GetCurrentState()].push_back(transition);
//				}
//
//			}
//
//		}
//
//	}
//	m_transitions = newTransitions;
//}

void GNFA::TransitionsToRegex() noexcept
{
	std::map<std::pair<char, char>, int> toDeleteTransitions;

	// Count occurrences of transitions
	for (auto transitions : m_transitions)
	{
		for (auto transition : transitions.second)
		{
			std::pair auxPair = std::make_pair(transition.GetCurrentState(), transition.GetNextState());
			if (toDeleteTransitions.contains(auxPair))
			{
				toDeleteTransitions[auxPair]++;
			}
			else
			{
				toDeleteTransitions[auxPair] = 1;
			}
		}
	}

	// Iterate through transitions to be deleted and replace them
	for (auto transitionToDelete : toDeleteTransitions)
	{
		if (transitionToDelete.second > 1)
		{
			std::vector<Transition> newTransitions;

			// Gather transitions to be replaced
			for (auto transition : m_transitions[transitionToDelete.first.first])
			{
				if (transition.GetNextState() == transitionToDelete.first.second)
				{
					newTransitions.push_back(transition);
				}
			}

			std::string newInput;
			for (auto transition : newTransitions)
			{
				if (newInput.empty())
				{
					newInput = transition.GetInput();
				}
				else
				{
					newInput += "|" + transition.GetInput();
				}
			}

			// Create a new transition with combined inputs
			Transition newTransition = Transition(transitionToDelete.first.first, newInput, transitionToDelete.first.second);

			// Erase old transitions and insert the new one
			m_transitions.at(transitionToDelete.first.first).erase(std::remove_if(
				m_transitions.at(transitionToDelete.first.first).begin(),
				m_transitions.at(transitionToDelete.first.first).end(),
				[&](const Transition& transition) {
					return transition.GetNextState() == transitionToDelete.first.second;
				}),
				m_transitions.at(transitionToDelete.first.first).end());

			m_transitions.at(transitionToDelete.first.first).push_back(newTransition);
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