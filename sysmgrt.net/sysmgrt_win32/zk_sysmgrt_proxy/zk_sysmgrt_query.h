#pragma once

#include "zk_sysmgrt_common.h"

#ifdef __cplusplus
extern "C" {
#endif

/** ������� */
typedef enum zksysmgrt_Catalog
{
	T_HOST,
	T_SERVICE,
	T_DEVICE,
	T_LOGIC,
} zksysmgrt_Catalog;

/** һ����ַ����б�ips, children, urls ��ʹ�øýṹ */
typedef struct zksysmgrt_ss
{
	const char **ptr;
	int cnt;
} zksysmgrt_ss;

/** �������� */
typedef struct zksysmgrt_Host
{
	const char *name;
	const char *showname;

	const zksysmgrt_ss *ips;

} zksysmgrt_Host;

/** �������� */
typedef struct zksysmgrt_Service
{
	const char *name;
	const char *type;
	const char *showname;

	const char *hostname;

	const zksysmgrt_ss *urls;

	const char *version;
} zksysmgrt_Service;

/** logic ���� */
typedef struct zksysmgrt_Logic
{
	const char *name;
	const char *showname;

	const zksysmgrt_ss *children;

} zksysmgrt_Logic;

/** MSE ���� */
typedef struct zksysmgrt_Mse
{
	const char *name;
	zksysmgrt_Catalog catalog;
	const char *showname;

} zksysmgrt_Mse;

/** Brother ���� */
typedef struct zksysmgrt_Brother
{
	const char *parent;	/* ��ͬ�ĸ��ڵ� */
	const zksysmgrt_ss *brothers; /* �� parent �µ������ڵ� */

} zksysmgrt_Brother;

/* ��ѯ�������������� */
int zksysmgrt_q_getallhosts(zksysmgrt_t *sm, 
							int offline,				/* �Ƿ������������ */
							zksysmgrt_Host **hosts,	/* ���Σ��б�����ʹ�� zksysmgrt_q_freehosts() �ͷ� */
							int *num);					/* ���Σ����� hosts ��Ŀ */
void zksysmgrt_q_freehosts(zksysmgrt_t *sm,
						   zksysmgrt_Host *hosts,
						   int num);

/* ��ѯ�����ط��� */
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

/* ��ѯ������ָ�� name �� XXX �ṹ,
	���ر���ʹ�� zksysmgrt_q_freeXXXX(sm, XXX, 1) �����ͷ�
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

/* ��ѯ���õ���/�ֵܣ�mse ���ͣ���ϣ���õ���ϸ�����������ٴε��� zksysmgrt_q_getXXXX() */
int zksysmgrt_q_getparents(zksysmgrt_t *sm,
						   const char *name,
						   zksysmgrt_Mse **mses,
						   int *num);		/* ���ܶ�����ڵ� */
int zksysmgrt_q_getbrothers(zksysmgrt_t *sm,
							const char *name,
							zksysmgrt_Mse **mses,
							int *num);

#ifdef __cplusplus
}
#endif 
