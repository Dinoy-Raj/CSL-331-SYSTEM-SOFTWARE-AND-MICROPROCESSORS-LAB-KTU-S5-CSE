#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

char *sub_string(char *str, int s, int e)
{
    int i = 0;
    char *res = malloc(e * sizeof(char));

    while (i < e)
    {
        *(res + i) = *(str + s + i);
        i++;
    }
    *(res + i) = '\0';

    return res;
}

char *name_parse(char *str)
{
    int i = 0;
    int e = strlen(str);
    while (i < e)
    {
        if (isdigit(str[i + 1]))
        {
            break;
        }
        i++;
    }

    char *res = sub_string(str, 1, i);
    return res;
}

void modi_record(char *rec, FILE *file, int st)
{
}

void text_record(char *rec, FILE *file, int st)
{
    char *start;
    int length, update, i = 9;
    char *bit;
    char *temp = malloc(sizeof(char) * 10);
    start = sub_string(rec, 1, 6);
    length = (int)strtol(sub_string(rec, 7, 2), NULL, 16);

    update = (int)strtol(start, NULL, 16);
    update = update + st;

    while (length)
    {

        fprintf(file, "%06x", update);
        bit = sub_string(rec, i, 2);
        fprintf(file, "\t%s\n", bit);
        i += 2;
        length--;
        update++;
    }

    fprintf(file, "\n");
}

char *start_record(char *rec)
{
    char *name, *start, *length;
    name = name_parse(rec);
    printf("\n - Name Of the program  : %s", name);
    start = sub_string(rec, strlen(name) + 2, 6);
    printf("\n - Start Address Of the program  : %s", start);
    length = sub_string(rec, strlen(name) + 8, 6);
    printf("\n - Length Of the program  : %s", length);
    return start;
}

void main()
{
    FILE *obj, *out;
    char rec[200];
    char *st_addr;
    int start_addr;
    obj = fopen("obj.txt", "r");
    out = fopen("out.txt", "w");

    // if object file is does not exist in specified position to open
    if (obj == NULL)
    {
        printf("\nThe Object File Does not exist in current directory.....\nTerminating ..............");
        exit(1);
    }
    else
    {
        printf("\nThe Object File Sucessfully readed - >\n");
    }

    // check each record whether it is text record or header record or end record
    fscanf(obj, "%s", rec);

    printf("Enter the start address of the program ? [6 digit format]");
    scanf("%s", st_addr);

    start_addr = strtol(st_addr, NULL, 16);

    while (rec[0] != 'E')
    {

        if (rec[0] == 'H')
        {
            st_addr = start_record(rec);
        }
        else if (rec[0] == 'T')
        {
            text_record(rec, out, start_addr);
        }
        else if (rec[0] == 'M')
        {
            modi_record(rec, out, start_addr);
        }
        fscanf(obj, "%s", rec);
    }
    printf("\n\n  -----  All Text Records Allocated and Saved To Output File  -----  ", rec);
    exit(0);
}