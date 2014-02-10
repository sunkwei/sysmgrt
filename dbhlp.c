//
//  storage.c
//  zonekey_sysmgrt
//
//  Created by sunkw on 14-1-24.
//  Copyright (c) 2014年 sunkw. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <malloc.h>
#include "dbhlp.h"
#include "heartBeatCheck.h"

/** 执行 select 语句. 
 */
int db_exec_select(sqlite3 *db, const char *sql, int (*callback)(void *opaque, size_t row, sqlite3_stmt *stmt), void *opaque)
{
    size_t row = 0;
    sqlite3_stmt *stmt;
	int rc;

	if (sqlite3_prepare_v2(db, sql, (int)strlen(sql), &stmt, 0) != SQLITE_OK) {
        fprintf(stderr, "%s: sqlite3_prepare_v2 err\n", __func__);
        exit(-1);
        return -1;
    }
    
    rc = sqlite3_step(stmt);
    while (rc == SQLITE_ROW) {
        if (callback(opaque, row++, stmt) < 0)
            break;
        
        rc = sqlite3_step(stmt);
    }
    
    // sqlite3_reset(stmt);
    sqlite3_finalize(stmt);
    
    return 0;
}

int db_exec_sql(sqlite3 *db, const char *sql)
{
    return sqlite3_exec(db, sql, 0, 0, 0);
}

struct cbparamSelectCount
{
    int cnt;
};

static int cb_select_count(void *opaque, size_t row, sqlite3_stmt *stmt)
{
    struct cbparamSelectCount *cnt = (struct cbparamSelectCount*)opaque;
    
    // select COUNT(*) from ....
    if (sqlite3_column_count(stmt) == 1) {
        cnt->cnt = sqlite3_column_int(stmt, 0);
    }
    
    return -1;
}

/** 判断是否存在. 
 */
static int db_table_exist(sqlite3 *db, const char *name)
{
    char sql[128];
	int rc;
    struct cbparamSelectCount cnt = { 0 };
    
	_snprintf(sql, sizeof(sql), "SELECT COUNT(*) FROM sqlite_master where name=\"%s\"", name);
    rc = db_exec_select(db, sql, cb_select_count, &cnt);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "ERR: %s: db_exec_select err, %d\n", __func__, rc);
    }
    
    return cnt.cnt;
}

/** 初始化数据库. 
 */
int db_init(sqlite3 *db)
{
    fprintf(stdout, "INFO: %s calling...\n", __func__);
    /** 检查是否存在 host, service, 等...
     */
    if (!db_table_exist(db, "mse")) {
        const char *sql = SQL_CREATE_MSE;
        int rc = db_exec_sql(db, sql);
        if (rc != SQLITE_OK) {
            
        }
    }
    
    if (!db_table_exist(db, "host")) {
        const char *sql = SQL_CREATE_HOST;
        int rc = db_exec_sql(db, sql);
        if (rc != SQLITE_OK) {
            
        }
    }
    
    if (!db_table_exist(db, "service")) {
        const char *sql = SQL_CREATE_SERVICE;
        int rc = db_exec_sql(db, sql);
        if (rc != SQLITE_OK) {
            
        }
    }
    
    if (!db_table_exist(db, "token")) {
        const char *sql = SQL_CREATE_TOKEN_MAP;
        int rc = db_exec_sql(db, sql);
        if (rc != SQLITE_OK) {
            
        }
    }

	if (!db_table_exist(db, "config")) {
		char *sql = SQL_CREATE_CONFIG;
		int rc = db_exec_sql(db, sql);
		if (rc != SQLITE_OK) {

		}
	}
    
    // 清空 token table 中所有超时的. 
    fprintf(stdout, "INFO:    to remove timeouted tokens\n");
	{
		unsigned curr = (unsigned)time(0) - CHECK_INTERVAL;
		char *sql = (char*)alloca(1024);
		_snprintf(sql, 1024, "DELETE FROM token WHERE last_stamp < %u", curr);
		db_exec_sql(db, sql);
	}
    fprintf(stdout, "\tOK\n");
    
    return 0;
}

struct tmpParam
{
	struct dbhlpColumn *desc;
	int cn;

	struct dbhlpColumn **cols;
	int rows;
};

static int db_select_cb(void *opaque, size_t row, sqlite3_stmt *stmt)
{
	int i;
	struct tmpParam *p = (struct tmpParam*)opaque;
	p->rows = row+1;
	p->cols = (struct dbhlpColumn**)realloc(p->cols, p->rows * sizeof(struct dbhlpColumn*));

	// 保存一行. 
	p->cols[row] = (struct dbhlpColumn*)malloc(p->cn * sizeof(struct dbhlpColumn));
	for (i = 0; i < p->cn; i++) {
		
		p->cols[row]->type = p->desc[i].type;

		switch (p->desc[i].type) {
		case DBT_INT:
			p->cols[row][i].data.i = sqlite3_column_int(stmt, i);
			break;

		case DBT_DOUBLE:
			p->cols[row][i].data.d = sqlite3_column_double(stmt, i);
			break;

		case DBT_STRING:
			p->cols[row][i].data.s = strdup((char*)sqlite3_column_text(stmt, i)); // FIXME: 此处返回的是 utf-8
			break;

		default:
			fprintf(stderr, "ERR: %s: NOT supported type '%d'\n", __func__, p->desc[i].type);
			exit(-1);
			break;
		}
	}

	return 0;
}

int db_exec_select2(sqlite3 *db, const char *sql, struct dbhlpColumn col_desc[], int cn, struct dbhlpColumn ***cols, int *rows)
{
	struct tmpParam param = { col_desc, cn, 0, 0 };

	int rc = db_exec_select(db, sql, db_select_cb, &param);
	if (rc >= 0) {
		*cols = param.cols;
		*rows = param.rows;
	}
	else {
		*cols = 0, *rows = 0;
		return -1;
	}

	return 0;
}

void db_free_select2(struct dbhlpColumn col_desc[], int cn, struct dbhlpColumn **all, int rows)
{
	int r, c;

	if (all) {
		for (r = 0; r < rows; r++) {
			struct dbhlpColumn *row = all[r];
			for (c = 0; c < cn; c++) {
				switch (row[c].type) {
				case DBT_STRING:
					free(row[c].data.s); // 对应着 strdup()
					break;
				}
			}
			free(row); // 对应着 malloc()
		}

		free(all); // 对应着 realloc() ..
	}
}
