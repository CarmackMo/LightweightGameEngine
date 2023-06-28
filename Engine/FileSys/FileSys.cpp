#include "FileSys.h"

using std::ios;
using std::ifstream;
using std::string;

void ReadJSONFile()
{
	// Using fstream to get the file pointer in "file"
	ifstream file("JsonTest.json", ios::in);
	
	Json::Value actualJson;
	Json::Reader reader;

	if (file.fail())
	{
		DEBUG_PRINT("Read file fail!! \n ");
	}
	else
	{
		// Using reader we are parsing the JSON
		reader.parse(file, actualJson);

		string str = actualJson["Name"].asString();

	}


	

}