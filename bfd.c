/*************************************************************************
    > File Name: bfd.c
    > Author: hao
    > Mail: onerhao@gmail.com
    > Created Time: Fri 14 Dec 2012 10:16:53 PM CST
 ************************************************************************/
#include <stdio.h>
#include <config.h>
#include <bfd.h>

int main()
{
    bfd_init();

    bfd* file = bfd_openr("a.out", 0);

    if (!file)
        return -1;

    if (bfd_check_format(file, bfd_object))
        printf("object file\n");
    else
        printf("not object file\n");

    bfd_close(file);

    return 0;
}
