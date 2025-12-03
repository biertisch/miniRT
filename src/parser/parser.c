#include "parser.h"

void parse_config_file(const char *filename)
{
    FILE *fp;
    char line[256];
    int  line_num = 0;

    fp = fopen(filename, "r");
    if (!fp)
    {
        perror("Error opening file");
        exit(1);
    }
    while (fgets(line, sizeof(line), fp))
    {
        char keyword[8];
        line_num++;
        if (sscanf(line, "%7s", keyword) != 1)
            continue;
        t_object_rule *rule = find_rule(keyword);
        if (!rule)
        {
            fprintf(stderr, "Error line %d: unknown object '%s'\n",
                    line_num, keyword);
            continue;
        }
        parse_and_validate(line + strlen(keyword), rule, line_num);
    }
    fclose(fp);
}
