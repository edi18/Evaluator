
#include <iostream>
#include <stack>
#include <cmath>

bool isOpeningBrackets(char c)
{
	return c == '(' || c == '[' || c == '{';
}

bool isClosingBrackets(char c)
{
	return c == ')' || c == ']' || c == '}';
}

bool isDigit(char c)
{
	return c >= '0' && c <= '9';
}

bool isOperation(char c)
{
	return c == '+' || c == '-' || c == '*' || c == '/' || c == '^' ||  c == 's' || c == 'a';
}

char matchingBrackets(char c)
{
	if (c == ')')
	{
		return '(';
	}
	else if (c == ']')
	{
		return '[';
	}
	else if (c == '}')
	{
		return '{';
	}
}

enum class TOKEN
{
	OPEN_BRACKETS,
	CLOSE_BRACKETS,
	OPERATION,
	NUMBER
};

double evaluate_operation(double operand1, double operand2, char operation)
{
	if (operation == '+')
	{
		return operand1 + operand2;
	}
	else if (operation == '-')
	{
		return operand1 - operand2;
	}
	else if (operation == '*')
	{
		return operand1 * operand2;
	}
	else if (operation == '/')
	{
		return operand1 / operand2;
	}
	else if (operation == '^')
	{
		return pow(operand1, operand2);
	}
}

double evaluate_operation(double operand, char operation)
{
	if (operation == 's')
	{
		return sin(operand);
	}
	else if (operation == 'a')
	{
		return abs(operand);
	}
}

int priority(char c)
{
	if (c == 's' || c == 'a')
	{
		return 3;
	}
	else if (c == '^')
	{
		return 2;
	}
	else if (c == '*' || c == '/')
	{
		return 1;
	}
	else if (c == '+' || c == '-')
	{
		return 0;
	}
}

void excecute_operation(std::stack<double>& numbers, std::stack<char>& symbols)
{
	if (priority(symbols.top()) < 3)
	{
		if (numbers.size() < 2)
			throw "Expression is not balanced.";
		double operand2 = numbers.top();
		numbers.pop();
		double operand1 = numbers.top();
		numbers.pop();
		char operation = symbols.top();
		symbols.pop();
		numbers.push(evaluate_operation(operand1, operand2, operation));
	}
	else
	{
		if (numbers.size() < 1)
			throw "Expression is not balanced.";
		double operand = numbers.top();
		numbers.pop();
		char operation = symbols.top();
		symbols.pop();
		numbers.push(evaluate_operation(operand, operation));
	}
}

int main()
{
	while (true)
	{
		try
		{
			std::stack<char> brackets;
			std::stack<char> symbols;
			std::stack<double> numbers;\

			TOKEN previous = TOKEN::OPEN_BRACKETS;  // begin with open brackets

			while (std::cin.peek() == '\n')
				std::cin.get();		// skip any new line char

			while (std::cin.peek() != '\n')  // read until new line char
			{
				while (std::cin.peek() == ' ')
					std::cin.get();		// skip whitespace

				if (isOpeningBrackets(std::cin.peek()))
				{
					if (previous == TOKEN::NUMBER || previous == TOKEN::CLOSE_BRACKETS)
						throw "Open brackets can't go after a number or closed brackets!";
					brackets.push(std::cin.peek());  // remove read char from stream and push to brackets
					previous = TOKEN::OPEN_BRACKETS;
					
					symbols.push(std::cin.get());
				}
				else if (isDigit(std::cin.peek()))
				{
					if (previous == TOKEN::NUMBER || previous == TOKEN::CLOSE_BRACKETS)
						throw "Numbers can't go after a number or closed brackets!";
					double operand;
					std::cin >> operand;	// remove entire num from stream
					previous = TOKEN::NUMBER;

					numbers.push(operand);
				}
				else if (isOperation(std::cin.peek()))
				{
					if (std::cin.peek() == '-' && previous == TOKEN::OPEN_BRACKETS)
					{
						numbers.push(0);
						previous = TOKEN::NUMBER;
					}
					else
					{
						if (std::cin.peek() != 's' && std::cin.peek() != 'a')
						{
							if (previous == TOKEN::OPEN_BRACKETS || previous == TOKEN::OPERATION)
								throw "Operations can't go after an operation or open brackets!";
						}
						else
						{
							if (previous == TOKEN::CLOSE_BRACKETS)
								throw "Invalid expression.";
						}

						previous = TOKEN::OPERATION;

						char operation = std::cin.get();	// remove read char from stream and push to stack
						if (operation == 's')	// if it's sin expression, then check for valid input
						{
							if (std::cin.get() != 'i')
							{
								throw "Invalid expression.";
							}
							if (std::cin.get() != 'n')
							{
								throw "Invalid expression.";
							}
						}
						else if (operation == 'a')
						{
							if (std::cin.get() != 'b')
							{
								throw "Invalid expression.";
							}
							if (std::cin.get() != 's')
							{
								throw "Invalid expression.";
							}
						}
						while (!symbols.empty() && isOperation(symbols.top()) && priority(operation) <= priority(symbols.top()))
						{
							excecute_operation(numbers, symbols);
						}
						symbols.push(operation);
					}
				}
				else if (isClosingBrackets(std::cin.peek()))
				{
					if (previous == TOKEN::OPEN_BRACKETS || previous == TOKEN::OPERATION)
						throw "Closed brackets can't go after an operation!";
					char close_bracket = std::cin.get();
					if (!brackets.empty())
					{
						if(matchingBrackets(close_bracket) != brackets.top())
						{
							throw "Brackets are not matching!";
						}
						brackets.pop();
					}
					else
					{
						throw "Brackets are not balanced!";
					}
					previous = TOKEN::CLOSE_BRACKETS;

					while (!symbols.empty() && !isOpeningBrackets(symbols.top()))
					{
						excecute_operation(numbers, symbols);
					}
					symbols.pop();	// remove open brackets from stack 
				}
				else
				{
					throw "Unknown symbol!";
				}

			}
			if (!brackets.empty())	// brackets has to be empty by end for balance
			{
				throw "Brackets are not balanced!";
			}
			if (previous != TOKEN::NUMBER && previous != TOKEN::CLOSE_BRACKETS)
			{
				throw "Expression has to end with a number or closed brackets!";
			}

			while (!symbols.empty())
			{
				excecute_operation(numbers, symbols);
			}
			std::cout << numbers.top() << std::endl;
		}
		catch (const char* exception)
		{
			std::cout << exception << std::endl;
			std::cin.ignore(10000, '\n');
		}
	}
	
	return 0;
}