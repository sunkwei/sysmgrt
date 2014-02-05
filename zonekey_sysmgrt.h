/** zonekey system mgrt
 */

//gsoap zkreg service namespace: http://www.zonekey.com/sysmgrt/register
//gsoap zkreg service name: zkreg
//gsoap zkreg service port: http://localhost:8899
//gsoap zkreg schema form: qualified
//gsoap zkreg schema namespace: http://www.zonekey.com/sysmgrt/register

//gsoap zkq service namespace: http://www.zonekey.com/sysmgrt/query
//gsoap zkq service name: zkquery
//gsoap zkq service port: http://localhost:8899
//gsoap zkq schema form: qualified
//gsoap zkq schema namespace: http://www.zonekey.com/sysmgrt/query


/// Built-in type "SOAP-ENV:Envelope".
//struct SOAP_ENV__Envelope { struct SOAP_ENV__Header *SOAP_ENV__Header; _XML SOAP_ENV__Body; };

/// Built-in type "xs:base64Binary".
struct xsd__base64Binary
{	unsigned char *__ptr;
	int __size;
	char *id, *type, *options; // NOTE: non-NULL for DIMEM/MIME/MTOM XOP attachments only
};

/// Built-in type "xs:boolean".
enum xsd__boolean { xsd__boolean__false_, xsd__boolean__true_ };

/// Built-in type "xs:hexBinary".
struct xsd__hexBinary { unsigned char *__ptr; int __size; };

// Imported type "http://www.w3.org/2005/08/addressing":EndpointReferenceType defined by wsa5__EndpointReferenceType

/// Primitive built-in type "xs:NCName"
typedef char* xsd__NCName;

/// Primitive built-in type "xs:anySimpleType"
typedef char* xsd__anySimpleType;

/// Primitive built-in type "xs:anyURI"
typedef char* xsd__anyURI;

/// Primitive built-in type "xs:integer"
typedef char* xsd__integer;

/// Primitive built-in type "xs:nonNegativeInteger"
typedef char* xsd__nonNegativeInteger;

/// Primitive built-in type "xs:string"
typedef char* xsd__string;

// Imported element "http://www.w3.org/2004/08/xop/include":Include declared as _xop__Include

/// Built-in attribute "xml:lang".
typedef char* _xml__lang;

/** 声明 */
/** Host，Service, Device, Logic  */
enum zkreg__Catalog
{
	zkreg__Catalog__Host,		// 主机类型
	zkreg__Catalog__Service,	// 服务类型
	zkreg__Catalog__Device,	// 设备类型
	zkreg__Catalog__Logic,		// 逻辑类型
};

// ip array
struct zkreg__Ips
{
	xsd__string 		*__ptr		0;
	int			__size		0;
};

// xaddr array
struct zkreg__Urls
{
	xsd__anyURI 		*__ptr		0;
	int			__size		0;
};

/** 基类型
 */
struct zkreg__Mse
{
	xsd__string		name		1;	// 唯一名字
	enum zkreg__Catalog	catalog		1;	// 类型
	xsd__string		showname	0;	// 显示名字
};

/** mse 列表 */
struct zkreg__Mses
{
	struct zkreg__Mse	*__ptr		0;	
	int			__size		0;
};

/** 主机类型 */
struct zkreg__Host
{
	xsd__string		name		1;
	enum zkreg__Catalog	catalog		1;	// 类别，必须为 zkreg__Catalog__Host
	struct zkreg__Ips	*ips		1;
	xsd__string		showname	0;
};

// 主机列表
struct zkreg__Hosts
{
	struct zkreg__Host	*__ptr		0;
	int			__size		0;
};

/** 服务 */
struct zkreg__Service
{
	xsd__string		name		1;	// 服务名字.
	enum zkreg__Catalog	catalog		1;	// 类别，必须为 zkreg__Catalog__Service
	xsd__string		hostname	1;	// 绑定的主机名字.
	xsd__string		type		1;	// 服务类型.
	struct zkreg__Urls	*urls		1;	// 各种url
	xsd__string		version		0;	// 版本号，可选.
	xsd__string		showname	0;	// 可选的显示名字.
};

// 服务列表
struct zkreg__Services
{
	struct zkreg__Service	*__ptr		0;
	int			__size		0;
};

/** 设备 */
struct zkreg__Device
{
	xsd__string		name		1;
	enum zkreg__Catalog	catalog		1;	// 类别，必须为 zkreg__Catalog__Device
	xsd__string		hostname	1;
	xsd__string		type		1;	// 设备类型，如ptz, encoder ...
	xsd__string		vendor		0;
	xsd__string		model		0;
	xsd__string		serial		0;
	xsd__string		version		0;
	xsd__string		showname	0;
};

// 设备列表
struct zkreg__Devices
{
	struct zkreg__Device	*__ptr		0;
	int			__size		0;
};

/** 逻辑服务 */
struct zkreg__Logic
{
	xsd__string		name		1;
	enum zkreg__Catalog	catalog		1;	// 类别，必须为 zkreg__Catalog__Logic
	struct zkreg__Mse	*parent		0;	// 可选的父对象
	struct zkreg__Mses	*children	0;	// 可选的子对象
	xsd__string		showname	0;
};

struct zkreg__Logics
{
	struct zkreg__Logic 	*__ptr		0;
	int			__size		0;
};

// 主机注册, token 为返回
int zkreg__regHost(struct zkreg__Host *zkreg__regHostReq, xsd__string *token);
int zkreg__unregHost(xsd__string zkreg__unRegHoststring, int *code);

// 服务注册
int zkreg__regService(struct zkreg__Service *zkreg__regServiceReq, xsd__string *token);
int zkreg__unregService(xsd__string zkreg__unregServicestring, int *code);

// 设备注册
int zkreg__regDevice(struct zkreg__Device *zkreg__regDeviceReq, xsd__string *token);
int zkreg__unregDevice(xsd__string zkreg__unregDevciestring, int *code);

// 心跳
int zkreg__heartBeat(xsd__string zkreg__heartBeatstring, int *code);

// 修改 mse 显示信息
int zkreg__setShowName(xsd__string zkreg__setShowNameReq, xsd__string showname, int *code);

// 逻辑设备的绑定

//////////////////////////////////////////////////////////

// 查询返回所有 mse 对象
int zkq__getAllMses(enum xsd__boolean zkq__getAllMsesoffline, struct zkreg__Mses *mses);

// 返回所有主机
int zkq__getAllHosts(enum xsd__boolean zkq__getAllHostsoffline, struct zkreg__Hosts *hosts);

// 返回所有服务
int zkq__getAllServices(enum xsd__boolean zkq__getAllServiceoffline, struct zkreg__Services *services);

// 返回所有设备
int zkq__getAllDevices(enum xsd__boolean zkq__getAllDevicesoffline, struct zkreg__Devices *devices);

// 返回所有 logic 对象
int zkq__getAllLogics(enum xsd__boolean zkq__getAllLogicsoffline, struct zkreg__Logics *logics);

// 根据服务类型查询服务
int zkq__getServicesByType(enum xsd__boolean zkq__getServiceByTypeoffline, xsd__string type, struct zkreg__Services *services);

