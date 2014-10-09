#include <iostream>
#include <string.h>
#include "addressbookmessage.h"

// PhoneNumberMessage
PhoneNumberMessage::PhoneNumberMessage()
	: type_(0)
{
}

PhoneNumberMessage::~PhoneNumberMessage()
{
}

std::string PhoneNumberMessage::GetMessageName()
{
	static const char* PhoneNumberMessageName = "PhoneNumber";
	return PhoneNumberMessageName;
}

bool PhoneNumberMessage::GetIntegerField(const char* name, int index, 
		int64_t& value)
{
	if (strcmp(name, "type") == 0)
	{
		value = type_;
		return true;
	}
	else
	{
		return false;
	}
}

bool PhoneNumberMessage::SetIntegerField(const char* name, int index, 
		int64_t value)
{
	if (strcmp(name, "type") == 0)
	{
		type_ = (int)value;
		return true;
	}
	else
	{
		return false;
	}
}

const char* PhoneNumberMessage::GetStringField(const char* name, int index, int& len)
{
	if (strcmp(name, "number") == 0)
	{
		if (number_.empty())
		{
			len = 0;
			return NULL;
		}
		else
		{
			len = number_.length();
			return &number_[0];
		}
	}
	else
	{
		return NULL;
	}
}

bool PhoneNumberMessage::SetStringField(const char* name, int index, 
	const char* value, int len)
{
	if (strcmp(name, "number") == 0)
	{
		number_.assign(value, len);
		return true;
	}
	else
	{
		return false;
	}
}

void PhoneNumberMessage::Dump()
{
	std::cout << "\t\tnumber: " << number_ << std::endl;
	std::cout << "\t\ttype: ";
	switch (type_)
	{
		case 1:
			std::cout << "telephone" << std::endl;
			break;
		case 2:
			std::cout << "mobile phone" << std::endl;
			break;
		default:
			std::cout << "unknown" << std::endl;
			break;
	}
}

// PersonMessage
PersonMessage::PersonMessage()
	: id_(0)
{
}

PersonMessage::~PersonMessage()
{
	for (PhoneNumberVector::iterator it = phonenumbers_.begin();
			it != phonenumbers_.end(); ++it)
	{
		PhoneNumberMessage* phonenumber = *it;
		delete phonenumber;
	}
}

std::string PersonMessage::GetMessageName()
{
	static const char* PersonMessageName = "Person";
	return PersonMessageName;
}

bool PersonMessage::GetIntegerField(const char* name, int index, int64_t& value)
{
	if (strcmp(name, "id") == 0)
	{
		value = id_;
		return true;
	}
	else
	{
		return false;
	}
}

bool PersonMessage::SetIntegerField(const char* name, int index, 
		int64_t value)
{
	if (strcmp(name, "id") == 0)
	{
		id_ = (int)value;
		return true;
	}
	else
	{
		return false;
	}
}

const char* PersonMessage::GetStringField(const char* name, int index, int& len)
{
	if (strcmp(name, "name") == 0)
	{
		if (name_.empty())
		{
			len = 0;
			return NULL;
		}
		else
		{
			len = name_.length();
			return &name_[0];
		}
	}
	else if (strcmp(name, "email") == 0)
	{
		if (email_.empty())
		{
			len = 0;
			return NULL;
		}
		else
		{
			len = email_.length();
			return &email_[0];
		}
	}
	else
	{
		return NULL;
	}
}

bool PersonMessage::SetStringField(const char* name, int index, 
	const char* value, int len)
{
	if (strcmp(name, "name") == 0)
	{
		name_.assign(value, len);
		return true;
	}
	else if (strcmp(name, "email") == 0)
	{
		email_.assign(value, len);
		return true;
	}
	else
	{
		return false;
	}
}

SprotoMessage* PersonMessage::GetStructField(const char* name, 
		int index)
{
	if (strcmp(name, "phones") == 0)
	{
		if (index >=0 && index < (int)phonenumbers_.size())
			return phonenumbers_[index];
	}

	return NULL;
}

SprotoMessage* PersonMessage::SetStructField(const char* name, 
		int index)
{
	if (strcmp(name, "phones") == 0)
	{
		PhoneNumberMessage* msg = new PhoneNumberMessage;
		phonenumbers_.push_back(msg);
		return msg;
	}

	return NULL;
}

void PersonMessage::Dump()
{
	std::cout << "\tid: " << id_ << std::endl;
	std::cout << "\tname: " << name_ << std::endl;
	std::cout << "\temail: " << email_ << std::endl;

	for (PhoneNumberVector::iterator it = phonenumbers_.begin();
			it != phonenumbers_.end(); ++it)
	{
		PhoneNumberMessage* phonenumber = *it;
		phonenumber->Dump();
	}
}

// AddressBookMessage
AddressBookMessage::AddressBookMessage()
{
}

AddressBookMessage::~AddressBookMessage()
{
	for (PersonVector::iterator it = persons_.begin();
			it != persons_.end(); ++it)
	{
		PersonMessage* person = *it;
		delete person;
	}
}

std::string AddressBookMessage::GetMessageName()
{
	static const char* AddressBookMessageName = "AddressBook";
	return AddressBookMessageName;
}

SprotoMessage* AddressBookMessage::GetStructField(const char* name, 
		int index)
{
	if (strcmp(name, "persons") == 0) 
	{
		if (index >= 0 && index < (int)persons_.size())
			return persons_[index];
	}

	return NULL;
}

SprotoMessage* AddressBookMessage::SetStructField(const char* name, 
		int index)
{
	if (strcmp(name, "persons") == 0)
	{
		PersonMessage* msg = new PersonMessage;
		persons_.push_back(msg);
		return msg;
	}

	return NULL;
}

void AddressBookMessage::Dump()
{
	std::cout << "{" << std::endl;
	for (PersonVector::iterator it = persons_.begin();
			it != persons_.end(); ++it)
	{
		PersonMessage* person = *it;
		person->Dump();
	}
	std::cout << "}" << std::endl;
}

