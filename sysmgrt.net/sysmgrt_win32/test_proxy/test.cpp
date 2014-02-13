#include <stdio.h>
#include "../zk_sysmgrt_proxy/zk_sysmgrt_proxy.h"
#include <Windows.h>

int main()
{
	zksysmgrt_t *sm = zksysmgrt_open("http://localhost:8899");

	zksysmgrt_Service *services = 0;
	int scnt = 0;

	int rc = zksysmgrt_q_getallservices(sm, 1, &services, &scnt);
	zksysmgrt_q_freeservices(sm, services, scnt);

	char **url = (char**)malloc(sizeof(char*));
	url[0] = strdup("192.168.1.10");

	char name[64];

	for (int i = 0; i < 100; i++) {
		sprintf(name, "test_service_id_%d", i);
		rc = zksysmgrt_r_regservice(sm, name, "", "test_service", "", (const char**)url, 1, "0.0.9");
	}

	for (int i = 0; i < 1000000; i++) {
		rc = zksysmgrt_q_getallservices(sm, 1, &services, &scnt);
		if (rc != 0) {
			fprintf(stderr, "ERR£ºcode=%d\n", rc);
			exit(-1);
		}
		else {
			zksysmgrt_q_freeservices(sm, services, scnt);

			fprintf(stderr, "%d: get %d services\n", i, scnt);

			Sleep(100);
		}
	}

	return 0;
}
