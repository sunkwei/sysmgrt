#pragma once

#include "zk_sysmgrt_common.h"

#ifdef __cplusplus
extern "C" {
#endif

/** ����ע��
 */
int zksysmgrt_r_reghost(zksysmgrt_t *sm, 
						const char *name,	/* Ψһ�����֣�һ����Բ��� mac ��ַ֮��� ... */
						const char *showname,	/* ��ʾ���� */
						const char **ips,	/* ������ ip ��ַ�����ܶ�� */
						int ips_cnt);		/* ips ����Ŀ */
int zksysmgrt_r_unreghost(zksysmgrt_t *sm,
						  const char *name);

/** ����ע�� */
int zksysmgrt_r_regservice(zksysmgrt_t *sm,
						   const char *name,	/* �����Ψһ���֣�����Ψһ������ʹ�� mac + type + id ����� */
						   const char *hostname,	/* ���ڵ��������֣�һ��������� null */
						   const char *type,	/* �������ͣ��� eval, ptz , ... */
						   const char *showname, 
						   const char **urls,	/* ����� url �����ܶ�� */
						   int urls_cnt,		/* urls ����Ŀ */
						   const char *version);	/* ����İ汾�ţ�... */
int zksysmgrt_r_unregservice(zksysmgrt_t *sm,
							 const char *name);

/** �½��߼����� */
int zksysmgrt_r_addlogic(zksysmgrt_t *sm,
						 const char *name,		/* Ψһ�����֣�����ʹ�� uuid .. */
						 const char *showname);

/* ɾ���Ѿ�ע��Ķ��󣬿����� host, service, device, logic */
int zksysmgrt_r_delobj(zksysmgrt_t *sm,
					   const char *name);		/* name ��Ψһ�� host/service/device/logic ������ */

/* �� subname ��Ӧ�Ķ��󣬰󶨵� name ָ���� logic ������ */
int zksysmgrt_r_bind(zksysmgrt_t *sm,
					 const char *name,		/* �����Ӧ�� logic ���� */
					 const char *subname);	/* ������ host/service/device/logic */
int zksysmgrt_r_unbind(zksysmgrt_t *sm,
					   const char *name,
					   const char *subname);

/** �޸Ķ���� showname */
int zksysmgrt_r_setshowname(zksysmgrt_t *sm,
							const char *name,	/* ϣ���޸ĵĶ������� */
							const char *title); /* �µ���ʾ���� */


#ifdef __cplusplus
}
#endif 
