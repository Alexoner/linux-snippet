#!/bin/bash
trap 'killall' INT
#trap 'killall' TERM

killall() {
    trap '' INT TERM        #ignore INT and TERM 
    echo "**** shutting down... ****" #added double quotes

    #kill -TERM 0
    for i in $(jobs -pr); do
        kill -TERM $i
    done

    wait
    echo DONE
}

./long.sh &
./long.sh &
./long.sh &

wait
echo "Waited enough"
#cat  # wait forever
