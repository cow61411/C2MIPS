#include <iostream>
#include <vector>
#include <string>
#include <stack>
#include <stdio.h>
#include <stdlib.h>
#include <map>
#include <sstream>
#include <limits>

using namespace std;

int priority(char a) {
    int temp;
    if (a == '^')
	temp = 1;
    else  if (a == '*' || a == '/')
	temp = 2;
    else  if (a == '+' || a == '-')
	temp = 3;
    return temp;
}

string int2str(int &i)
{
    string s;
    stringstream ss(s);
    ss << i;
    return ss.str();
}

string arithmetic_expression_into_stack(string input) {
    string infix = input;
    //cout << "Enter an arithmetic expression: " << endl;
    //getline(cin, infix);

    stack<char> operator_stack;

    stringstream output;

    for (unsigned i = 0; i < infix.length(); i++) 
    {
	if (infix[i] == '+' || infix[i] == '-' || infix[i] == '*' || infix[i] == '/' || infix[i] == '^') 
	{
	    while (!operator_stack.empty() && priority(operator_stack.top()) <= priority(infix[i])) 
	    {
		output << operator_stack.top();
		operator_stack.pop();
	    }
	    operator_stack.push(infix[i]);
	} 
	else if (infix[i] == '(') 
	{
	    operator_stack.push(infix[i]);
	}
	else if (infix[i] == ')') 
	{
	    while (operator_stack.top() != '(') 
	    {
		output << operator_stack.top();
		operator_stack.pop();
	    }
	    operator_stack.pop();
	} 
	else 
	{
	    output << infix[i];
	}
    }

    while (!operator_stack.empty()) 
    {
	output << operator_stack.top();
	operator_stack.pop();
    }

    //cout << output.str() << endl;
    //cout << operator_stack.empty() << endl;

    //cin.ignore(numeric_limits<streamsize>::max(), '\n');

    return output.str();
}

bool isinregisters(string input , vector<string> registers , int length)
{
    for (int i = 0 ; i < length ; i++)
	if(input == registers[i])
	    return true;
    return false;
}

string getregister(string input , vector<string> registers , vector<string> names , int length)
{
    for(int i = 0 ; i < length ; i++)
	if(registers[i] == input)
	    return names[i];
    return "";
}

bool neednewregisters(vector<bool> changeable , int length)
{
    for(int i = 0 ; i < length ; i++)
	if(changeable[i] == true)
	    return false;
    return true;
}

int main()
{
    //string ax,bx,cx;
    stack<string> commands;
    string input;
    //string registernames[] = {};
    map<string , string> opermap;
    opermap.insert(pair<string , string>("+" , "ADD"));
    opermap.insert(pair<string , string>("-" , "SUB"));
    opermap.insert(pair<string , string>("*" , "MUL"));
    opermap.insert(pair<string , string>("/" , "DIV"));

    cout << "        START" << endl;
    while(getline(cin,input))
    {
	//cout << input << endl;
	string num, val,num2;
	string ax , bx , cx;
	if(input.find("INT") != -1 && input.find("PRINT") == -1)
	{
	    //cout << input << endl;
	    while(1)
	    {
		num=input.substr((input.find(" ")+1),input.find(",")-(input.find(" ")+1));
		//cout<<num<<endl;
		if(num.find('=')==-1)
		{
		    num2=num;
		    if(num2.find(';')!=-1)
			num2=num2.erase(num2.find(';'));
		    cout<<num2<<"     RESW    1"<<endl;
		}
		else 
		{
		    num2=num;
		    if(num2.find(';')!=-1)
			num2=num2.erase(num2.find(';'));
		    val=num2.substr(num.find("=")+1,num.length()-(num.find("=")+1));
		    num2=num.substr(0,num.find("="));
		    cout<<num2<<"     WORD    "<<val<<endl;
		}
		input=input.erase(0,input.find(",")+1);
		if(num[num.length()-1]==';')
		    break;
	    }
	}

	else if (input.find("PRINT") != -1)
	{
	    //cout << input << endl;
	    input.erase(0 , 6);
	    input.erase(input.length() - 2 , 2);
	    cout << "        WRT " << input << endl;
	}

	else
	{
	    //cout << input << endl;
	    stack<string> operators , temp;
	    string processed = input;
	    string result;
	    int count = 0;
	    for(int i = 0 ; i < input.length() ; i++)
	    {
		if(input[i] == '(')
		{
		    processed.insert(i + count + 1, " ");
		    count++;
		}
		else if(input[i] == ')')
		{
		    processed.insert(i + count , " ");
		    count++;
		}
	    }
	    processed.erase(processed.find(";") , 1);
	    //cout << processed << endl;
	    processed.append(" ");
	    //cout << processed << endl;
	    processed = arithmetic_expression_into_stack(processed);
	    //cout << processed << endl;
	    char ori[processed.length()];
	    strcpy(ori , processed.c_str());
	    char * pch;
	    pch = strtok(ori , " ");
	    string target , oper1 , oper2;
	    map<string , string> resultlocation;
	    int resultcount = 0;
	    vector<string> registernames;
	    vector<bool> changeable;
	    vector<string> registers;
	    char registercount = 'A';
	    string registernamepart = "X";
	    while(pch != NULL)
	    {
		//cout << pch << "!" << endl;
		//temp.push(pch);
		//cout << operators.top() << endl;
		if(!strcmp(pch , "="))
		{
		    target = temp.top();
		    //cout << target << endl;
		    temp.pop();
		}
		else if(!strcmp(pch , "+") || !strcmp(pch , "-") || !strcmp(pch , "*") || !strcmp(pch , "/"))
		{
		    oper1 = temp.top();
		    temp.pop();
		    if(isinregisters(oper1 , registers , registers.size()))
			oper1 = getregister(oper1 , registers , registernames , registers.size());
		    else if(!isinregisters(oper1 , registernames , registers.size()))
		    {
			if(! neednewregisters(changeable , changeable.size()))
			{
			    for(int i = 0 ; i < sizeof(changeable) ; i++)
				if(changeable[i] == true)
				{
				    cout << "        MOV " << oper1 << " " << registernames[i] << endl;
				    //registercontents[i] = oper1;
				    registers[i] = oper1;
				    oper1 = registernames[i];
				    changeable[i] = false;
				    break;
				}
			}
			else
			{
			    string newregister = registercount + registernamepart;
			    registercount += 1;
			    registers.push_back(oper1);
			    registernames.push_back(newregister);
			    cout << "        MOV " << oper1 << " " << newregister << endl;
			    oper1 = newregister;
			    changeable.push_back(false);
			}
		    }
		    oper2 = temp.top();
		    temp.pop();
		    if(isinregisters(oper2 , registers , registers.size()))
			oper2 = getregister(oper2 , registers , registernames , registers.size());
		    else if(!isinregisters(oper2 , registernames , registers.size()))
		    {
			if(!neednewregisters(changeable , changeable.size()))
			{
			    for(int i = 0 ; i < sizeof(changeable) ; i++)
				if(changeable[i] == true)
				{
				    cout << "        MOV " << oper2 << " " << registernames[i] << endl;
				    registers[i] = oper2;
				    oper2 = registernames[i];
				    changeable[i] = false;
				}
			}
			else
			{
			    string newregister = registercount + registernamepart;
			    registercount += 1;
			    registers.push_back(oper2);
			    registernames.push_back(newregister);
			    cout << "        MOV " << oper2 << " " << newregister << endl;
			    oper2 = newregister;
			    changeable.push_back(false);
			}
		    }
		    cout << "        " << opermap[pch] << " " << oper2 << " " << oper1 << endl;
		    temp.push(oper1);
		    for(int i = 0 ; i < registernames.size() ; i++)
			if(registernames[i] == oper1)
			    registers[i] = "";
		    for(int i = 0 ; i < changeable.size() ; i++)
			if(registernames[i] == oper2)
			    changeable[i] = true;
		}
		else
		{
		    temp.push(pch);
		}
		pch = strtok(NULL , " ");
	    }
	    cout << "        MOV " << result << temp.top() << " " << target << endl;
	    temp.pop();
	}
    }
    return 0;
}
