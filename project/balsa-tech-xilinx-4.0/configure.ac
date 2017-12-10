dnl Process this file with autoconf to produce a configure script

AC_INIT(xilinx/startup.scm)

AM_INIT_AUTOMAKE(balsa-tech-xilinx, 4.0)
AC_PROG_INSTALL
AC_PATH_PROG(BALSA_CONFIG, balsa-config)

if test ${BALSA_CONFIG}; then :
else
	AC_MSG_ERROR([cannot find required balsa-config, install Balsa and add it to your path])
fi

dnl Don't allow use of --prefix
ac_default_prefix=`${BALSA_CONFIG} -d`
prefix=${ac_default_prefix}
echo installed Balsa prefix is: ${ac_default_prefix}

balsadatadir=${prefix}/share
AC_SUBST(balsadatadir)

AC_OUTPUT([
	Makefile
	xilinx/Makefile
])
