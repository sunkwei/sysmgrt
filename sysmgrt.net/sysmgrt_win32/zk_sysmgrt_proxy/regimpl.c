#include <stdio.h>
#include <stdlib.h>
#include "zk_sysmgrt_proxy.h"
#include "impl.h"

int zksysmgrt_r_reghost(zksysmgrt_t *sm, const char *name, const char *showname,
						const char **ips, int cnt)
{
	return -1;
}

int zksysmgrt_r_unreghost(zksysmgrt_t *sm, const char *name)
{
	// TODO:
	return -1;
}

int zksysmgrt_r_regservice(zksysmgrt_t *sm, const char *name, 
						   const char *hostname, const char *type,
						   const char *showname,
						   const char **urls, int cnt,
						   const char *version)
{
	// TODO:
	return -1;
}

int zksysmgrt_r_unregservice(zksysmgrt_t *sm, const char *name)
{
	// TODO:
	return -1;
}

int zksysmgrt_r_addlogic(zksysmgrt_t *sm, const char *name, const char *showname)
{
	// TODO:
	return -1;
}

int zksysmgrt_r_delobj(zksysmgrt_t *sm, const char *name)
{
	// TODO:
	return -1;
}

int zksysmgrt_r_bind(zksysmgrt_t *sm, const char *name, const char *sname)
{
	// TODO:
	return -1;
}

int zksysmgrt_r_unbind(zksysmgrt_t *sm, const char *name, const char *sname)
{
	// TODO:
	return -1;
}

int zksysmgrt_r_setshowname(zksysmgrt_t *sm, const char *name, const char *showname)
{
	// TODO:
	return -1;
}
