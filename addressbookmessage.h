#pragma once

#include <string>
#include <vector>
#include "sprotomessage.h"

class PhoneNumberMessage : public SprotoMessage
{
public:
	PhoneNumberMessage();
	virtual ~PhoneNumberMessage();

	virtual std::string GetMessageName();
	virtual bool GetIntegerField(const char* name, int index, int64_t& value);
	virtual bool SetIntegerField(const char* name, int index, int64_t value);
	virtual const char* GetStringField(const char* name, int index, int& len);
	virtual bool SetStringField(const char* name, int index, 
			const char* value, int len);

	void SetNumber(const std::string& number) { number_ = number; }
	void SetType(int type) { type_ = type; }
	void Dump();

private:
	std::string number_;
	int type_;
};

class PersonMessage : public SprotoMessage
{
public:
	PersonMessage();
	virtual ~PersonMessage();

	virtual std::string GetMessageName();
	virtual bool GetIntegerField(const char* name, int index, int64_t& value);
	virtual bool SetIntegerField(const char* name, int index, int64_t value);
	virtual const char* GetStringField(const char* name, int index, int& len);
	virtual bool SetStringField(const char* name, int index, 
			const char* value, int len);
	virtual SprotoMessage* GetStructField(const char* name, int index);
	virtual SprotoMessage* SetStructField(const char* name, int index);

	void SetName(const std::string& name) { name_ = name; }
	void SetId(int id) { id_ = id; }
	void SetEmail(const std::string& email) { email_ = email; }
	void AddPhoneNumber(PhoneNumberMessage* phonenumber) { 
		phonenumbers_.push_back(phonenumber); 
	}
	void Dump();

private:
	std::string name_;
	int id_;
	std::string email_;
	typedef std::vector<PhoneNumberMessage*> PhoneNumberVector;
	PhoneNumberVector phonenumbers_;
};

class AddressBookMessage : public SprotoMessage
{
public:
	AddressBookMessage();
	virtual ~AddressBookMessage();

	virtual std::string GetMessageName();
	virtual SprotoMessage* GetStructField(const char* name, int index);
	virtual SprotoMessage* SetStructField(const char* name, int index);

	void AddPerson(PersonMessage* person) { persons_.push_back(person); }
	void Dump();

private:
	typedef std::vector<PersonMessage*> PersonVector;
	PersonVector persons_;
};

