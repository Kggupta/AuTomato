AUTOMAKE_OPTIONS=foreign

ACLOCAL_AMFLAGS=-I config

EXTRA_DIST=include
AM_CXXFLAGS=-I$(top_srcdir)/include/ -std="c++17" -g
bin_PROGRAMS=automato

automato_CXXFLAGS=$(AM_CXXFLAGS)
automato_SOURCES=src/*
automato_LDADD=-lreadline