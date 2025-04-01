#
# Makefile for utils/adt/mysl
#
# src/backend/utils/adt/mysl/Makefile
#

subdir = src/backend/utils/adt/mysql
top_builddir = ../../../../..
include $(top_builddir)/src/Makefile.global

override CPPFLAGS := -I. -I$(srcdir) $(CPPFLAGS)

# keep this list arranged alphabetically or it gets to be a mess
OBJS = \
	mys_adtext.o \
	mys_date.o \
	mys_ri_trigger.o \
	mys_ruleutils.o \
	mys_timestamp.o \
	mys_varlena.o

include $(top_srcdir)/src/backend/common.mk

$(recurse)