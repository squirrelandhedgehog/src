#	$OpenBSD: Makefile,v 1.18 2022/01/19 11:00:56 martijn Exp $

PROG=		snmpd
MAN=		snmpd.8 snmpd.conf.5
SRCS=		parse.y log.c snmpe.c application.c application_legacy.c \
		    mps.c trap.c mib.c smi.c kroute.c snmpd.c timer.c \
		    pf.c proc.c usm.c traphandler.c util.c

LDADD=		-levent -lutil -lkvm -lcrypto
DPADD=		${LIBEVENT} ${LIBUTIL}
CFLAGS+=	-Wall -I${.CURDIR}
CFLAGS+=	-Wstrict-prototypes -Wmissing-prototypes
CFLAGS+=	-Wmissing-declarations
CFLAGS+=	-Wshadow -Wpointer-arith -Wcast-qual
CFLAGS+=	-Wsign-compare
YFLAGS=

.include <bsd.prog.mk>
