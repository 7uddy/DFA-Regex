#include <iostream>
#include "DFA.h"
#include "GNFA.h"

int main()
{
	DFA dfa;
	dfa.ReadDFA("DFAInput.txt");
	std::cout << dfa;
	GNFA gnfa = dfa.ConvertDFAtoGNFA();
	std::cout<<"\nThe regex expression is "<<gnfa.GNFAToRegex();
}