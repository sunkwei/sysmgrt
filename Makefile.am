bin_PROGRAMS= sysmgrt test_client
sysmgrt_SOURCES=	dbhlp.c dboper.c heartBeatCheck.c zkqimpl.c zkregimpl.c zkcfgimpl.c \
			soapC.c soapServer.c zonekey_sysmgrt/main.c
sysmgrt_CFLAGS=		-std=c99
sysmgrt_LDADD=	-lgsoap -lsqlite3

test_client_SOURCES=	test.c soapC.c soapClient.c
test_client_CFLAGS=	-std=c99
test_client_LDADD= -lgsoap -lpthread

