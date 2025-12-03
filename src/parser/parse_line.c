#include "parser.h"

void parse_and_validate(char *params, t_object_rule *rule, int line)
{
    int i;

    i = 0;
    while (i < rule->field_count)
    {
        t_field_rule *f = &rule->fields[i];
        skip_spaces(&params);
        if (f->type == FIELD_FLOAT)
        {
            float val = parse_float(&params);
            if (val < f->min || val > f->max)
                error_line(line, f->name);
        }
        else if (f->type == FIELD_VEC3)
        {
            parse_vec3(&params);
        }
        else if (f->type == FIELD_COLOR)
        {
            t_color c = parse_color(&params);
            if (c.r < 0 || c.r > 255)
                error_line(line, f->name);
        }
        i++;
    }
}
