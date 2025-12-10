#ifndef PARSER_H
#define PARSER_H

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <math.h>
# include <limits.h>
# include "vec3.h"
# include "color.h"
# include "libft.h"

typedef enum e_field_type {
    FIELD_FLOAT,
    FIELD_VEC3,
    FIELD_COLOR
}   t_field_type;

typedef struct s_field_rule {
    char            *name;
    t_field_type    type;
    float           min;
    float           max;
}   t_field_rule;

typedef struct s_object_rule {
    char            *keyword;
    t_field_rule    *fields;
    int             field_count;
}   t_object_rule;

/* main parsing API */
void    parse_config_file(const char *filename);
void    parse_and_validate(char *params, t_object_rule *rule, int line);
t_object_rule   *find_rule(const char *keyword);

/* helpers */
float   parse_float(char **str);
t_vec3  parse_vec3(char **str);
t_color parse_color(char **str);
void    skip_spaces(char **str);
void    error_line(int line, const char *msg);

//validator.c
int validate_config_file_full(const char *filename);

double	ft_atof_simple(const char *str, char **endptr);

#endif