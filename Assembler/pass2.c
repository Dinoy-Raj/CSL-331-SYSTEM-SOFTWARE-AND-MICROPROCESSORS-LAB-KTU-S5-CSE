#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <stdlib.h>

int check(int tot, int add)
{
    if ((tot + add) > 30)
    {
        return 1;
    }

    return 0;
}

void break_t(char *t, int len, int st, FILE *f)
{
    fprintf(f, "T^%06x^%02x %s\n", st, len, t);
}


char *sub_string(char *str, int s, int e)
{
    int i = 0;
    char *res = (char *)malloc(e * sizeof(char));

    while (i < e)
    {
        *(res + i) = *(str + s + i);
        i++;
    }
    *(res + i) = '\0';

    return res;
}

int main()
{
    char a[10], ad[10], label[10], opcode[10], operand[10], symbol[10], ch, addr[10], op[10], eq[10], add[10];
    int st, diff, i, len, actual_len, finaddr, prevaddr, j = 0;
    char *address = (char *)malloc(10 * sizeof(char));
    FILE *fp1, *fp2, *fp3, *fp4, *fp5;
    // clrscr();
    fp1 = fopen("asm.txt", "w");
    fp2 = fopen("symtbl.txt", "r");
    fp3 = fopen("out.txt", "r");
    fp4 = fopen("obj.txt", "w");
    fp5 = fopen("optab.txt", "r");

    // read starting address
    fscanf(fp3, "%s%s%s", label, opcode, operand);
    int st_addr = strtol(operand, NULL, 16);

    // find end record address
    fscanf(fp3, "%s%s%s%s", addr, label, opcode, operand);
    while (strcmp(opcode, "END") != 0)
    {
        fscanf(fp3, "%s%s%s%s", addr, label, opcode, operand);
    }
    fscanf(fp3, "%s%s%s%s", addr, label, opcode, operand);

    // find length  of program (final-start)
    finaddr = strtol(addr, NULL, 16) - st_addr;

    // like reopen
    rewind(fp3);

    // read first line (starting)
    fscanf(fp3, "%s%s%s", label, opcode, operand);

    // start record created
    if (strcmp(opcode, "START") == 0)
    {
        fprintf(fp1, "%s\t%s\t%s\n", label, opcode, operand);

        // HEADER record written to header record
        fprintf(fp4, "H^%s^%06X^%06X\n\n", label, strtol(operand, NULL, 16), finaddr);

        fscanf(fp3, "%s%s%s%s", address, label, opcode, operand);
    }

    char textr[50];
    textr[0] = '\0';
    int ck = 0, TexLen = 0;
    int TexStart = st_addr;

    while (strcmp(opcode, "END") != 0)
    {
        printf("%s\t%s\t%s\t%s\n", address, label, opcode, operand);
        if (strcmp(opcode, "BYTE") == 0)
        {
            fprintf(fp1, "%d\t%s\t%s\t%s\t", address, label, opcode, operand);
            len = strlen(operand);
            actual_len = len - 3;
            strcat(textr, "^");

            if (operand[0] == 'X')
            {
                if (check(TexLen, actual_len) == 1)
                {
                    break_t(textr, TexLen, TexStart, fp4);
                    TexStart = TexStart + TexLen;
                    TexLen = 0;
                    textr[0] = '\0';
                }
                strcat(textr, sub_string(operand, 2, actual_len));
                TexLen = TexLen + actual_len;
            }
            else if (operand[0] == 'C')
            {
                if (check(TexLen, actual_len) == 1)
                {
                    break_t(textr, TexLen, TexStart, fp4);
                    TexStart = TexStart + TexLen;
                    TexLen = 0;
                    textr[0] = '\0';
                }

                strcat(textr, sub_string(operand, 2, actual_len));
                TexLen = TexLen + actual_len;
            }
        }
        else if (strcmp(opcode, "WORD") == 0)
        {
            actual_len = 3;
            if (check(TexLen, 3) == 1)
            {
                break_t(textr, TexLen, TexStart, fp4);
                TexStart = TexStart + TexLen;
                TexLen = 0;
                textr[0] = '\0';
            }
            else
            {
                strcat(textr, "^");
                sprintf(operand,"%06x",atoi(operand));
                strcat(textr, operand);
                TexLen = TexLen + actual_len;
                fprintf(fp1, "%d\t%s\t%s\t%s\n", address, label, opcode, operand);
            }
        }
        else if ((strcmp(opcode, "RESB") == 0) || (strcmp(opcode, "RESW") == 0))
        {
            actual_len = atoi(operand);
            if ((strcmp(opcode, "RESB") == 0))
            {
                break_t(textr, TexLen, TexStart, fp4);
                TexStart = TexStart + TexLen +actual_len;
                TexLen = 0;
                textr[0] = '\0';
            }
            else
            {
                break_t(textr, TexLen, TexStart, fp4);
                TexStart = TexStart + TexLen + 3 * actual_len;
                TexLen = 0;
                textr[0] = '\0';
            }

            fprintf(fp1, "%d\t%s\t%s\t%s\n", address, label, opcode, operand);
        }
        else
        {
            actual_len = 3;

            if (check(TexLen, actual_len) == 1)
            {
                break_t(textr, TexLen, TexStart, fp4);
                TexStart = TexStart + TexLen;
                TexLen = 0;
                textr[0] = '\0';
            }

            strcat(textr, "^");
            int flag = 0;

            rewind(fp5);

            while (!feof(fp5))
            {
                fscanf(fp5, "%s%s", op, eq);
                if (strcmp(opcode, op) == 0)
                {
                    flag = 1;
                    break;
                }
            }

            if (flag == 0)
            {
                printf("error opcode not found");
                exit(0);
            }

            rewind(fp2);
            fscanf(fp2, "%s%s", symbol, add);

            while (strcmp(operand, symbol) != 0)
            {

                fscanf(fp2, "%s%s", symbol, add);
            }

            strcat(textr, eq);
            strcat(textr, add);
            TexLen = TexLen + actual_len;
        }

        fscanf(fp3, "%s%s%s%s", address, label, opcode, operand);
          
    }
    if(TexLen!=0)
    {
     break_t(textr, TexLen, TexStart, fp4);
    }

    fprintf(fp1, "%d\t%s\t%s\t%s\n", address, label, opcode, operand);
    fprintf(fp4, "\nE^%06x", st_addr);
    printf("\n Intermediate file is converted into object code");
    fclose(fp1);
    fclose(fp2);
    fclose(fp3);
    fclose(fp4);
    exit(0);
}