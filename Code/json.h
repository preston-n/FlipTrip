#pragma once
#include <document.h>
#include <writer.h>
#include <stringbuffer.h>
#include <cstdio>
#include <filewritestream.h>
#include <filereadstream.h>
#include <istreamwrapper.h>
#include <rapidjson/ostreamwrapper.h>
#include <rapidjson/writer.h>
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include "Trace.h"

using namespace rapidjson;
using namespace std;

//Class to read and write JSON files using rapidJson lib
class JSON
{
public:
	//Ctor/Dtors
	JSON(const char* jsonNAME);
	JSON();
	~JSON();

	//Ops
	void operator()(const char* jsonNAME);

	void readJFile(); //Reads JsonFile
	Document& getDoc(); //Gets Document needed to read Json file
	const char* getJSONPath(); //Gets local Json path
	void setJSONPath(const char* nameOfJsonFile); //Sets local Json path

	//Used to check if JSON file has value before accessing it
	bool hasMember(const char* member1);
	bool hasMember(const char* member1, const char* member2);
	bool hasMember(const char* member1, const char* member2, const char* member3);
	
	//Read and return JsonValue of anytype (except std::string and std::vector)
	template <typename T>
	T readJSONValue(const char* member1);
	template <typename T>
	T readJSONValue(const char* member1, const char* member2);
	template <typename T>
	T readJSONValue(const char* member1, const char* member2, const char* member3);

	//Read and return JsonValue of std::string (only need seprate function for reading)
	std::string readJSONstring(const char* member1);
	std::string readJSONstring(const char* member1, const char* member2);
	std::string readJSONstring(const char* member1, const char* member2, const char* member3);

	//Read std::vector from JSON file if it exists,
	//otherwise return empty vector (only need seprate function for reading)
	template <typename T>
	std::vector<T> readJSON_Vector(const char* member1);
	template <typename T> 
	std::vector<T> readJSON_Vector(const char* member1, const char* member2);

	//Read std::vector of strings (only need seprate function for reading)
	std::vector<std::string> readJSON_StringVector(const char* member1);
	std::vector<std::string> readJSON_StringVector(const char* member1, const char* member2);

	//Write to JSON
	template <typename T>
	void changeJSONValue(const char* member1, T value);
	template <typename T>
	void changeJSONValue(const char* member1, const char* member2, T value);
	template <typename T>
	void changeJSONValue(const char* member1, const char* member2, const char* member3, T value);

	//Write to Array
	template <typename T>
	void changeJSONVector(const char* member1, std::vector<T> value);
	template <typename T>
	void changeJSONVector(const char* member1, const char* member2, std::vector<T> value);

	bool toggleExtraDebug(); //Used to toggle debug logs
private:
	Document* doc_; //Doc to read Json file
	std::string jsonPath_; //Json path
	bool enableDEBUG_; //used to enable/disable debug logging

	//Get specfic rapidJson value from file
	Value readJSONJVal(const char* member1);
	Value readJSONJVal(const char* member1, const char* member2);
	Value readJSONJVal(const char* member1, const char* member2, const char* member3);

	//Print Debug logs
	template <typename T>
	void printDebugData(const char* member1, T val);
	template <typename T>
	void printDebugDataArray(const char* member1, std::vector<T> val);
	template <typename T>
	void printDebugChangeData(const char* member1, T oldVal, T newVal);
	template <typename T>
	void printDebugDataChangeArray(const char* member1, std::vector<T> oldVal, std::vector<T> newVal);

	void writeFile(); //Save changes to Json file
};

#include "jsonTemp.cpp"