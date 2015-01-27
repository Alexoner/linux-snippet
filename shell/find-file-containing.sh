#!/bin/sh
find . -regextype egrep -regex ".*.(java|vm|xml|js)" |xargs -0 grep --color=auto -slnie "tr_tr"
