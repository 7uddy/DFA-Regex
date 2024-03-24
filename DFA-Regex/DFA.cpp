#include "DFA.h"
#include <fstream>
#include <iostream>
#include "GNFA.h"

DFA::DFA(const std::vector<char>& possibleStates, const std::vector<char>& alphabet, const char& initialState,
	const std::vector<char>& finalStates, const std::map<char, std::vector<Transition>>& transitions) noexcept
	: m_possibleStates(possibleStates), m_alphabet(alphabet), m_initialState(initialState), m_finalStates(finalStates), m_transitions(transitions)
{
	// empty
}


bool DFA::IsFinalState(const char& state) const noexcept
{
	for (const char& finalState : m_finalStates)
	{
		if (state == finalState)
		{
			return true;
		}
	}
	return false;
}

bool DFA::IsAlphabet(const char& symbol) const noexcept
{
	for (const char& letter : m_alphabet)
	{
		if (symbol == letter)
		{
			return true;
		}
	}
	return false;
}

char DFA::GetInitialState() const noexcept
{
	return m_initialState;
}

char DFA::GetNextState(const char& state, const char& symbol) const noexcept
{
	for(const Transition& transition: m_transitions.at(state))
	{
		if (transition.GetDFAInput()== symbol)
		{
			return transition.GetNextState();
		}
	}
    return state;
}

char DFA::NextAvailableState() const noexcept
{
    return m_possibleStates.back()+1;
}

std::vector<char> DFA::GetFinalStates() const noexcept
{
	return m_finalStates;
}

std::vector<char> DFA::GetPossibleStates() const noexcept
{
	return m_possibleStates;
}

std::vector<char> DFA::GetAlphabet() const noexcept
{
	return m_alphabet;
}

std::map<char, std::vector<Transition>> DFA::GetTransitions() const noexcept
{
	return m_transitions;
}

void DFA::ReadDFA(const std::string& fileName)
{
    std::ifstream file { fileName };
    if (!file.is_open())
    {
        std::cerr << "Error opening file" << std::endl;
        return;
    }

    std::string line;
    std::getline(file, line);
    std::vector<char> possibleStates;
    for (const char& state : line)
    {
        if(state != ' ') possibleStates.push_back(state);
    }

    std::getline(file, line);
    std::vector<char> alphabet;
    for (const char& letter : line)
    {
        if(letter != ' ') alphabet.push_back(letter);
    }

    std::getline(file, line);
    char initialState = line[0];

    std::getline(file, line);
    std::vector<char> finalStates;
    for (const char& state : line)
    {
        if (state != ' ') finalStates.push_back(state);
    }

    std::map<char, std::vector<Transition>> transitions;
    while (std::getline(file, line))
    {
        char state = line[0];
        std::string input(1, line[2]);
        char nextState = line[4];
        transitions[state].push_back(Transition(state,input,nextState));
    }

    m_possibleStates = possibleStates;
    m_alphabet = alphabet;
    m_initialState = initialState;
    m_finalStates = finalStates;
    m_transitions = transitions;
    file.close();
}
std::ostream& operator<<(std::ostream& out, const DFA& automaton)
{
	out << "POSSIBLE STATES = { ";
	for (const auto& element : automaton.m_possibleStates)
		out << element << " ";

	out << "}\nALPHABET = { ";
	for (const auto& element : automaton.m_alphabet)
		out << element << " ";

	out << "}\nInitial state: " << automaton.m_initialState;

	out << "\nFunctions:\n";
	for (const auto& functie : automaton.m_transitions)
	{
		for (const auto& dreapta : functie.second)
		{
			out << "(" << functie.first << ", ";
			out << dreapta.GetInput() << ") = { ";
			out<<dreapta.GetNextState()<< " }\n";
		}
	}
	out << "Final states = { ";
	for (const auto& element : automaton.m_finalStates)
		out << element << " ";
	out << "}\n";
	return out;
}

