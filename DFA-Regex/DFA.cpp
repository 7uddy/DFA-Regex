#include "DFA.h"

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

GNFA DFA::ConvertDFAtoGNFA() const noexcept
{
	//Connect every state with others and with itself with epsilon transitions
	std::map<char, std::vector<Transition>> newTransitions = m_transitions;
	std::string epsilonInput(1, GNFA::k_epsilon);
	for(const char& fromState:m_possibleStates)
	{
		for(const char& toState: m_possibleStates)
		{
			//If the state has no transitions
			if (newTransitions.find(fromState) == newTransitions.end())
			{
				newTransitions[fromState] = std::vector<Transition>(1, Transition(fromState, epsilonInput, toState));
			}
			//If the state has transitions
			else {
				for(const Transition& transition: newTransitions.at(fromState))
				{
					//If the transition already exists
					if (transition.GetNextState() == toState)
					{
						break;
					}
					else 
					{
						newTransitions.at(fromState).push_back(Transition(fromState, epsilonInput, toState));
					}
				}
			}
		}
	 }

	//Create a new state for the initial state and connect it with epsilon transitions to the other states
	char newInitialState = NextAvailableState();
	std::vector<char> newPossibleStates = m_possibleStates;
	newPossibleStates.push_back(newInitialState);
	for (const char& state : m_possibleStates)
	{
		//For the first state we create the transition vector
		if (newTransitions.find(newInitialState) == newTransitions.end())
		{
			newTransitions[newInitialState] = std::vector<Transition>(1, Transition(newInitialState, epsilonInput, state));
		}
		else newTransitions.at(newInitialState).push_back(Transition(newInitialState, epsilonInput, state));
	}

	// Create a new state for the final state and connect all states to it with epsilon transitions
	char newFinalState = newInitialState+1;
	newPossibleStates.push_back(newFinalState);
	for (const char& state : m_possibleStates)
	{
		if(newTransitions.find(state) == newTransitions.end())
		{
			newTransitions[state] = std::vector<Transition>(1, Transition(state, epsilonInput, newFinalState));
		}
		else newTransitions.at(state).push_back(Transition(state, epsilonInput, newFinalState));
	}
	
	//Create connection between initial state and final state
	newTransitions.at(newInitialState).push_back(Transition(newInitialState, epsilonInput, newFinalState));

	return GNFA(newPossibleStates, m_alphabet, newInitialState, newFinalState, newTransitions);
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


