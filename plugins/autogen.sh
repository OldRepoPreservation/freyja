#!/bin/sh
# What the wolf you say?  Just use these simple make 
# rules to install libfreyja and the test plugins.
#
# Or if you don't want to type them, use this as a
# shell script as root.
# 
# /bin/sh INSTALL

for i in modules/image/*; do cd $i; make; cd ..; done
for i in modules/model/*; do cd $i; make; cd ..; done

rm -f depend
make debug
#make install
echo
echo Now do make install
# && make plugins && make install-plugins
#make plugins
#make install-plugins
