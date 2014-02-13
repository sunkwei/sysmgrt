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
#include "dboper.h"

#ifdef WIN32
#  include <malloc.h>
#  include <Windows.h>
#  define snprintf _snprintf
#else
#  include <alloca.h>
#  include <uuid/uuid.h>
#endif
/** 用于判断是否存在，一般执行: 
        SELECT COUNT(*) FROM table WHERE .... 
    的查询.
 */
static int cb_exist(void *opaque, size_t row, sqlite3_stmt *stmt)
{
    *(int*)opaque = sqlite3_column_int(stmt, 0);
    return -1;
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
	struct zkreg__Service *s;
	char *urls, *t;
    
    /** 从 stmt 中提取，并保存到 p 中.
     
     name, hostname, type, urls, version, showname
     */
    p->_n = (int)row+1;
    p->_p = (struct zkreg__Service**)realloc(p->_p, p->_n*sizeof(struct zkreg__Service*));
    p->_p[p->_n] = (struct zkreg__Service*)malloc(sizeof(struct zkreg__Service));
    
    s = p->_p[p->_n];
    
    s->name = strdup((const char*)sqlite3_column_text(stmt, 0));
    s->hostname = strdup((const char*)sqlite3_column_text(stmt, 1));
    s->catalog = sqlite3_column_int(stmt, 2);
    s->type = strdup((const char*)sqlite3_column_text(stmt, 3));
    s->version = strdup((const char*)sqlite3_column_text(stmt, 5));
    s->showname = strdup((const char*)sqlite3_column_text(stmt, 6));
    
    s->urls = (struct zkreg__Urls*)malloc(sizeof(struct zkreg__Urls));
    s->urls->__size = 0;
    s->urls->__ptr = 0;
    
    urls = strdup((const char*)sqlite3_column_text(stmt, 4));
    t = strtok(urls, ",");    // FIXME: url 使用“，”分割合理么.
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
    char *st = (char*)alloca(1024);
    struct paramGetServices p = { 0, 0 };
    *service = 0, *n = 0;
    
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
    
    db_exec_select(db, st, cb_get_services, &p);
    *service = p._p;
    *n = p._n;
    
    return 0;
}
