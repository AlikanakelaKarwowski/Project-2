/*
* Alex Karwowski
* akk42@zips.uakron.edu
* infixPostfix.cpp for Project 2
* Data Structures - Prof Diane Foreback
*/
#include <iostream>
#include <stack>
#include <string>
#include <sstream>
#include <ctype.h>

/*
* infixToPostfix takes and infix expression as input and changes it to a postfix expression
* @param input; takes user input for infix expression
*/
void infixToPostfix(std::string& input);

/*
* calculateStack calculates the value of the operands and operators of a postfix expression
* @param operand1; the first operand (2nd operand popped off the stack)
* @param operand2; the second operand(1st operand popped off the stack)
* @param operators; the operator that needs to be applied to the stack
* @param numberStack; double stack that holds the results of the postfix expression
*/
void calculateStack(double& operand1, double& operand2, char operators, std::stack<double>& numberStack);

/*
* printMenu prints the menu choices and recieves the users input from
* @param choice; the choice the user made for the menu
* @param input; the users input that was read in
*/
void printMenu(int&, std::string&);

/*
* evalPostfix evaluates a postfix expression
* @param input; the postfix expression to be evaluated
* @param result; the result of the evaluation of the postfix expression
* @return "True", "Error: To Many Operators", "Error: Not Enough Operators" based on the input recieved
*/
std::string evalPostfix(std::string& input, double& result);

/*
* priority gives each operator a number based off of a priority scale
* @param operator1; the stacks top operator
* @param operator2; the current operator being parsed
* @return true or false depending on if the top operator has a higher priority than the current operator
*/
bool priority(char operator1, char operator2);

/*
* is_operator determines if a char being passed in is a valid operator
* @param x; a char that will be tested to see if it is one of the following operators +, -, *, /
* @return true or false if the char passed in is a valid operator
*/
bool is_operator(char x);

int main() {
	//initalize all variables needed for menu selection
	int choice;
	bool valid = false;
	std::string input;
	//loop through menu choices until user chooses to exit
	while(choice != 0)
	{
		//input validation for menu choices
		while (!valid)
		{
			//prints menu and processes choice
			printMenu(choice, input);

			//valididates user input for choice
			if (choice >= 0 && choice <= 2)
						valid = true;
			else
				std::cout << "\n" << choice << " is not a valid option. Please try again." << "\n\n";
		}
		//switch statement that calls specific function dependin on menu choice
		switch (choice)
		{
			//exit case
			case 0:
				break;
			//infix to postfix menu choice
			case 1:
				{

					std::cout << "Please enter an infix expression:" << '\n';
					//read input and pass it to function infixToPostfix
					std::getline(std::cin, input);
					infixToPostfix(input);
					//print out postfix expression
					std::cout << "The postfix expression is:\n"<< input << '\n';
					//evaluate postfix expression (answer must be returned as a double)
					double evaluated = 0.0;
					std::string itWorks = evalPostfix(input, evaluated);
					//error checking for evaluating postfix expressions
					//catches unexpected inputs
					if(itWorks == "True")
					{
						std::cout << "The postfix expression evaluates to: " << evaluated << '\n';
					}
					else
					{
						//prints Error Message
						std::cout << itWorks << '\n';
					}
					break;
				}
			//evaluate postfix menu choice
			case 2:
			{
				//read input and pass it into function to be evaluated
				std::cout << "Please enter a postfix expression to evaluate:" << '\n';
				std::getline(std::cin, input);
				double evaluated = 0.0;
				std::string itWorks = evalPostfix(input, evaluated);
				//error checking for evaluating postfix expression
				//catches unexpected inputs
				if(itWorks == "True")
				{
					std::cout << "The postfix expression evaluates to: " << evaluated << '\n';
				}
				else
					//prints Error Message
					std::cout << itWorks << '\n';
				break;
			}
		}
		//restart menu input loop
		valid = false;
		}
		return 0;
}

//takes an infix expression and converts it to a postfix expression
void infixToPostfix(std::string& expression)
{
	//create a stack to hold the postfix expression
	//create a string to hold the postfix expression
	std::stack<char> operatorStack;
	std::string postFix;
	//create an iterator for the infix expression
	auto iter = expression.begin();

	//iterate through the infix expression and convert it to a postfix expression
	while(iter != expression.end())
	{
		//push '(' onto the stack as it has the highest priority
		if(*iter == '(')
		{
			operatorStack.push(*iter);
			++iter;
		}
		//if the iterator is a digit add it to the postFix expression
		else if(isdigit(*iter) || *iter == '.')
		{
			// append .0 to numbers with no decimal points
			if(((*(iter - 1) != '.') && (*(iter + 1) == ' ')) || ((*(iter - 1) != '.') && ((iter + 1) == expression.end())))
			{
					postFix += *iter;
					postFix += ".0";
					++iter;
			}
			else
			{
				//add the current iterator to postFix
				postFix += *iter;
				++iter;
			}
		}
		//if the iterator is a space add it to the postFix expression
		else if(isspace(*iter))
		{
			//as long as the previous iterator is not an operator add a space
			if(is_operator(*(iter -1)))
			{

				++iter;
			}
			else
			{
				postFix += *iter;
				++iter;
			}
		}
		//if the iterator is an operator push it to the stack based on its priority and pop others off the stack based on their priority
		else if(is_operator(*iter))
		{
			//add operators to the postFix expression while the stack is not empty, and while the top of the stack is not '('
			//and the priority of the iterator is not higher than the priority of the top of the stack
			while(!operatorStack.empty() && operatorStack.top() != '(' && priority(operatorStack.top(), *iter))
			{
				postFix += operatorStack.top();
				//add a space because it is the only way to get it to work
				postFix += ' ';
				operatorStack.pop();
			}
			//after popping off all operators of lower priority push the current operator onto the operator stack
			operatorStack.push(*iter);
			++iter;
		}
		//if the iterator is ')' pop all operators off the operator stack until '(' is popped off
		else if(*iter == ')')
		{
			//pop all operators off the operator stack until '(' is popped off and add those operators to postFix
			while(!operatorStack.empty() && operatorStack.top() != '(')
			{
				postFix += operatorStack.top();
				operatorStack.pop();
			}
			operatorStack.pop();
			++iter;
		}
		//if something else is inputed to the string than exit with an error message
		else
		{
			std::cout << "Invalid input: " << *iter << "\n Please try again.\n";
			std::exit(1);
		}
	}
	//once the iterator has reached the end of the string pop all operators off the operator stack and add them to postFix appropriately
	while (!operatorStack.empty())
	{
		if(operatorStack.top() == '(')
			operatorStack.pop();
		else
		{
			//add a space because its the only way i can get it to work
			postFix += ' ';
			postFix += operatorStack.top();
			operatorStack.pop();
		}
	}
	//clear the expression string and then add postFix to the expression to be passed by reference
	expression.clear();
	expression = postFix;
}

//calculates the operands for the stack of a postfix expression
void calculateStack(double& operand1, double& operand2, char operators, std::stack<double>& numberStack)
{
	//create a result variable to store on the stack
	double result = 0.0;
	//based on the operator perform the mathmatical calculation
	if (operators == '*')
	{
		//perform the mathmatical calculation and push the result to the stack
		result = operand1 * operand2;
		numberStack.push(result);
	}
	else if (operators == '/')
	{
		//perform the mathmatical calculation and push the result to the stack
		result = operand1 / operand2;
		numberStack.push(result);
	}
	else if (operators == '+')
	{
		//perform the mathmatical calculation and push the result to the stack
		result = operand1 + operand2;
		numberStack.push(result);
	}
	else if (operators == '-')
	{
		//perform the mathmatical calculation and push the result to the stack
		result = operand1 - operand2;
		numberStack.push(result);
	}
}

//prints menu choices and takes users input
void printMenu(int& choice, std::string& input)
{
	std::cout << "0. Exit" << "\n";
	std::cout << "1. Infix to postfix" << '\n';
	std::cout << "2. Postfix evaluation" << '\n';
	std::cout << "Please enter a menu option [0-2]: ";
	//get menu choices
	std::getline(std::cin, input);
	std::stringstream(input) >> choice;
}

//evaluates a postfix expression and gives a result back to the user
std::string evalPostfix(std::string& postFix, double& result)
{
	//create an iterator for postfix pointing to the begining of the string
	auto iter = postFix.begin();
	std::stack<double> numberStack;
	//create an index for string to double conversion
	int index = 0;

	while (iter != postFix.end())
	{
		//determines if the current iterator position
		if (isdigit(*iter))
		{
			//create a length variable for correctly changing string to double
			std::string::size_type length;
			double number = std::stod(&postFix[index], &length);
			//indexs begin at - so we need to subtract 1 from length before adding it to the index
			index += length - 1;
			//add to the iterator eqaul to the length size so it does not reprocess the same number
			for( size_t i = 0; i < length - 1; ++i)
				++iter;
			//push the number onto the stack
			numberStack.push(number);
		}
		// skip white space
		else if (isspace(*iter))
			{
				//do nothing so might as well as create a bunny
			}
		// operator read in and calculation
		else if(!numberStack.empty())
			{
				//operand 2 is first operand on the stack
				double operand2 = numberStack.top();
				numberStack.pop();

				if (!numberStack.empty())
				{
					//operand 1 is the 2nd operand on the stack
					double operand1 = numberStack.top();
					numberStack.pop();
					//calulate the result of the 2 operands and the ooperator
					calculateStack(operand1, operand2, *iter, numberStack);

				}
			// return too many operators status
				else
				{
					return "Error: To Many Operators\n";
				}
			}
			++iter;
			++index;
	}
	//Error Checking
	if (numberStack.size() > 1)
	{
		return "Error: Not Enough Operators\n";
	}
	//take the result and set it equal the final calculation
	//then return the Output message
	else
	{
		result = numberStack.top();
		numberStack.pop();
		return "True";
	}
}

//determines operator priority based on scale given in class
bool priority(char symbol1, char symbol2)
{
	//create values for comparison of operators
	//symbol1 is the stacks top, and symbol2 is the current iterator
	int value1 = 0, value2 = 0;
	// '*' and '/' are highest priority
	if(symbol1 == '*' || symbol1 == '/')
		value1 = 2;
	//'+' and '-' are lowest priority
	else if(symbol1 == '+' || symbol1 == '-')
		value1 = 1;
	//error checking
	else
		value1 = 0;
	// '*' and '/' are highest priority
	if(symbol2 == '*' || symbol2 == '/')
		value2 = 2;
	//'+' and '-' are lowest priority
	else if(symbol2 == '+' || symbol2 == '-')
		value2 = 1;
	//error checking
	else
		value2 = 0;
	//return the comparison of priority
	return value1 >= value2;
}

//determines if a char is an operator
bool is_operator(char x)
{
	return (x == '+' || x == '-' || x == '/' || x == '*');
}
