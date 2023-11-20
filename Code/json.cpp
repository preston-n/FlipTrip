#include"json.h"

//Ctors/Dtors
JSON::JSON(const char* jsonNAME)
{
	enableDEBUG_ = false;
	doc_ = new Document;
	char pathName[128] = ""; //MAX LENGTH OF PATHNAME
	sprintf_s(pathName, _countof(pathName), "Data/%s.json", jsonNAME);
	ifstream ifile;
	ifile.open(pathName);
	if (ifile.is_open())
	{
		jsonPath_ = pathName;
		readJFile();
		return;
	}
	if ("save" == jsonNAME)
		jsonPath_ = "Data/saveMain.json";
	else if ("score" == jsonNAME)
		jsonPath_ = "Data/scoreMain.json";
	else
	{
		LogError("ERROR IN JSON! Data/{}.json could not be open/found!", jsonNAME)
			return;
	}
	FILE* test;
	fopen_s(&test, pathName, "w+b");
	if (!test)
	{
		LogError("ERROR IN JSON WITH {}!", jsonNAME);
	}
	else
	{
		fclose(test);
	}
	readJFile();

	jsonPath_ = pathName;

	writeFile();
}

JSON::JSON()
{
	enableDEBUG_ = false;
	doc_ = new Document;
	jsonPath_ = "error";
}

JSON::~JSON()
{
	if (doc_)
	{
		delete doc_; //TEMP FIX
	}
}

//Operators
void JSON::operator()(const char* jsonNAME)
{
	char pathName[128] = ""; //MAX LENGTH OF PATHNAME
	sprintf_s(pathName, _countof(pathName), "Data/%s.json", jsonNAME);
	jsonPath_ = pathName;

	readJFile();
}


//Get/Set
void JSON::setJSONPath(const char* nameOfJsonFile)
{
	jsonPath_ = nameOfJsonFile;
}

const char* JSON::getJSONPath()
{
	return jsonPath_.c_str();
}

Document& JSON::getDoc()
{
	return *this->doc_;
}


//Read jsonString
std::string JSON::readJSONstring(const char* member1)
{
	std::string data;
	if (doc_->HasMember(member1))
	{
		data = doc_->operator[](member1).GetString();
		return data;
	}
	LogError("ERROR! JSON MISSING MEMBER {}!", member1);
	return data;
}

std::string JSON::readJSONstring(const char* member1, const char* member2)
{
	std::string data;
	if (doc_->HasMember(member1))
	{
		if (doc_->operator[](member1).HasMember(member2))
		{
			data = doc_->operator[](member1)[member2].GetString();
			return data;
		}
		LogError("ERROR! FOUND MEMBER {} BUT MISSING MEMBER {}!", member1, member2);
		return data;
	}
	LogError("ERROR! JSON MISSING MEMBER {}!", member1);
	return data;
}

std::string JSON::readJSONstring(const char* member1, const char* member2, const char* member3)
{
	std::string data;
	if (doc_->HasMember(member1))
	{
		if (doc_->operator[](member1).HasMember(member2))
		{
			if (doc_->operator[](member1)[member2].HasMember(member3))
			{
				data = doc_->operator[](member1)[member2][member3].GetString();
				return data;
			}
			LogError("ERROR! FOUND MEMBER {} AND {} BUT MISSING MEMBER {}!", member1, member2, member3);
			return data;
		}
		LogError("ERROR! FOUND MEMBER {} BUT MISSING MEMBER {}!", member1, member2);
		return data;
	}
	LogError("ERROR! JSON MISSING MEMBER {}!", member1);
	return data;
}

//Read jsonValue
Value JSON::readJSONJVal(const char* member1)
{
	Value data;
	if (doc_->HasMember(member1))
	{
		data = doc_->operator[](member1);
		return data;
	}
	LogError("ERROR! JSON MISSING MEMBER {}!", member1);
	return data;
}

Value JSON::readJSONJVal(const char* member1, const char* member2)
{
	Value data;
	if (doc_->HasMember(member1))
	{
		if (doc_->operator[](member1).HasMember(member2))
		{
			data = doc_->operator[](member1)[member2];
			return data;
		}
		LogError("ERROR! FOUND MEMBER {} BUT MISSING MEMBER {}!", member1, member2);
		return data;
	}
	LogError("ERROR! JSON MISSING MEMBER {}!", member1);
	return data;
}

Value JSON::readJSONJVal(const char* member1, const char* member2, const char* member3)
{
	Value data;
	if (doc_->HasMember(member1))
	{
		if (doc_->operator[](member1).HasMember(member2))
		{
			if (doc_->operator[](member1)[member2].HasMember(member3))
			{
				data = doc_->operator[](member1)[member2][member3];
				return data;
			}
			LogError("ERROR! FOUND MEMBER {} AND {} BUT MISSING MEMBER {}!", member1, member2, member3);
			return data;
		}
		LogError("ERROR! FOUND MEMBER {} BUT MISSING MEMBER {}!", member1, member2);
		return data;
	}
	LogError("ERROR! JSON MISSING MEMBER {}!", member1);
	return data;
}

//read string Vector
std::vector<std::string> JSON::readJSON_StringVector(const char* member1)
{
	readJFile();
	std::vector<std::string> vec;
	Value data = readJSONJVal(member1);
	if (!data.IsNull())
	{
		if (!data.IsArray())
		{
			//LogError("DATA IN '{}' IS NOT AN ARRAY PUSHING VALUE OF OBJECT INTO A VECTOR!", member1);
			//LogError("USE 'readJSONValue' INSTEAD OF 'readJSON_Vector'");
			vec.push_back(data.GetString());
			return vec;
		}
		for (unsigned int i = 0; i < data.Size(); i++)
		{
			vec.push_back(data[i].GetString());
		}
		printDebugDataArray(member1, vec);
		return vec;
	}
	LogError("ERROR WITH {}! EITHER NOT FOUND OR ERROR INSIDE JSON FILE!", member1);
	return vec;
}

std::vector<std::string> JSON::readJSON_StringVector(const char* member1, const char* member2)
{
	readJFile();
	std::vector<std::string> vec;
	Value data = readJSONJVal(member1, member2);
	if (!data.IsNull())
	{
		if (!data.IsArray())
		{
			LogError("DATA IN '{}' IS NOT AN ARRAY PUSHING VALUE OF OBJECT INTO A VECTOR!", member2);
			LogError("USE 'readJSONValue' INSTEAD OF 'readJSON_Vector'");
			vec.push_back(data.GetString());
			return vec;
		}
		for (unsigned int i = 0; i < data.Size(); i++)
		{
			vec.push_back(data[i].GetString());
		}
		printDebugDataArray(member2, vec);
		return vec;
	}
	LogError("ERROR WITH {} {}! EITHER NOT FOUND OR ERROR INSIDE JSON FILE!", member1, member2);
	return vec;
}


//HasMember
bool JSON::hasMember(const char* member1)
{
	readJFile();
	bool val = doc_->HasMember(member1);
	if (val == false)
	{
		LogWarning("WARNING! could not find the member {}", member1);
	}
	return val;
}

bool JSON::hasMember(const char* member1, const char* member2)
{
	readJFile();
	if (doc_->HasMember(member1))
	{
		bool val = doc_->operator[](member1).HasMember(member2);
		if (val == false)
		{
			LogWarning("WARNING! could not find the member {}", member2);
		}
		return val;
	}
	LogError("ERROR could not find the first member {}", member1);
	return false;
}

bool JSON::hasMember(const char* member1, const char* member2, const char* member3)
{
	readJFile();
	if (doc_->HasMember(member1))
	{
		if (doc_->operator[](member1).HasMember(member2))
		{
			bool val = doc_->operator[](member1)[member2].HasMember(member3);
			if (val == false)
			{
				LogWarning("WARNING! could not find the member {}", member3);
			}
			return val;
		}
		LogError("ERROR Found {} but could not find the member {}", member1, member2);
		return false;
	}
	LogError("ERROR could not find the first member {}", member1);
	return false;
}

//write/read File
void JSON::writeFile()
{
	FILE* outfile;
	fopen_s(&outfile, jsonPath_.c_str(), "w");
	if (!outfile)
	{
		LogWarning("(write)Couldn't open file {}! If everything is still working then can ignore!", jsonPath_.c_str());
		return;
	}
	char writeBuffer[65536];
	FileWriteStream os(outfile, writeBuffer, sizeof(writeBuffer));
	Writer<FileWriteStream> writer(os);
	doc_->Accept(writer);
	fclose(outfile);
}

void JSON::readJFile()
{
	FILE* fp = NULL;
	fopen_s(&fp, jsonPath_.c_str(), "r"); // non-Windows use "r"
	if (!fp)
	{
		LogWarning("(read)Couldn't open file {}! If everything is still working then can ignore!", jsonPath_.c_str());
		return;
	}
	char readBuffer[65536];
	FileReadStream is(fp, readBuffer, sizeof(readBuffer));
	doc_->ParseStream(is);  // Parses UTF-16LE file into UTF-8 in memory
	fclose(fp);
}

//extraDebug
bool JSON::toggleExtraDebug()
{
	enableDEBUG_ = !enableDEBUG_;
	if (enableDEBUG_ == true)
	{
		LogTrace("Enabled Extra Debug for JSON files!")
	}
	else
	{
		LogTrace("Disabled Extra Debug for JSON files!")
	}
	return enableDEBUG_;
}
