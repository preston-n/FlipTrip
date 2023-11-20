#include "json.h"

//ReadJsonValue
template <typename T>
T JSON::readJSONValue(const char* member1)
{
	readJFile();
	Value data = readJSONJVal(member1);
	if (!data.IsNull())
	{
		printDebugData<T>(member1, data.Get<T>());
		return data.Get<T>();
	}
	LogError("ERROR WITH {}! EITHER NOT FOUND OR ERROR INSIDE JSON FILE!", member1);
	return NULL;
}

template <typename T>
T JSON::readJSONValue(const char* member1, const char* member2)
{
	readJFile();
	Value data = readJSONJVal(member1, member2);
	if (!data.IsNull())
	{
		printDebugData<T>(member2, data.Get<T>());
		return data.Get<T>();
	}
	LogError("ERROR WITH {}: {}! EITHER NOT FOUND OR ERROR INSIDE JSON FILE!", member1, member2);
	return NULL;
}

template <typename T>
T JSON::readJSONValue(const char* member1, const char* member2, const char* member3)
{
	readJFile();
	Value data = readJSONJVal(member1, member2, member3);
	if (!data.IsNull())
	{
		printDebugData<T>(member3, data.Get<T>());
		return data.Get<T>();
	}
	LogError("ERROR WITH {}: {}: {}! EITHER NOT FOUND OR ERROR INSIDE JSON FILE!", member1, member2, member3);
	return NULL;
}

//Read std::vector from JSON file if it exists, otherwise return empty vector
template <typename T>
std::vector<T> JSON::readJSON_Vector(const char* member1)
{

	readJFile();
	std::vector<T> vec;
	Value data = readJSONJVal(member1);
	if (!data.IsNull())
	{
		if (!data.IsArray())
		{
			LogError("DATA IN '{}' IS NOT AN ARRAY PUSHING VALUE OF OBJECT INTO A VECTOR!",member1);
			LogError("USE 'readJSONValue' INSTEAD OF 'readJSON_Vector'");
			vec.push_back(data.Get<T>());
			return vec;
		}
		for (int i = 0; i < data.Size(); i++)
		{
			vec.push_back(data[i].Get<T>());
		}
		printDebugDataArray(member1, vec);
		return vec;
	}
	LogError("ERROR WITH {}! EITHER NOT FOUND OR ERROR INSIDE JSON FILE!", member1);
	return vec;
}

template <typename T>
std::vector<T> JSON::readJSON_Vector(const char* member1, const char* member2)
{
	readJFile();
	std::vector<T> vec;
	Value data = readJSONJVal(member1, member2);
	if (!data.IsNull())
	{
		if (!data.IsArray())
		{
			LogError("DATA IN '{}' IS NOT AN ARRAY PUSHING VALUE OF OBJECT INTO A VECTOR!", member2);
			LogError("USE 'readJSONValue' INSTEAD OF 'readJSON_Vector'");
			vec.push_back(data.Get<T>());
			return vec;
		}
		for (unsigned int i = 0; i < data.Size(); i++)
		{
			vec.push_back(data[i].Get<T>());
		}
		printDebugDataArray(member2, vec);
		return vec;
	}
	LogError("ERROR WITH {} {}! EITHER NOT FOUND OR ERROR INSIDE JSON FILE!", member1, member2);
	return vec;
}

//WriteJsonValue
template <typename T>
void JSON::changeJSONValue(const char* member1, T value)
{
	readJFile();
	if (doc_->HasMember(member1))
	{
		T tempVal = doc_->operator[](member1).Get<T>();
		doc_->operator[](member1).Set(value);
		writeFile();
		printDebugChangeData<T>(member1, tempVal, doc_->operator[](member1).Get<T>());
		return;
	}
	LogError("ERROR WITH {}! COULD NOT FIND MEMBER!", member1);
}

template <typename T>
void JSON::changeJSONValue(const char* member1, const char* member2, T value)
{
	readJFile();
	if (doc_->HasMember(member1))
	{
		if (doc_->operator[](member1).HasMember(member2))
		{
			T tempVal = doc_->operator[](member1)[member2].Get<T>();
			doc_->operator[](member1)[member2].Set<T>(value);
			writeFile();
			printDebugChangeData<T>(member2, tempVal, doc_->operator[](member1)[member2].Get<T>());
			return;
		}
		LogError("ERROR WITH {} {}! COULD NOT FIND MEMBER {}!", member1, member2, member2);
		return;
	}
	LogError("ERROR WITH {}! COULD NOT FIND MEMBER!", member1);
}

template <typename T>
void JSON::changeJSONValue(const char* member1, const char* member2, const char* member3, T value)
{
	readJFile();
	if (doc_->HasMember(member1))
	{
		if (doc_->operator[](member1).HasMember(member2))
		{
			if (doc_->operator[](member1)[member2].HasMember(member3))
			{
				T tempVal = doc_->operator[](member1)[member2][member3].Get<T>();
				doc_->operator[](member1)[member2][member3].Set<T>(value);
				writeFile();
				printDebugChangeData<T>(member2, tempVal, doc_->operator[](member1)[member2][member3].Get<T>());
				return;
			}
			LogError("ERROR WITH {} {}! COULD NOT FIND MEMBER {}!", member1, member2, member2);
			return;
		}
		LogError("ERROR WITH {} {}! COULD NOT FIND MEMBER {}!", member1, member2, member2);
		return;
	}
	LogError("ERROR WITH {}! COULD NOT FIND MEMBER!", member1);
}

//Change std::vector from JSON file
template <typename T>
void JSON::changeJSONVector(const char* member1, std::vector<T> value)
{
	readJFile();
	if (doc_->HasMember(member1))
	{
		std::vector<T> tempAry = readJSON_Vector<T>(member1);
		doc_->operator[](member1).SetArray();
		doc_->operator[](member1).Clear();
		for (unsigned int i = 0; i < value.size(); ++i)
		{
			doc_->operator[](member1).PushBack(value[i], doc_->GetAllocator());
		}
		writeFile();
		printDebugDataChangeArray<T>(member1, tempAry, value);
		return;
	}
	LogError("ERROR WITH {}! COULD NOT FIND MEMBER!", member1);
}

template <typename T>
void JSON::changeJSONVector(const char* member1, const char* member2, std::vector<T> value)
{
	readJFile();
	if (doc_->HasMember(member1))
	{
		if (doc_->operator[](member1).HasMember(member2))
		{
			std::vector<T> tempAry = readJSON_Vector<T>(member1, member2);
			doc_->operator[](member1)[member2].SetArray();
			doc_->operator[](member1)[member2].Clear();
			for (unsigned int i = 0; i < value.size(); ++i)
			{
				doc_->operator[](member1)[member2].PushBack(value[i], doc_->GetAllocator());
			}
			writeFile();
			printDebugDataChangeArray<T>(member1, tempAry, value);
			return;
		}
		LogError("ERROR WITH {} {}! COULD NOT FIND MEMBER {}!", member1, member2, member2);
		return;
	}
	LogError("ERROR WITH {}! COULD NOT FIND MEMBER!", member1);
}

//DebugReadValues
template <typename T>
void JSON::printDebugData(const char* member1, T val)
{
	if (enableDEBUG_ == true)
	{
		LogInfo("'{}': CONTAINS VALUE OF: '{}'!", member1, val);
	}
}

template <typename T>
void JSON::printDebugDataArray(const char* member1, std::vector<T> val)
{
	if (enableDEBUG_ == true)
	{
		LogInfo("START OF ARRAY CONTAINED IN '{}':", member1);
		for (unsigned int i = 0; i < val.size(); i++)
		{
			LogInfo("[{}]", val[i]);
		}
		LogInfo("END OF ARRAY CONTAINED IN '{}':", member1);
	}
}


//DebugChangeValues
template <typename T>
void JSON::printDebugChangeData(const char* member1, T oldVal, T newVal)
{
	if (enableDEBUG_ == true)
	{
		LogInfo("'{}': USED TO HAVE VALUE: '{}'!", member1, oldVal);
		LogInfo("'{}': NOW HAS VALUE OF: '{}'!", member1, newVal);
	}
}

template <typename T>
void JSON::printDebugDataChangeArray(const char* member1, std::vector<T> oldVal, std::vector<T> newVal)
{
	if (enableDEBUG_ == true)
	{
		LogInfo("START OF OLD ARRAY CONTAINED IN '{}':", member1);
		for (unsigned int i = 0; i < oldVal.size(); i++)
		{
			LogInfo("[{}]", oldVal[i]);
		}
		LogInfo("END OF ARRAY OLD CONTAINED IN '{}':", member1);
		LogInfo("-----------------------------------------------------------------------------------");
		LogInfo("START OF NEW ARRAY CONTAINED IN '{}':", member1);
		for (unsigned int i = 0; i < newVal.size(); i++)
		{
			LogInfo("[{}]", newVal[i]);
		}
		LogInfo("END OF ARRAY NEW CONTAINED IN '{}':", member1);
	}
}
