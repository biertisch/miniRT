#include "minirt.h"

double	default_hitable_pdf_value(t_pdf *self, t_vec3 origin, t_vec3 direction)
{
	(void)self;
	(void)direction;
	(void)origin;
	return (0.0);
}

t_vec3	default_hitable_pdf_generate(t_pdf *self, t_vec3 origin)
{
	(void)self;
	(void)origin;
	return (new_vec3(1, 0, 0));
}

void	add_object_to_world(t_world *world, t_geo_type geo_type, void *geo)
{
	t_object	*new_object;

	if (world->num_objects >= 50)
	{
		printf("World object limit reached\n");
		return ;
	}
	new_object = (t_object *)malloc(sizeof(t_object));
	if (!new_object)
		return ;
	new_object->type = geo_type;
	switch (geo_type)
	{
		case SPHERE:
			new_object->geo.sphere = *(t_sphere *)geo;
			new_object->hit = sphere_hit;
			new_object->bbox = new_object->geo.sphere.bbox;
			break;
		case PLANE:
			new_object->geo.plane = *(t_plane *)geo;
			new_object->hit = plane_hit;
			new_object->bbox = new_object->geo.plane.bbox;
			break;
		case CYLINDER:
			new_object->geo.cylinder = *(t_cylinder *)geo;
			new_object->hit = cylinder_hit;
			new_object->bbox = new_object->geo.cylinder.bbox;
			break;
		case QUAD:
			new_object->geo.quad = *(t_quad *)geo;
			new_object->hit = quad_hit;
			new_object->bbox = new_object->geo.quad.bbox;
			break;
		default:
			free(new_object);
			printf("Unsupport geometry type\n");
			return ;
	}
	world->objects[world->num_objects] = new_object;
	world->num_objects++;
}

int	world_hit(t_object *objects, t_ray *ray, t_interval ray_t, t_hit_record *rec)
{
	t_hit_record	temp_rec;
	int				hit_anything;
	double			closest_so_far;
	t_object		*current;

	hit_anything = 0;
	closest_so_far = ray_t.max;
	current = objects;
	while (current != NULL)
	{
		ray_t.max = closest_so_far;
		
		if (current->hit && current->hit(ray, ray_t, *current, &temp_rec))
		{
			hit_anything = 1;
			closest_so_far = temp_rec.t;
			*rec = temp_rec;
		}
		current = current->next;
	}
	return (hit_anything);
}