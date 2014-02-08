/* soapStub.h
   Generated by gSOAP 2.8.17r from zonekey_sysmgrt.h

Copyright(C) 2000-2013, Robert van Engelen, Genivia Inc. All Rights Reserved.
The generated code is released under one of the following licenses:
GPL or Genivia's license for commercial use.
This program is released under the GPL with the additional exemption that
compiling, linking, and/or using OpenSSL is allowed.
*/

#ifndef soapStub_H
#define soapStub_H
#include "stdsoap2.h"
#if GSOAP_VERSION != 20817
# error "GSOAP VERSION MISMATCH IN GENERATED CODE: PLEASE REINSTALL PACKAGE"
#endif

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************\
 *                                                                            *
 * Enumerations                                                               *
 *                                                                            *
\******************************************************************************/


#ifndef SOAP_TYPE_xsd__boolean
#define SOAP_TYPE_xsd__boolean (7)
/* xsd:boolean */
enum xsd__boolean { xsd__boolean__false_ = 0, xsd__boolean__true_ = 1 };
#endif

#ifndef SOAP_TYPE_zkreg__Catalog
#define SOAP_TYPE_zkreg__Catalog (11)
/* zkreg:Catalog */
enum zkreg__Catalog { zkreg__Catalog__Host = 0, zkreg__Catalog__Service = 1, zkreg__Catalog__Device = 2, zkreg__Catalog__Logic = 3 };
#endif

/******************************************************************************\
 *                                                                            *
 * Types with Custom Serializers                                              *
 *                                                                            *
\******************************************************************************/


/******************************************************************************\
 *                                                                            *
 * Classes and Structs                                                        *
 *                                                                            *
\******************************************************************************/


#if 0 /* volatile type: do not declare here, declared elsewhere */

#endif

#ifndef SOAP_TYPE_zkreg__Ips
#define SOAP_TYPE_zkreg__Ips (12)
/* Sequence of zkreg:Ips schema type: */
struct zkreg__Ips
{
	char **__ptr;
	int __size;
};
#endif

#ifndef SOAP_TYPE_zkreg__Urls
#define SOAP_TYPE_zkreg__Urls (14)
/* Sequence of zkreg:Urls schema type: */
struct zkreg__Urls
{
	char **__ptr;
	int __size;
};
#endif

#ifndef SOAP_TYPE_zkreg__Mse
#define SOAP_TYPE_zkreg__Mse (16)
/* zkreg:Mse */
struct zkreg__Mse
{
	char *name;	/* required element of type xsd:string */
	enum zkreg__Catalog catalog;	/* required element of type zkreg:Catalog */
	char *showname;	/* optional element of type xsd:string */
};
#endif

#ifndef SOAP_TYPE_zkreg__Mses
#define SOAP_TYPE_zkreg__Mses (17)
/* Sequence of zkreg:Mses schema type: */
struct zkreg__Mses
{
	struct zkreg__Mse *__ptr;
	int __size;
};
#endif

#ifndef SOAP_TYPE_zkreg__Host
#define SOAP_TYPE_zkreg__Host (19)
/* zkreg:Host */
struct zkreg__Host
{
	char *name;	/* required element of type xsd:string */
	enum zkreg__Catalog catalog;	/* required element of type zkreg:Catalog */
	struct zkreg__Ips *ips;	/* required element of type zkreg:Ips */
	char *showname;	/* optional element of type xsd:string */
};
#endif

#ifndef SOAP_TYPE_zkreg__Hosts
#define SOAP_TYPE_zkreg__Hosts (21)
/* Sequence of zkreg:Hosts schema type: */
struct zkreg__Hosts
{
	struct zkreg__Host *__ptr;
	int __size;
};
#endif

#ifndef SOAP_TYPE_zkreg__Service
#define SOAP_TYPE_zkreg__Service (23)
/* zkreg:Service */
struct zkreg__Service
{
	char *name;	/* required element of type xsd:string */
	enum zkreg__Catalog catalog;	/* required element of type zkreg:Catalog */
	char *hostname;	/* required element of type xsd:string */
	char *type;	/* required element of type xsd:string */
	struct zkreg__Urls *urls;	/* required element of type zkreg:Urls */
	char *version;	/* optional element of type xsd:string */
	char *showname;	/* optional element of type xsd:string */
};
#endif

#ifndef SOAP_TYPE_zkreg__Services
#define SOAP_TYPE_zkreg__Services (25)
/* Sequence of zkreg:Services schema type: */
struct zkreg__Services
{
	struct zkreg__Service *__ptr;
	int __size;
};
#endif

#ifndef SOAP_TYPE_zkreg__Device
#define SOAP_TYPE_zkreg__Device (27)
/* zkreg:Device */
struct zkreg__Device
{
	char *name;	/* required element of type xsd:string */
	enum zkreg__Catalog catalog;	/* required element of type zkreg:Catalog */
	char *hostname;	/* required element of type xsd:string */
	char *type;	/* required element of type xsd:string */
	char *vendor;	/* optional element of type xsd:string */
	char *model;	/* optional element of type xsd:string */
	char *serial;	/* optional element of type xsd:string */
	char *version;	/* optional element of type xsd:string */
	char *showname;	/* optional element of type xsd:string */
};
#endif

#ifndef SOAP_TYPE_zkreg__Devices
#define SOAP_TYPE_zkreg__Devices (28)
/* Sequence of zkreg:Devices schema type: */
struct zkreg__Devices
{
	struct zkreg__Device *__ptr;
	int __size;
};
#endif

#ifndef SOAP_TYPE_zkreg__Logic
#define SOAP_TYPE_zkreg__Logic (30)
/* zkreg:Logic */
struct zkreg__Logic
{
	char *name;	/* required element of type xsd:string */
	enum zkreg__Catalog catalog;	/* required element of type zkreg:Catalog */
	struct zkreg__Mse *parent;	/* optional element of type zkreg:Mse */
	struct zkreg__Mses *children;	/* optional element of type zkreg:Mses */
	char *showname;	/* optional element of type xsd:string */
};
#endif

#ifndef SOAP_TYPE_zkreg__Logics
#define SOAP_TYPE_zkreg__Logics (32)
/* Sequence of zkreg:Logics schema type: */
struct zkreg__Logics
{
	struct zkreg__Logic *__ptr;
	int __size;
};
#endif

#ifndef SOAP_TYPE_zkcfg__keys
#define SOAP_TYPE_zkcfg__keys (34)
/* Sequence of zkcfg:keys schema type: */
struct zkcfg__keys
{
	char **__ptr;
	int __size;
};
#endif

#ifndef SOAP_TYPE_zkreg__regHostResponse
#define SOAP_TYPE_zkreg__regHostResponse (36)
/* zkreg:regHostResponse */
struct zkreg__regHostResponse
{
	char **token;	/* SOAP 1.2 RPC return element (when namespace qualified) */	/* optional element of type xsd:string */
};
#endif

#ifndef SOAP_TYPE_zkreg__regHost
#define SOAP_TYPE_zkreg__regHost (37)
/* zkreg:regHost */
struct zkreg__regHost
{
	struct zkreg__Host *zkreg__regHostReq;	/* optional element of type zkreg:Host */
};
#endif

#ifndef SOAP_TYPE_zkreg__unregHostResponse
#define SOAP_TYPE_zkreg__unregHostResponse (40)
/* zkreg:unregHostResponse */
struct zkreg__unregHostResponse
{
	int *code;	/* SOAP 1.2 RPC return element (when namespace qualified) */	/* optional element of type xsd:int */
};
#endif

#ifndef SOAP_TYPE_zkreg__unregHost
#define SOAP_TYPE_zkreg__unregHost (41)
/* zkreg:unregHost */
struct zkreg__unregHost
{
	char *zkreg__unRegHoststring;	/* optional element of type xsd:string */
};
#endif

#ifndef SOAP_TYPE_zkreg__regServiceResponse
#define SOAP_TYPE_zkreg__regServiceResponse (43)
/* zkreg:regServiceResponse */
struct zkreg__regServiceResponse
{
	char **token;	/* SOAP 1.2 RPC return element (when namespace qualified) */	/* optional element of type xsd:string */
};
#endif

#ifndef SOAP_TYPE_zkreg__regService
#define SOAP_TYPE_zkreg__regService (44)
/* zkreg:regService */
struct zkreg__regService
{
	struct zkreg__Service *zkreg__regServiceReq;	/* optional element of type zkreg:Service */
};
#endif

#ifndef SOAP_TYPE_zkreg__unregServiceResponse
#define SOAP_TYPE_zkreg__unregServiceResponse (46)
/* zkreg:unregServiceResponse */
struct zkreg__unregServiceResponse
{
	int *code;	/* SOAP 1.2 RPC return element (when namespace qualified) */	/* optional element of type xsd:int */
};
#endif

#ifndef SOAP_TYPE_zkreg__unregService
#define SOAP_TYPE_zkreg__unregService (47)
/* zkreg:unregService */
struct zkreg__unregService
{
	char *zkreg__unregServicestring;	/* optional element of type xsd:string */
};
#endif

#ifndef SOAP_TYPE_zkreg__regDeviceResponse
#define SOAP_TYPE_zkreg__regDeviceResponse (49)
/* zkreg:regDeviceResponse */
struct zkreg__regDeviceResponse
{
	char **token;	/* SOAP 1.2 RPC return element (when namespace qualified) */	/* optional element of type xsd:string */
};
#endif

#ifndef SOAP_TYPE_zkreg__regDevice
#define SOAP_TYPE_zkreg__regDevice (50)
/* zkreg:regDevice */
struct zkreg__regDevice
{
	struct zkreg__Device *zkreg__regDeviceReq;	/* optional element of type zkreg:Device */
};
#endif

#ifndef SOAP_TYPE_zkreg__unregDeviceResponse
#define SOAP_TYPE_zkreg__unregDeviceResponse (52)
/* zkreg:unregDeviceResponse */
struct zkreg__unregDeviceResponse
{
	int *code;	/* SOAP 1.2 RPC return element (when namespace qualified) */	/* optional element of type xsd:int */
};
#endif

#ifndef SOAP_TYPE_zkreg__unregDevice
#define SOAP_TYPE_zkreg__unregDevice (53)
/* zkreg:unregDevice */
struct zkreg__unregDevice
{
	char *zkreg__unregDevciestring;	/* optional element of type xsd:string */
};
#endif

#ifndef SOAP_TYPE_zkreg__heartBeatResponse
#define SOAP_TYPE_zkreg__heartBeatResponse (55)
/* zkreg:heartBeatResponse */
struct zkreg__heartBeatResponse
{
	int *code;	/* SOAP 1.2 RPC return element (when namespace qualified) */	/* optional element of type xsd:int */
};
#endif

#ifndef SOAP_TYPE_zkreg__heartBeat
#define SOAP_TYPE_zkreg__heartBeat (56)
/* zkreg:heartBeat */
struct zkreg__heartBeat
{
	char *zkreg__heartBeatstring;	/* optional element of type xsd:string */
};
#endif

#ifndef SOAP_TYPE_zkreg__setShowNameResponse
#define SOAP_TYPE_zkreg__setShowNameResponse (58)
/* zkreg:setShowNameResponse */
struct zkreg__setShowNameResponse
{
	int *code;	/* SOAP 1.2 RPC return element (when namespace qualified) */	/* optional element of type xsd:int */
};
#endif

#ifndef SOAP_TYPE_zkreg__setShowName
#define SOAP_TYPE_zkreg__setShowName (59)
/* zkreg:setShowName */
struct zkreg__setShowName
{
	char *zkreg__setShowNameReq;	/* optional element of type xsd:string */
	char *showname;	/* optional element of type xsd:string */
};
#endif

#ifndef SOAP_TYPE_zkq__getAllMsesResponse
#define SOAP_TYPE_zkq__getAllMsesResponse (61)
/* zkq:getAllMsesResponse */
struct zkq__getAllMsesResponse
{
	struct zkreg__Mses *mses;	/* SOAP 1.2 RPC return element (when namespace qualified) */	/* optional element of type zkreg:Mses */
};
#endif

#ifndef SOAP_TYPE_zkq__getAllMses
#define SOAP_TYPE_zkq__getAllMses (62)
/* zkq:getAllMses */
struct zkq__getAllMses
{
	enum xsd__boolean zkq__getAllMsesoffline;	/* required element of type xsd:boolean */
};
#endif

#ifndef SOAP_TYPE_zkq__getAllHostsResponse
#define SOAP_TYPE_zkq__getAllHostsResponse (65)
/* zkq:getAllHostsResponse */
struct zkq__getAllHostsResponse
{
	struct zkreg__Hosts *hosts;	/* SOAP 1.2 RPC return element (when namespace qualified) */	/* optional element of type zkreg:Hosts */
};
#endif

#ifndef SOAP_TYPE_zkq__getAllHosts
#define SOAP_TYPE_zkq__getAllHosts (66)
/* zkq:getAllHosts */
struct zkq__getAllHosts
{
	enum xsd__boolean zkq__getAllHostsoffline;	/* required element of type xsd:boolean */
};
#endif

#ifndef SOAP_TYPE_zkq__getAllServicesResponse
#define SOAP_TYPE_zkq__getAllServicesResponse (69)
/* zkq:getAllServicesResponse */
struct zkq__getAllServicesResponse
{
	struct zkreg__Services *services;	/* SOAP 1.2 RPC return element (when namespace qualified) */	/* optional element of type zkreg:Services */
};
#endif

#ifndef SOAP_TYPE_zkq__getAllServices
#define SOAP_TYPE_zkq__getAllServices (70)
/* zkq:getAllServices */
struct zkq__getAllServices
{
	enum xsd__boolean zkq__getAllServiceoffline;	/* required element of type xsd:boolean */
};
#endif

#ifndef SOAP_TYPE_zkq__getAllDevicesResponse
#define SOAP_TYPE_zkq__getAllDevicesResponse (73)
/* zkq:getAllDevicesResponse */
struct zkq__getAllDevicesResponse
{
	struct zkreg__Devices *devices;	/* SOAP 1.2 RPC return element (when namespace qualified) */	/* optional element of type zkreg:Devices */
};
#endif

#ifndef SOAP_TYPE_zkq__getAllDevices
#define SOAP_TYPE_zkq__getAllDevices (74)
/* zkq:getAllDevices */
struct zkq__getAllDevices
{
	enum xsd__boolean zkq__getAllDevicesoffline;	/* required element of type xsd:boolean */
};
#endif

#ifndef SOAP_TYPE_zkq__getAllLogicsResponse
#define SOAP_TYPE_zkq__getAllLogicsResponse (77)
/* zkq:getAllLogicsResponse */
struct zkq__getAllLogicsResponse
{
	struct zkreg__Logics *logics;	/* SOAP 1.2 RPC return element (when namespace qualified) */	/* optional element of type zkreg:Logics */
};
#endif

#ifndef SOAP_TYPE_zkq__getAllLogics
#define SOAP_TYPE_zkq__getAllLogics (78)
/* zkq:getAllLogics */
struct zkq__getAllLogics
{
	enum xsd__boolean zkq__getAllLogicsoffline;	/* required element of type xsd:boolean */
};
#endif

#ifndef SOAP_TYPE_zkq__getServicesByTypeResponse
#define SOAP_TYPE_zkq__getServicesByTypeResponse (80)
/* zkq:getServicesByTypeResponse */
struct zkq__getServicesByTypeResponse
{
	struct zkreg__Services *services;	/* SOAP 1.2 RPC return element (when namespace qualified) */	/* optional element of type zkreg:Services */
};
#endif

#ifndef SOAP_TYPE_zkq__getServicesByType
#define SOAP_TYPE_zkq__getServicesByType (81)
/* zkq:getServicesByType */
struct zkq__getServicesByType
{
	enum xsd__boolean zkq__getServiceByTypeoffline;	/* required element of type xsd:boolean */
	char *type;	/* optional element of type xsd:string */
};
#endif

#ifndef SOAP_TYPE_zkcfg__getAllKeysResponse
#define SOAP_TYPE_zkcfg__getAllKeysResponse (86)
/* zkcfg:getAllKeysResponse */
struct zkcfg__getAllKeysResponse
{
	struct zkcfg__keys *keys;	/* SOAP 1.2 RPC return element (when namespace qualified) */	/* optional element of type zkcfg:keys */
};
#endif

#ifndef SOAP_TYPE_zkcfg__getAllKeys
#define SOAP_TYPE_zkcfg__getAllKeys (87)
/* zkcfg:getAllKeys */
struct zkcfg__getAllKeys
{
	void *notuse;	/* transient */
};
#endif

#ifndef SOAP_TYPE_zkcfg__getValueResponse
#define SOAP_TYPE_zkcfg__getValueResponse (89)
/* zkcfg:getValueResponse */
struct zkcfg__getValueResponse
{
	char **value;	/* SOAP 1.2 RPC return element (when namespace qualified) */	/* optional element of type xsd:string */
};
#endif

#ifndef SOAP_TYPE_zkcfg__getValue
#define SOAP_TYPE_zkcfg__getValue (90)
/* zkcfg:getValue */
struct zkcfg__getValue
{
	char *zkcfg__getValuekey;	/* optional element of type xsd:string */
};
#endif

#ifndef SOAP_TYPE_zkcfg__setValue
#define SOAP_TYPE_zkcfg__setValue (92)
/* zkcfg:setValue */
struct zkcfg__setValue
{
	char *zkcfg__setValuekey;	/* optional element of type xsd:string */
	char *value;	/* optional element of type xsd:string */
};
#endif

#ifndef SOAP_TYPE_zkcfg__delKey
#define SOAP_TYPE_zkcfg__delKey (94)
/* zkcfg:delKey */
struct zkcfg__delKey
{
	char *zkcfg__delKeyKey;	/* optional element of type xsd:string */
};
#endif

#ifndef WITH_NOGLOBAL

#ifndef SOAP_TYPE_SOAP_ENV__Header
#define SOAP_TYPE_SOAP_ENV__Header (95)
/* SOAP Header: */
struct SOAP_ENV__Header
{
#ifdef WITH_NOEMPTYSTRUCT
	char dummy;	/* dummy member to enable compilation */
#endif
};
#endif

#endif

#ifndef WITH_NOGLOBAL

#ifndef SOAP_TYPE_SOAP_ENV__Code
#define SOAP_TYPE_SOAP_ENV__Code (96)
/* SOAP Fault Code: */
struct SOAP_ENV__Code
{
	char *SOAP_ENV__Value;	/* optional element of type xsd:QName */
	struct SOAP_ENV__Code *SOAP_ENV__Subcode;	/* optional element of type SOAP-ENV:Code */
};
#endif

#endif

#ifndef WITH_NOGLOBAL

#ifndef SOAP_TYPE_SOAP_ENV__Detail
#define SOAP_TYPE_SOAP_ENV__Detail (98)
/* SOAP-ENV:Detail */
struct SOAP_ENV__Detail
{
	char *__any;
	int __type;	/* any type of element <fault> (defined below) */
	void *fault;	/* transient */
};
#endif

#endif

#ifndef WITH_NOGLOBAL

#ifndef SOAP_TYPE_SOAP_ENV__Reason
#define SOAP_TYPE_SOAP_ENV__Reason (99)
/* SOAP-ENV:Reason */
struct SOAP_ENV__Reason
{
	char *SOAP_ENV__Text;	/* optional element of type xsd:string */
};
#endif

#endif

#ifndef WITH_NOGLOBAL

#ifndef SOAP_TYPE_SOAP_ENV__Fault
#define SOAP_TYPE_SOAP_ENV__Fault (100)
/* SOAP Fault: */
struct SOAP_ENV__Fault
{
	char *faultcode;	/* optional element of type xsd:QName */
	char *faultstring;	/* optional element of type xsd:string */
	char *faultactor;	/* optional element of type xsd:string */
	struct SOAP_ENV__Detail *detail;	/* optional element of type SOAP-ENV:Detail */
	struct SOAP_ENV__Code *SOAP_ENV__Code;	/* optional element of type SOAP-ENV:Code */
	struct SOAP_ENV__Reason *SOAP_ENV__Reason;	/* optional element of type SOAP-ENV:Reason */
	char *SOAP_ENV__Node;	/* optional element of type xsd:string */
	char *SOAP_ENV__Role;	/* optional element of type xsd:string */
	struct SOAP_ENV__Detail *SOAP_ENV__Detail;	/* optional element of type SOAP-ENV:Detail */
};
#endif

#endif

/******************************************************************************\
 *                                                                            *
 * Typedefs                                                                   *
 *                                                                            *
\******************************************************************************/

#ifndef SOAP_TYPE__QName
#define SOAP_TYPE__QName (5)
typedef char *_QName;
#endif

#ifndef SOAP_TYPE__XML
#define SOAP_TYPE__XML (6)
typedef char *_XML;
#endif

#ifndef SOAP_TYPE_xsd__anyURI
#define SOAP_TYPE_xsd__anyURI (8)
typedef char *xsd__anyURI;
#endif

#ifndef SOAP_TYPE_xsd__integer
#define SOAP_TYPE_xsd__integer (9)
typedef char *xsd__integer;
#endif

#ifndef SOAP_TYPE_xsd__string
#define SOAP_TYPE_xsd__string (10)
typedef char *xsd__string;
#endif


/******************************************************************************\
 *                                                                            *
 * Externals                                                                  *
 *                                                                            *
\******************************************************************************/


/******************************************************************************\
 *                                                                            *
 * Server-Side Operations                                                     *
 *                                                                            *
\******************************************************************************/


SOAP_FMAC5 int SOAP_FMAC6 zkreg__regHost(struct soap*, struct zkreg__Host *zkreg__regHostReq, char **token);

SOAP_FMAC5 int SOAP_FMAC6 zkreg__unregHost(struct soap*, char *zkreg__unRegHoststring, int *code);

SOAP_FMAC5 int SOAP_FMAC6 zkreg__regService(struct soap*, struct zkreg__Service *zkreg__regServiceReq, char **token);

SOAP_FMAC5 int SOAP_FMAC6 zkreg__unregService(struct soap*, char *zkreg__unregServicestring, int *code);

SOAP_FMAC5 int SOAP_FMAC6 zkreg__regDevice(struct soap*, struct zkreg__Device *zkreg__regDeviceReq, char **token);

SOAP_FMAC5 int SOAP_FMAC6 zkreg__unregDevice(struct soap*, char *zkreg__unregDevciestring, int *code);

SOAP_FMAC5 int SOAP_FMAC6 zkreg__heartBeat(struct soap*, char *zkreg__heartBeatstring, int *code);

SOAP_FMAC5 int SOAP_FMAC6 zkreg__setShowName(struct soap*, char *zkreg__setShowNameReq, char *showname, int *code);

SOAP_FMAC5 int SOAP_FMAC6 zkq__getAllMses(struct soap*, enum xsd__boolean zkq__getAllMsesoffline, struct zkreg__Mses *mses);

SOAP_FMAC5 int SOAP_FMAC6 zkq__getAllHosts(struct soap*, enum xsd__boolean zkq__getAllHostsoffline, struct zkreg__Hosts *hosts);

SOAP_FMAC5 int SOAP_FMAC6 zkq__getAllServices(struct soap*, enum xsd__boolean zkq__getAllServiceoffline, struct zkreg__Services *services);

SOAP_FMAC5 int SOAP_FMAC6 zkq__getAllDevices(struct soap*, enum xsd__boolean zkq__getAllDevicesoffline, struct zkreg__Devices *devices);

SOAP_FMAC5 int SOAP_FMAC6 zkq__getAllLogics(struct soap*, enum xsd__boolean zkq__getAllLogicsoffline, struct zkreg__Logics *logics);

SOAP_FMAC5 int SOAP_FMAC6 zkq__getServicesByType(struct soap*, enum xsd__boolean zkq__getServiceByTypeoffline, char *type, struct zkreg__Services *services);

SOAP_FMAC5 int SOAP_FMAC6 zkcfg__getAllKeys(struct soap*, void *notuse, struct zkcfg__keys *keys);

SOAP_FMAC5 int SOAP_FMAC6 zkcfg__getValue(struct soap*, char *zkcfg__getValuekey, char **value);

SOAP_FMAC5 int SOAP_FMAC6 zkcfg__setValue(struct soap*, char *zkcfg__setValuekey, char *value);

SOAP_FMAC5 int SOAP_FMAC6 zkcfg__delKey(struct soap*, char *zkcfg__delKeyKey);

/******************************************************************************\
 *                                                                            *
 * Server-Side Skeletons to Invoke Service Operations                         *
 *                                                                            *
\******************************************************************************/

SOAP_FMAC5 int SOAP_FMAC6 soap_serve(struct soap*);

SOAP_FMAC5 int SOAP_FMAC6 soap_serve_request(struct soap*);

SOAP_FMAC5 int SOAP_FMAC6 soap_serve_zkreg__regHost(struct soap*);

SOAP_FMAC5 int SOAP_FMAC6 soap_serve_zkreg__unregHost(struct soap*);

SOAP_FMAC5 int SOAP_FMAC6 soap_serve_zkreg__regService(struct soap*);

SOAP_FMAC5 int SOAP_FMAC6 soap_serve_zkreg__unregService(struct soap*);

SOAP_FMAC5 int SOAP_FMAC6 soap_serve_zkreg__regDevice(struct soap*);

SOAP_FMAC5 int SOAP_FMAC6 soap_serve_zkreg__unregDevice(struct soap*);

SOAP_FMAC5 int SOAP_FMAC6 soap_serve_zkreg__heartBeat(struct soap*);

SOAP_FMAC5 int SOAP_FMAC6 soap_serve_zkreg__setShowName(struct soap*);

SOAP_FMAC5 int SOAP_FMAC6 soap_serve_zkq__getAllMses(struct soap*);

SOAP_FMAC5 int SOAP_FMAC6 soap_serve_zkq__getAllHosts(struct soap*);

SOAP_FMAC5 int SOAP_FMAC6 soap_serve_zkq__getAllServices(struct soap*);

SOAP_FMAC5 int SOAP_FMAC6 soap_serve_zkq__getAllDevices(struct soap*);

SOAP_FMAC5 int SOAP_FMAC6 soap_serve_zkq__getAllLogics(struct soap*);

SOAP_FMAC5 int SOAP_FMAC6 soap_serve_zkq__getServicesByType(struct soap*);

SOAP_FMAC5 int SOAP_FMAC6 soap_serve_zkcfg__getAllKeys(struct soap*);

SOAP_FMAC5 int SOAP_FMAC6 soap_serve_zkcfg__getValue(struct soap*);

SOAP_FMAC5 int SOAP_FMAC6 soap_serve_zkcfg__setValue(struct soap*);

SOAP_FMAC5 int SOAP_FMAC6 soap_serve_zkcfg__delKey(struct soap*);

/******************************************************************************\
 *                                                                            *
 * Client-Side Call Stubs                                                     *
 *                                                                            *
\******************************************************************************/


SOAP_FMAC5 int SOAP_FMAC6 soap_call_zkreg__regHost(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct zkreg__Host *zkreg__regHostReq, char **token);

SOAP_FMAC5 int SOAP_FMAC6 soap_call_zkreg__unregHost(struct soap *soap, const char *soap_endpoint, const char *soap_action, char *zkreg__unRegHoststring, int *code);

SOAP_FMAC5 int SOAP_FMAC6 soap_call_zkreg__regService(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct zkreg__Service *zkreg__regServiceReq, char **token);

SOAP_FMAC5 int SOAP_FMAC6 soap_call_zkreg__unregService(struct soap *soap, const char *soap_endpoint, const char *soap_action, char *zkreg__unregServicestring, int *code);

SOAP_FMAC5 int SOAP_FMAC6 soap_call_zkreg__regDevice(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct zkreg__Device *zkreg__regDeviceReq, char **token);

SOAP_FMAC5 int SOAP_FMAC6 soap_call_zkreg__unregDevice(struct soap *soap, const char *soap_endpoint, const char *soap_action, char *zkreg__unregDevciestring, int *code);

SOAP_FMAC5 int SOAP_FMAC6 soap_call_zkreg__heartBeat(struct soap *soap, const char *soap_endpoint, const char *soap_action, char *zkreg__heartBeatstring, int *code);

SOAP_FMAC5 int SOAP_FMAC6 soap_call_zkreg__setShowName(struct soap *soap, const char *soap_endpoint, const char *soap_action, char *zkreg__setShowNameReq, char *showname, int *code);

SOAP_FMAC5 int SOAP_FMAC6 soap_call_zkq__getAllMses(struct soap *soap, const char *soap_endpoint, const char *soap_action, enum xsd__boolean zkq__getAllMsesoffline, struct zkreg__Mses *mses);

SOAP_FMAC5 int SOAP_FMAC6 soap_call_zkq__getAllHosts(struct soap *soap, const char *soap_endpoint, const char *soap_action, enum xsd__boolean zkq__getAllHostsoffline, struct zkreg__Hosts *hosts);

SOAP_FMAC5 int SOAP_FMAC6 soap_call_zkq__getAllServices(struct soap *soap, const char *soap_endpoint, const char *soap_action, enum xsd__boolean zkq__getAllServiceoffline, struct zkreg__Services *services);

SOAP_FMAC5 int SOAP_FMAC6 soap_call_zkq__getAllDevices(struct soap *soap, const char *soap_endpoint, const char *soap_action, enum xsd__boolean zkq__getAllDevicesoffline, struct zkreg__Devices *devices);

SOAP_FMAC5 int SOAP_FMAC6 soap_call_zkq__getAllLogics(struct soap *soap, const char *soap_endpoint, const char *soap_action, enum xsd__boolean zkq__getAllLogicsoffline, struct zkreg__Logics *logics);

SOAP_FMAC5 int SOAP_FMAC6 soap_call_zkq__getServicesByType(struct soap *soap, const char *soap_endpoint, const char *soap_action, enum xsd__boolean zkq__getServiceByTypeoffline, char *type, struct zkreg__Services *services);

SOAP_FMAC5 int SOAP_FMAC6 soap_call_zkcfg__getAllKeys(struct soap *soap, const char *soap_endpoint, const char *soap_action, void *notuse, struct zkcfg__keys *keys);

SOAP_FMAC5 int SOAP_FMAC6 soap_call_zkcfg__getValue(struct soap *soap, const char *soap_endpoint, const char *soap_action, char *zkcfg__getValuekey, char **value);

SOAP_FMAC5 int SOAP_FMAC6 soap_send_zkcfg__setValue(struct soap *soap, const char *soap_endpoint, const char *soap_action, char *zkcfg__setValuekey, char *value);

SOAP_FMAC5 int SOAP_FMAC6 soap_recv_zkcfg__setValue(struct soap *soap, struct zkcfg__setValue *notused);


SOAP_FMAC5 int SOAP_FMAC6 soap_send_zkcfg__delKey(struct soap *soap, const char *soap_endpoint, const char *soap_action, char *zkcfg__delKeyKey);

SOAP_FMAC5 int SOAP_FMAC6 soap_recv_zkcfg__delKey(struct soap *soap, struct zkcfg__delKey *notused);


#ifdef __cplusplus
}
#endif

#endif

/* End of soapStub.h */
