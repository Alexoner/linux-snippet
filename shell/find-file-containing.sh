#!/bin/sh
find . -regextype egrep -regex ".*.(java|vm|xml|js)" |xargs -0 grep --color=auto -slnie "tr_tr"

grep --include=\*.{c,h} -rnw . -e "pattern"
grep --exclude=*.o -rnw 'directory' -e "pattern"
grep --exclude-dir={dir1,dir2,*.dst} -rnw 'directory' -e "pattern"
