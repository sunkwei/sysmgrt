#include <stdio.h>
#include <stdlib.h>
#include "zk_sysmgrt_proxy.h"
#include "impl.h"

int zksysmgrt_r_reghost(zksysmgrt_t *sm, const char *name, const char *showname,
						const char **ips, int cnt)
{
	struct zkreg__Host host;
	char *token;
	int rc;
	struct soap soap;

	host.name = (char*)name;
	host.showname = (char*)showname;
	host.catalog = zkreg__Catalog__Host;
	host.parent = "";
	host.ips = (struct zkreg__Ips*)malloc(sizeof(struct zkreg__Ips));
	host.ips->__ptr = (char**)ips;
	host.ips->__size = cnt;

	soap_init(&soap);
	rc = soap_call_zkreg__regHost(&soap, sm->url, 0, &host, &token);

	free(host.ips);

	if (rc != SOAP_OK) {
		soap_end(&soap);

		return rc;
	}
	else {
		if (token) {
			save_token(sm, token, name);
		}

		soap_end(&soap);
		return 0;
	}
}

int zksysmgrt_r_unreghost(zksysmgrt_t *sm, const char *name)
{
	int rc, code;
	struct soap soap;
	soap_init(&soap);
	rc = soap_call_zkreg__unregHost(&soap, sm->url, 0, (char*)name, &code);
	soap_end(&soap);

	if (rc == SOAP_OK) {
		remove_token(sm, name);
	}

	return 0;
}

int zksysmgrt_r_regservice(zksysmgrt_t *sm, const char *name, 
						   const char *hostname, const char *type,
						   const char *showname,
						   const char **urls, int cnt,
						   const char *version)
{
	struct zkreg__Service service;
	char *token;
	int rc;
	struct soap soap;

	service.name = (char*)name;
	service.hostname = (char*)hostname;
	service.catalog = zkreg__Catalog__Service;
	service.parent = "";
	service.showname = (char*)showname;
	service.type = (char*)type;
	service.version = (char*)version;
	service.urls = (struct zkreg__Urls*)malloc(sizeof(struct zkreg__Urls));
	service.urls->__ptr = (char**)urls;
	service.urls->__size = cnt;

	soap_init(&soap);
	rc = soap_call_zkreg__regService(&soap, sm->url, 0, &service, &token);

	free(service.urls);

	if (rc != SOAP_OK) {
		soap_end(&soap);
		return rc;
	}
	else {
		if (token)
			save_token(sm, token, name);
	
		soap_end(&soap);
		return 0;
	}
}

int zksysmgrt_r_unregservice(zksysmgrt_t *sm, const char *name)
{
	int rc, code;
	struct soap soap;

	soap_init(&soap);
	rc = soap_call_zkreg__unregService(&soap, sm->url, 0, (char*)name, &code);

	if (rc == SOAP_OK) {
		remove_token(sm, name);
	}
	soap_end(&soap);

	return 0;
}

int zksysmgrt_r_addlogic(zksysmgrt_t *sm, const char *name, const char *showname)
{
	int rc;
	struct zkreg__Logic logic;
	char *token;
	struct soap soap;

	logic.catalog = zkreg__Catalog__Logic;
	logic.name = (char*)name;
	logic.childnum = 0;
	logic.children = 0;
	logic.showname = (char*)showname;
	logic.parent = "";
	
	soap_init(&soap);
	rc = soap_call_zkreg__regLogic(&soap, sm->url, 0, &logic, &token);
	soap_end(&soap);
	if (rc != SOAP_OK)
		return rc;
	else {
		return 0;
	}
}

int zksysmgrt_r_delobj(zksysmgrt_t *sm, const char *name)
{
	int rc, code;
	struct soap soap;
	
	soap_init(&soap);
	rc = soap_call_zkreg__delMse(&soap, sm->url, 0, (char*)name, &code);
	soap_end(&soap);
	
	return code;
}

int zksysmgrt_r_bind(zksysmgrt_t *sm, const char *name, const char *sname)
{
	int rc, code;
	struct soap soap;
	
	soap_init(&soap);
	rc = soap_call_zkreg__bind(&soap, sm->url, 0, (char*)name, (char*)sname, &code);
	soap_end(&soap);

	return code;
}

int zksysmgrt_r_unbind(zksysmgrt_t *sm, const char *name, const char *sname)
{
	int rc, code;
	struct soap soap;

	soap_init(&soap);
	rc = soap_call_zkreg__unbind(&soap, sm->url, 0, (char*)name, (char*)sname, &code);
	soap_end(&soap);

	return code;
}

int zksysmgrt_r_setshowname(zksysmgrt_t *sm, const char *name, const char *showname)
{
	int rc, code;
	struct soap soap;

	soap_init(&soap);
	rc = soap_call_zkreg__setShowName(&soap, sm->url, 0, (char*)name, (char*)showname, &code);
	soap_end(&soap);

	return code;
}
