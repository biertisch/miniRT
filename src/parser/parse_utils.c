#include "parser.h"

void skip_spaces(char **str)
{
    while (**str == ' ' || **str == '\t')
        (*str)++;
}

float parse_float(char **str)
{
    float val;
    int n = 0;

    skip_spaces(str);
    sscanf(*str, "%f%n", &val, &n);
    *str += n;
    return (val);
}

t_vec3 parse_vec3(char **str)
{
    t_vec3 v;
    int n = 0;

    skip_spaces(str);
    sscanf(*str, "%lf,%lf,%lf%n", &v.x, &v.y, &v.z, &n);
    *str += n;
    return (v);
}

t_color parse_color(char **str)
{
    t_color c;
    int n = 0;

    skip_spaces(str);
    sscanf(*str, "%le,%le,%le%n", &c.r, &c.g, &c.b, &n);
    *str += n;
    return (c);
}

void error_line(int line, const char *msg)
{
    fprintf(stderr, "Error line %d: invalid value for '%s'\n", line, msg);
}
