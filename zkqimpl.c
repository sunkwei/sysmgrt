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
#include "soapStub.h"
#include "dbhlp.h"
#include "dboper.h"

int zkq__getAllMses(struct soap *soap, enum xsd__boolean offline, struct zkreg__Mses *mses)
{
    /** 返回 mse table 的内容 */

    struct zkreg__Mse **m;
    int n, i;
    
    if (db_getAllMses(_db, offline, &m, &n) < 0) {
        mses->__size = 0;
        mses->__ptr = 0;
        
        return SOAP_OK;
    }
    
    // 从 p._p 中复制到 mses 中.
    mses->__size = n;
    mses->__ptr = (struct zkreg__Mse*)soap_malloc(soap, sizeof(struct zkreg__Mse) * n);
    for (i = 0; i < n; i++) {
        mses->__ptr[i].name = soap_strdup(soap, m[i]->name);
        mses->__ptr[i].catalog = m[i]->catalog;
        mses->__ptr[i].showname = soap_strdup(soap, m[i]->showname);
    }
    
    // 释放 p._p 的内存.
    for (i = 0; i < n; i++) {
        free(m[i]->name);
        free(m[i]->showname);
        free(m[i]);
    }
    free(m);
    
    return SOAP_OK;
}

int zkq__getAllHosts(struct soap *soap, enum xsd__boolean offline, struct zkreg__Hosts *hosts)
{
    struct zkreg__Host **hs = 0;
    int n = 0, i, j;
    int rc = db_getAllHosts(_db, offline, &hs, &n);
    if (rc < 0) {
        //
    }
    
    // 从 _p 复制到 hosts 中.
    hosts->__size = n;
    hosts->__ptr = (struct zkreg__Host*)soap_malloc(soap, sizeof(struct zkreg__Host) * n);
    for (i = 0; i < n; i++) {
        struct zkreg__Host *host = &hosts->__ptr[i];
        host->name = soap_strdup(soap, hs[i]->name);
        host->catalog = hs[i]->catalog;
        host->showname = soap_strdup(soap, hs[i]->showname);
        host->ips = (struct zkreg__Ips*)soap_malloc(soap, sizeof(struct zkreg__Ips));
        host->ips->__size = hs[i]->ips->__size;
        host->ips->__ptr = (char**)soap_malloc(soap, sizeof(char*) * host->ips->__size);
        for (j = 0; j < host->ips->__size; j++) {
            host->ips->__ptr[j] = soap_strdup(soap, hs[i]->ips->__ptr[j]);
        }
    }
    
    // 释放 _p
    for (i = 0; i < n; i++) {
        free(hs[i]->name);
        free(hs[i]->showname);
        
        for (j = 0; j < hs[i]->ips->__size; j++) {
            free(hs[i]->ips->__ptr[j]);
        }
        free(hs[i]->ips->__ptr);
        free(hs[i]->ips);
        free(hs[i]);
    }
    free(hs);
    
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
