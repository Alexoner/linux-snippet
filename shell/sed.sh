#!/bin/sh

# extract regular expression pattern with sed.Use "[]" to work around the problem that ungreedy search isn't supported
echo "200 792414 \"http://www.alibaba-inc.com/param1=1&param2=4\" \"http://www.fuckjingdong.com/yes/\" This is a log " |
    sed "s/.*\(200 [0-9]* \"[^\"]*\"\).*/\1/"
    
sed -i 's/oldstring/newstring/g' file.txt
