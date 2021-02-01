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


using namespace std;




int noOfLines, noOfChars;
int noOfMatches = 0;
int wheelCombinationsdivided = 0;

vector<string> wheelChars;
vector<string> dictonary;
vector<string> wheelCombinations;

vector<string> finalStrings;



string fullCombinationList = "";
string diagonalForwardList = "";
string diagonalBackwardList = "";

mutex mu;




template <typename T>
void RemoveDuplicates(std::vector<T>& vec)
{
	std::sort(vec.begin(), vec.end());
	vec.erase(std::unique(vec.begin(), vec.end()), vec.end());
}

template<typename T>
void PrintVector(vector<T>& vec)
{
	for (auto it = vec.begin(); it != vec.end(); it++)
	{
		cout << *it << endl;
	}
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
		wheelChars.emplace_back("");
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




void CreateWheelCombinationsStr(int lockedX, int lockedY, int yPointer, int xPointer)
{

	if (yPointer == noOfLines)
	{
		yPointer = 0;
		xPointer++;
	}

	if (xPointer == noOfChars)
	{
		xPointer = yPointer = 0;
		lockedX++;
	}

	if (lockedX == noOfChars)
	{
		xPointer = yPointer = lockedX = 0;
		lockedY++;
	}
	if (lockedY == noOfLines)
		return;

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

	CreateWheelCombinationsStr(lockedX, lockedY, ++yPointer, xPointer);
}


void CreateWheelCombinationsFwdDiag(int lockedX, int lockedY, int yPointer, int xPointer, int noOfIterations, int xJumpValue)
{

	if (yPointer == noOfLines)
	{
		yPointer = 0;
	}

	if (xPointer >= noOfChars)
	{
		xPointer = -1 + xJumpValue;
	}

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
		diagonalForwardList.push_back(ch);
		//mu.unlock();
	}
	else
	{
		char ch = wheelChars[yPointer].at(xPointer);
		//mu.lock();
		diagonalForwardList.push_back(ch);
		//mu.unlock();
	}

	xPointer += xJumpValue;

	CreateWheelCombinationsFwdDiag(lockedX, lockedY, ++yPointer, xPointer, --noOfIterations, xJumpValue);
}



void CreateWheelCombinationsBckDiag(int lockedX, int lockedY, int yPointer, int xPointer, int noOfIterations, int xJumpValue)
{
	if (yPointer == noOfLines)
	{
		yPointer = 0;
	}

	if (xPointer < 0)
	{
		xPointer = noOfChars - 1;
	}

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
		diagonalBackwardList.push_back(ch);
		//mu.unlock();
	}
	else
	{
		char ch = wheelChars[yPointer].at(xPointer);
		//mu.lock();
		diagonalBackwardList.push_back(ch);
		//mu.unlock();
	}

	xPointer -= xJumpValue;

	CreateWheelCombinationsBckDiag(lockedX, lockedY, ++yPointer, xPointer, --noOfIterations, xJumpValue);
}




void CreateCombinationsStraight()
{
	CreateWheelCombinationsStr(0, 0, 0, 0);
}


void CreateCombinationsDiagonalForward()
{
	for (int y = 0; y < noOfLines; y++)
	{
		for (int x = 0; x < noOfChars; x++)
		{
			CreateWheelCombinationsFwdDiag(x, y, 0, 0, noOfLines * noOfChars, 1);
		}
	}
}
void CreateCombinationsDiagonalBackword()
{
	for (int y = 0; y < noOfLines; y++)
	{
		for (int x = 0; x < noOfChars; x++)
		{
			CreateWheelCombinationsBckDiag(x, y, 0, noOfChars - 1, noOfLines * noOfChars, noOfChars - 1);
		}
	}
}

void FindInDictonary(int startIndex, int endIndex, vector<string>& outFile)
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


	if (!ReadWheel(argv[1]))
	{
		cout << "Wheel file not Read";
		return 0;
	}

	if (!ReadDictonary(argv[2]))
	{
		cout << "Dictory file not Read";
		return 0;
	}

	//
	// pattern Generation find all combinations 
	//

	thread patternThread1(&CreateCombinationsStraight);
	thread patternThread2(&CreateCombinationsDiagonalForward);
	thread patternThread3(&CreateCombinationsDiagonalBackword);

	patternThread1.join();
	patternThread2.join();
	patternThread3.join();

	fullCombinationList.append(diagonalForwardList);
	fullCombinationList.append(diagonalBackwardList);


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

	RemoveDuplicates(wheelCombinations);

	//
	// divide all combinations into 4 parts for multi threading 
	//

	wheelCombinationsdivided = wheelCombinations.size() / 4;

	vector<string> finalStrings2;
	vector<string> finalStrings3;
	vector<string> finalStrings4;


	//auto start = chrono::high_resolution_clock::now();

	thread t1(FindInDictonary, 0, wheelCombinationsdivided, std::ref(finalStrings));
	thread t2(FindInDictonary, wheelCombinationsdivided, wheelCombinationsdivided * 2, std::ref(finalStrings2));
	thread t3(FindInDictonary, wheelCombinationsdivided * 2, wheelCombinationsdivided * 3, std::ref(finalStrings3));
	thread t4(FindInDictonary, wheelCombinationsdivided * 2, wheelCombinations.size(), std::ref(finalStrings4));

	t1.join();
	t2.join();
	t3.join();
	t4.join();

	//FindInDictonary(0, wheelCombinations.size(), finalStrings);

	// combine strings as mutex was slower compared to single thread
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

	RemoveDuplicates(finalStrings);


	//auto stop = chrono::high_resolution_clock::now();
	//auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);
	//cout << endl << "Matches found: " << duration.count();

	PrintVector(finalStrings);
	cout << "Number of Matches Found: " << finalStrings.size();

	return 0;
}

