/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/13 16:51:22 by beatde-a          #+#    #+#             */
/*   Updated: 2025/12/13 16:59:01 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include <fcntl.h>
# include <stdio.h>
# include <stdlib.h>
# include "vec3.h"
# include "color.h"
# include "libft.h"
# include "minirt.h"

# define ERR_BUFFER_SIZE 120

typedef struct s_world	t_world;

typedef enum e_error_type
{
	ERR_KEYWORD,
	ERR_FORMAT,
	ERR_RANGE,
	ERR_TRAILING,
	ERR_NORMAL,
	ERR_AMBIENT_COUNT,
	ERR_CAMERA_COUNT,
	ERR_LIGHT_COUNT,
	ERR_VALIDATE
}	t_error_type;

typedef enum e_field_type
{
	FIELD_INT,
	FIELD_FLOAT,
	FIELD_VEC3,
	FIELD_COLOR
}	t_field_type;

typedef struct s_field_rule
{
	char			*name;
	t_field_type	type;
	float			min;
	float			max;
	int				precision;
}	t_field_rule;

typedef struct s_object_rule
{
	char			*keyword;
	t_field_rule	*fields;
	int				field_count;
}	t_object_rule;

typedef struct s_metadata
{
	t_object_rule	*rule;
	int				camera_count;
	int				ambient_count;
	int				light_count;
	int				line_no;
	int				errors;
}	t_metadata;

// error.c
int				report_error(t_error_type code, t_metadata *meta,
					t_field_rule *field, double value);

// error_utils.c
void			append_int(char *buffer, int value);
void			append_float(char *buffer, double value, int precision);
void			append_line(char *buffer, int line_no);
void			append_header(char *buffer, int errors);
void			normal_error_message(char *buffer, t_field_rule *field);

// keyword.c
int				is_valid_keyword(const char *p, char *key, t_metadata *meta);

// parser.c
int				parser(t_world *scene, char *file);

// parse_color.c
int				parse_color(t_color *out, const char **s, t_field_rule *field,
					t_metadata *meta);

// parse_line.c
int				parse_line(t_world *scene, const char *line, t_metadata *meta);

// parse_object.c
int				parse_sphere_fields(t_world *scene, const char **s,
					t_metadata *meta);
int				parse_plane_fields(t_world *scene, const char **s,
					t_metadata *meta);
int				parse_cylinder_fields(t_world *scene, const char **s,
					t_metadata *meta);
int				parse_cone_fields(t_world *scene, const char **s,
					t_metadata *meta);

// parse_vector.c
int				parse_vec3(t_vec3 *out, const char **s, t_field_rule *field,
					t_metadata *meta);

// parse_scalar.c
int				parse_int(int *out, const char **s, t_field_rule *field,
					t_metadata *meta);
int				parse_float(double *out, const char **s, t_field_rule *field,
					t_metadata *meta);

// rules.c
t_object_rule	*find_rule(const char *keyword);

// rules2.c
t_field_rule	*get_sphere_fields(void);
t_field_rule	*get_plane_fields(void);
t_field_rule	*get_cylinder_fields(void);
t_field_rule	*get_cone_fields(void);

// utils.c
int				check_trailing(const char *p);
void			skip_spaces(const char **s);
int				ft_atoi_count(const char *str, int *read);
double			ft_atof_count(const char *str, int *read);
char			*ft_ftoa(double n, int precision);

#endif