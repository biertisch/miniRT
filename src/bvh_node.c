#include "minirt.h"

int	box_compare(t_object a, t_object b, int axis_index)
{
	t_interval a_axis_interval;
	t_interval b_axis_interval;

	a_axis_interval = aabb_axis_interval(&a.bbox, axis_index);
	b_axis_interval = aabb_axis_interval(&b.bbox, axis_index);
	return (a_axis_interval.min < b_axis_interval.min);
}

int box_x_compare (const void *a, const void *b)
{
	return box_compare(*(t_object *)a, *(t_object *)b, 0);
}

int box_y_compare (const void *a, const void *b)
{
	return box_compare(*(t_object *)a, *(t_object *)b, 1);
}

int box_z_compare (const void *a, const void *b)
{
	return box_compare(*(t_object *)a, *(t_object *)b, 2);
}

t_object	*bvh_from_objects(t_object **objects, int start, int end)
{
	int			num_objects;
	int			axis;
	int			mid;
	int			i;
	t_bvh_node	*node;
	t_object	*left;
	t_object	*right;
	t_aabb		bbox;

	bbox = aabb_empty();
	i = start;
	while (i < end)
	{
		bbox = get_aabb_surrounding(&bbox, &objects[i]->bbox);
		i++;
	}
	axis = aabb_longest_axis(&bbox);
	num_objects = end - start;
	if (num_objects == 1)
	{
		left = objects[start];
		right = objects[start];
	}
	else if (num_objects == 2)
	{
		left = objects[start];
		right = objects[start + 1];
	}
	else
	{
		if (axis == 0)
			qsort(objects + start, num_objects, sizeof(t_object *), box_x_compare);
		else if (axis == 1)
			qsort(objects + start, num_objects, sizeof(t_object *), box_y_compare);
		else
			qsort(objects + start, num_objects, sizeof(t_object *), box_z_compare);

		mid = start + num_objects / 2;
		left = bvh_from_objects(objects, start, mid);
		right = bvh_from_objects(objects, mid, end);
	}
	node = malloc(sizeof(t_bvh_node));
	if (!node)
		return (NULL);
	node->left = left;
	node->right = right;
	node->bbox = bbox;

	printf("BVH Node created: [%d - %d], axis: %d\n", start, end, axis);
	printf("  Left bbox: x[%f,%f] y[%f,%f] z[%f,%f]\n",
		node->left->bbox.x.min, node->left->bbox.x.max,
		node->left->bbox.y.min, node->left->bbox.y.max,
		node->left->bbox.z.min, node->left->bbox.z.max);
	printf("  Right bbox: x[%f,%f] y[%f,%f] z[%f,%f]\n",
		node->right->bbox.x.min, node->right->bbox.x.max,
		node->right->bbox.y.min, node->right->bbox.y.max,
		node->right->bbox.z.min, node->right->bbox.z.max);
	printf("  Node bbox: x[%f,%f] y[%f,%f] z[%f,%f]\n",
		node->bbox.x.min, node->bbox.x.max,
		node->bbox.y.min, node->bbox.y.max,
		node->bbox.z.min, node->bbox.z.max);

	t_object	*obj_node;
	obj_node = malloc(sizeof(t_object));
	if (!obj_node)
	{
		free(node);
		return (NULL);
	}
	memset(obj_node, 0, sizeof(t_object));//need change to libft_memset
	*obj_node = (t_object){
		.geo.bvh_node = *node,
		.type = BVH_NODE,
		.hit = &bvh_node_hit,
		.bbox = node->bbox,
		.next = NULL};
	free(node);
	return (obj_node);
}

int	bvh_node_hit(t_ray *ray, t_interval ray_t, t_object obj,  t_hit_record *record)
{
	int	hit_left;
	int	hit_right;

	if (!aabb_hit(&obj.bbox, ray, ray_t))
		return (0);

	if (obj.type == BVH_NODE)
		hit_left = obj.hit(ray, ray_t, *(t_object *)obj.geo.bvh_node.left, record);
	else
		hit_left = obj.hit(ray, ray_t, obj, record);
	if (hit_left)
		ray_t.max = record->t;
	if (obj.type == BVH_NODE)
		hit_right = obj.hit(ray, ray_t, *(t_object *)obj.geo.bvh_node.right, record);
	else
		hit_right = obj.hit(ray, ray_t, obj, record);
	return (hit_left || hit_right);
}
