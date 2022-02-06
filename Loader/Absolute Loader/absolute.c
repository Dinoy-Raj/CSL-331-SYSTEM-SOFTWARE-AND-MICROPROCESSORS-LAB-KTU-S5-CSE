#include <stdio.h>

void main()
{
    FILE *obj;
    obj = fopen("objr.txt", "r");

    // if object file is does not exist in specified position to open
    if (obj == NULL)
    {
        printf("\n The Object File Does not exist in current directory.....\nTerminating ..............");
        exit(1);
    }

    // check each record whether it is text record or header record or end record
}