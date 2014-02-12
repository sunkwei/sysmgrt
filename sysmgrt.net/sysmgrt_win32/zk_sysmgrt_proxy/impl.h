#pragma once

#include "zk_sysmgrt_common.h"
#include "../../../soapH.h"
#include "list.h"
#include "thread.h"
#include "mutex.h"

struct zksysmgrt_t
{
	struct soap soap;
	char *url;

	list_head tokens;	// ��Ҫ���������� token
	mutex_t *lock;		// tokens ������
	thread_t *th;		// �����߳�
};

typedef struct token_string
{
	list_head list;
	const char *str;
} token_string;

#ifdef WIN32
#	define strdup _strdup
#	define snprintf _snprintf
#endif //
