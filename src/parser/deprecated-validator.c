/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   deprecated-validator.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/07 20:41:51 by bliu              #+#    #+#             */
/*   Updated: 2025/12/11 12:31:06 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"


static void	skip_spaces_local(const char **s)
{
    while (**s && ft_isspace((unsigned char)**s))
        (*s)++;
}

static int	ft_isdigit_simple(char c)
{
    return (c >= '0' && c <= '9');
}

double	ft_atof_simple(const char *str, char **endptr)
{
    double	result;
    double	fraction;
    int		sign;
    int		has_digits;

    sign = 1;
    if (*str == '-' || *str == '+')
    {
        if (*str == '-')
            sign = -1;
        str++;
    }
    result = 0.0;
    has_digits = 0;
    while (ft_isdigit_simple(*str))
    {
        result = result * 10.0 + (*str - '0');
        str++;
        has_digits = 1;
    }
    if (*str == '.')
    {
        str++;
        fraction = 0.1;
        while (ft_isdigit_simple(*str))
        {
            result += (*str - '0') * fraction;
            fraction *= 0.1;
            str++;
            has_digits = 1;
        }
    }
    if (endptr)
        *endptr = (char *)str;
    if (!has_digits)
        return (0.0);
    return (result * sign);
}

static int	parse_float_local(const char *s, double *out, int *nread)
{
    char	*end;
    double	v;

    end = NULL;
    v = ft_atof_simple(s, &end);
    if (end == s || (*end != '\0' && !ft_isspace((unsigned char)*end) && *end != ','))
        return (0);
    *out = v;
    *nread = (int)(end - s);
    return (1);
}

static int	parse_vec3_component(const char **p, double *val, int *total, int i)
{
    int	r;

    skip_spaces_local(p);
    r = 0;
    if (!parse_float_local(*p, val, &r))
        return (0);
    *p += r;
    *total += r;
    skip_spaces_local(p);
    if (i < 2)
    {
        if (**p != ',')
            return (0);
        (*p)++;
        (*total)++;
    }
    return (1);
}

static int	parse_vec3_local(const char *s, double v[3], int *nread)
{
    int			total;
    const char	*p;
    int			i;

    total = 0;
    p = s;
    i = 0;
    while (i < 3)
    {
        if (!parse_vec3_component(&p, &v[i], &total, i))
            return (0);
        i++;
    }
    *nread = total;
    return (1);
}

static int	parse_color_component(const char **p, int *color, int *total, int i)
{
    char	*end;
    long	val;
    int		r;

    skip_spaces_local(p);
    end = NULL;
    val = strtol(*p, &end, 10);
    if (end == *p)
        return (0);
    if (val < INT_MIN || val > INT_MAX)
        return (0);
    *color = (int)val;
    r = (int)(end - *p);
    *total += r;
    *p = end;
    skip_spaces_local(p);
    if (i < 2)
    {
        if (**p != ',')
            return (0);
        (*p)++;
        (*total)++;
    }
    return (1);
}

static int	parse_color_local(const char *s, int c[3], int *nread)
{
    int			total;
    const char	*p;
    int			i;

    total = 0;
    p = s;
    i = 0;
    while (i < 3)
    {
        if (!parse_color_component(&p, &c[i], &total, i))
            return (0);
        i++;
    }
    *nread = total;
    return (1);
}

static int	validate_float_field(const t_field_rule *fr, const char **ps,
    int line, char *errbuf, size_t errlen)
{
    double	val;
    int		n;

    n = 0;
    if (!parse_float_local(*ps, &val, &n))
    {
        snprintf(errbuf, errlen, "Line %d: expected float for '%s'",
            line, fr->name);
        return (0);
    }
    if ((fr->min != 0.0 || fr->max != 0.0)
        && (val < fr->min || val > fr->max))
    {
        snprintf(errbuf, errlen,
            "Line %d: '%s' value %.1f out of range [%.1f,%.1f]",
            line, fr->name, val, fr->min, fr->max);
        return (0);
    }
    *ps += n;
    return (1);
}

static int	validate_vec3_range(double v[3], const t_field_rule *fr,
    int line, char *errbuf, size_t errlen)
{
    int	i;

    if (!(fr->min == fr->max && fr->min == 0.0))
    {
        i = 0;
        while (i < 3)
        {
            if (v[i] < fr->min || v[i] > fr->max)
            {
                snprintf(errbuf, errlen,
                    "Line %d: component of '%s' out of range [%.3f,%.3f]",
                    line, fr->name, fr->min, fr->max);
                return (0);
            }
            i++;
        }
    }
    return (1);
}

static int	validate_vec3_field(const t_field_rule *fr, const char **ps,
    int line, char *errbuf, size_t errlen)
{
    double	v[3];
    int		n;

    n = 0;
    if (!parse_vec3_local(*ps, v, &n))
    {
        snprintf(errbuf, errlen, "Line %d: expected vec3 'x,y,z' for '%s'",
            line, fr->name);
        return (0);
    }
    if (!validate_vec3_range(v, fr, line, errbuf, errlen))
        return (0);
    *ps += n;
    return (1);
}

static int	validate_color_range(int c[3], const t_field_rule *fr,
    int line, char *errbuf, size_t errlen)
{
    int	i;

    i = 0;
    while (i < 3)
    {
        if (c[i] < (int)fr->min || c[i] > (int)fr->max)
        {
            snprintf(errbuf, errlen,
                "Line %d: color component of '%s' must be in [%d,%d]",
                line, fr->name, (int)fr->min, (int)fr->max);
            return (0);
        }
        i++;
    }
    return (1);
}

static int	validate_color_field(const t_field_rule *fr, const char **ps,
    int line, char *errbuf, size_t errlen)
{
    int	c[3];
    int	n;

    n = 0;
    if (!parse_color_local(*ps, c, &n))
    {
        snprintf(errbuf, errlen, "Line %d: expected color 'r,g,b' for '%s'",
            line, fr->name);
        return (0);
    }
    if (!validate_color_range(c, fr, line, errbuf, errlen))
        return (0);
    *ps += n;
    return (1);
}

static int	validate_field(const t_field_rule *fr, const char **ps,
    int line, char *errbuf, size_t errlen)
{
    skip_spaces_local(ps);
    if (fr->type == FIELD_FLOAT)
        return (validate_float_field(fr, ps, line, errbuf, errlen));
    else if (fr->type == FIELD_VEC3)
        return (validate_vec3_field(fr, ps, line, errbuf, errlen));
    else if (fr->type == FIELD_COLOR)
        return (validate_color_field(fr, ps, line, errbuf, errlen));
    else
    {
        snprintf(errbuf, errlen, "Line %d: unknown field type for '%s'",
            line, fr->name);
        return (0);
    }
}

static int	check_trailing(const char *p)
{
    skip_spaces_local(&p);
    if (*p == '\0' || *p == '\n')
        return (1);
    return (0);
}

static void	print_error_header(int *errors)
{
    if (*errors == 0)
        printf("Error\n");
}

void	check_global_rules(t_color acl, int *errors)
{
    if (acl.r != 1)
    {
        print_error_header(errors);
        fprintf(stderr,
            "Global check: ambient light 'A' must appear exactly once (found %.0f)\n",
            acl.r);
        (*errors)++;
    }
    if (acl.g != 1)
    {
        print_error_header(errors);
        fprintf(stderr,
            "Global check: camera 'C' must appear exactly once (found %.0f)\n",
            acl.g);
        (*errors)++;
    }
    if (acl.b != 1)
    {
        print_error_header(errors);
        fprintf(stderr,
            "Global check: light 'L' must appear exactly once (found %.0f)\n",
            acl.b);
        (*errors)++;
    }
    if (*errors != 0)
        printf("Validation failed: %d error(s) found in 'rt' file.\n",
            *errors);
}

static int	check_keyword_a(const char *p, char *key, t_color *rgb_acl)
{
    if (ft_memcmp(p, "A ", 2) == 0)
    {
        ft_strlcpy(key, "A", 4);
        rgb_acl->r++;
        return (1);
    }
    return (0);
}

static int	check_keyword_c(const char *p, char *key, t_color *rgb_acl)
{
    if (ft_memcmp(p, "C ", 2) == 0)
    {
        ft_strlcpy(key, "C", 4);
        rgb_acl->g++;
        return (1);
    }
    return (0);
}

static int	check_keyword_l(const char *p, char *key, t_color *rgb_acl)
{
    if (ft_memcmp(p, "L ", 2) == 0)
    {
        ft_strlcpy(key, "L", 4);
        rgb_acl->b++;
        return (1);
    }
    return (0);
}

static int	check_geometry_keywords(const char *p, char *key)
{
    if (ft_memcmp(p, "sp ", 3) == 0)
    {
        ft_strlcpy(key, "sp", 4);
        return (1);
    }
    else if (ft_memcmp(p, "pl ", 3) == 0)
    {
        ft_strlcpy(key, "pl", 4);
        return (1);
    }
    else if (ft_memcmp(p, "cy ", 3) == 0)
    {
        ft_strlcpy(key, "cy", 4);
        return (1);
    }
	//added bea
	else if (ft_memcmp(p, "co ", 3) == 0)
	{
		ft_strlcpy(key, "co", 4);
		return (1);
	}
    return (0);
}

int	is_a_valid_keyword(const char *p, char *key, t_color *rgb_acl)
{
    key[0] = '\0';
    if (check_keyword_a(p, key, rgb_acl))
        return (1);
    if (check_keyword_c(p, key, rgb_acl))
        return (1);
    if (check_keyword_l(p, key, rgb_acl))
        return (1);
    if (check_geometry_keywords(p, key))
        return (1);
    return (0);
}

static int	process_line_validation(const char *p, int line, char *key,
    int *errors)
{
    t_object_rule	*rule;
    char			errbuf[128];
    int				i;

    rule = find_rule(key);
    if (!rule)
    {
        print_error_header(errors);
        fprintf(stderr, "Line %d: unknown keyword '%s'\n", line, key);
        (*errors)++;
        return (0);
    }
    p += ft_strlen(key);
    i = 0;
    while (i < rule->field_count)
    {
        if (!validate_field(&rule->fields[i], &p, line, errbuf,
                sizeof(errbuf)))
        {
            print_error_header(errors);
            fprintf(stderr, "%s\n", errbuf);
            (*errors)++;
            return (0);
        }
        i++;
    }
    if (!check_trailing(p))
    {
        print_error_header(errors);
        fprintf(stderr,
            "Line %d: extra tokens or malformed input after fields\n",
            line);
        (*errors)++;
        return (0);
    }
    return (1);
}

static int	process_single_line(char *line_str, int line_num, t_color *rgb_acl,
    int *errors)
{
    const char	*p;
    char		key[4];

    p = line_str;
    while (ft_isspace((unsigned char)*p))
        p++;
    if (*p == '\0' || *p == '\n' || *p == '#')
        return (1);
    if (!is_a_valid_keyword(p, key, rgb_acl))
    {
        print_error_header(errors);
        fprintf(stderr, "Line %d: unknown or malformed keyword\n", line_num);
        (*errors)++;
        return (0);
    }
    return (process_line_validation(p, line_num, key, errors));
}

int	validate_config_file_full(const char *filename)
{
    char	*line_str;
    int		line;
    t_color	rgb_acl;
    int		errors;

    rgb_acl = (t_color){0, 0, 0};
    errors = 0;
    line = 0;
    line_str = NULL;
    while (1)
    {
        if (line_str)
            free(line_str);
        line_str = get_next_line(fd);
        if (!line_str)
            break ;
        line++;
        process_single_line(line_str, line, &rgb_acl, &errors);
    }
    if (line_str)
        free(line_str);
    close(fd);
    check_global_rules(rgb_acl, &errors);
    return (errors == 0);
}


/*
static void	skip_spaces_local(const char **s)
{
    while (**s && ft_isspace((unsigned char)**s))
        (*s)++;
}

static int	parse_float_local(const char *s, double *out, int *nread)
{
    char	*end;
    double	v;

    end = NULL;
    errno = 0;
    v = strtod(s, &end);
    if (end == s)
        return (0);
    if (errno == ERANGE)
        return (0);
    *out = v;
    *nread = (int)(end - s);
    return (1);
}

static int	parse_vec3_component(const char **p, double *val, int *total, int i)
{
    int	r;

    skip_spaces_local(p);
    r = 0;
    if (!parse_float_local(*p, val, &r))
        return (0);
    *p += r;
    *total += r;
    skip_spaces_local(p);
    if (i < 2)
    {
        if (**p != ',')
            return (0);
        (*p)++;
        (*total)++;
    }
    return (1);
}

static int	parse_vec3_local(const char *s, double v[3], int *nread)
{
    int			total;
    const char	*p;
    int			i;

    total = 0;
    p = s;
    i = 0;
    while (i < 3)
    {
        if (!parse_vec3_component(&p, &v[i], &total, i))
            return (0);
        i++;
    }
    *nread = total;
    return (1);
}

static int	parse_color_component(const char **p, int *color, int *total, int i)
{
    char	*end;
    long	val;
    int		r;

    skip_spaces_local(p);
    end = NULL;
    val = strtol(*p, &end, 10);
    if (end == *p)
        return (0);
    if (val < INT_MIN || val > INT_MAX)
        return (0);
    *color = (int)val;
    r = (int)(end - *p);
    *total += r;
    *p = end;
    skip_spaces_local(p);
    if (i < 2)
    {
        if (**p != ',')
            return (0);
        (*p)++;
        (*total)++;
    }
    return (1);
}

static int	parse_color_local(const char *s, int c[3], int *nread)
{
    int			total;
    const char	*p;
    int			i;

    total = 0;
    p = s;
    i = 0;
    while (i < 3)
    {
        if (!parse_color_component(&p, &c[i], &total, i))
            return (0);
        i++;
    }
    *nread = total;
    return (1);
}

static int	validate_float_field(const t_field_rule *fr, const char **ps,
    int line, char *errbuf, size_t errlen)
{
    double	val;
    int		n;

    n = 0;
    if (!parse_float_local(*ps, &val, &n))
    {
        snprintf(errbuf, errlen, "Line %d: expected float for '%s'",
            line, fr->name);
        return (0);
    }
if ((fr->min != 0.0 || fr->max != 0.0) && (val < fr->min || val > fr->max))
    {
        snprintf(errbuf, errlen,
            "Line %d: '%s' value %.1f out of range [%.1f,%.1f]",
            line, fr->name, val, fr->min, fr->max);
        return (0);
    }
    *ps += n;
    return (1);
}

static int	validate_vec3_range(double v[3], const t_field_rule *fr,
    int line, char *errbuf, size_t errlen)
{
    int	i;

    if (!(fr->min == fr->max && fr->min == 0.0))
    {
        i = 0;
        while (i < 3)
        {
            if (v[i] < fr->min || v[i] > fr->max)
            {
                snprintf(errbuf, errlen,
                    "Line %d: component of '%s' out of range [%.3f,%.3f]",
                    line, fr->name, fr->min, fr->max);
                return (0);
            }
            i++;
        }
    }
    return (1);
}

static int	validate_vec3_field(const t_field_rule *fr, const char **ps,
    int line, char *errbuf, size_t errlen)
{
    double	v[3];
    int		n;

    n = 0;
    if (!parse_vec3_local(*ps, v, &n))
    {
        snprintf(errbuf, errlen, "Line %d: expected vec3 'x,y,z' for '%s'",
            line, fr->name);
        return (0);
    }
    if (!validate_vec3_range(v, fr, line, errbuf, errlen))
        return (0);
    *ps += n;
    return (1);
}

static int	validate_color_range(int c[3], const t_field_rule *fr,
    int line, char *errbuf, size_t errlen)
{
    int	i;

    i = 0;
    while (i < 3)
    {
        if (c[i] < (int)fr->min || c[i] > (int)fr->max)
        {
            snprintf(errbuf, errlen,
                "Line %d: color component of '%s' must be in [%d,%d]",
                line, fr->name, (int)fr->min, (int)fr->max);
            return (0);
        }
        i++;
    }
    return (1);
}

static int	validate_color_field(const t_field_rule *fr, const char **ps,
    int line, char *errbuf, size_t errlen)
{
    int	c[3];
    int	n;

    n = 0;
    if (!parse_color_local(*ps, c, &n))
    {
        snprintf(errbuf, errlen, "Line %d: expected color 'r,g,b' for '%s'",
            line, fr->name);
        return (0);
    }
    if (!validate_color_range(c, fr, line, errbuf, errlen))
        return (0);
    *ps += n;
    return (1);
}

static int	validate_field(const t_field_rule *fr, const char **ps,
    int line, char *errbuf, size_t errlen)
{
    skip_spaces_local(ps);
    if (fr->type == FIELD_FLOAT)
        return (validate_float_field(fr, ps, line, errbuf, errlen));
    else if (fr->type == FIELD_VEC3)
        return (validate_vec3_field(fr, ps, line, errbuf, errlen));
    else if (fr->type == FIELD_COLOR)
        return (validate_color_field(fr, ps, line, errbuf, errlen));
    else
    {
        snprintf(errbuf, errlen, "Line %d: unknown field type for '%s'",
            line, fr->name);
        return (0);
    }
}

static int	check_trailing(const char *p)
{
    skip_spaces_local(&p);
    if (*p == '\0' || *p == '\n')
        return (1);
    return (0);
}

static void	print_error_header(int *errors)
{
    if (*errors == 0)
        printf("Error\n");
}

void	check_global_rules(t_color acl, int *errors)
{
    if (acl.r != 1)
    {
        print_error_header(errors);
        fprintf(stderr,
            "Global check: ambient light 'A' must appear exactly once (found %.0f)\n",
            acl.r);
        (*errors)++;
    }
    if (acl.g != 1)
    {
        print_error_header(errors);
        fprintf(stderr,
            "Global check: camera 'C' must appear exactly once (found %.0f)\n",
            acl.g);
        (*errors)++;
    }
    if (acl.b != 1)
    {
        print_error_header(errors);
        fprintf(stderr,
            "Global check: light 'L' must appear exactly once (found %.0f)\n",
            acl.b);
        (*errors)++;
    }
    if (*errors != 0)
        printf("Validation failed: %d error(s) found in 'rt' file.\n", *errors);
}

static int	check_keyword_a(const char *p, char *key, t_color *rgb_acl)
{
    if (ft_memcmp(p, "A ", 2) == 0)
    {
        ft_strlcpy(key, "A", 4);
        rgb_acl->r++;
        return (1);
    }
    return (0);
}

static int	check_keyword_c(const char *p, char *key, t_color *rgb_acl)
{
    if (ft_memcmp(p, "C ", 2) == 0)
    {
        ft_strlcpy(key, "C", 4);
        rgb_acl->g++;
        return (1);
    }
    return (0);
}

static int	check_keyword_l(const char *p, char *key, t_color *rgb_acl)
{
    if (ft_memcmp(p, "L ", 2) == 0)
    {
        ft_strlcpy(key, "L", 4);
        rgb_acl->b++;
        return (1);
    }
    return (0);
}

static int	check_geometry_keywords(const char *p, char *key)
{
    if (ft_memcmp(p, "sp ", 3) == 0)
    {
        ft_strlcpy(key, "sp", 4);
        return (1);
    }
    else if (ft_memcmp(p, "pl ", 3) == 0)
    {
        ft_strlcpy(key, "pl", 4);
        return (1);
    }
    else if (ft_memcmp(p, "cy ", 3) == 0)
    {
        ft_strlcpy(key, "cy", 4);
        return (1);
    }
    return (0);
}

int	is_a_valid_keyword(const char *p, char *key, t_color *rgb_acl)
{
    key[0] = '\0';
    if (check_keyword_a(p, key, rgb_acl))
        return (1);
    if (check_keyword_c(p, key, rgb_acl))
        return (1);
    if (check_keyword_l(p, key, rgb_acl))
        return (1);
    if (check_geometry_keywords(p, key))
        return (1);
    return (0);
}

static int	process_line_validation(const char *p, int line, char *key,
    int *errors)
{
    t_object_rule	*rule;
    char			errbuf[128];
    int				i;

    rule = find_rule(key);
    if (!rule)
    {
        print_error_header(errors);
        fprintf(stderr, "Line %d: unknown keyword '%s'\n", line, key);
        (*errors)++;
        return (0);
    }
    p += ft_strlen(key);
    i = 0;
    while (i < rule->field_count)
    {
        if (!validate_field(&rule->fields[i], &p, line, errbuf,
            sizeof(errbuf)))
        {
            print_error_header(errors);
            fprintf(stderr, "%s\n", errbuf);
            (*errors)++;
            return (0);
        }
        i++;
    }
    if (!check_trailing(p))
    {
        print_error_header(errors);
        fprintf(stderr,
            "Line %d: extra tokens or malformed input after fields\n",
            line);
        (*errors)++;
        return (0);
    }
    return (1);
}

static int	process_single_line(char *line_str, int line_num, t_color *rgb_acl,
    int *errors)
{
    const char	*p;
    char		key[4];

    p = line_str;
    while (ft_isspace((unsigned char)*p))
        p++;
    if (*p == '\0' || *p == '\n' || *p == '#')
        return (1);
    if (!is_a_valid_keyword(p, key, rgb_acl))
    {
        print_error_header(errors);
        fprintf(stderr, "Line %d: unknown or malformed keyword\n", line_num);
        (*errors)++;
        return (0);
    }
    return (process_line_validation(p, line_num, key, errors));
}

int	validate_config_file_full(const char *filename)
{
    int		fd;
    char	*line_str;
    int		line;
    t_color	rgb_acl;
    int		errors;

    fd = open(filename, O_RDONLY);
    if (fd < 0)
    {
        perror("Error\nfopen");
        return (0);
    }
    rgb_acl = (t_color){0, 0, 0};
    errors = 0;
    line = 0;
    line_str = NULL;
    while (1)
    {
        if (line_str)
            free(line_str);
        line_str = get_next_line(fd);
        if (!line_str)
            break ;
        line++;
        process_single_line(line_str, line, &rgb_acl, &errors);
    }
    if (line_str)
        free(line_str);
    close(fd);
    check_global_rules(rgb_acl, &errors);
    return (errors == 0);
}
*/

