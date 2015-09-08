MAINTAINERCLEANFILES = Makefile.in

pkgdir = $(libdir)/elementary/modules/naviframe_effect/$(MODULE_ARCH)
pkg_LTLIBRARIES = module.la

module_la_SOURCES = naviframe_effect.c
module_la_CPPFLAGS = @NAVIFRAME_EFFECT_CFLAGS@

module_la_LIBADD = @NAVIFRAME_EFFECT_LIBS@

module_la_LDFLAGS = -no-undefined -module -avoid-version
module_la_LIBTOOLFLAGS = --tag=disable-static
