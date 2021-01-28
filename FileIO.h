#pragma once
#include <string>
#include <vector>

using namespace std;

class FileIO
{
public:
	FileIO() {}

	vector<string> ReadWheelFile(string wheelPath);
	vector<string> ReadDictoryFile(string dictonaryPath);


protected:

	int noOfLines;
	int noOfCharacters;

};

