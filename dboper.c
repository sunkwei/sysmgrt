//
//  dboper.c
//  zonekey_sysmgrt
//
//  Created by sunkw on 14-1-26.
//  Copyright (c) 2014年 sunkw. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <uuid/uuid.h>
#include <strings.h>
#include <alloca.h>
#include "dboper.h"

/** 用于判断是否存在，一般执行
        SELECT COUNT(*) FROM table WHERE .... 
    的查询
 */
static int cb_exist(void *opaque, size_t row, sqlite3_stmt *stmt)
{
    *(int*)opaque = sqlite3_column_int(stmt, 0);    // 总是 COUNT(*)
    return -1;  // 总是中断，
}

int db_regHost(sqlite3 *db, struct zkreg__Host *host, const char *token)
{
    char *st = (char*)alloca(1024);
    char *ips = (char*)alloca(128); // FIXME: 应该足够了吧 :)
    int exist = 0;
    unsigned now = (unsigned)time(0);
    
    ips[0] = 0;
    
    // 串行化ips
    if (host->ips->__size > 0)
        strcpy(ips, host->ips->__ptr[0]);
    for (int i = 1; i < host->ips->__size; i++) {
        strcat(ips, ",");
        strcat(ips, host->ips->__ptr[i]);
    }
    
    // FIXME: 总是从 token 中删除 name
    snprintf(st, 1024, "DELETE FROM token WHERE name='%s'", host->name);
    db_exec_sql(db, st);
    
    /** 查询，如果 mse和host中有 name，则更新，name，showname 和 reg_stamp 字段不更新 */
    snprintf(st, 1024, "SELECT COUNT(*) FROM host JOIN mse ON host.name=mse.name WHERE host.name='%s'", host->name);
    db_exec_select(db, st, cb_exist, &exist);
    
    if (!exist) {
        // 不管三七二十一，删除可能的垃圾记录
        snprintf(st, 1024, "DELETE FROM mse WHERE name='%s';"
                 "DELETE FROM host WHERE name='%s';",
                 host->name, host->name);
        db_exec_sql(db, st);
        
        // 新建记录
        snprintf(st, 1024, "INSERT INTO host (name, ips) VALUES ('%s', '%s')", host->name, ips);
        db_exec_sql(db, st);
        
        snprintf(st, 1024, "INSERT INTO mse (name, catalog, showname, reg_stamp, access_stamp)"
                 " VALUES('%s', %d, '%s', %u, %u)",
                 host->name, zkreg__Catalog__Host, host->showname, now, now);
        db_exec_sql(db, st);
    }
    else {
        // 更新记录
        snprintf(st, 1024, "UPDATE host SET ips='%s' WHERE name='%s'", ips, host->name);
        db_exec_sql(db, st);
        
        snprintf(st, 1024, "UPDATE mse SET access_stamp=%u WHERE name='%s'", now, host->name);
        db_exec_sql(db, st);
    }
    
    // 新建 token 记录
    snprintf(st, 1024, "INSERT INTO token (token, name, last_stamp)"
             " VALUES('%s', '%s', %u)",
             token, host->name, now);
    db_exec_sql(db, st);

    return 0;
}

int db_regService(sqlite3 *db, struct zkreg__Service *service, const char *token)
{
    return 0;
}

int db_unregXXX(sqlite3 *db, const char *token)
{
    /** 对于 unRegXXX: 仅仅在 token 中删除，而不会删除 mse/host 表中的记录 
        永远返回0，不管 token 是否存在 :)
     */
    char *st = (char*)alloca(1024);
    snprintf(st, 1024, "DELETE FROM token WHERE token='%s'", token);
    db_exec_sql(db, st);
    
    return 0;
}

int db_heartBeat(sqlite3 *db, const char *token)
{
    /** 首先查询 token table，如果不存在，返回 -1，否则更新 last_stamp，返回 0
     */
    char *st = (char*)alloca(1024);
    int exist = 0;
    snprintf(st, 1024, "SELECT COUNT(*) FROM token WHERE token='%s'", token);
    db_exec_select(db, st, cb_exist, &exist);
    
    if (!exist)
        return -1;
    else {
        snprintf(st, 1024, "UPDATE token SET last_stamp=%u WHERE token='%s'",
                 (unsigned)time(0), token);
        db_exec_sql(db, st);
        
        return 0;
    }
}

int db_setShowname(sqlite3 *db, const char *name, const char *showname)
{
    /** 首先查询 mse table，如果存在 name，则更新 showname，否则返回 -1 */
    char *st = (char*)alloca(1024);
    int exist = 0;
    snprintf(st, 1024, "SELECT COUNT(*) FROM mse WHERE name='%s'", name);
    db_exec_select(db, st, cb_exist, &exist);
    
    if (!exist)
        return -1;
    else {
        snprintf(st, 1024, "UPDATE mse SET showname='%s' WHERE name='%s'",
                 showname, name);
        db_exec_sql(db, st);
        
        return 0;
    }
}

struct paramGetMses
{
    struct zkreg__Mse **_p;
    int _n;
};

static int cb_get_mses(void *opaque, size_t row, sqlite3_stmt *stmt)
{
    struct paramGetMses *p = (struct paramGetMses*)opaque;
    
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

int db_getAllMses(sqlite3 *db, int offline, struct zkreg__Mse ***mses, int *n)
{
    *mses = 0, *n = 0;
    
    char *st = (char*)alloca(1024);
    if (offline) {
        snprintf(st, 1024, "SELECT * FROM mse");
    }
    else {
        snprintf(st, 1024, "SELECT mse.* FROM mse JOIN token ON token.name=mse.name");
    }
    
    struct paramGetMses p = { 0, 0 };
    db_exec_select(db, st, cb_get_mses, &p);
    *mses = p._p;
    *n = p._n;
    
    return 0;
}

struct paramGetHosts
{
    struct zkreg__Host **_p;
    int _n;
};

static int cb_get_hosts(void *opaque, size_t row, sqlite3_stmt *stmt)
{
    struct paramGetHosts *p = (struct paramGetHosts*)opaque;
    
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

int db_getAllHosts(sqlite3 *db, int offline, struct zkreg__Host ***host, int *n)
{
    *host = 0, *n = 0;

    /** 根据 offline，从 host, mse, token 中返回查询记录，从将每行信息分配保存到 zkreg__Host 结构中.
     */
    char *st = (char*)alloca(1024);
    if (offline) {
        snprintf(st, 1024, "SELECT host.*,mse.showname FROM host"
                 " JOIN mse ON mse.name=host.name");
    }
    else {
        snprintf(st, 1024, "SELECT host.*,mse.showname FROM host"
                 " JOIN mse ON mse.name=host.name"
                 " JOIN token ON token.name=host.name");
    }
    
    struct paramGetHosts p = { 0, 0 };
    db_exec_select(db, st, cb_get_hosts, &p);
    *host = p._p;
    *n = p._n;
    
    return 0;
}

int db_getServiceList(sqlite3 *db, int offline, struct zkreg__Service ***services, int *n)
{
    return db_getServiceListByType(db, offline, 0, services, n);
}

struct paramGetServices
{
    struct zkreg__Service **_p;
    int _n;
};

static int cb_get_services(void *opaque, size_t row, sqlite3_stmt *stmt)
{
    struct paramGetServices *p = (struct paramGetServices*)opaque;
    
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

int db_getServiceListByType(sqlite3 *db, int offline, const char *type, struct zkreg__Service ***service, int *n)
{
    *service = 0, *n = 0;
    
    char *st = (char*)alloca(1024);
    if (offline) {
        if (type) {
            snprintf(st, 1024, "SELECT service.*,mse.showname FROM service"
                     " JOIN mse ON mse.name=service.name"
                     " WHERE service.type='%s'", type);
        }
        else {
            snprintf(st, 1024, "SELECT service.*,mse.showname FROM service"
                     " JOIN mse ON mse.name=service.name");
        }
    }
    else {
        if (type) {
            snprintf(st, 1024, "SELECT service.*,mse.showname FROM service"
                     " JOIN mse ON mse.name=service.name"
                     " JOIN token ON token.name=service.name"
                     " WHERE service.type='%s'", type);
        }
        else {
            snprintf(st, 1024, "SELECT service.*,mse.showname FROM service"
                     " JOIN mse ON mse.name=service.name"
                     " JOIN token ON token.name=service.name");
        }
    }
    
    struct paramGetServices p = { 0, 0 };
    db_exec_select(db, st, cb_get_services, &p);
    *service = p._p;
    *n = p._n;
    
    return 0;
}
