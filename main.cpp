// Activision Test.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <thread>
#include <mutex>

#include "FileIO.h"

using namespace std;




int noOfLines, noOfChars;
vector<string> wheelChars;
vector<string> dictonary;
vector<string> wheelCombinations;

vector<string> finalStrings;


int noOfMatches = 0;

string fullCombinationList = "";
string fullCombinationList2 = "";
string fullCombinationList3 = "";

mutex mu;


template <typename T>
void remove_duplicates(std::vector<T>& vec)
{
	std::sort(vec.begin(), vec.end());
	vec.erase(std::unique(vec.begin(), vec.end()), vec.end());
}


bool ReadWheel(string wheelPath)
{

	std::ifstream wheelFile(wheelPath);
	if (wheelFile.fail())
	{
		return false;
	}

	wheelFile >> noOfLines;
	wheelFile >> noOfChars;


	for (int i = 0; i < noOfLines; i++)
	{
		for (int j = 0; j < noOfChars; j++)
		{
			char currentChar;
			wheelFile >> currentChar;
			wheelChars[i].push_back(currentChar);
		}
	}
	return true;
}


bool ReadDictonary(string path)
{
	ifstream dictoryFile(path);

	if (dictoryFile.fail())
	{
		return false;
	}

	for (string line; getline(dictoryFile, line);)
	{
		if (line.length() <= noOfLines)
		{
			std::transform(line.begin(), line.end(), line.begin(), ::toupper);
			dictonary.emplace_back(line);
		}
	}
	return true;
}




void CreateOffset(int lockedX, int xPointer)
{

	if (xPointer == noOfChars)
	{
		return;
	}
	else
	{
		string test = "";

		test.push_back(wheelChars[0].at(lockedX));
		for (int y = 1; y < noOfLines; y++)
		{
			test.push_back(wheelChars[y].at(xPointer));
		}
		wheelCombinations.emplace_back(test);

		CreateOffset(lockedX, ++xPointer);
	}
}


void CreateWheelCombinations(int lockedX, int lockedY, int yPointer, int xPointer)
{

	if (yPointer == noOfLines)
	{
		yPointer = 0;
		xPointer++;
	}

	if (xPointer == noOfChars) 	 return;

	if (yPointer == lockedY)
	{
		char ch = wheelChars[yPointer].at(lockedX);
		////mu.lock();
		fullCombinationList.push_back(ch);
		//mu.unlock();
	}
	else
	{
		char ch = wheelChars[yPointer].at(xPointer);
		//mu.lock();
		fullCombinationList.push_back(ch);
		//mu.unlock();
	}

	CreateWheelCombinations(lockedX, lockedY, ++yPointer, xPointer);
}


void CreateWheelCombinations2(int lockedX, int lockedY, int yPointer, int xPointer, int noOfIterations, int xJumpValue)
{

	if (yPointer == noOfLines)
	{
		yPointer = 0;
	}

	if (xPointer >= noOfChars)
	{
		xPointer = 0;
	}
	/*if (xPointer > noOfChars)
	{
		xPointer = noOfChars - xJumpValue - 1;
	}*/

	if (noOfIterations == 0)
	{
		noOfIterations = noOfChars * noOfLines;
		xJumpValue++;
	}

	if (xJumpValue == noOfChars) return;


	if (yPointer == lockedY)
	{
		char ch = wheelChars[yPointer].at(lockedX);
		//mu.lock();
		fullCombinationList2.push_back(ch);
		//mu.unlock();
	}
	else
	{
		char ch = wheelChars[yPointer].at(xPointer);
		//mu.lock();
		fullCombinationList2.push_back(ch);
		//mu.unlock();
	}

	xPointer += xJumpValue;

	CreateWheelCombinations2(lockedX, lockedY, ++yPointer, xPointer, --noOfIterations, xJumpValue);
}



void CreateWheelCombinations3(int lockedX, int lockedY, int yPointer, int xPointer, int noOfIterations, int xJumpValue)
{
	if (yPointer == noOfLines)
	{
		yPointer = 0;
	}

	if (xPointer <= 0)
	{
		xPointer = noOfChars - 1;
	}
	/*if (xPointer < 0)
	{
		xPointer = xJumpValue - 1;
	}*/

	if (noOfIterations == 0)
	{
		noOfIterations = noOfChars * noOfLines;
		xJumpValue--;
	}

	if (xJumpValue == 0) return;


	if (yPointer == lockedY)
	{
		char ch = wheelChars[yPointer].at(lockedX);
		//mu.lock();
		fullCombinationList3.push_back(ch);
		//mu.unlock();
	}
	else
	{
		char ch = wheelChars[yPointer].at(xPointer);
		//mu.lock();
		fullCombinationList3.push_back(ch);
		//mu.unlock();
	}

	xPointer -= xJumpValue;

	CreateWheelCombinations3(lockedX, lockedY, ++yPointer, xPointer, --noOfIterations, xJumpValue);
}




void CreateCombinationsStraight()
{
	for (int y = 0; y < noOfLines; y++)
	{
		for (int x = 0; x < noOfChars; x++)
		{
			CreateWheelCombinations(x, y, 0, 0);
		}
	}
}


void CreateCombinationsDiagonalFwd()
{
	for (int y = 0; y < noOfLines; y++)
	{
		for (int x = 0; x < noOfChars; x++)
		{
			CreateWheelCombinations2(x, y, 0, 0, noOfLines * noOfChars, 1);
		}
	}
}
void CreateCombinationsDiagonalBckwd()
{
	for (int y = 0; y < noOfLines; y++)
	{
		for (int x = 0; x < noOfChars; x++)
		{
			CreateWheelCombinations3(x, y, 0, noOfChars - 1, noOfLines * noOfChars, noOfChars - 1);
		}
	}
}



void FindInDictonary(int startIndex, int endIndex, vector<string> outFile)
{
	for (int i = startIndex; i < endIndex; i++)
	{
		for (int j = 0; j < dictonary.size(); j++)
		{
			const char* dict = dictonary[j].c_str();
			const char* currentCombination = wheelCombinations[i].c_str();

			const char* result;
			result = strstr(currentCombination, dict);

			if (result != NULL)
			{
				outFile.emplace_back(dictonary[j]);
			}
		}
	}
}



int main(int argc, char* argv[])
{

	if (argc != 3)
	{
		cout << "Invalid Parameters \n";
		return 0;
	}


	bool wheelRead = ReadWheel(argv[1]);
	if (!wheelRead)
	{
		cout << "Wheel file not Read";
		return 0;
	}

	bool DictoryRead = ReadDictonary(argv[2]);
	if (!DictoryRead)
	{
		cout << "Dictory file not Read";
		return 0;
	}





	//CreateCombinationsStraight();
	//CreateCombinationsDiagonalFwd();
	//CreateCombinationsDiagonalBckwd();
	thread t11(&CreateCombinationsStraight);
	thread t12(&CreateCombinationsDiagonalFwd);
	thread t13(&CreateCombinationsDiagonalBckwd);

	t11.join();
	t12.join();
	t13.join();

	//string::append()

	fullCombinationList.append(fullCombinationList2);
	fullCombinationList.append(fullCombinationList3);






	int i = 0;
	while (fullCombinationList.size() != 0)
	{
		wheelCombinations.emplace_back(fullCombinationList.substr(i, noOfLines));
		i += noOfLines;
		if (i == fullCombinationList.size())
		{
			break;
		}
	}

	remove_duplicates(wheelCombinations);



	/*for (int i = 0; i < wheelCombinations.size(); i++)
	{
		cout << endl << wheelCombinations[i];
	}*/

	//cout << endl << endl << endl;

	int wheelCombinationsdivided = wheelCombinations.size() / 4;

	vector<string> finalStrings2;
	vector<string> finalStrings3;
	vector<string> finalStrings4;


	auto start = chrono::high_resolution_clock::now();

	thread t1(FindInDictonary,                            0, wheelCombinationsdivided    , finalStrings);
	thread t2(FindInDictonary, wheelCombinationsdivided    , wheelCombinationsdivided * 2, finalStrings2);
	thread t3(FindInDictonary, wheelCombinationsdivided * 2, wheelCombinationsdivided * 3, finalStrings3);
	thread t4(FindInDictonary, wheelCombinationsdivided * 2, wheelCombinations.size()    , finalStrings4);

	t1.join();
	t2.join();
	t3.join();
	t4.join();


	for (int i = 0; i < finalStrings2.size(); i++)
	{
		finalStrings.emplace_back(finalStrings2[i]);
	}
	for (int i = 0; i < finalStrings3.size(); i++)
	{
		finalStrings.emplace_back(finalStrings3[i]);
	}
	for (int i = 0; i < finalStrings4.size(); i++)
	{
		finalStrings.emplace_back(finalStrings4[i]);
	}

	remove_duplicates(finalStrings);

	auto stop = chrono::high_resolution_clock::now();

	auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);
	cout << endl << "Matches found: " << duration.count();

	
	return 0;
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
