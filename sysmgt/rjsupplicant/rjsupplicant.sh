#!/usr/bin/env bash

arg="";
until [ $# -eq 0 ]
do
    opt=$1;
    if [ "${opt/ /}" != "$opt" ]; then 
        arg="$arg \"$opt\"";
    else
        arg="$arg $opt";
    fi
    shift
done

function main()
{
    cd "/opt/rjsupplicant"
    #sh -c "/opt/rjsupplicant/rjsupplicant $arg"
    sh -c "/opt/rjsupplicant/rjsupplicant.sh --nic enp4s0f0 -uU201114507 -p145265 -d1"
}

main;
exit 0;
