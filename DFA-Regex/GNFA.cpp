#include "GNFA.h"

GNFA::GNFA(const std::vector<char>& possibleStates, const std::vector<char>& alphabet, const char& initialState, 
	const char& finalState, const std::map<char, std::vector<Transition>>& transitions) noexcept
	: m_possibleStates(possibleStates), m_alphabet(alphabet), m_initialState(initialState), m_finalState(finalState), 
	m_transitions(transitions)
{
	//empty
}
