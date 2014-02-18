//
//  heartBeatCheck.h
//  zonekey_sysmgrt
//
//  Created by sunkw on 14-1-25.
//  Copyright (c) 2014年 sunkw. All rights reserved.
//
//  实现心跳线程，每隔一段时间，检查一次 token table，将那些 last_stamp 超时的删除
//

#ifndef zonekey_sysmgrt_heartBeatCheck_h
#define zonekey_sysmgrt_heartBeatCheck_h

// 超时时间，5分钟
#define CHECK_INTERVAL (5*60)

#ifdef WIN32
#include <Windows.h>
DWORD __stdcall heartBeatCheck_run(void *param);
#else
void *heartBeatCheck_run(void *param);
#endif 

void heartCheck(sqlite3 *db);

#endif
