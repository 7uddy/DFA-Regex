#pragma once
#include <string>
class Transition
{

public:
	Transition()=default;
	Transition(const char& currentState, const std::string& input, const char& nextState) noexcept;

	char GetNextState() const noexcept;
	char GetCurrentState() const noexcept;
	char GetDFAInput() const noexcept;
	void SetInput(const std::string& input) noexcept;
	std::string GetInput() const noexcept;

	friend bool operator==(const Transition& one,const Transition& other) noexcept;
private:
	char m_nextState;
	char m_currentState;
	std::string m_input;

};

