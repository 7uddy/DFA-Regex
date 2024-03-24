#pragma once
#include "Transition.h"
#include <vector>
#include <map>

class GNFA
{
public:
	//Constructors
	GNFA(const std::vector<char>& possibleStates, const std::vector<char>& alphabet, const char& initialState,
				const char& finalState, const std::map<char, std::vector<Transition>>& transitions) noexcept;

public:

	//Static Constants
	static const char k_epsilon = 'e';

private:

	//Member Variables
	std::vector<char> m_possibleStates;
	std::vector<char> m_alphabet;
	char m_initialState;
	char m_finalState;
	std::map<char, std::vector<Transition>> m_transitions;

};

