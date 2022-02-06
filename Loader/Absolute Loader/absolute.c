#include <stdio.h>
#include <stdlib.h>

void main()
{
    FILE *obj, *out;
    char rec[200];
    obj = fopen("obj.txt", "r");
    out = fopen("out.txt", "w");

    // if object file is does not exist in specified position to open
    if (obj == NULL)
    {
        printf("\nThe Object File Does not exist in current directory.....\nTerminating ..............");
        exit(1);
    }

    // check each record whether it is text record or header record or end record
    fscanf(obj, "%s", rec);

    while (rec[0] != "E")
    {
    }
    fscanf(obj, "%s", rec);
    printf("\n%s", rec);
}