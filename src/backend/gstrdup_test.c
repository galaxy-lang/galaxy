#include "gstrdup.c"

int main()
{
    char source[] = "TesteTeste";

    char* target = gstrdup(source); 

    printf("%s", target);
    return 0;
}