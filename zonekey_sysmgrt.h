/** zonekey system mgrt

  	add 
# zonekey ...
zkreg	= "http://www.zonekey.com/ver09/sysmgrt/reg"
zkq	= "http://www.zonekey.com/ver09/sysmgrt/query"

 */

/******************************************************************************\
 *                                                                            *
 * Import                                                                     *
 *                                                                            *
\******************************************************************************/

#import "wsdd10.h"	// wsdd10 = <http://schemas.xmlsoap.org/ws/2005/04/discovery>
#import "xop.h"	// xop = <http://www.w3.org/2004/08/xop/include>
#import "wsa5.h"	// wsa5 = <http://www.w3.org/2005/08/addressing>

/******************************************************************************\
 *                                                                            *
 * Built-in Schema Types and Top-Level Elements and Attributes                *
 *                                                                            *
\******************************************************************************/


/// Built-in type "SOAP-ENV:Envelope".
struct SOAP_ENV__Envelope { struct SOAP_ENV__Header *SOAP_ENV__Header; _XML SOAP_ENV__Body; };

/// Built-in type "xs:base64Binary".
struct xsd__base64Binary
{	unsigned char *__ptr;
	int __size;
	char *id, *type, *options; // NOTE: non-NULL for DIMEM/MIME/MTOM XOP attachments only
};

/// Built-in type "xs:boolean".
enum xsd__boolean { xsd__boolean__false_, xsd__boolean__true_ };

/// Built-in type "xs:duration".
#import "custom/duration.h"

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

/// Primitive built-in type "xs:token"
typedef char* xsd__token;

// Imported element "http://www.w3.org/2004/08/xop/include":Include declared as _xop__Include

/// Built-in attribute "xml:lang".
typedef char* _xml__lang;

/** 声明 */
typedef xsd__token zkreg__Hostname;
typedef xsd__token zkreg__Showname;
typedef xsd__token zkreg__Ip;
typedef xsd__token zkq__Hostname;
typedef xsd__token zkq__Showname;

/** 主机 */
struct _zkreg__RegHost
{
	zkreg__Hostname hostName	1;	// 
	zkreg__Showname showName	1;	//
	$int		__sizeIp	1;	// array of ips
	zkreg__Ip 	*ips		1;	
};

struct _zkreg__RegHostResponse
{
	xsd__token	token		1;	// 
};

int __zkreg__regHost(struct _zkreg__RegHost *req, struct _zkreg__RegHostResponse *res);

