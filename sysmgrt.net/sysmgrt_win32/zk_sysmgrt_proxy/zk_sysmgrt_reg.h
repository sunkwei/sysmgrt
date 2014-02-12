#pragma once

#include "zk_sysmgrt_common.h"

#ifdef __cplusplus
extern "C" {
#endif

/** 主机注册
 */
int zksysmgrt_r_reghost(zksysmgrt_t *sm, 
						const char *name,	/* 唯一的名字，一般可以采用 mac 地址之类的 ... */
						const char *showname,	/* 显示名字 */
						const char **ips,	/* 主机的 ip 地址，可能多个 */
						int ips_cnt);		/* ips 的数目 */
int zksysmgrt_r_unreghost(zksysmgrt_t *sm,
						  const char *name);

/** 服务注册 */
int zksysmgrt_r_regservice(zksysmgrt_t *sm,
						   const char *name,	/* 服务的唯一名字，必须唯一，可以使用 mac + type + id 的组合 */
						   const char *hostname,	/* 所在的主机名字，一般可以设置 null */
						   const char *type,	/* 服务类型，如 eval, ptz , ... */
						   const char *showname, 
						   const char **urls,	/* 服务的 url ，可能多个 */
						   int urls_cnt,		/* urls 的数目 */
						   const char *version);	/* 服务的版本号，... */
int zksysmgrt_r_unregservice(zksysmgrt_t *sm,
							 const char *name);

/** 新建逻辑对象 */
int zksysmgrt_r_addlogic(zksysmgrt_t *sm,
						 const char *name,		/* 唯一的名字，建议使用 uuid .. */
						 const char *showname);

/* 删除已经注册的对象，可以是 host, service, device, logic */
int zksysmgrt_r_delobj(zksysmgrt_t *sm,
					   const char *name);		/* name 是唯一的 host/service/device/logic 的名字 */

/* 将 subname 对应的对象，绑定到 name 指定的 logic 对象上 */
int zksysmgrt_r_bind(zksysmgrt_t *sm,
					 const char *name,		/* 必须对应着 logic 对象 */
					 const char *subname);	/* 可以是 host/service/device/logic */
int zksysmgrt_r_unbind(zksysmgrt_t *sm,
					   const char *name,
					   const char *subname);

/** 修改对象的 showname */
int zksysmgrt_r_setshowname(zksysmgrt_t *sm,
							const char *name,	/* 希望修改的对象名字 */
							const char *title); /* 新的显示名字 */


#ifdef __cplusplus
}
#endif 
