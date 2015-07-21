#!/bin/sh

# export CATALINA_HOME=~/
for i in `ls $CATALINA_HOME/deploy`
  if [ "$i" != "taobao-hsf.sar"
    do rm $CATALINA_HOME/deploy/$i -v
  fi
done

rsync -aAxv $1 $CATALINA_HOME/deploy/
