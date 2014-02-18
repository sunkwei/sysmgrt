//
//  zkregimpl.c
//  zonekey_sysmgrt
//
//  Created by sunkw on 14-1-24.
//  Copyright (c) 2014年 sunkw. All rights reserved.
//

#include <stdio.h>
#include <time.h>
#include <assert.h>
#include "soapStub.h"
#include "dbhlp.h"
#include "dboper.h"
#ifdef WIN32
#  include <malloc.h>
#  include <Windows.h>
#else
#  include <alloca.h>
#  include <uuid/uuid.h>
#endif

#ifdef WIN32
static void uuid_generate(uuid_t *id)
{
	CoCreateGuid(id);
}

static void uuid_unparse(uuid_t *id, char s[40])
{
	char *ss;
	UuidToString(id, &ss);
	strcpy(s, ss);
	RpcStringFree(&ss);
}

#endif // win32

int zkreg__version(struct soap *soap, void *notused, char **info)
{
    *info = soap_strdup(soap, "en, VERSION is 0.0.9");
    return SOAP_OK;
}

int cb_exist(void *opaque, size_t row, sqlite3_stmt *stmt)
{
    *(int*)opaque = sqlite3_column_int(stmt, 0);    // 总是 COUNT(*)
    return -1;  // 总是中断.
}

int db_regHost(struct zkreg__Host *host, const char *token)
{
    char *st = (char*)alloca(1024);
    char *ips = (char*)alloca(128); // FIXME: 应该足够了吧 :).
    int exist = 0, i;
    unsigned now = (unsigned)time(0);
    sqlite3 *db = db_get();
    ips[0] = 0;
    
    // 串行化ips.
    if (host->ips->__size > 0)
        strcpy(ips, host->ips->__ptr[0]);
    for (i = 1; i < host->ips->__size; i++) {
        strcat(ips, ",");
        strcat(ips, host->ips->__ptr[i]);
    }
    
    // FIXME: 总是从 token 中删除 name.
    snprintf(st, 1024, "DELETE FROM token WHERE name='%s'", host->name);
    db_exec_sql(db, st);
    
    /** 查询，如果 mse和host中有 name，则更新，name，parent, showname 和 reg_stamp 字段不更新 */
    _snprintf(st, 1024, "SELECT COUNT(*) FROM host JOIN mse ON host.name=mse.name WHERE host.name='%s'", host->name);
    db_exec_select(db, st, cb_exist, &exist);
    
    if (!exist) {
        // 不管三七二十一，删除可能的垃圾记录.
        _snprintf(st, 1024, "DELETE FROM mse WHERE name='%s';"
                 "DELETE FROM host WHERE name='%s';",
                 host->name, host->name);
        db_exec_sql(db, st);
        
        // 新建记录.
        snprintf(st, 1024, "INSERT INTO host (name, ips) VALUES ('%s', '%s');"
			"INSERT INTO mse (name, catalog, parent, showname, reg_stamp, access_stamp) VALUES('%s', %d, '', '%s', %u, %u)",
			host->name, ips,
			host->name, zkreg__Catalog__Host, host->showname, now, now);
        db_exec_sql(db, st);
    }
    else {
        // 更新记录.
        snprintf(st, 1024, "UPDATE host SET ips='%s' WHERE name='%s';"
			"UPDATE mse SET access_stamp=%u WHERE name='%s'", 
			ips, host->name, 
			now, host->name);
        db_exec_sql(db, st);
    }
    
    // 新建 token 记录.
    snprintf(st, 1024, "INSERT INTO token (token, name, last_stamp)"
             " VALUES('%s', '%s', %u)",
             token, host->name, now);
    db_exec_sql(db, st);
    db_release(db);
    
    return 0;
}

int zkreg__regHost(struct soap* soap, struct zkreg__Host *req, char **token)
{
    uuid_t id;
    char s[40];
	int rc;
    
	memset(&id, 0, sizeof(uuid_t));

#ifdef WIN32
    uuid_generate(&id);
    uuid_unparse(&id, s);
#else
    uuid_generate(id);
    uuid_unparse(id, s);
#endif
    
    rc = db_regHost(req, s);
    if (rc >= 0) {
        fprintf(stderr, "INFO: %s: reg %s OK\n", __func__, req->name);
        *token = soap_strdup(soap, s);
    }
    else {
        *token = "";
        fprintf(stderr, "ERR: %s: reg %s ERR\n", __func__, req->name);
    }
    
    return SOAP_OK;
}

int zkreg__unregHost(struct soap *soap, char *token, int *code)
{
    char *st = (char*)alloca(1024);
    sqlite3 *db = db_get();

	// 从 token table 中找到，删除，但不应该删除 host table 中的记录，或许将来增加一个 removeHost 的函数.
    *code = 0;

    /** 对于 unRegXXX: 仅仅在 token 中删除，而不会删除 mse/host 表中的记录 .
        永远返回0，不管 token 是否存在 :).
     */
    snprintf(st, 1024, "DELETE FROM token WHERE token='%s'", token);
    
    db_exec_sql(db, st);
    db_release(db);

    return SOAP_OK;
}

static int db_regService(struct zkreg__Service *s, const char *token)
{
	char *st = (char*)alloca(4096);
	char *url = (char*)alloca(1024);	// FIXME: 希望空间足够了 :)
	int i, exist = 0;
    unsigned now = (unsigned)time(0);
    sqlite3 *db = db_get();
	
	url[0] = 0;
	for (i = 0; i < s->urls->__size; i++) {
		strcat(url, s->urls->__ptr[i]);
		strcat(url, "\n");	// 使用”回车“分割.
	}

	snprintf(st, 1024, "DELETE FROM token WHERE name='%s'", s->name);
	db_exec_sql(db, st);

	snprintf(st, 1024, "SELECT COUNT(*) FROM service JOIN mse ON mse.name=service.name WHERE service.name='%s'", s->name);
	db_exec_select(db, st, cb_exist, &exist);

	if (!exist) {
		snprintf(st, 4096, "DELETE FROM mse WHERE name='%s';"
			"DELETE FROM service WHERE name='%s'",
			s->name, s->name);
		db_exec_sql(db, st);

		// 新建记录
        snprintf(st, 4096, "INSERT INTO mse (name, catalog, parent, showname, reg_stamp, access_stamp)"
                 " VALUES('%s', %d, '', '%s', %u, %u);"
				 "INSERT INFO service (name, hostname, type, urls, version)"
				 " VALUES('%s', '%s', '%s', '%s', '%s');",
				 s->name, zkreg__Catalog__Service, s->showname, now, now,
				 s->name, s->hostname, s->type, s->urls, s->version);
		db_exec_sql(db, st);
	}
	else {
		// 更新 access time, type, urls, version
		snprintf(st, 4096, "UPDATE mse SET access_stamp=%u WHERE name='%s';"
			"UPDATE service SET type='%s', urls='%s', version='%s' WHERE name='%s'",
			now, s->name,
			s->type, s->urls, s->version, s->name);
		db_exec_sql(db, st);
	}

    snprintf(st, 1024, "INSERT INTO token (token, name, last_stamp)"
             " VALUES('%s', '%s', %u)",
             token, s->name, now);
    db_exec_sql(db, st);
    db_release(db);

	return 0;
}

int zkreg__regService(struct soap *soap, struct zkreg__Service *req, char **token)
{
    uuid_t id;
	int rc;
    char s[40];
#ifdef WIN32
    uuid_generate(&id);
    uuid_unparse(&id, s);
#else
    uuid_generate(id);
    uuid_unparse(id, s);
#endif
    
    rc = db_regService(req, s);
    if (rc >= 0) {
        fprintf(stdout, "INFO: %s: reg %s OK\n", __func__, req->name);
        *token = soap_strdup(soap, s);
    }
    else {
        *token = "";
        fprintf(stderr, "ERR: %s: reg %s ERR\n", __func__, req->name);
    }
    
    return SOAP_OK;
}

int zkreg__unregService(struct soap *soap, char *token, int *code)
{
    char *st = (char*)alloca(1024);
    sqlite3 *db = db_get();

	// 从 token table 中找到，删除，但不应该删除 host table 中的记录，或许将来增加一个 removeHost 的函数.
    *code = 0;

    /** 对于 unRegXXX: 仅仅在 token 中删除，而不会删除 mse/host 表中的记录 .
        永远返回0，不管 token 是否存在 :).
     */
    snprintf(st, 1024, "DELETE FROM token WHERE token='%s'", token);
    db_exec_sql(db, st);
    db_release(db);
    
	return SOAP_OK;
}

int zkreg__regDevice(struct soap *soap, struct zkreg__Device *req, char **token)
{
	// TODO: 
	return SOAP_OK;
}

int zkreg__unregDevice(struct soap *soap, char *token, int *code)
{
    char *st = (char*)alloca(1024);
    sqlite3 *db = db_get();

	// 从 token table 中找到，删除，但不应该删除 host table 中的记录，或许将来增加一个 removeHost 的函数.
    *code = 0;

    /** 对于 unRegXXX: 仅仅在 token 中删除，而不会删除 mse/host 表中的记录 .
        永远返回0，不管 token 是否存在 :).
     */
    snprintf(st, 1024, "DELETE FROM token WHERE token='%s'", token);
    db_exec_sql(db, st);
    db_release(db);

    return SOAP_OK;
}

int zkreg__heartBeat(struct soap *soap, char *token, int *code)
{
    /** 心跳，更新 token table 的 last_stamp */
    /** 首先查询 token table，如果不存在，返回 -1，否则更新 last_stamp，返回 0
     */
    char *st = (char*)alloca(1024);
    int exist = 0;
    sqlite3 *db = db_get();
    
	snprintf(st, 1024, "SELECT COUNT(*) FROM token WHERE token='%s'", token);
    db_exec_select(db, st, cb_exist, &exist);
    
    if (!exist)
        *code = -1;
    else {
        snprintf(st, 1024, "UPDATE token SET last_stamp=%u WHERE token='%s'",
                 (unsigned)time(0), token);
        db_exec_sql(db, st);
        
        *code = 0;
    }
    db_release(db);

    return SOAP_OK;
}

int zkreg__setShowName(struct soap *soap, char *name, char *showname, int *code)
{
    /** 首先查询 mse table，如果存在 name，则更新 showname，否则返回 -1 */
    char *st = (char*)alloca(1024);
    int exist = 0;
    sqlite3 *db = db_get();

	snprintf(st, 1024, "SELECT COUNT(*) FROM mse WHERE name='%s'", name);
    db_exec_select(db, st, cb_exist, &exist);
    
    if (!exist)
        *code = -1;
    else {
        snprintf(st, 1024, "UPDATE mse SET showname='%s' WHERE name='%s'",
                 showname, name);
        db_exec_sql(db, st);
        
        *code = -1;
    }
    db_release(db);
    
    return SOAP_OK;
}

int zkreg__regLogic(struct soap *soap, struct zkreg__Logic *logic, char **token)
{
	char *st = (char*)alloca(1024);
	char *children = (char*)alloca(768);
	int i, exist = 0;
	unsigned now = (unsigned)time(0);
    sqlite3 *db = db_get();

	children[0] = 0;
	for (i = 0; i < logic->childnum; i++) {
		strcat(children, logic->children[i]);	// 使用"\n"分割的子对象名列表.
		strcat(children, "\n");
	}

	*token = soap_strdup(soap, "000000");

	// 检查是否存在.
	snprintf(st, 1024, "SELECT COUNT(*) FROM logic JOIN mse ON mse.name=logic.name WHERE logic.name='%s'", logic->name);
	db_exec_select(db, st, cb_exist, &exist);

	if (!exist) {
		snprintf(st, 4096, "DELETE FROM mse WHERE name='%s';"
			"DELETE FROM logic WHERE name='%s'",
			logic->name, logic->name);
		db_exec_sql(db, st);

		// 新建记录
        snprintf(st, 1024, "INSERT INTO mse (name, catalog, parent, showname, reg_stamp, access_stamp)"
                 " VALUES('%s', %d, '', '%s', %u, %u);"
				 "INSERT INFO logic (name, children)"
				 " VALUES('%s', '%s');",
				 logic->name, zkreg__Catalog__Logic, logic->showname, now, now,
				 logic->name, children);
		db_exec_sql(db, st);
	}
	else {
		// 更新
		snprintf(st, 1024, "UPDATE mse SET access_stamp WHERE name='%s';"
			"UPDATE logic SET children='%s'",
			now,
			children);

		db_exec_sql(db, st);
	}
    db_release(db);

	return SOAP_OK;
}

int zkreg__delMse(struct soap *soap, char *name, int *code)
{
	// 删除记录
	char *st = (char*)alloca(1024);
    sqlite3 *db = db_get();

	struct dbhlpColumn desc[] = {
		{ { 0 }, DBT_STRING },	// name
		{ { 0 }, DBT_INT },		// catalog
	};

	struct dbhlpColumn **all;
	int rows = 0;

	snprintf(st, 1024, "SELECT name FROM mse WHERE name='%s'", name);
	db_exec_select2(db, st, desc, sizeof(desc)/sizeof(struct dbhlpColumn), &all, &rows);

	if (rows > 0) {
		assert(rows == 1);	// 必须的 :)
		
		// 说明找到了. 
		switch (all[0][1].data.i) {
		case zkreg__Catalog__Host:
			snprintf(st, 1024, "DELETE FROM mse WHERE name='%s';"
				"DELETE FROM host WHERE name='%s'", name, name);
			break;

		case zkreg__Catalog__Service:
			snprintf(st, 1024, "DELETE FROM mse WHERE name='%s';"
				"DELETE FROM service WHERE name='%s'", name, name);
			break;

		case zkreg__Catalog__Device:
			snprintf(st, 1024, "DELETE FROM mse WHERE name='%s';"
				"DELETE FROM device WHERE name='%s'", name, name);
			break;

		case zkreg__Catalog__Logic:
			snprintf(st, 1024, "DELETE FROM mse WHERE name='%s';"
				"DELETE FROM logic WHERE name='%s'", name, name);
			break;

		default:
			fprintf(stderr, "ERR: %s:%d unknown type!\n", __func__, __LINE__);
			exit(-1);
			break;
		}

		db_free_select2(desc, sizeof(desc)/sizeof(struct dbhlpColumn), all, rows);

		db_exec_sql(db, st);

		*code = 0;	// 成功
	}
	else {
		*code = -1;	// 没有找到
	}
    db_release(db);

	return SOAP_OK;
}

int zkreg__bind(struct soap *soap, char *logic, char *mse, int *code)
{
	char *st = (char*)alloca(1024);
    sqlite3 *db = db_get();
	struct dbhlpColumn desc[] = {
		{ { 0 }, DBT_STRING },	// name
		{ { 0 }, DBT_STRING },	// children
	};

	struct dbhlpColumn **all;
	int rows = 0;

	snprintf(st, 1024, "SELECT name,children FROM logic WHERE name='%s'", logic);
	db_exec_select2(db, st, desc, sizeof(desc)/sizeof(struct dbhlpColumn), &all, &rows);

	if (rows > 0) {
		assert(rows == 1);	// 必须的 :)

		// 检查 mse 是否已经在 children 中. 
		if (strstr(all[0][1].data.s, mse)) {
			*code = 1;	// 已经存在. 
		}
		else {
			snprintf(st, 1024, "UPDATE logic SET children='%s\n%s' WHERE name=%s",
				all[0][1].data.s, mse, logic);
			db_exec_sql(db, st);
		}

		db_free_select2(desc, sizeof(desc)/sizeof(struct dbhlpColumn), all, rows);
	}
	else {
		*code = -1;	// 没有找到. 
	}
    db_release(db);

	return SOAP_OK;
}

int zkreg__unbind(struct soap *soap, char *logic, char *mse, int *code)
{
	char *st = (char*)alloca(1024);
    sqlite3 *db = db_get();
	struct dbhlpColumn desc[] = {
		{ { 0 }, DBT_STRING },	// name
		{ { 0 }, DBT_STRING },	// children
	};

	struct dbhlpColumn **all;
	int rows = 0;

	snprintf(st, 1024, "SELECT name,children FROM logic WHERE name='%s'", logic);
	db_exec_select2(db, st, desc, sizeof(desc)/sizeof(struct dbhlpColumn), &all, &rows);

	if (rows > 0) {
		assert(rows == 1);	// 必须的 :)

		// 检查 mse 是否已经在 children 中. 
		if (strstr(all[0][1].data.s, mse)) {
			// 从 children 字符串中删除 ..
			char *p = (char*)alloca(strlen(all[0][1].data.s) + 1), *child;
			p[0] = 0;

			child = strtok(all[0][1].data.s, "\n");
			while (child) {
				if (strcmp(child, mse))
					strcat(p, child);
			}

			snprintf(st, 1024, "UPDATE logic SET children='%s' WHERE name=%s",
				all[0][1].data.s, logic);

			db_exec_sql(db, st);
		}
		else {
			*code = 1;	// 本来就不存在 .
		}

		db_free_select2(desc, sizeof(desc)/sizeof(struct dbhlpColumn), all, rows);
	}
	else {
		*code = -1;	// 没有找到.
	}
    db_release(db);

	return SOAP_OK;
}

int zkreg__setParent(struct soap *soap, char *mse, char *parent, int *code)
{
	char *st = (char*)alloca(1024);
    sqlite3 *db = db_get();
	struct dbhlpColumn desc[] = {
		{ { 0 }, DBT_STRING },	// name
		{ { 0 }, DBT_STRING },	// parent
	};

	struct dbhlpColumn **all;
	int rows = 0;

	snprintf(st, 1024, "SELECT name,parent FROM mse WHERE name='%s'", mse);
	db_exec_select2(db, st, desc, sizeof(desc)/sizeof(struct dbhlpColumn), &all, &rows);

	if (rows > 0) {
		assert(rows == 1);	// 必须的 :)

		snprintf(st, 1024, "UPDATE mse SET parent='s' WHERE name='%s'", parent, mse);
		db_exec_sql(db, st);

		db_free_select2(desc, sizeof(desc)/sizeof(struct dbhlpColumn), all, rows);
	}
	else {
		*code = -1; // 没有找到.
	}
    db_release(db);

	return SOAP_OK;
}
