#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include "impl.h"

zksysmgrt_t *zksysmgrt_open(const char *url)
{
	zksysmgrt_t *sm = (zksysmgrt_t *)malloc(sizeof(zksysmgrt_t*));
	sm->url = strdup(url);

	soap_init(&sm->soap);

	return sm;
}

void zksysmgrt_close(zksysmgrt_t *sm)
{
	soap_done(&sm->soap);
	free(sm->url);
	free(sm);
}
