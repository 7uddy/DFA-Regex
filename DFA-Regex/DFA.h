#pragma once
#include "Transition.h"
#include "GNFA.h"
#include <iostream>  
#include <vector>
#include <string>
#include <map>
#include <fstream>

class DFA
{
public:

	//Constructors
	DFA()=default;
	DFA(const std::vector<char>& possibleStates, const std::vector<char>& alphabet, const char& initialState, 
		const std::vector<char>& finalStates, const std::map<char, std::vector<Transition>>& transitions) noexcept;

	//Methods
	bool IsFinalState(const char& state) const noexcept;
	bool IsAlphabet(const char& symbol) const noexcept;
	char GetInitialState() const noexcept;
	char GetNextState(const char& state, const char& symbol) const noexcept;
	char NextAvailableState() const noexcept;
	std::vector<char> GetFinalStates() const noexcept;
	std::vector<char> GetPossibleStates() const noexcept;
	std::vector<char> GetAlphabet() const noexcept;
	std::map<char, std::vector<Transition>> GetTransitions() const noexcept;
	void ReadDFA(const std::string& fileName);
	GNFA ConvertDFAtoGNFA() const noexcept;

	//Operator Overloads
	friend std::ostream& operator << (std::ostream&, const DFA&);

private:
	//Member Variables
	std::vector<char> m_possibleStates;
	std::vector<char> m_alphabet;
	char m_initialState;
	std::vector<char> m_finalStates;
	std::map<char, std::vector<Transition>> m_transitions;
};

