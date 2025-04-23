#include<stdlib.h>
#include "impressao.c"

int main(){
    char buf[20];

    cents_to_reais(-123,buf);

    printf("%s\n", buf);
}