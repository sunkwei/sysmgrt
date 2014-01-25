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

struct paramGetAllMses
{
    struct zkreg__Mse **_p;
    int _n;
};

static int cb_get_all_mses(void *opaque, size_t row, sqlite3_stmt *stmt)
{
    struct paramGetAllMses *p = (struct paramGetAllMses*)opaque;
    
    // name, catalog, showname
    p->_n = (int)row+1;     // 行数
    p->_p = (struct zkreg__Mse**)realloc(p->_p, sizeof(struct zkreg__Mse*) * p->_n);
    p->_p[row] = (struct zkreg__Mse*)malloc(sizeof(struct zkreg__Mse));
    
    // 提取行记录
    struct zkreg__Mse *mse = p->_p[row];
    mse->name = strdup((const char *)sqlite3_column_text(stmt, 0));
    mse->catalog = sqlite3_column_int(stmt, 1);
    mse->showname = strdup((const char*)sqlite3_column_text(stmt, 2));
    
    return 0;
}

int __zkq__getAllMses(struct soap *soap, enum xsd__boolean offline, struct zkreg__Mses *mses)
{
    /** 返回 mse table 的内容 */
    
    char *sql = (char*)alloca(1024);
    if (offline) {
        snprintf(sql, 1024, "SELECT * FROM mse");
    }
    else {
        snprintf(sql, 1024, "SELECT mse.* FROM host JOIN token WHERE mse.name=token.name");
    }
    
    struct paramGetAllMses p = { 0, 0 };
    db_exec_select(_db, sql, cb_get_all_mses, &p);
    
    // 从 p._p 中复制到 mses 中
    mses->__size = p._n;
    mses->__ptr = (struct zkreg__Mse*)soap_malloc(soap, sizeof(struct zkreg__Mse) * p._n);
    for (int i = 0; i < p._n; i++) {
        mses->__ptr[i].name = soap_strdup(soap, p._p[i]->name);
        mses->__ptr[i].catalog = p._p[i]->catalog;
        mses->__ptr[i].showname = soap_strdup(soap, p._p[i]->showname);
    }
    
    // 释放 p._p 的内存
    for (int i = 0; i < p._n; i++) {
        free(p._p[i]->name);
        free(p._p[i]->showname);
        free(p._p[i]);
    }
    free(p._p);
    
    return SOAP_OK;
}

struct paramGetAllHosts
{
    struct zkreg__Host **_p;
    int _n;
};

static int cb_get_all_hosts(void *opaque, size_t row, sqlite3_stmt *stmt)
{
    struct paramGetAllHosts *p = (struct paramGetAllHosts*)opaque;
    
    /** FIXME: 将每行保存到 hosts 中，
            这里采用非常低效的内存分配方式 :(
     */
    p->_n = (int)row+1;  // 行数；
    p->_p = (struct zkreg__Host**)realloc(p->_p, p->_n * sizeof(struct zkreg__Host*));
    p->_p[row] = (struct zkreg__Host*)malloc(sizeof(struct zkreg__Host));
    
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
        snprintf(sql, 1024, "SELECT host.*,mse.showname FROM host JOIN mse ON host.name=mse.name");
    }
    else {
        // FIXME: 这个查询语句可能有问题！！！！
        snprintf(sql, 1024, "SELECT host.*,mse.showname FROM host"
                 " JOIN mse ON mse.name=host.name"
                 " JOIN token ON host.name=token.name");
    }
    
    struct paramGetAllHosts p = { 0, 0};
    db_exec_select(_db, sql, cb_get_all_hosts, &p);
    
    // 从 _p 复制到 hosts 中
    hosts->__size = p._n;
    hosts->__ptr = (struct zkreg__Host*)soap_malloc(soap, sizeof(struct zkreg__Host) * p._n);
    for (int i = 0; i < p._n; i++) {
        struct zkreg__Host *host = &hosts->__ptr[i];
        host->name = soap_strdup(soap, p._p[i]->name);
        host->catalog = p._p[i]->catalog;
        host->showname = soap_strdup(soap, p._p[i]->showname);
        host->ips = (struct zkreg__Ips*)soap_malloc(soap, sizeof(struct zkreg__Ips));
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
        free(p._p[i]->ips->__ptr);
        free(p._p[i]->ips);
        free(p._p[i]);
    }
    free(p._p);
    
    return SOAP_OK;
}

struct paramGetAllServices
{
    struct zkreg__Service **_p;
    int _n;
};

static int cb_get_all_services(void *opaque, size_t row, sqlite3_stmt *stmt)
{
    struct paramGetAllServices *p = (struct paramGetAllServices*)opaque;
    
    /** 从 stmt 中提取，并保存到 p 中 
     
            name, hostname, type, urls, version, showname
     */
    p->_n = (int)row+1;
    p->_p = (struct zkreg__Service**)realloc(p->_p, p->_n);
    p->_p[p->_n] = (struct zkreg__Service*)malloc(sizeof(struct zkreg__Service));
    
    struct zkreg__Service *s = p->_p[p->_n];
    
    s->name = strdup((const char*)sqlite3_column_text(stmt, 0));
    s->hostname = strdup((const char*)sqlite3_column_text(stmt, 1));
    s->catalog = sqlite3_column_int(stmt, 2);
    s->type = strdup((const char*)sqlite3_column_text(stmt, 3));
    s->version = strdup((const char*)sqlite3_column_text(stmt, 5));
    s->showname = strdup((const char*)sqlite3_column_text(stmt, 6));
    
    s->urls = (struct zkreg__Urls*)malloc(sizeof(struct zkreg__Urls));
    s->urls->__size = 0;
    s->urls->__ptr = 0;

    char *urls = strdup((const char*)sqlite3_column_text(stmt, 4));
    char *t = strtok(urls, ",");    // FIXME: url 使用“，”分割合理么？
    while (t) {
        s->urls->__size ++;
        s->urls->__ptr = ((char**)realloc(s->urls->__ptr, s->urls->__size * sizeof(char*)));
        s->urls->__ptr[s->urls->__size-1] = strdup(t);
        
        t = strtok(0, ",");
    }
    
    free(urls);
    
    return 0;
}

int __zkq__getAllServices(struct soap *soap, enum xsd__boolean offline, struct zkreg__Services *services)
{
    char *sql = (char*)alloca(1024);
    if (offline) {
        snprintf(sql, 1024, "SELECT service.*,mse.showname FROM service JOIN mse ON service.name=mse.name");
    }
    else {
        snprintf(sql, 1024, "SELECT service.*,mse.showname FROM service "
                 " JOIN token ON token.name=service.name "
                 " JOIN mse ON mse.name=service.name");
    }
    
    struct paramGetAllServices p = { 0, 0 };
    db_exec_select(_db, sql, cb_get_all_services, &p);
    
    // 从 p 中复制到 services 中
    services->__size = p._n;
    services->__ptr = (struct zkreg__Service*)soap_malloc(soap, sizeof(struct zkreg__Service) * p._n);
    for (int i = 0; i < p._n; i++) {
        struct zkreg__Service *s = &services->__ptr[i];
        s->name = soap_strdup(soap, p._p[i]->name);
        s->hostname = soap_strdup(soap, p._p[i]->hostname);
        s->catalog = p._p[i]->catalog;
        s->type = soap_strdup(soap, p._p[i]->type);
        s->showname = soap_strdup(soap, p._p[i]->showname);
        
        s->urls->__size = p._p[i]->urls->__size;
        s->urls->__ptr = (char**)soap_malloc(soap, sizeof(char*) * s->urls->__size);
        for (int j = 0; j < s->urls->__size; j++) {
            s->urls->__ptr[j] = p._p[i]->urls->__ptr[j];
        }
    }
    
    // 释放 p._p
    for (int i = 0; i < p._n; i++) {
        free(p._p[i]->name);
        free(p._p[i]->hostname);
        free(p._p[i]->type);
        free(p._p[i]->version);
        free(p._p[i]->showname);
        
        for (int j = 0; j < p._p[i]->urls->__size; j++) {
            free(p._p[i]->urls->__ptr[j]);
        }
        free(p._p[i]->urls->__ptr);
        free(p._p[i]->urls);
        
        free(p._p[i]);
    }
    free(p._p);
    
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

#define paramGetServiceByType paramGetAllServices
#define cb_get_service_by_type cb_get_all_services

int __zkq__getServicesByType(struct soap *soap, enum xsd__boolean offline, char *type, struct zkreg__Services *services)
{
    char *sql = (char*)alloca(1024);
    if (offline) {
        
    }
    else {
        
    }
    return SOAP_OK;
}
