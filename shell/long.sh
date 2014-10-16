#!/bin/bash
trap 'killall' TERM

echo "Started...$$" >>file.txt

killall() {
    # shutting down gracefully
    echo "Finished... $$" >> file.txt
    echo "Finished... $$"
    exit # don't forget this!
}

# infinite loop
while true;do echo loop > /dev/null;done
