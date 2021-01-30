// Activision Test.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>

#include "FileIO.h"

using namespace std;




int noOfLines, noOfChars;
vector<string> wheelChars;
vector<string> dictonary;
vector<string> wheelCombinations;

vector<string> finalStrings;


int noOfMatches = 0;

string test = "";


template <typename T>
void remove_duplicates(std::vector<T>& vec)
{
	std::sort(vec.begin(), vec.end());
	vec.erase(std::unique(vec.begin(), vec.end()), vec.end());
}


void ReadWheel(string wheelPath)
{
	std::ifstream wheelFile(wheelPath);
	wheelFile >> noOfLines;
	wheelFile >> noOfChars;


	for (int i = 0; i < noOfLines; i++)
	{
		wheelChars.push_back("");
		for (int j = 0; j < noOfChars; j++)
		{
			char currentChar;
			wheelFile >> currentChar;
			wheelChars[i].push_back(currentChar);
		}
	}
}


void ReadDictonary(string path)
{
	ifstream dictoryFile(path);
	for (string line; getline(dictoryFile, line);)
	{
		if (line.size() <= noOfChars)
		{
			//char* dict = 
			//strstr()

			std::transform(line.begin(), line.end(), line.begin(), ::toupper);
			dictonary.emplace_back(line);
		}
	}

	/*for (int i = 0; i < dictonary.size(); i++)
	{
		cout << dictonary[i] << endl;
	}*/


}




void CreateOffset(int mainX, int k)
{

	if (k == noOfChars)
	{
		return;
	}
	else
	{
		string test = "";

		test.push_back(wheelChars[0].at(mainX));
		for (int y = 1; y < noOfLines; y++)
		{
			test.push_back(wheelChars[y].at(k));
		}
		wheelCombinations.emplace_back(test);

		CreateOffset(mainX, ++k);

	}
}


void CreateWheelCombinations(int lockedX, int lockedY, int variableY, int variableX)
{

	if (variableY == noOfLines)
	{
		variableY = 0;
		++variableX;
	}

	if (variableX == noOfChars)
	{
		return;
	}


	if (variableY == lockedY)
	{
		char ch = wheelChars[variableY].at(lockedX);
		test.push_back(ch);
		//variableX--;

	}
	else
	{
		char ch = wheelChars[variableY].at(variableX);
		test.push_back(ch);
	}



	CreateWheelCombinations(lockedX, lockedY, ++variableY, variableX);
}


void CreateWheelCombinations2(int lockedX, int lockedY, int variableY, int variableX)
{

	if (variableY == 0)
	{
		variableY = noOfLines;
		--variableX;
	}

	if (variableX == 0)
	{
		return;
	}


	if (variableY == lockedY)
	{
		char ch = wheelChars[variableY].at(lockedX);
		test.push_back(ch);

	}
	else
	{
		char ch = wheelChars[variableY].at(variableX);
		test.push_back(ch);
	}



	CreateWheelCombinations(lockedX, lockedY, --variableY, variableX);
}








int main(int argc, char* argv[])
{

	if (argc != 3)
	{
		cout << "Invalid Parameters \n";
		return 0;
	}


	ReadWheel(argv[1]);
	ReadDictonary(argv[2]);
	////string 


	int c = 0;


	cout << c << endl << endl << endl;

	//for (int i = 0; i < noOfChars; i++)
	//{
		//CreateOffset2(0, 1, 0, 0);
		//CreateOffset(i, 0);
	//}


	/*for (int mainX = 0; mainX < noOfChars; mainX++)
	{*/
	/*for (int x = 0; x < noOfChars; x++)
	{
		test = "";
		for (int y = 0; y < noOfLines; y++)
		{
			test.push_back(wheelChars[y].at(x));
		}
		finalStrings.emplace_back(test);
	}*/
	//}

	for (int y = 0; y < noOfLines; y++)
	{
		for (int x = 0; x < noOfChars; x++)
		{
			CreateWheelCombinations(x, y, 0, 0);
		}
	}

	for (int y = noOfLines; y == 0; y--)
	{
		for (int x = noOfChars; x == 0; x--)
		{
			CreateWheelCombinations2(x, y, noOfChars, noOfLines);
		}
	}





	int i = 0;
	while (test.size() != 0)
	{

		wheelCombinations.emplace_back(test.substr(i, noOfLines));
		i += noOfLines;
		if (i == test.size())
		{
			break;
		}
	}

	remove_duplicates(wheelCombinations);



	for (int i = 0; i < wheelCombinations.size(); i++)
	{
		cout << endl << wheelCombinations[i];
	}

	cout << endl << endl << endl;


	for (int i = 0; i < wheelCombinations.size(); i++)
	{
		for (int j = 0; j < dictonary.size(); j++)
		{

			const char* dict = dictonary[j].c_str();
			const char* currentCombination = wheelCombinations[i].c_str();

			const char* result = strstr(currentCombination, dict);

			if (result != NULL)
			{
				finalStrings.emplace_back(dict);
			}
		}
	}

	remove_duplicates(finalStrings);





	for (int i = 0; i < finalStrings.size(); i++)
	{
		cout << endl << finalStrings[i];
	}

	cout << endl << "Matches found: " << finalStrings.size();


	// Loop through each argument and print its number and value
	for (int count{ 0 }; count < argc; ++count)
	{
		std::cout << endl << count << ' ' << argv[count] << '\n';
	}
	//}

}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
