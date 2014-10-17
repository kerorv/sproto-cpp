#include <iostream>
#include <fstream>
#include "cppsproto.h"
#include "addressbookmessage.h"

bool LoadPbfile(const char* filename, std::string& pb)
{
	std::ifstream ifs(filename, std::ifstream::binary);
	if (!ifs)
		return false;

	ifs.seekg(0, ifs.end);
	int length = ifs.tellg();
	ifs.seekg(0, ifs.beg);
	pb.resize(length, ' ');

	char* begin = &*pb.begin();
	ifs.read(begin, length);
	ifs.close();

	return true;
}

bool SaveMsgfile(const char* filename, const char* msg, size_t len)
{
	std::ofstream ofs(filename, std::ofstream::binary);
	if (!ofs)
		return false;

	ofs.write(msg, len);
	ofs.close();
	return true;
}

int main(int argc, char* argv[])
{
	std::string pb;
	if (!LoadPbfile("addressbook.pb", pb))
	{
		std::cout << "open addressbook.pb failed" << std::endl;
		return -1;
	}

	AddressBookMessage abm;

	PersonMessage* jack = new PersonMessage;
	jack->SetId(100);
	jack->SetName("Jack");
	jack->SetEmail("jack@cartoon.com");
	PhoneNumberMessage* phonenumber1 = new PhoneNumberMessage;
	phonenumber1->SetNumber("001-123456");
	phonenumber1->SetType(1);	// telephone
	jack->AddPhoneNumber(phonenumber1);
	PhoneNumberMessage* phonenumber2 = new PhoneNumberMessage;
	phonenumber2->SetNumber("139xxxxxxxx");
	phonenumber2->SetType(2);	// mobile phone
	jack->AddPhoneNumber(phonenumber2);
	abm.AddPerson(jack);

	PersonMessage* tom = new PersonMessage;
	tom->SetId(101);
	tom->SetName("Tom");
	tom->SetEmail("tom@cartoon.com");
	abm.AddPerson(tom);
	std::cout << "abm1 dump:" << std::endl;
	abm.Dump();

	static char buffer[1024];
	int size = sizeof(buffer);
	CppSproto sp;
	if (!sp.Init(pb.data(), pb.length()))
	{
		std::cout << "create CppSproto fail" << std::endl;
		return -1;
	}

	if (!sp.Encode(&abm, buffer, size))
	{
		std::cout << "Encode fail" << std::endl;
		if (size == -1)
		{
			std::cout << "buffer size is too small" << std::endl;
		}
		return -1;
	}

	// SaveMsgfile("addressbook2.msg", sp.GetEncodedBuffer(), len);
	std::cout << "Encode ok.(" << size << " bytes)" << std::endl;

	AddressBookMessage abm2;
	if (!sp.Decode(&abm2, buffer, size))
	{
		std::cout << "Decode fail" << std::endl;
		return -1;
	}

	std::cout << "Decode ok." << std::endl;

	std::cout << "abm2 dump:" << std::endl;
	abm2.Dump();
}

