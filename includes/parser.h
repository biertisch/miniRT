#ifndef PARSER_H
#define PARSER_H

# include <errno.h>
# include <limits.h>
# include <math.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include "vec3.h"
# include "color.h"
# include "libft.h"
# include "minirt.h" // necessary?

typedef enum e_field_type {
    FIELD_FLOAT,
    FIELD_VEC3,
    FIELD_COLOR
}   t_field_type;

typedef struct s_field_rule {
    char            *name;
    t_field_type    type;
    float           min;
    float           max; //replace with t_interval?
}   t_field_rule;

typedef struct s_object_rule {
    char            *keyword;
    t_field_rule    *fields;
    int             field_count;
}   t_object_rule;

// error.c
int	report_error(int *errors, int line_no, char *message, char *arg);
int	report_range_error(int *errors, int line_no, char *type, float value, t_interval range);

// keyword.c
int	is_valid_keyword(const char *p, char *key, t_color *acl);

// parser.c
int	parser(t_world *scene, char *file);

// parse_field.c
int	parse_int(const	t_field_rule *rule, int *out, const char **s, int line_no, int *errors);
int	parse_float(const t_field_rule *rule, float *out, const char **s, int line_no, int *errors);
int	parse_color(const t_field_rule *rule, t_color *out, const char **s, int line_no, int *errors);
int	parse_vec3(const t_field_rule *rule, t_vec3 *out, const char **s, int line_no, int *errors);

// parse_line.c
int	parse_line(t_world *scene, char *line, int line_no, t_color *acl, int *errors);

// rules.c
t_object_rule	*find_rule(const char *keyword);

// rules2.c
t_field_rule	*get_sphere_fields(void);
t_field_rule	*get_plane_fields(void);
t_field_rule	*get_cylinder_fields(void);
t_field_rule	*get_cone_fields(void);

// utils.c
int	check_trailing(const char *p);
void	skip_spaces(const char **s);
int		ft_atoi_count(const char *str, int *read);
double	ft_atof_count(const char *str, int *read);
char	*ft_ftoa(double n, int precision);



// /* main parsing API */
// void    parse_config_file(const char *filename);
// void    parse_and_validate(char *params, t_object_rule *rule, int line);
// t_object_rule   *find_rule(const char *keyword);

// /* helpers */
// float   parse_float(char **str);
// t_vec3  parse_vec3(char **str);
// t_color parse_color(char **str);
// void    skip_spaces(char **str);
// void    error_line(int line, const char *msg);

// //validator.c
// int validate_config_file_full(const char *filename);

// double	ft_atof_simple(const char *str, char **endptr);

#endif