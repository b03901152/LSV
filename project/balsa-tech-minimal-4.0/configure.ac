dnl Process this file with autoconf to produce a configure script

AC_INIT(minimal/startup.scm)

AM_INIT_AUTOMAKE(balsa-tech-minimal, 4.0)
AC_PROG_INSTALL
AC_PREFIX_DEFAULT(`balsa-config -d`)

balsadatadir=${prefix}/share
AC_SUBST(balsadatadir)

AC_OUTPUT([
	Makefile
	minimal/Makefile
])
