MAINTAINERCLEANFILES = Makefile.in

pkgdir = $(libdir)/elementary/modules/access_output_tts/$(MODULE_ARCH)
pkg_LTLIBRARIES = module.la

module_la_SOURCES = mod.c

module_la_CPPFLAGS = \
-I. \
@ACCESS_OUTPUT_CFLAGS@

module_la_LIBADD = -ltts
module_la_LDFLAGS = -no-undefined -module -avoid-version
module_la_LIBTOOLFLAGS = --tag=disable-static
