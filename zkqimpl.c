//
//  zkqimpl.c
//  zonekey_sysmgrt
//
//  Created by sunkw on 14-1-24.
//  Copyright (c) 2014年 sunkw. All rights reserved.
//

#include <stdio.h>
#include "soapStub.h"
#include "dbhlp.h"

int __zkq__getAllMses(struct soap *soap, enum xsd__boolean offline, struct _zkreg__Mses *mses)
{
    return SOAP_OK;
}

int __zkq__getAllHosts(struct soap *soap, enum xsd__boolean offline, struct _zkreg__Hosts *hosts)
{
    return SOAP_OK;
}

int __zkq__getAllServices(struct soap *soap, enum xsd__boolean offline, struct _zkreg__Services *services)
{
    return SOAP_OK;
}

int __zkq__getAllDevices(struct soap* soap, enum xsd__boolean offline, struct _zkreg__Devices *devices)
{
    return SOAP_OK;
}

int __zkq__getAllLogics(struct soap *soap, enum xsd__boolean offline, struct _zkreg__Logics *logics)
{
    return SOAP_OK;
}

int __zkq__getServicesByType(struct soap *soap, enum xsd__boolean offline, char *type, struct _zkreg__Services *services)
{
    return SOAP_OK;
}
