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
#include <assert.h>
#include "soapStub.h"
#include "dbhlp.h"
#include "dboper.h"

static void ut_copy_array(struct soap *soap, char ***ptr, int *size, char *data, char *split)
{
	/** data 通过 split 分割的字符串，分解保存到 ptr, size 中 */
	char **p = 0, *q;
	int n = 0, i;

	q = strtok(data, split);
	while (q) {
		p = (char**)realloc(p, (n+1) * sizeof(char*));
		p[n] = strdup(q);
		n++;
		q = strtok(0, split);
	}

	*size = n;

	if (n == 0)
		*ptr = 0;
	else {
		*ptr = (char**)soap_malloc(soap, n * sizeof(char*));

		for (i = 0; i < n; i++) {
			(*ptr)[i] = soap_strdup(soap, p[i]);
			free(p[i]);
		}
		free(p);
	}
}

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
	int n = sizeof(desc) / sizeof(struct dbhlpColumn);

	if (offline) {
		snprintf(st, 1024, "SELECT name,parent,catalog,showname FROM mse");
	}
	else {
		// 这次仅仅返回 mse
		snprintf(st, 1024, "SELECT mse.name,mse.parent,mse.catalog,mse.showname FROM mse JOIN token ON mse.name=token.name WHERE catalog!=%d", 
			zkreg__Catalog__Logic);
	}

	db_exec_select2(_db, st, desc, n, &all, &rows);

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

	db_free_select2(desc, n, all, rows);
	db_free_select2(desc, n, all_logic, row_logic);

	return SOAP_OK;
}

int zkq__getMsesByShowname(struct soap *soap, char *showname, struct zkreg__Mses *mses)
{
	char *st = (char*)alloca(1024);
	struct dbhlpColumn desc[] = {
		{ {0}, DBT_STRING },	// mse.name
		{ {0}, DBT_INT },		// mse.catalog
		{ {0}, DBT_STRING },	// parent
		{ {0}, DBT_STRING },	// showname
	};
	struct dbhlpColumn **all = 0;
	int rows = 0, i;

	snprintf(st, 1024, "SELECT name,catalog,parent,showname FROM mse WHERE showname='%s'", showname);
	db_exec_select2(_db, st, desc, sizeof(desc)/sizeof(struct dbhlpColumn), &all, &rows);

	if (rows > 0) {
		mses->__size = rows;
		mses->__ptr = (struct zkreg__Mse*)soap_malloc(soap, rows * sizeof(struct zkreg__Mse));

		for (i = 0; i < rows; i++) {
			struct zkreg__Mse *p = &mses->__ptr[i];
			p->name = soap_strdup(soap, all[i][0].data.s);
			p->catalog = (enum zkreg__Catalog)all[i][1].data.i;
			p->parent = soap_strdup(soap, all[i][2].data.s);
			p->showname = soap_strdup(soap, all[i][3].data.s);
		}
	}
	else {
		mses->__size = 0;
		mses->__ptr = 0;
	}

	db_free_select2(desc, sizeof(desc)/sizeof(struct dbhlpColumn), all, rows);

	return SOAP_OK;
}

int zkq__getAllHosts(struct soap *soap, enum xsd__boolean offline, struct zkreg__Hosts *hosts)
{
	char *st = (char*)alloca(1024);

	struct dbhlpColumn desc[] = {
		{ { 0 }, DBT_STRING },	// host.name
		{ { 0 }, DBT_STRING },	// host.ips
		{ { 0 }, DBT_STRING },	// mse.showname
		{ { 0 }, DBT_STRING },	// mse.parent
	};

	int n = sizeof(desc) / sizeof(struct dbhlpColumn);
	struct dbhlpColumn **all = 0;
	int rows = 0, i, j;

    /** 根据 offline，从 host, mse, token 中返回查询记录，从将每行信息分配保存到 zkreg__Host 结构中.
     */
    if (offline) {
        snprintf(st, 1024, "SELECT host.name,host.ips,mse.showname,mse.parent FROM host"
                 " JOIN mse ON mse.name=host.name");
    }
    else {
        snprintf(st, 1024, "SELECT host.name,host.ips,mse.showname,mse.parent FROM host"
                 " JOIN mse ON mse.name=host.name"
                 " JOIN token ON token.name=host.name");
    }

	db_exec_select2(_db, st, desc, n, &all, &rows);

	hosts->__size = rows;
	hosts->__ptr = (struct zkreg__Host*)soap_malloc(soap, sizeof(struct zkreg__Host) * rows);

	for (i = 0; i < rows; i++) {
		hosts->__ptr[i].name = soap_strdup(soap, all[i][0].data.s);
		hosts->__ptr[i].catalog = zkreg__Catalog__Host;
		ut_copy_array(soap, &hosts->__ptr[i].ips->__ptr, &hosts->__ptr[i].ips->__size, all[i][1].data.s, ",");
		hosts->__ptr[i].showname = soap_strdup(soap, all[i][2].data.s);
		hosts->__ptr[i].parent = soap_strdup(soap, all[i][3].data.s);
	}

	db_free_select2(desc, n, all, rows);

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
	// TODO:
	devices->__ptr = 0;
	devices->__size = 0;

    return SOAP_OK;
}

int zkq__getAllLogics(struct soap *soap, enum xsd__boolean offline, struct zkreg__Logics *logics)
{
	char *st = (char*)alloca(1024);
	struct dbhlpColumn desc[] = {
		{ {0}, DBT_STRING },		// name
		{ {0}, DBT_STRING },		// parent
		{ {0}, DBT_STRING },		// showname
		{ {0}, DBT_STRING },		// children
	};
	struct dbhlpColumn **all = 0;
	int rows = 0;

	snprintf(st, 1024, "SELECT mse.name,mse.parent,mse.showname,logic.children"
		" FROM mse JOIN logic ON mse.name=logic.name");

	db_exec_select2(_db, st, desc, sizeof(desc)/sizeof(struct dbhlpColumn), &all, &rows);

	if (rows > 0) {
		int i;

		logics->__size = rows;
		logics->__ptr = (struct zkreg__Logic*)soap_malloc(soap, sizeof(struct zkreg__Logic) * rows);

		for (i = 0; i < rows; i++) {
			struct zkreg__Logic *p = &logics->__ptr[i];

			p->name = soap_strdup(soap, all[i][0].data.s);
			p->catalog = zkreg__Catalog__Logic;
			p->parent = soap_strdup(soap, all[i][1].data.s);
			p->showname = soap_strdup(soap, all[i][2].data.s);
			ut_copy_array(soap, &p->children, &p->childnum, all[i][3].data.s, "\n");
		}
	}
	else {
		logics->__size = 0;
		logics->__ptr = 0;
	}

	db_free_select2(desc, sizeof(desc)/sizeof(struct dbhlpColumn), all, rows);
	
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

int zkq__getParent(struct soap *soap, char *name, struct zkreg__Logics *ls)
{
	char *st = (char*)alloca(1024);
	struct dbhlpColumn desc[] = {
		{ {0}, DBT_STRING },		// name
		{ {0}, DBT_STRING },		// parent
		{ {0}, DBT_STRING },		// showname
		{ {0}, DBT_STRING },		// children
	};
	struct dbhlpColumn **all = 0;
	int rows = 0;

	snprintf(st, 1024, "SELECT mse.name,mse.parent,mse.showname,logic.children"
		" FROM mse JOIN logic ON mse.name=logic.name");

	db_exec_select2(_db, st, desc, sizeof(desc)/sizeof(struct dbhlpColumn), &all, &rows);

	if (rows > 0) {
		int i, n = 0;
		// 首先需要知道有多少个parent，才能分配 ls 的内存.
		for (i = 0; i < rows; i++) {
			if (strstr(all[i][3].data.s, name))
				n++;
		}

		ls->__size = n;
		if (n == 0) {
			ls->__ptr = 0;
		}
		else {
			int m = 0;
			struct zkreg__Logic *p;

			ls->__ptr = (struct zkreg__Logic*)soap_malloc(soap, n * sizeof(struct zkreg__Logic));

			for (i = 0; i < rows; i++) {
				if (!strstr(all[i][3].data.s, name))
					continue;

				p = &ls->__ptr[m];				
				p->name = soap_strdup(soap, all[i][0].data.s);
				p->parent = soap_strdup(soap, all[i][1].data.s);
				p->showname = soap_strdup(soap, all[i][2].data.s);
				ut_copy_array(soap, &p->children, &p->childnum, all[i][3].data.s, "\n");

				m++;
			}

			assert(n == m);
		}
	}
	else {
		ls->__ptr = 0;
		ls->__size = 0;
	}

	db_free_select2(desc, sizeof(desc)/sizeof(struct dbhlpColumn), all, rows);

	return SOAP_OK;
}

int zkq__getBrothers(struct soap *soap, char *name, struct zkq__Brothers *brothers)
{
	/** 先查询所 parents，然后再获取每个 logic 对象.
	 */

	char *st = (char*)alloca(1024);
	struct zkreg__Logics parents;

	brothers->__ptr = 0;
	brothers->__size = 0;

	zkq__getParent(soap, name, &parents);

	if (parents.__size > 0) {
		int i, n = 0;

		// 首先得算出所有 brother 的数目.
		for (i = 0; i < parents.__size; i++) {
			n += parents.__ptr[i].childnum;
			n -= 1;	// XXX: 必须剔除自己！！！.
		}

		if (n > 0) {
			int m = 0;

			brothers->__ptr = (struct zkq__Brother*)soap_malloc(soap, n * sizeof(struct zkq__Brother));
			brothers->__size = n;

			for (i = 0; i < parents.__size; i++) {
				struct zkq__Brother *b = &brothers->__ptr[m];
				struct zkreg__Logic *l = &parents.__ptr[i];
				int j;

				b->parent = soap_strdup(soap, l->name);
				b->brothernum = l->childnum - 1;
				b->brothers = (char**)soap_malloc(soap, b->brothernum * sizeof(char*));
				for (j = 0; j < l->childnum; j++) {
					if (!strcmp(name, l->children[j])) continue;	// 不保存自己.
					b->brothers[j] = soap_strdup(soap, l->children[j]);
				}

				m++;
			}
		}
	}

	return SOAP_OK;
}

int zkq__getMseDesc(struct soap *soap, char *name, struct zkreg__Mse **mse)
{
	char *st = (char*)alloca(1024);
	struct dbhlpColumn desc[] = {
		{ { 0 }, DBT_STRING, },		// name
		{ { 0 }, DBT_INT, },		// catalog
		{ { 0 }, DBT_STRING, },		// parent
		{ { 0 }, DBT_STRING, },		// showname
	};
	struct dbhlpColumn **all = 0;
	int rows = 0;

	snprintf(st, 1024, "SELECT name,catalog,parent,showname FROM mse WHERE name='%s'", name);
	db_exec_select2(_db, st, desc, sizeof(desc)/sizeof(struct dbhlpColumn), &all, &rows);

	if (rows > 0) {
		struct zkreg__Mse *p = (struct zkreg__Mse*)soap_malloc(soap, sizeof(struct zkreg__Mse));

		assert(rows == 1);
		p->name = soap_strdup(soap, all[0][0].data.s);
		p->catalog = (enum zkreg__Catalog)all[0][1].data.i;
		p->parent = soap_strdup(soap, all[0][2].data.s);
		p->showname = soap_strdup(soap, all[0][3].data.s);

		*mse = p;
	}
	else {
		*mse = 0;
	}

	db_free_select2(desc, sizeof(desc)/sizeof(struct dbhlpColumn), all, rows);

	return SOAP_OK;
}

int zkq__getHostDesc(struct soap *soap, char *name, struct zkreg__Host **host)
{
	char *st = (char*)alloca(1024);
	struct dbhlpColumn desc[] = {
		{ {0}, DBT_STRING },	// name
		{ {0}, DBT_STRING },	// parent
		{ {0}, DBT_STRING },	// showname
		{ {0}, DBT_STRING },	// ips
	};
	struct dbhlpColumn **all = 0;
	int rows = 0;

	snprintf(st, 1024, "SELECT mse.name,mse.parent,mse.showname,host.ips FROM mse JOIN host ON mse.name=host.name WHERE mse.name='%s'", name);
	db_exec_select2(_db, st, desc, sizeof(desc)/sizeof(struct dbhlpColumn), &all, &rows);

	if (rows > 0) {
		struct zkreg__Host *p = (struct zkreg__Host*)soap_malloc(soap, sizeof(struct zkreg__Host));

		assert(rows == 1);
		p->name = soap_strdup(soap, all[0][0].data.s);
		p->parent = soap_strdup(soap, all[0][1].data.s);
		p->showname = soap_strdup(soap, all[0][2].data.s);
		ut_copy_array(soap, &p->ips->__ptr, &p->ips->__size, all[0][3].data.s, "\n");

		*host = p;
	}
	else {
		*host = 0;
	}

	db_free_select2(desc, sizeof(desc)/sizeof(struct dbhlpColumn), all, rows);

	return SOAP_OK;
}

int zkq__getServiceDesc(struct soap *soap, char *name, struct zkreg__Service **service)
{
	char *st = (char*)alloca(1024);
	struct dbhlpColumn desc[] = {
		{ {0}, DBT_STRING },		// name
		{ {0}, DBT_STRING },		// parent
		{ {0}, DBT_STRING },		// showname
		{ {0}, DBT_STRING },		// hostname
		{ {0}, DBT_STRING },		// type
		{ {0}, DBT_STRING },		// urls
		{ {0}, DBT_STRING },		// version
	};
	struct dbhlpColumn **all = 0;
	int rows = 0;

	snprintf(st, 1024, "SELECT mse.name,mse.parent,mse.showname,service.hostname,service.type,service.urls,service.version"
		" FROM mse JOIN service ON mse.name=service.name"
		" WHERE mse.name='%s'",
		name);
	db_exec_select2(_db, st, desc, sizeof(desc)/sizeof(struct dbhlpColumn), &all, &rows);

	if (rows > 0) {
		struct zkreg__Service *p = (struct zkreg__Service*)soap_malloc(soap, sizeof(struct zkreg__Service));

		assert(rows == 1);

		p->name = soap_strdup(soap, all[0][0].data.s);
		p->parent = soap_strdup(soap, all[0][1].data.s);
		p->showname = soap_strdup(soap, all[0][2].data.s);
		p->hostname = soap_strdup(soap, all[0][3].data.s);
		p->type = soap_strdup(soap, all[0][4].data.s);
		ut_copy_array(soap, &p->urls->__ptr, &p->urls->__size, all[0][5].data.s, "\n");
		p->version = soap_strdup(soap, all[0][6].data.s);

		*service = p;
	}
	else {
		*service = 0;
	}

	db_free_select2(desc, sizeof(desc)/sizeof(struct dbhlpColumn), all, rows);

	return SOAP_OK;
}

int zkq__getDeviceDesc(struct soap *soap, char *name, struct zkreg__Device **device)
{
	// TODO:
	*device = 0;
	return SOAP_OK;
}

int zkq__getLogicDesc(struct soap *soap, char *name, struct zkreg__Logic **logic)
{
	char *st = (char*)alloca(1024);
	struct dbhlpColumn desc[] = {
		{ {0}, DBT_STRING },		// name
		{ {0}, DBT_STRING },		// parent
		{ {0}, DBT_STRING },		// showname
		{ {0}, DBT_STRING },		// children
	};
	struct dbhlpColumn **all = 0;
	int rows = 0;

	snprintf(st, 1024, "SELECT mse.name,mse.parent,mse.showname,logic.children"
		" FROM mse JOIN logic ON mse.name=logic.name"
		" WHERE mse.name='%s'",
		name);

	db_exec_select2(_db, st, desc, sizeof(desc)/sizeof(struct dbhlpColumn), &all, &rows);

	if (rows > 0) {
		struct zkreg__Logic *p = (struct zkreg__Logic*)soap_malloc(soap, sizeof(struct zkreg__Logic));

		assert(rows == 1);

		p->name = soap_strdup(soap, all[0][0].data.s);
		p->parent = soap_strdup(soap, all[0][1].data.s);
		p->showname = soap_strdup(soap, all[0][2].data.s);
		ut_copy_array(soap, &p->children, &p->childnum, all[0][3].data.s, "\n");

		*logic = p;
	}
	else {
		*logic = 0;
	}

	db_free_select2(desc, sizeof(desc)/sizeof(struct dbhlpColumn), all, rows);

	return SOAP_OK;
}
