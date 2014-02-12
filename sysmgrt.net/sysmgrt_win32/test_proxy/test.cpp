#include <stdio.h>
#include "../zk_sysmgrt_proxy/zk_sysmgrt_proxy.h"

int main()
{
	zksysmgrt_t *sm = zksysmgrt_open("http://localhost:8899");

	zksysmgrt_Service *services = 0;
	int scnt = 0;

	int rc = zksysmgrt_q_getallservices(sm, 1, &services, &scnt);
	zksysmgrt_q_freeservices(sm, services, scnt);

	return 0;
}
