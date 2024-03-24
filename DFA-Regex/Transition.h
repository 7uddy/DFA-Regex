#pragma once
#include <string>
class Transition
{

public:
	Transition(const char& currentState, const std::string& input, const char& nextState) noexcept;

	char GetNextState() const noexcept;
	char GetCurrentState() const noexcept;
	char GetDFAInput() const noexcept;
	std::string GetInput() const noexcept;
private:
	char m_nextState;
	char m_currentState;
	std::string m_input;

};

