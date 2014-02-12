#pragma once

#include "zk_sysmgrt_common.h"

#ifdef __cplusplus
extern "C" {
#endif

/** 对象类别 */
typedef enum zksysmgrt_Catalog
{
	T_HOST,
	T_SERVICE,
	T_DEVICE,
	T_LOGIC,
} zksysmgrt_Catalog;

/** 一般的字符串列表，ips, children, urls 都使用该结构 */
typedef struct zksysmgrt_ss
{
	const char **ptr;
	int cnt;
} zksysmgrt_ss;

/** 主机描述 */
typedef struct zksysmgrt_Host
{
	const char *name;
	const char *showname;

	const zksysmgrt_ss *ips;

} zksysmgrt_Host;

/** 服务描述 */
typedef struct zksysmgrt_Service
{
	const char *name;
	const char *type;
	const char *showname;

	const char *hostname;

	const zksysmgrt_ss *urls;

	const char *version;
} zksysmgrt_Service;

/** logic 描述 */
typedef struct zksysmgrt_Logic
{
	const char *name;
	const char *showname;

	const zksysmgrt_ss *children;

} zksysmgrt_Logic;

/** MSE 描述 */
typedef struct zksysmgrt_Mse
{
	const char *name;
	zksysmgrt_Catalog catalog;
	const char *showname;

} zksysmgrt_Mse;

/** Brother 描述 */
typedef struct zksysmgrt_Brother
{
	const char *parent;	/* 共同的父节点 */
	const zksysmgrt_ss *brothers; /* 此 parent 下的其他节点 */

} zksysmgrt_Brother;

/* 查询，返回所有主机 */
int zksysmgrt_q_getallhosts(zksysmgrt_t *sm, 
							int offline,				/* 是否包含离线主机 */
							zksysmgrt_Host **hosts,	/* 出参，列表，必须使用 zksysmgrt_q_freehosts() 释放 */
							int *num);					/* 出参，返回 hosts 数目 */
void zksysmgrt_q_freehosts(zksysmgrt_t *sm,
						   zksysmgrt_Host *hosts,
						   int num);

/* 查询，返回服务 */
int zksysmgrt_q_getallservices(zksysmgrt_t *sm,
							   int offline,
							   zksysmgrt_Service **service,
							   int *num);
void zksysmgrt_q_freeservices(zksysmgrt_t *sm,
							  zksysmgrt_Service *service,
							  int num);
int zksysmgrt_q_getservicesbytype(zksysmgrt_t *sm,
								  int offline,
								  const char *type,
								  zksysmgrt_Service **service,
								  int *num);
void zksysmgrt_q_freemses(zksysmgrt_t *sm,
						 zksysmgrt_Mse *mses,
						 int num);
void zksysmgrt_q_freebrothers(zksysmgrt_t *sm,
							 zksysmgrt_Brother *brothers,
							 int num);

/* 查询，返回指定 name 的 XXX 结构,
	返回必须使用 zksysmgrt_q_freeXXXX(sm, XXX, 1) 进行释放
 */
int zksysmgrt_q_gethost(zksysmgrt_t *sm,
						const char *name,	/* host id */
						zksysmgrt_Host **host);
int zksysmgrt_q_getservice(zksysmgrt_t *sm,
						   const char *name,
						   zksysmgrt_Service **service);
int zksysmgrt_q_getlogic(zksysmgrt_t *sm,
						 const char *name,
						 zksysmgrt_Logic **logic);

/* 查询，得到父/兄弟，mse 类型，如希望得到详细描述，必须再次调用 zksysmgrt_q_getXXXX() */
int zksysmgrt_q_getparents(zksysmgrt_t *sm,
						   const char *name,
						   zksysmgrt_Mse **mses,
						   int *num);		/* 可能多个父节点 */
int zksysmgrt_q_getbrothers(zksysmgrt_t *sm,
							const char *name,
							zksysmgrt_Mse **mses,
							int *num);

#ifdef __cplusplus
}
#endif 
