#pragma once

#include <stdint.h>
#include <string>

class SprotoMessage
{
public:
	virtual ~SprotoMessage() {}

	virtual std::string GetMessageName() = 0;
	virtual bool GetIntegerField(const char* name, int index, int64_t& value);
	virtual bool SetIntegerField(const char* name, int index, int64_t value);
	virtual bool GetBooleanField(const char* name, int index, bool& value);
	virtual bool SetBooleanField(const char* name, int index, bool value);
	virtual const char* GetStringField(const char* name, int index, int& len);
	virtual bool SetStringField(const char* name, int index, const char* value,
			int len);
	virtual SprotoMessage* GetStructField(const char* name, int index);
	virtual SprotoMessage* SetStructField(const char* name, int index);
};

