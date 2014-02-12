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

	list_head tokens;	// 需要“心跳”的 token
	mutex_t *lock;		// tokens 队列锁
	thread_t *th;		// 心跳线程
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
