#pragma once

struct sproto;
class SprotoMessage;
class CppSproto
{
public:
	CppSproto();
	~CppSproto();

	bool Init(const char* pbfiles);
	bool Init(const char* proto_bin, size_t pbsize);
	bool Encode(SprotoMessage* msg, char* encbuf, int& size);
	bool Decode(SprotoMessage* msg, const char* decbuf, int size);
	int Pack(const char* src, int src_size, char* dest, int dest_size);
	int Unpack(const char* src, int src_size, char* dest, int dest_size);

private:
	sproto* sp_;
};

