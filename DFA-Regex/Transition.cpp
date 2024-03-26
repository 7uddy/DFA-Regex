#include "Transition.h"
#include <string>

Transition::Transition(const char& currentState, const std::string& input, const char& nextState) noexcept
	: m_currentState(currentState), m_input(input), m_nextState(nextState)
{
	//empty
}


char Transition::GetNextState() const noexcept
{
	return m_nextState;
}

char Transition::GetCurrentState() const noexcept
{
	return m_currentState;
}

char Transition::GetDFAInput() const noexcept
{
	return m_input[0];
}

void Transition::SetInput(const std::string& input) noexcept
{
	m_input = input;
}

std::string Transition::GetInput() const noexcept
{
	return m_input;
}

bool operator==(const Transition& one, const Transition& other) noexcept
{
	return one.m_currentState == other.m_currentState &&
		one.m_input == other.m_input &&
		one.m_nextState == other.m_nextState;
}
