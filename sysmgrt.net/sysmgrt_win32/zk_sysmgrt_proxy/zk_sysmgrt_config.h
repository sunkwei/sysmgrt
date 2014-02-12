#pragma once

#include "zk_sysmgrt_common.h"

#ifdef __cplusplus
extern "C" {
#endif // c++

int zksysmgrt_c_getallkeys(zksysmgrt_t *sm, const char **keys, int *num);
int zksysmgrt_c_getvalue(zksysmgrt_t *sm, const char *key, const char **value);
int zksysmgrt_c_setvalue(zksysmgrt_t *sm, const char *key, const char *value);

#ifdef __cplusplus
}
#endif //
