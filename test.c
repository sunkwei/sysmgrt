/** 多线程测试 sysmgrt 的稳定性 */

#include <stdio.h>
#include <pthread.h>
#include "zkreg.nsmap"

static void *proc_regHost(void *param)
{
	// 模拟实现注册 1000 个主机
	int id = (int)param;
	id *= 1000;

	char name[64];
	for (int i = id; i < id+1000; i++) {
		struct soap soap;
		soap_init(&soap);

		struct zkreg__Host host;

		snprintf(name, sizeof(name), "test_host_%d", i);
		host.name = soap_strdup(&soap, name);
		host.catalog = zkreg__Catalog__Host;
		host.ips = (struct zkreg__Ips*)soap_malloc(&soap, sizeof(struct zkreg__Ips));
		host.ips->__ptr = 0;
		host.ips->__size = 0;
		host.showname = 0;
		
		char *token = 0;

		int rc = soap_call_zkreg__regHost(&soap, "http://localhost:8899", 0, &host, &token);

		fprintf(stderr, "%d: reghost ret %d\n", id/1000, rc);

		soap_done(&soap);
	}

	return 0;
}

static int start_threads(pthread_t th[], int n)
{
	for (int i = 0; i < n; i++) {
		pthread_create(&th[i], 0, proc_regHost, i);
	}
	return n;
}

static void join_threads(pthread_t ths[], int n)
{
	for (int i = 0; i < n; i ++) {
		pthread_join(ths[i], 0);
	}
}

int main()
{
	pthread_t ths[8];
	int n = start_threads(ths, 8);
	join_threads(ths, 8);

	return 0;
}

