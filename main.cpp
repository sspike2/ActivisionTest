// Activision Test.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <iostream>
#include <string>
#include <fstream>
#include <vector>

#include "FileIO.h"

using namespace std;




int noOfLines, noOfChars;
vector<string> wheelChars;
vector<string> dictonary;
vector<string> finalStrings;


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
			dictonary.emplace_back(line);
		}
	}

	/*for (int i = 0; i < dictonary.size(); i++)
	{
		cout << dictonary[i] << endl;
	}*/


}




void createOffset(int mainX, int k)
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
		finalStrings.emplace_back(test);

		createOffset(mainX, ++k);

	}
}

string test = "";


void CreateOffset2(int lockedX, int lockedY, int k, int l)
{
	if (k == noOfChars)
	{
		if (lockedY < noOfChars)
			lockedX++;
		k = 0;
	}


	if (l == noOfLines)
	{
		if (lockedY < noOfLines)
			lockedY++;
		l = 0;
	}

	if (lockedX == noOfChars && lockedY == noOfLines)
	{
		return;
	}

	for (int y = 0; y < noOfLines; y++)
	{
		if (y == 0)
		{
			test.push_back(wheelChars[y].at(lockedX));
		}
		else
		{
			if (y == lockedY)
			{
				test.push_back(wheelChars[y].at(l));
				l++;
			}
			else
			{
				test.push_back(wheelChars[y].at(k));
				k++;
			}
		}
	}

	finalStrings.push_back(test);
	test = "";


	CreateOffset2(lockedX, lockedY, k, l);

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

	string test = "";

	int c = 0;


	cout << c << endl << endl << endl;

	for (int i = 0; i < noOfChars; i++)
	{
		//CreateOffset2(0, 1, 0, 0);
		createOffset(i, 0);
	}


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


	for (int i = 0; i < finalStrings.size(); i++)
	{
		cout << endl << finalStrings[i];
	}



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
