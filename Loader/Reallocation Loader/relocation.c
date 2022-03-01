#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

char *sub_string(char *str, int s, int e)
{
    int i = 0;
    char *res = (char*) malloc(e * sizeof(char));

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

void modi_record(char *rec, int st)
{
    int i = 0, r, m, start = 0;
    char *addr, *faddr;
    char *str1 = (char*) malloc(10 * sizeof(char));
    char *str2 = (char*) malloc(10 * sizeof(char));
    char *str3 = (char*) malloc(10 * sizeof(char));
    long int pos;

    addr = sub_string(rec, 1, 6);
    r = strtol(addr, NULL, 16);
    r = r + st;
    sprintf(addr, "%06x", r);

    FILE *f = fopen("out.txt", "r");

    while (1)
    {
        if (feof(f))
        {
            i = -1;
            break;
        }
        fscanf(f, "%s %s", str1, str2);

        if (!strcmp(addr, str1))
        {
            pos = ftell(f);
            break;
        }
        i++;
    }
    pos = pos - 10;
    fseek(f, pos, SEEK_SET);
    fscanf(f, "%s %s", str1, str2);
    printf("\n%s", str2);
    fscanf(f, "%s %s", str1, str3);
    strcat(str2, str3);
    fscanf(f, "%s %s", str1, str3);
    strcat(str2, str3);

    m = strtol(str2, NULL, 16);
    m = m + st;
    sprintf(str2, "%06x", m);
    fclose(f);

    printf("\n%s", str2);

    pos = pos + 8;

    f = fopen("out.txt", "r+");
    i = 3;

    while (i)
    {
        fseek(f, pos, SEEK_SET);
        fputs(sub_string(str2, start, 2), f);
        start = start + 2;
        pos = pos + 11;
        i--;
    }
}

void text_record(char *rec, FILE *file, int st)
{
   
    char *start;
    int length, update, i = 9;
    char *temp = (char*)  malloc(sizeof(char) * 10);
    start = sub_string(rec, 1, 6);
    length = (int)strtol(sub_string(rec, 7, 2), NULL, 16);

    update = (int)strtol(start, NULL, 16);
    update = update + st;
    char out[50];

    while (length)
    {

        sprintf(out, "%06X", update);
        fprintf(file, "%s %s\n", out, sub_string(rec, i, 2));
       
        i += 2;
        length--;
        update++;
    }

    fprintf(file, "\n");
}

char *start_record(char *rec, int st)
{
    char *name, *start, *length;

    name = name_parse(rec);
    printf("\n - Name Of the program  : %s", name);

    start = sub_string(rec, strlen(name) + 2, 6);
    printf("\n - Start Address Of the program  : %s", start);

    printf("\n - Allocated Address Of the program  : %06x", st);

    length = sub_string(rec, strlen(name) + 8, 6);
    printf("\n - Length Of the program  : %s", length);

    return start;
}

int main()
{
    FILE *obj, *out;
    char rec[200];
    char *st_addr = (char*) malloc(10 * sizeof(char));;
    int start_addr;
    obj = fopen("obj.txt", "r");
    out = fopen("out.txt", "w+");

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
      printf("done2");

    printf("Enter address to be the  program  allocated ? : ");
    scanf("%s", st_addr);
        printf("done3");
    
    start_addr = (int) strtol(st_addr, NULL, 16);
        printf("done4");
     

    while (rec[0] != 'E')
    {

        if (rec[0] == 'H')
        {
            st_addr = start_record(rec, start_addr);
        }
        else if (rec[0] == 'T')
        {
            text_record(rec, out, start_addr);
        }
        else if (rec[0] == 'M')
        {
            fclose(out);
            modi_record(rec, start_addr);
        }
        fscanf(obj, "%s", rec);
    }
    printf("\n\n  -----  All Text Records Allocated and Saved To Output File  -----  ", rec);
    exit(0);
}