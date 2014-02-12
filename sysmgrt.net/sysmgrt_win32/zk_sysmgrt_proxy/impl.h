#pragma once

#include "zk_sysmgrt_common.h"
#include "../../../soapH.h"

struct zksysmgrt_t
{
	struct soap soap;
	char *url;
};

#ifdef WIN32
#	define strdup _strdup
#	define snprintf _snprintf
#endif //
