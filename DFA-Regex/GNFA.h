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

	//Methods
	std::string GNFAToRegex() noexcept;
private:
	std::string SimplifyTransitionToRegex(const char& inputState, const char& state, const char& outputState) noexcept;
	void DeleteState(const char& state) noexcept;
	void DeleteStateFromTransitions(const char& state) noexcept;
	void TransitionsToRegex() noexcept;
	std::string RemoveEpsilonFromTransition() noexcept;
	bool IsFromAlphabetOrParanthesis(const char& character) const noexcept;

public:

	//Static Constants
	static const char k_epsilon = 'e';
	static const char k_union = '|';
	static const char k_star= '*';

private:

	//Member Variables
	std::vector<char> m_possibleStates;
	std::vector<char> m_alphabet;
	char m_initialState;
	char m_finalState;
	std::map<char, std::vector<Transition>> m_transitions;

};

