//#include "GNFA.h"
//#include <unordered_set>
//
//GNFA::GNFA(const DFA& dfa) noexcept
//{
//	// Copy the states from the DFA
//	m_alphabet = dfa.GetAlphabet();
//	m_possibleStates = dfa.GetPossibleStates();
//	m_transitions = dfa.GetTransitions();
//	m_initialState = dfa.GetInitialState();
//
//	// Create a new state for the start state
//	char newStartState = dfa.NextAvailableState();
//	m_possibleStates.push_back(newStartState);
//	m_transitions.at(m_initialState).push_back(Transition(newStartState, k_epsilon, m_initialState));
//	m_initialState = newStartState;
//
//	// Create a new state for the final state
//	char newFinalState = dfa.NextAvailableState();
//	m_possibleStates.push_back(newFinalState);
//	for (const char& state : dfa.GetFinalStates())
//	{
//		m_transitions.at(state).push_back(Transition(state, k_epsilon, newFinalState));
//	}
//	m_finalState = newFinalState;
//
//	for (const char& state : m_possibleStates)
//	{
//		// Add the epsilon transition from the initial state to the others if states have not a direct transition
//		if (state != m_initialState)
//		{
//			bool hasTransition = false;
//			for (const Transition& transition : m_transitions.at(m_initialState))
//			{
//				if (transition.GetNextState() == state)
//				{
//					hasTransition = true;
//					break;
//				}
//			}
//			if (!hasTransition)
//			{
//				m_transitions.at(m_initialState).push_back(Transition(m_initialState, k_epsilon, state));
//			}
//		}
//
//		// Add the epsilon transition from a state to the final state if the state has not a direct transition
//		/*if (state != m_finalState)
//		{
//			bool hasTransition = false;
//			for (const Transition& transition : m_transitions.at(state))
//			{
//				if (transition.GetNextState() == m_finalState)
//				{
//					hasTransition = true;
//					break;
//				}
//			}
//			if (!hasTransition)
//			{
//				m_transitions.at(state).push_back(Transition(state, k_epsilon, m_finalState));
//			}
//		}*/
//
//		//For any non-initial/final state, add epsilon transitions to all other states, including itself
//		if(state!=m_initialState && state!= m_finalState)
//		{
//			std::unordered_set<char> states;
//			for (const Transition& transition : m_transitions.at(state))
//			{
//				if(states.find(transition.GetNextState()) == states.end())
//				{
//					states.insert(transition.GetNextState());
//				}
//			}
//			if(states.size() < m_possibleStates.size()-1)
//			{
//				for (const char& possibleDisconnectedState : m_possibleStates)
//				{
//					if(states.find(possibleDisconnectedState) == states.end())
//					{
//						m_transitions.at(state).push_back(Transition(state, k_epsilon, possibleDisconnectedState));
//					}
//				}
//			}
//		}
//		
//
//	}
//}
