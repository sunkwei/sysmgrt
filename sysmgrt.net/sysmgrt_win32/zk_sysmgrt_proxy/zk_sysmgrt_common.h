#pragma once

#ifdef __cplusplus
extern "C" {
#endif

typedef struct zksysmgrt_t zksysmgrt_t;

/** url Ϊ�ṩ��������
 */
zksysmgrt_t *zksysmgrt_open(const char *url);
void zksysmgrt_close(zksysmgrt_t *sm);

#ifdef __cplusplus
}
#endif
