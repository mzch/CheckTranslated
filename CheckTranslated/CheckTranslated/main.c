//
//  main.c
//  CheckTranslated
//
//  Created by Koichi MATSUMOTO on 2019/05/26.
//  Copyright © 2019 Koichi MATSUMOTO. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "structure.h"

char * trim(char * buf)
{
    for (char * p = buf + strlen(buf) - 1; buf < p; p--)
    {
        if (* p == SPC || * p == CRLF)
            * p = '\0';
        else
            break;
    }
    
    return buf;
}

LocaleTSV getLocaleData(char * buf)
{
    LocaleTSV l;
    
    l.translation = l.value = l.index = l.ref_key = l.key = l.name = l.ref_name = "";
    
    char * p = strchr(buf, TAB);
    * p = '\0';
    l.name = trim(buf);
    
    p++;
    buf = p;
    p = strchr(buf, TAB);
    if (! p)
        return l;
    * p = '\0';
    l.key = trim(buf);
    
    p++;
    buf = p;
    p = strchr(buf, TAB);
    if (! p)
        return l;
    * p = '\0';
    l.ref_name = trim(buf);
    
    p++;
    buf = p;
    p = strchr(buf, TAB);
    if (! p)
        return l;
    * p = '\0';
    l.ref_key = trim(buf);
    
    p++;
    buf = p;
    p = strchr(buf, TAB);
    if (! p)
        return l;
    * p = '\0';
    l.index = trim(buf);
    
    p++;
    buf = p;
    p = strchr(buf, TAB);
    if (! p)
        return l;
    * p = '\0';
    l.value = trim(buf);
    
    return l;
}


TranslatedTSV getTranslationData(char * buf)
{
    TranslatedTSV t;
    
    t.line_num = t.index = t.value = t.id = t.key = t.translation = "";
    
    char * p = strchr(buf, TAB);
    if (! p)
        return t;
    * p = '\0';
    t.line_num = trim(buf);
    
    p++;
    buf = p;
    p = strchr(buf, TAB);
    if (! p)
        return t;
    * p = '\0';
    t.id = trim(buf);
    
    p++;
    buf = p;
    p = strchr(buf, TAB);
    if (! p)
        return t;
    * p = '\0';
    t.key = trim(buf);
    
    p++;
    buf = p;
    p = strchr(buf, TAB);
    if (! p)
        return t;
    * p = '\0';
    t.index = trim(buf);
    
    p++;
    buf = p;
    p = strchr(buf, TAB);
    if (! p)
        return t;
    * p = '\0';
    t.value = trim(buf);
    
    p++;
    buf = p;
    p = strchr(buf, TAB);
    if (! p)
        return t;
    * p = '\0';
    t.translation = trim(buf);
    
    return t;
}

int main(int argc, const char * argv[])
{
    // check arguments
    if (argc < 3)
    {
        fprintf(stderr, "Missing arguments...\n");
        fprintf(stderr, "%s En.tsv Ja.tsv\n\n", argv[0]);
        exit (1);
    }
    
    // check read in file
    FILE * fp_t = fopen(argv[1], "r");
    if (! fp_t)
    {
        fprintf(stderr, "File not found: %s\n", argv[1]);
        exit (2);
    }
    FILE * fp_s = fopen(argv[2], "r");
    if (! fp_s)
    {
        fprintf(stderr, "File not found: %s\n", argv[2]);
        exit (3);
    }
    
    // Read Localization TSV
    char buf[BUFFER_SIZE];
    char org[BUFFER_SIZE];
    int line_num = 1;
    
    fgets(buf, sizeof(buf) - 1, fp_t);          // 1行目は無条件で出力する
    fputs(buf, fp_s);
    
    while (fgets(buf, sizeof(buf) - 1, fp_t))
    {
        line_num++;
        strcpy(org, buf);
        
        LocaleTSV l = getLocaleData(buf);
        
        if (l.value[0] == '[' && l.value[1] == '[')
        {
            ;
        }
        else
            if (! strcmp(l.name, "###HEADER") && ! strcmp(l.ref_name, "###HEADER"))
            {
                goto rewind_top;
            }
            else
            {
                char guf[BUFFER_SIZE];
                int t_line_num = 1;
                
                while (fgets(guf, sizeof(guf) - 1, fp_s))
                {
                    
                    TranslatedTSV t = getTranslationData(guf);
                    t_line_num++;
                    
                    if (! strcmp(l.ref_name, t.key) && ! strcmp(l.ref_key, t.id) && ! strcmp(l.index, t.index))
                    {
                        if (! strlen(t.translation))
                        {
                            printf("Missing translation: [%d(%d)] %s, %s, %s\n", line_num, t_line_num, l.ref_name, l.ref_key, l.index);
                            exit (6);
                        }
                        goto rewind_top;
                    }
                    
                }
                if (feof(fp_s))
                {
                    printf("Missing translated line: [%d] %s, %s, %s\n", line_num, l.ref_name, l.ref_key, l.index);
                    exit (8);
                }
            }
    rewind_top:
        rewind(fp_s);
    }

    fclose(fp_s);
    fclose(fp_t);

    printf("Completed!\n");
    
    return 0;
}
