#include "minirt.h"

t_color	texture_value(t_texture *self, double u, double v, t_vec3 p)
{
	return (self->value(self, u, v, p));
}