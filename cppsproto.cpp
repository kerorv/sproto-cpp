#include <string.h>
#include <stdexcept>
extern "C" {
#include "sproto.h"
}
#include "sprotomessage.h"
#include "cppsproto.h"

#define MAX_DEEPLEVEL			64	

struct EncodeParam
{
	SprotoMessage* msg;
	int deeplevel;
};

struct DecodeParam
{
	SprotoMessage* msg;
	int deeplevel;
};

static int EncodeCallback(void *ud, const char *tagname, int type, 
		int index, struct sproto_type *st, void *value, int length)
{
	EncodeParam* ep = (EncodeParam*)ud;

	if (ep->deeplevel > MAX_DEEPLEVEL)
		throw std::runtime_error("max deep level");

	if (index > 0)
		index -= 1;
	else
		index = -1;

	switch (type)
	{
	case SPROTO_TINTEGER:
		{
			int64_t field_value = 0;
			if (!ep->msg->GetIntegerField(tagname, index, field_value))
				return 0;

			if (field_value > INT32_MAX || field_value < INT32_MIN)
			{
				*(int64_t*)value = field_value;
				return 8;
			}
			else
			{
				*(int32_t*)value = field_value;
				return 4;
			}
		}
		break;
	case SPROTO_TBOOLEAN:
		{
			bool field_value = false;
			if (!ep->msg->GetBooleanField(tagname, index, field_value))
				return 0;

			*(int32_t*)value = field_value?1:0;
			return 4;
		}
		break;
	case SPROTO_TSTRING:
		{
			int field_length = 0;
			const char* field_value = ep->msg->GetStringField(tagname, index, 
					field_length);
			if (field_value == NULL)
				return 0;

			if (field_length > length)
				return -1;

			memcpy(value, field_value, field_length);
			return field_length;
		}
		break;
	case SPROTO_TSTRUCT:
		{
			SprotoMessage* submsg = ep->msg->GetStructField(tagname, index);
			if (submsg == NULL)
				return 0;

			EncodeParam subep;
			subep.msg = submsg;
			subep.deeplevel = ep->deeplevel + 1;
			return sproto_encode(st, value, length, EncodeCallback, &subep);
		}
		break;
	default:
		throw std::runtime_error("error tag type");
		break;
	}

	return 0;
}

static int DecodeCallback(void *ud, const char *tagname, int type, 
		int index, struct sproto_type *st, void *value, int length)
{
	DecodeParam* dp = (DecodeParam*)ud;
	if (dp->deeplevel > MAX_DEEPLEVEL)
		throw std::runtime_error("max deep level");

	if (index > 0)
		index -= 1;
	else
		index = -1;

	switch (type)
	{
	case SPROTO_TINTEGER:
		{
			int64_t real_value = 0;
			if (length == 4)
				real_value = *(int32_t*)value;
			else if (length == 8)
				real_value = *(int64_t*)value;

			dp->msg->SetIntegerField(tagname, index, real_value);
		}
		break;
	case SPROTO_TBOOLEAN:
		{
			int32_t real_value = *(int32_t*)value;
			dp->msg->SetBooleanField(tagname, index, real_value==1);
		}
		break;
	case SPROTO_TSTRING:
		{
			dp->msg->SetStringField(tagname, index, (char*)value, length);
		}
		break;
	case SPROTO_TSTRUCT:
		{
			SprotoMessage* submsg = dp->msg->SetStructField(tagname, index);
			if (submsg == NULL)
				return 0;

			DecodeParam subdp;
			subdp.msg = submsg;
			subdp.deeplevel = dp->deeplevel + 1;
			int ret = sproto_decode(st, value, length, DecodeCallback, &subdp);
			if (ret < 0 || ret != length)
				return ret;
		}
		break;
	default:
		throw std::runtime_error("error tag type");
		break;
	}

	return 0;
}

CppSproto::CppSproto()
	: sp_(NULL)
{
}

CppSproto::~CppSproto()
{
	if (sp_)
	{
		sproto_release(sp_);
		sp_ = NULL;
	}
}

bool CppSproto::Init(const char* pbfiles)
{
	// TODO
	return false;
}

bool CppSproto::Init(const char* proto_bin, size_t pbsize)
{
	sp_ = sproto_create(proto_bin, pbsize);
	if (sp_ == NULL)
		return false;

	return true;
}

bool CppSproto::Encode(SprotoMessage* msg, char* encbuf, int& size)
{
	struct sproto_type* st = sproto_type(sp_, msg->GetMessageName().c_str());
	if (st == NULL)
		return false;

	try
	{
		EncodeParam ep;
		ep.msg = msg;
		ep.deeplevel = 0;

		int ret = sproto_encode(st, encbuf, size, EncodeCallback, &ep);
		if (ret == -1)
		{
			size = -1;
			return false;
		}
		else
		{
			size = ret;
			return true;
		}
	}
	catch (std::runtime_error e)
	{
		// TODO
		e.what();
		return false;
	}
	catch (...)
	{
		return false;
	}
}

bool CppSproto::Decode(SprotoMessage* msg, const char* decbuf, int size)
{
	struct sproto_type* st = sproto_type(sp_, msg->GetMessageName().c_str());
	if (st == NULL)
		return false;

	DecodeParam dp;
	dp.msg = msg;
	dp.deeplevel = 0;
	try
	{
		int ret = sproto_decode(st, decbuf, (int)size, DecodeCallback, &dp);
		return (ret >= 0);
	}
	catch (std::runtime_error e)
	{
		// TODO
		e.what();
		return false;
	}
	catch (...)
	{
		return false;
	}
}

int CppSproto::Pack(const char* src, int src_size, char* dest, int dest_size)
{
	if (dest == NULL)
	{
		// from lsproto.c
		// the worst-case space overhead of packing is 2 bytes per 2 KiB of input (256 words = 2KiB).
		size_t maxsz = (src_size + 2047) / 2048 * 2 + src_size;
		return (int)maxsz;
	}
	else
	{
		return sproto_pack(src, src_size, dest, dest_size);
	}
}

int CppSproto::Unpack(const char* src, int src_size, char* dest, int dest_size)
{
	return sproto_unpack(src, src_size, dest, dest_size);
}

