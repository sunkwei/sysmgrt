//
//  zkqimpl.c
//  zonekey_sysmgrt
//
//  Created by sunkw on 14-1-24.
//  Copyright (c) 2014年 sunkw. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include "soapStub.h"
#include "dbhlp.h"
#include "dboper.h"

int zkq__getAllMses(struct soap *soap, enum xsd__boolean offline, struct zkreg__Mses *mses)
{
    /** 返回 mse table 的内容 */
	char *st = (char*)alloca(1024);
	struct dbhlpColumn desc[] = {
		{ { 0 }, DBT_STRING },	// name
		{ { 0 }, DBT_STRING },	// parent
		{ { 0 }, DBT_INT },		// catalog
		{ { 0 }, DBT_STRING },	// showname
	};
	struct dbhlpColumn **all = 0, **all_logic = 0;
	int rows = 0, row_logic = 0, i;

	if (offline) {
		snprintf(st, 1024, "SELECT name,parent,catalog,showname FROM mse");
	}
	else {
		// 这次仅仅返回 mse
		snprintf(st, 1024, "SELECT mse.name,mse.parent,mse.catalog,mse.showname FROM mse JOIN token ON mse.name=token.name WHERE catalog!=%d", 
			zkreg__Catalog__Logic);
	}

	db_exec_select2(_db, st, desc, 3, &all, &rows);

	if (!offline) {
		// 需要合并 logic
		snprintf(st, 1024, "SELECT name,parent,catalog,showname FROM mse WHERE catalog=%d", zkreg__Catalog__Logic);
		db_exec_select2(_db, st, desc, 3, &all_logic, &row_logic);
	}

	mses->__size = rows + row_logic;
	mses->__ptr = (struct zkreg__Mse*)soap_malloc(soap, mses->__size * sizeof(struct zkreg__Mse));

	for (i = 0; i < rows; i++) {
		mses->__ptr[i].name = soap_strdup(soap, all[i][0].data.s);
		mses->__ptr[i].parent = soap_strdup(soap, all[i][1].data.s);
		mses->__ptr[i].catalog = (enum zkreg__Catalog)all[i][2].data.i;
		mses->__ptr[i].showname = soap_strdup(soap, all[i][3].data.s);
	}

	for (i = 0; i < row_logic; i++) {
		mses->__ptr[i+rows].name = soap_strdup(soap, all_logic[i][0].data.s);
		mses->__ptr[i+rows].parent = soap_strdup(soap, all_logic[i][1].data.s);
		mses->__ptr[i+rows].catalog = (enum zkreg__Catalog)all_logic[i][2].data.i;
		mses->__ptr[i+rows].showname = soap_strdup(soap, all_logic[i][3].data.s);
	}

	db_free_select2(desc, 3, all, rows);
	db_free_select2(desc, 3, all_logic, row_logic);

	return SOAP_OK;
}

int zkq__getMsesByShowname(struct soap *soap, enum xsd__boolean offline, char *showname, struct zkreg__Mses *mses)
{
	char *sql = (char*)alloca(1024);


	return SOAP_OK;
}

int zkq__getAllHosts(struct soap *soap, enum xsd__boolean offline, struct zkreg__Hosts *hosts)
{
	char *st = (char*)alloca(1024);

	struct dbhlpColumn desc[3] = {
		{ { 0 }, DBT_STRING },	// host.name
		{ { 0 }, DBT_STRING },	// host.ips
		{ { 0 }, DBT_STRING },	// mse.showname
	};

	struct dbhlpColumn **all = 0;
	int rows = 0, i, j;

    /** 根据 offline，从 host, mse, token 中返回查询记录，从将每行信息分配保存到 zkreg__Host 结构中.
     */
    if (offline) {
        snprintf(st, 1024, "SELECT host.name,host.ips,mse.showname FROM host"
                 " JOIN mse ON mse.name=host.name");
    }
    else {
        snprintf(st, 1024, "SELECT host.name,host.ips,mse.showname FROM host"
                 " JOIN mse ON mse.name=host.name"
                 " JOIN token ON token.name=host.name");
    }

	db_exec_select2(_db, st, desc, 3, &all, &rows);

	hosts->__size = rows;
	hosts->__ptr = (struct zkreg__Host*)soap_malloc(soap, sizeof(struct zkreg__Host) * rows);

	for (i = 0; i < rows; i++) {
		hosts->__ptr[i].name = soap_strdup(soap, all[i][0].data.s);
		hosts->__ptr[i].catalog = zkreg__Catalog__Host;
		hosts->__ptr[i].showname = soap_strdup(soap, all[i][2].data.s);
		
		// 使用 , 分割 ips
		{
			char **ptmp = 0;
			int ntmp = 0;

			// gsoap 没有提供 soap_realloc() ???
			char *ip = strtok(all[i][1].data.s, ",");
			while (ip) {
				ntmp++;
				ptmp = (char**)realloc(ptmp, ntmp * sizeof(char*));
				ptmp[ntmp-1] = strdup(ip);

				ip = strtok(0, ",");
			}

			hosts->__ptr[i].ips = (struct zkreg__Ips*)soap_malloc(soap, sizeof(struct zkreg__Ips));
			hosts->__ptr[i].ips->__size = ntmp;
			hosts->__ptr[i].ips->__ptr = (char**)soap_malloc(soap, ntmp*sizeof(char*));

			for (j = 0; j < ntmp; j++) {
				hosts->__ptr[i].ips->__ptr[j] = soap_strdup(soap, ptmp[j]);
				free(ptmp[j]);	// strdup(ip)
			}
			free(ptmp);	// realloc
		}
	}

	db_free_select2(desc, 3, all, rows);

	return SOAP_OK;
}

static int copy_ss_and_release(struct soap *soap, struct zkreg__Service**ss, int n, struct zkreg__Services *services)
{
    /** 将 Services 数据从 paramGetAllServices 结构中，复制到 zkreg__Services，并且释放 paramGetAllServices
     */

    int i, j;
    
    services->__size = n;
    services->__ptr = (struct zkreg__Service*)soap_malloc(soap, sizeof(struct zkreg__Service) * n);
    for (i = 0; i < n; i++) {
        struct zkreg__Service *s = ss[i];
        s->name = soap_strdup(soap, ss[i]->name);
        s->hostname = soap_strdup(soap, ss[i]->hostname);
        s->catalog = ss[i]->catalog;
        s->type = soap_strdup(soap, ss[i]->type);
        s->showname = soap_strdup(soap, ss[i]->showname);
        
        s->urls->__size = ss[i]->urls->__size;
        s->urls->__ptr = (char**)soap_malloc(soap, sizeof(char*) * s->urls->__size);
        for (j = 0; j < s->urls->__size; j++) {
            s->urls->__ptr[j] = ss[i]->urls->__ptr[j];
        }
    }
    
    // 释放 p._p
    for (i = 0; i < n; i++) {
        free(ss[i]->name);
        free(ss[i]->hostname);
        free(ss[i]->type);
        free(ss[i]->version);
        free(ss[i]->showname);
        
        for (j = 0; j < ss[i]->urls->__size; j++) {
            free(ss[i]->urls->__ptr[j]);
        }
        free(ss[i]->urls->__ptr);
        free(ss[i]->urls);
        
        free(ss[i]);
    }
    free(ss);

    return 0;
}

int zkq__getAllServices(struct soap *soap, enum xsd__boolean offline, struct zkreg__Services *services)
{
    struct zkreg__Service **ss = 0;
    int n = 0;
    
    int rc = db_getServiceList(_db, offline, &ss, &n);
    if (rc < 0) {
        services->__ptr = 0;
        services->__size = 0;
        return SOAP_OK;
    }
    
    copy_ss_and_release(soap, ss, n, services);

    return SOAP_OK;
}

int zkq__getAllDevices(struct soap* soap, enum xsd__boolean offline, struct zkreg__Devices *devices)
{
    return SOAP_OK;
}

int zkq__getAllLogics(struct soap *soap, enum xsd__boolean offline, struct zkreg__Logics *logics)
{
    return SOAP_OK;
}

int zkq__getServicesByType(struct soap *soap, enum xsd__boolean offline, char *type, struct zkreg__Services *services)
{
    struct zkreg__Service **ss = 0;
    int n = 0;
    
    int rc = db_getServiceListByType(_db, offline, type, &ss, &n);
    if (rc < 0) {
        services->__ptr = 0;
        services->__size = 0;
        return SOAP_OK;
    }
    
    copy_ss_and_release(soap, ss, n, services);
    
    return SOAP_OK;
}

int zkq__getParent(struct soap *soap, char *name, struct zkreg__Logic **logic)
{
	*logic = 0;
	return SOAP_OK;
}
