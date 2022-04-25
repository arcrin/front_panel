#include <stdint.h>
#include <stdio.h>

const int DataTable[] = {100, 101, 102, 103};

static int Data01 = 1000;

int linking(int argc, char **argv)
{
    static int Bss01;
    int ret = 0;

    printf("Bss=%d, Data=%d\n", Bss01++, Data01++);
    Data01 = DataTable[Bss01%4];
//    ret = (int)Num2Text[Bss01][0];
//    const int localTab[] = {200, 201, 202, 203};
//    Data01 = localTab[Bss01%4];

}
