#include "sprotomessage.h"

bool SprotoMessage::GetIntegerField(const char* name, int index, int64_t& value)
{
	return false;
}

bool SprotoMessage::SetIntegerField(const char* name, int index, int64_t value)
{
	return false;
}

bool SprotoMessage::GetBooleanField(const char* name, int index, bool& value)
{
	return false;
}

bool SprotoMessage::SetBooleanField(const char* name, int index, bool value)
{
	return false;
}

const char* SprotoMessage::GetStringField(const char* name, int index, int& len)
{
	return NULL;
}

bool SprotoMessage::SetStringField(const char* name, int index, 
	const char* value, int len)
{
	return false;
}

SprotoMessage* SprotoMessage::GetStructField(const char* name, int index)
{
	return NULL;
}

SprotoMessage* SprotoMessage::SetStructField(const char* name, int index)
{
	return NULL;
}

