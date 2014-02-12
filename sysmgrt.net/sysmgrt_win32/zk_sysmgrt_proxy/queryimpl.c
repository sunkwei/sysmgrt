#include <stdio.h>
#include <stdlib.h>
#include "zk_sysmgrt_proxy.h"
#include "impl.h"

static zksysmgrt_ss *ut_clone_ss(const char **ptr, int size)
{
	zksysmgrt_ss *ss = (zksysmgrt_ss*)malloc(sizeof(zksysmgrt_ss));
	ss->cnt = size;
	if (ss->cnt == 0) {
		ss->ptr = 0;
	}
	else {
		int i;
		ss->ptr = (char**)malloc(ss->cnt * sizeof(char*));
		for (i = 0; i < ss->cnt; i++) {
			ss->ptr[i] = strdup(ptr[i]);
		}
	}

	return ss;
}

int zksysmgrt_q_getallhosts(zksysmgrt_t *sm, int offline,
							zksysmgrt_Host **hosts, int *cnt)
{
	int rc, i;
	struct zkreg__Hosts hs;
	hs.__ptr = 0;
	hs.__size = 0;
	
	rc = soap_call_zkq__getAllHosts(&sm->soap, sm->url, 0, offline ? xsd__boolean__true_ : xsd__boolean__false_, &hs);

	if (rc != SOAP_OK) {
		return rc;
	}
	else {
		if (hs.__size == 0) {
			// 没有获得 ..
			*cnt = 0;
			*hosts = 0;
		}
		else {
			zksysmgrt_Host *rhs = (zksysmgrt_Host*)malloc(hs.__size * sizeof(zksysmgrt_Host));

			for (i = 0; i < hs.__size; i++) {
				zksysmgrt_Host *h = &rhs[i];
				struct zkreg__Host *hh = &hs.__ptr[i];
				h->name = strdup(hh->name);
				h->showname = strdup(hh->showname);
				h->ips = ut_clone_ss(hh->ips->__ptr, hh->ips->__size);
			}

			*cnt = hs.__size;
			*hosts = rhs;
		}

		return 0;
	}
}

int zksysmgrt_q_getallservices(zksysmgrt_t *sm, int offline,
							   zksysmgrt_Service **services, int *cnt)
{
	return -1;
}

int zksysmgrt_q_getalllogics(zksysmgrt_t *sm, zksysmgrt_t **logics, int *cnt)
{
	return -1;
}

int zksysmgrt_q_getparents(zksysmgrt_t *sm, const char *name,
						   zksysmgrt_Mse **mses, int *cnt)
{
	return -1;
}

int zksysmgrt_q_getbrothers(zksysmgrt_t *sm, const char *name,
							zksysmgrt_Mse **brothers, int *cnt)
{
	return -1;
}

int zksysmgrt_q_gethost(zksysmgrt_t *sm, const char *name,
						zksysmgrt_Host **host)
{
	return -1;
}

int zksysmgrt_q_getservice(zksysmgrt_t *sm, const char *name, zksysmgrt_Service **service)
{
	return -1;
}

int zksysmgrt_q_getlogic(zksysmgrt_t *sm, const char *name, zksysmgrt_Logic **logic)
{
	return -1;
}

int zksysmgrt_q_getservicesbytype(zksysmgrt_t *sm, int offline, const char *type, 
								zksysmgrt_Service **services, int *cnt)
{
	return -1;
}

void zksysmgrt_q_freehosts(zksysmgrt_t *sm, zksysmgrt_Host *hosts, int cnt)
{
}

void zksysmgrt_q_freeservices(zksysmgrt_t *sm, zksysmgrt_Service *services, int cnt)
{
}

void zksysmgrt_q_freelogics(zksysmgrt_t *sm, zksysmgrt_Logic *logics, int cnt)
{
}

void zksysmgrt_q_freebrothers(zksysmgrt_t *sm, zksysmgrt_Brother *brothers, int cnt)
{
}

void zksysmgrt_q_freemses(zksysmgrt_t *sm, zksysmgrt_Mse *mses, int cnt)
{
}
