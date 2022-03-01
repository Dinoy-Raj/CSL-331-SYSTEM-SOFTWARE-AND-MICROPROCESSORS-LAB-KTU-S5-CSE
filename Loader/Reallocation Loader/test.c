#include <stdio.h>
#include <stdlib.h>

void main()
{
    FILE *f = fopen("out.txt", "r");
    char *str1 = malloc(7 * sizeof(char));
    // fscanf(f, "%s%s", str1, str2);
    fgets(str1, 7, f);
    printf("%s", str1);
}