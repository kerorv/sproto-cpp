#pragma once

#include "sprotomessage.h"

#define INIT_ENCBUFFER_SIZE		0x1000		// 4K
#define MAX_ENCBUFFER_SIZE		0x100000	// 1M

struct sproto;
class Sproto
{
public:
	Sproto(const char* proto_bin, size_t pbsize, 
			size_t init_encbuf_size = INIT_ENCBUFFER_SIZE);
	~Sproto();

	int Encode(SprotoMessage* msg);
	const char* GetEncodedBuffer() const { return encbuf_; }
	bool Decode(SprotoMessage* msg, const char* buffer, size_t size);

private:
	bool ResizeBuffer();

private:
	sproto* sp_;
	char* encbuf_;
	size_t encbuf_size_;
};

