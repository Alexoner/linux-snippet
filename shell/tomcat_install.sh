#!/bin/sh

TOMCAT_HOME="$HOME/ali/packages/taobao-tomcat"
TOMCAT_DEPLOY="$TOMCAT_HOME/deploy"

if [ $# -lt 1 ]
then
    echo "Which package do you want to install to $TOMCAT_DEPLOY?"
    exit
fi

if [ "x$1" != "x" ]
then
    for i in $(ls $TOMCAT_DEPLOY)
    do
        if [ "$i" != "taobao-hsf.sar" ]
        then
            rm -rv $TOMCAT_DEPLOY/$i
        fi
    done
    rsync -aAXv --progress $1 $TOMCAT_DEPLOY/
fi

$CATALINA_HOME/bin/catalina.sh run
