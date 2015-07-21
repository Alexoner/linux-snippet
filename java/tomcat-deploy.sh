#!/bin/sh

# export CATALINA_HOME=~/
for i in `ls $CATALINA_HOME/deploy`
do
    if [ `basename $i` != "taobao-hsf.sar" ]
    then
        rm -v $CATALINA_HOME/deploy/$i
    else
        echo $i
    fi
done

if [ "$1" != "" ]
then
    rsync -av $1 $CATALINA_HOME/deploy/
fi
