#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

char *strCat(char *dest, char *src, bool reverse)
{
    if (!reverse)
    {
        size_t i, j;
        for (i = 0; dest[i] != '\0'; i++)
            ;
        for (j = 0; src[j] != '\0'; j++)
            dest[i + j] = src[j];
        dest[i + j] = '\0';
    }
    else
    {
        char *tmp = strdup(dest);

        strcpy(dest, src);
        strCat(dest, tmp, false);

        free(tmp);
    }
    return dest;
}

char *computeArgs(char **args, int i)
{
    char temp[] = {' ', '\0'};
    char *store = (char *)calloc(100, 1);
    store[0] = temp[0];
    for (int j = 0; j < i; j++)
    {
        strCat(store, args[j], false);
        temp[0] = ' ';
        strCat(store, &temp[0], false);
        temp[0] = ('a' + j);
        strCat(store, &temp[0], false);
        if (i - j > 1)
        {
            temp[0] = ',';
            strCat(store, &temp[0], false);
        }
    }
    return store;
}

int main(int argc, char **argv)
{
    int action = 0;
    printf("Please confirm: this action will erase 'skeliton.c' file in the present directory!\nenter 1 to approve, any key to stop...\n ");
    scanf("%d", &action);
    if (action == 1)
    {
        int testCasesCount = 0;
        if (argc == 1)
        {
            printf("\nenter number of test cases:");
            scanf("%d", &testCasesCount);
        }
        else
        {
            testCasesCount = atoi(argv[1]);
        }

        char *returnType = calloc(20, 1);
        if (argc > 2)
        {
            returnType = argv[2];
        }
        else
        {
            printf("\nenter return type of the function:");
            scanf("%s", returnType);
        }

        char **args;
        char *arg;
        if (argc > 3)
        {
            args = calloc(argc - 3, sizeof(char *));
            for (int i = 0; i < argc - 3; i++)
            {
                args[i] = argv[i + 3];
            }
            arg = computeArgs(args, argc - 3);
        }
        else
        {
            printf("\nenter the number of arguments for the function:");
            int argCount = 0;
            scanf("%d", &argCount);
            args = calloc(argCount, sizeof(char *));
            for (int i = 0; i < argCount; i++)
            {
                args[i] = calloc(20, sizeof(char));
                printf("\n>");
                scanf("%s", args[i]);
            }
            arg = computeArgs(args, argCount);
        }

        FILE *filePointer;
        const char *filename = "skeliton.c";
        const char *mode = "w";
        filePointer = fopen(filename, mode);
        fprintf(filePointer, "#include<stdio.h>\n#include<stdbool.h>\n\n%s fun(", returnType);
        fprintf(filePointer, "%s", arg);
        fprintf(filePointer, "%s", "){\n\n");
        fprintf(filePointer, "%s", "}\n\n\n\n");

        // printing test cases
        for (int i = 0; i < testCasesCount; i++)
        {
            fprintf(filePointer, "bool testCase%d(%s (*funPointer)(%s)){\n //(*funPointer)(// place your argumnets)\n\nif(false){\nprintf(\"test case %d failed at line: %s\\n\",__LINE__);\n}\n return true;\n}\n\n",
                    i + 1, returnType, arg, i + 1, "%d");
        }

        //printing the actual function
        fprintf(filePointer, "\n%s solution(%s){\n//write your solution here!\n\n}\n", returnType, arg);

        //printing inside main
        fprintf(filePointer, "\n\nvoid main(){\n bool tests[%d];\n", testCasesCount);
        for (int i = 0; i < testCasesCount; i++)
        {
            fprintf(filePointer, "tests[%d]=testCase%d(&solution);\n", i, i + 1);
        }

        //asserting the test cases
        fprintf(filePointer, "\n\n\n\nbool check=true;\n");
        fprintf(filePointer, "for(int i=0;i<%d;i++){\n", testCasesCount);
        fprintf(filePointer, "if(!tests[i]){\n");
        fprintf(filePointer, "check=false;\nbreak;\n");
        fprintf(filePointer, "}\n}\n");
        fprintf(filePointer, "if(check){\n printf(\"All tests passed!\");\n}");
        fprintf(filePointer, "else{\n printf(\" some of the test cases did not pass:\");\n\n ");
        fprintf(filePointer, "for(int i=0;i<%d;i++){if(!tests[i]){printf(\"%s\",i);}}", testCasesCount, "%d");
        fprintf(filePointer, "}");
        fprintf(filePointer, "\n\n}");

        fclose(filePointer);
        // printf("%s", arg);
    }
}
