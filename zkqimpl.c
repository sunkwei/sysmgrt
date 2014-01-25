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

int __zkq__getAllMses(struct soap *soap, enum xsd__boolean offline, struct zkreg__Mses *mses)
{
    return SOAP_OK;
}

struct paramGetAllHosts
{
    struct soap *soap;
    struct zkreg__Hosts *hosts;
    struct zkreg__Host **_p;
    int _n;
};

static int cb_get_all_hosts(void *opaque, size_t row, sqlite3_stmt *stmt)
{
    struct paramGetAllHosts *p = (struct paramGetAllHosts*)opaque;
    
    /** FIXME: 将每行保存到 hosts 中，
            这里采用非常低效的内存分配方式 :(
     */
    p->_n = row+1;  // 行数；
    p->_p = (struct _zkreg__Host**)realloc(p->_p, p->_n * sizeof(struct _zkreg__Host*));
    p->_p[row] = (struct _zkreg__Host*)malloc(sizeof(struct zkreg__Host));
    
    // 提取行记录
    struct zkreg__Host *host = p->_p[row];
    host->catalog = zkreg__Catalog__Host;
    host->name = strdup((const char*)sqlite3_column_text(stmt, 0));
    host->showname = strdup((const char *)sqlite3_column_text(stmt, 2));
    host->ips = (struct zkreg__Ips*)malloc(sizeof(struct zkreg__Ips));
    host->ips->__ptr = 0;
    host->ips->__size = 0;
    
    char *ips = strdup((const char*)sqlite3_column_text(stmt, 1));
    char *ip = strtok(ips, ",");
    while (ip) {
        host->ips->__size++;
        host->ips->__ptr = (char**)realloc(host->ips->__ptr, host->ips->__size*sizeof(char*));
        host->ips->__ptr[host->ips->__size-1] = strdup(ip);
        
        ip = strtok(0, ",");
    }
    
    return 0;
}

int __zkq__getAllHosts(struct soap *soap, enum xsd__boolean offline, struct zkreg__Hosts *hosts)
{
    /** 从 host / token 表中提取
     */
    char *sql = (char*)alloca(1024);
    if (offline) {
        snprintf(sql, 1024, "SELECT * FROM host");
    }
    else {
        // FIXME: 这个查询语句可能有问题！！！！
        snprintf(sql, 1024, "SELECT host.* FROM host join token where token.name=host.name and token.catalog=0");
    }
    
    struct paramGetAllHosts p = { soap, hosts, 0, 0};
    db_exec_select(_db, sql, cb_get_all_hosts, &p);
    
    // 从 _p 复制到 hosts 中
    hosts->__size = p._n;
    hosts->__ptr = (struct zkreg__Host*)soap_malloc(soap, sizeof(struct zkreg__Host) * p._n);
    for (int i = 0; i < p._n; i++) {
        struct zkreg__Host *host = &hosts->__ptr[i];
        host->name = soap_strdup(soap, p._p[i]->name);
        host->catalog = p._p[i]->catalog;
        host->showname = soap_strdup(soap, p._p[i]->showname);
        host->ips = (struct _zkreg__Ips*)soap_malloc(soap, sizeof(struct zkreg__Ips));
        host->ips->__size = p._p[i]->ips->__size;
        host->ips->__ptr = (char**)soap_malloc(soap, sizeof(char*) * host->ips->__size);
        for (int j = 0; j < host->ips->__size; j++) {
            host->ips->__ptr[j] = soap_strdup(soap, p._p[i]->ips->__ptr[j]);
        }
    }
    
    // 释放 _p
    for (int i = 0; i < p._n; i++) {
        free(p._p[i]->name);
        free(p._p[i]->showname);
        
        for (int j = 0; j < p._p[i]->ips->__size; j++) {
            free(p._p[i]->ips->__ptr[j]);
        }
        free(p._p[i]->ips);
        free(p._p[i]);
    }
    free(p._p);
    
    return SOAP_OK;
}

int __zkq__getAllServices(struct soap *soap, enum xsd__boolean offline, struct zkreg__Services *services)
{
    return SOAP_OK;
}

int __zkq__getAllDevices(struct soap* soap, enum xsd__boolean offline, struct zkreg__Devices *devices)
{
    return SOAP_OK;
}

int __zkq__getAllLogics(struct soap *soap, enum xsd__boolean offline, struct zkreg__Logics *logics)
{
    return SOAP_OK;
}

int __zkq__getServicesByType(struct soap *soap, enum xsd__boolean offline, char *type, struct zkreg__Services *services)
{
    return SOAP_OK;
}
