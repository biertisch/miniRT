
#include "parser.h"
#include <ctype.h>
#include <fcntl.h>
#include <errno.h>

static void skip_spaces_local(const char **s)
{
    while (**s && ft_isspace((unsigned char)**s))
        (*s)++;
}

/* parse a float from string s (advance pointer via nread) */
static int parse_float_local(const char *s, double *out, int *nread)
{
    char *end = NULL;
    errno = 0;
    double v = strtod(s, &end);
    if (end == s)
        return (0);
    if (errno == ERANGE)
        return (0);
    *out = v;
    *nread = (int)(end - s);
    return (1);
}

/* parse vec3 as "x,y,z" with commas; return 1 on success */
static int parse_vec3_local(const char *s, double v[3], int *nread)
{
    int total = 0;
    const char *p = s;
    for (int i = 0; i < 3; i++)
    {
        skip_spaces_local(&p);
        int r = 0;
        if (!parse_float_local(p, &v[i], &r))
            return (0);
        p += r;
        total += r;
        skip_spaces_local(&p);
        if (i < 2)
        {
            if (*p != ',')
                return (0);
            p++;
            total++;
        }
    }
    *nread = total;
    return (1);
}

/* parse color "r,g,b" ints */
static int parse_color_local(const char *s, int c[3], int *nread)
{
    int total = 0;
    const char *p = s;
    for (int i = 0; i < 3; i++)
    {
        skip_spaces_local(&p);
        char *end = NULL;
        long val = strtol(p, &end, 10);
        if (end == p)
            return (0);
        if (val < INT_MIN || val > INT_MAX)
            return (0);
        c[i] = (int)val;
        int r = (int)(end - p);
        total += r;
        p = end;
        skip_spaces_local(&p);
        if (i < 2)
        {
            if (*p != ',')
                return (0);
            p++;
            total++;
        }
    }
    *nread = total;
    return (1);
}

/* validate one field according to its rule; advance pointer *ps on success */
static int validate_field(const t_field_rule *fr, const char **ps, int line, char *errbuf, size_t errlen)
{
    const char *p = *ps;
    skip_spaces_local(&p);
    if (fr->type == FIELD_FLOAT)
    {
        double val;
        int n = 0;
        if (!parse_float_local(p, &val, &n))
        {
            snprintf(errbuf, errlen, "Line %d: expected float for '%s'", line, fr->name);
            return (0);
        }
        if (!(fr->min == fr->max && fr->min == 0.0) && (val < fr->min || val > fr->max))
        {
            snprintf(errbuf, errlen, "Line %d: '%s' out of range [%.3f,%.3f]", line, fr->name, fr->min, fr->max);
            return (0);
        }
        p += n;
    }
    else if (fr->type == FIELD_VEC3)
    {
        double v[3];
        int n = 0;
        if (!parse_vec3_local(p, v, &n))
        {
            snprintf(errbuf, errlen, "Line %d: expected vec3 'x,y,z' for '%s'", line, fr->name);
            return (0);
        }
        if (!(fr->min == fr->max && fr->min == 0.0)) /* use min/max as per-component bounds if set */
        {
            for (int i = 0; i < 3; i++)
            {
                if (v[i] < fr->min || v[i] > fr->max)
                {
                    snprintf(errbuf, errlen, "Line %d: component of '%s' out of range [%.3f,%.3f]", line, fr->name, fr->min, fr->max);
                    return (0);
                }
            }
        }
        p += n;
    }
    else if (fr->type == FIELD_COLOR)
    {
        int c[3];
        int n = 0;
        if (!parse_color_local(p, c, &n))
        {
            snprintf(errbuf, errlen, "Line %d: expected color 'r,g,b' for '%s'", line, fr->name);
            return (0);
        }
        for (int i = 0; i < 3; i++)
        {
            if (c[i] < (int)fr->min || c[i] > (int)fr->max)
            {
                snprintf(errbuf, errlen, "Line %d: color component of '%s' must be in [%d,%d]", line, fr->name, (int)fr->min, (int)fr->max);
                return (0);
            }
        }
        p += n;
    }
    else
    {
        snprintf(errbuf, errlen, "Line %d: unknown field type for '%s'", line, fr->name);
        return (0);
    }
    *ps = p;
    return (1);
}

/* check that no extra tokens remain on the line */
static int check_trailing(const char *p)
{
    skip_spaces_local(&p);
    if (*p == '\0' || *p == '\n')
        return (1);
    return (0);
}

void	check_global_rules(t_color acl, int *errors)
{
	if (acl.r != 1)
	{
		if (*errors == 0)
				printf("Error\n");
		fprintf(stderr, "Global check: ambient light 'A' must appear exactly once (found %.0f)\n", acl.r);
		(*errors)++;
	}
	if (acl.g != 1)
	{
		if(*errors == 0)
				printf("Error\n");
		fprintf(stderr, "Global check: camera 'C' must appear exactly once (found %.0f)\n", acl.g);
		(*errors)++;
	}
	if (acl.b != 1)
	{
		if(*errors == 0)
				printf("Error\n");
		// fprintf(stderr, "Global check: at least one light 'L' is required (found %.0f)\n", acl.b);
		fprintf(stderr, "Global check: camera 'L' must appear exactly once (found %.0f)\n", acl.b);
		(*errors)++;
	}
	if (*errors != 0)
		printf("Validation failed: %d error(s) found in 'rt' file.\n", *errors);
}

int	is_a_valid_keyword(const char	*p, char *key, t_color *rgb_acl)
{
	if (ft_memcmp(p, "A ", 2) == 0){
		ft_strlcpy(key, "A", sizeof(key));
		rgb_acl->r++;
	}
	else if (ft_memcmp(p, "C ", 2) == 0)
	{
		ft_strlcat(key, "C", sizeof(key));
		rgb_acl->g++;
	}
	else if (ft_memcmp(p, "L ", 2) == 0)
	{
		ft_strlcpy(key, "L", sizeof(key));
		rgb_acl->b++;
	}
	else if (ft_memcmp(p, "sp ", 3) == 0)
		ft_strlcpy(key, "sp", sizeof(key));
	else if (ft_memcmp(p, "pl ", 3) == 0)
		ft_strlcpy(key, "pl", sizeof(key));
	else if (ft_memcmp(p, "cy ", 3) == 0)
		ft_strlcpy(key, "cy", sizeof(key));
	else	
		return (0);
	return (1);
}

/* main validator function for a file */
int validate_config_file_full(const char *filename)
{
	int fd;
	char *line_str;
	int line = 0;
	t_color	rgb_acl;
	int errors = 0;
	t_object_rule *rule;
	char key[4];
	const char *p;

	fd = open(filename, O_RDONLY);
	if (fd < 0)
	{
		perror("Error\nfopen");
		return (0);
	}
	rgb_acl = (t_color){0,0,0};
	line_str = NULL;
	while (1)
	{
		if (line_str)
			free(line_str);
		line_str = get_next_line(fd);
		if (!line_str)
			break;
		line++;
		p = line_str;
		while(ft_isspace((unsigned char)*p))
			p++;
		if (*p == '\0' || *p == '\n' || *p == '#')
			continue;
		key[0] = '\0';
		if (!is_a_valid_keyword(p, key, &rgb_acl))
		{
			if(errors == 0)
				printf("Error\n");
			fprintf(stderr, "Line %d: unknown or malformed keyword\n", line);
			errors++;
			continue;
		}
		rule = find_rule(key);
		if (!rule)
		{
			if(errors == 0)
				printf("Error\n");
			fprintf(stderr, "Line %d: unknown keyword '%s'\n", line, key);
			errors++;
			continue;
		}
		p += ft_strlen(key);
		for (int i = 0; i < rule->field_count; i++)
		{
			char errbuf[128];
			if (!validate_field(&rule->fields[i], &p, line, errbuf, sizeof(errbuf)))
			{
				if(errors == 0)
					printf("Error\n");
				fprintf(stderr, "%s\n", errbuf);
				errors++;
				break;
			}
		}
		if (!check_trailing(p))
		{
			if(errors == 0)
				printf("Error\n");
			fprintf(stderr, "Line %d: extra tokens or malformed input after fields\n", line);
			errors++;
		}
	}
	close(fd);
	check_global_rules(rgb_acl, &errors);
	return (errors == 0);
}
