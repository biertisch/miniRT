
#include "minirt.h"

t_aabb	surrounding_box(t_aabb a, t_aabb b)
{
	t_aabb	box;

	box.min.x = fmin(a.min.x, b.min.x);
	box.min.y = fmin(a.min.y, b.min.y);
	box.min.z = fmin(a.min.z, b.min.z);
	box.max.x = fmax(a.max.x, b.max.x);
	box.max.y = fmax(a.max.y, b.max.y);
	box.max.z = fmax(a.max.z, b.max.z);
	return (box);
}

static void	swap_tri(t_triangle *a, t_triangle *b)
{
	t_triangle tmp = *a;
	*a = *b;
	*b = tmp;
}
static int	longest_axis(t_aabb box)
{
	t_vec3 size;

	size.x = box.max.x - box.min.x;
	size.y = box.max.y - box.min.y;
	size.z = box.max.z - box.min.z;

	if (size.x > size.y && size.x > size.z)
		return (0);
	if (size.y > size.z)
		return (1);
	return (2);
}


t_aabb	triangles_get_aabb_c(t_triangle *tris, int count)
{
	t_aabb	box;
	int		i;

	if (count <= 0)
		return ((t_aabb){0});

	// 用第一个三角形初始化
	box = triangle_get_aabb(&tris[0]);

	i = 1;
	while (i < count)
	{
		box.min.x = fmin(box.min.x,
			fmin(fmin(tris[i].p1.x, tris[i].p2.x), tris[i].p3.x));
		box.min.y = fmin(box.min.y,
			fmin(fmin(tris[i].p1.y, tris[i].p2.y), tris[i].p3.y));
		box.min.z = fmin(box.min.z,
			fmin(fmin(tris[i].p1.z, tris[i].p2.z), tris[i].p3.z));

		box.max.x = fmax(box.max.x,
			fmax(fmax(tris[i].p1.x, tris[i].p2.x), tris[i].p3.x));
		box.max.y = fmax(box.max.y,
			fmax(fmax(tris[i].p1.y, tris[i].p2.y), tris[i].p3.y));
		box.max.z = fmax(box.max.z,
			fmax(fmax(tris[i].p1.z, tris[i].p2.z), tris[i].p3.z));
		i++;
	}
	return (box);
}

static int	partition(t_triangle *tris, int left, int right, int axis)
{
	double pivot;
	int i;
	int j;

	pivot = (axis == 0) ? tris[(left + right) / 2].centroid.x :
	        (axis == 1) ? tris[(left + right) / 2].centroid.y :
	                      tris[(left + right) / 2].centroid.z;

	i = left;
	j = right;
	while (i <= j)
	{
		while (((axis == 0) ? tris[i].centroid.x :
		        (axis == 1) ? tris[i].centroid.y :
		                      tris[i].centroid.z) < pivot)
			i++;
		while (((axis == 0) ? tris[j].centroid.x :
		        (axis == 1) ? tris[j].centroid.y :
		                      tris[j].centroid.z) > pivot)
			j--;
		if (i <= j)
		{
			swap_tri(&tris[i], &tris[j]);
			i++;
			j--;
		}
	}
	return (i);
}


static void	select_median(t_triangle *tris, int left, int right, int k, int axis)
{
	int index;

	if (left >= right)
		return;
	index = partition(tris, left, right, axis);
	if (k < index)
		select_median(tris, left, index - 1, k, axis);
	else if (k > index)
		select_median(tris, index, right, k, axis);
}


t_bvh_node *build_bvh(t_triangle *tris, int count)
{
	t_bvh_node *node;
	int axis;
	int mid;

	node = malloc(sizeof(t_bvh_node));
	if (!node)
		return (NULL);

	node->box = triangles_get_aabb_c(tris, count);

	// ✅ 叶子节点：小数量直接 brute force
	if (count <= 4)
	{
		node->triangles = tris;
		node->tri_count = count;
		node->left = NULL;
		node->right = NULL;
		return (node);
	}

	axis = longest_axis(node->box);
	mid = count / 2;

	// 🔥 O(N) 分割，而不是排序
	select_median(tris, 0, count - 1, mid, axis);

	node->left = build_bvh(tris, mid);
	node->right = build_bvh(tris + mid, count - mid);

	node->triangles = NULL;
	node->tri_count = 0;

	return (node);
}

// t_bvh_node *build_bvh(t_triangle *tris, int count)
// {
// 	t_bvh_node *node = malloc(sizeof(t_bvh_node));
// 	if (!node) return NULL;

// 	if (count == 1)
// 	{
// 		node->triangles = tris;
// 		node->tri_count = 1;
// 		node->left = node->right = NULL;
// 		node->box = triangle_get_aabb(&tris[0]);
// 	}
// 	else
// 	{
// 		// 随机选择轴：0=x,1=y,2=z
// 		int axis = rand() % 3;
// 		// 简单排序
// 		for (int i = 0; i < count-1; i++)
// 		{
// 			for (int j = i+1; j < count; j++)
// 			{
// 				t_vec3 center_i = {(tris[i].p1.x+tris[i].p2.x+tris[i].p3.x)/3.0,
// 					(tris[i].p1.y+tris[i].p2.y+tris[i].p3.y)/3.0,
// 					(tris[i].p1.z+tris[i].p2.z+tris[i].p3.z)/3.0};
// 				t_vec3 center_j = {(tris[j].p1.x+tris[j].p2.x+tris[j].p3.x)/3.0,
// 					(tris[j].p1.y+tris[j].p2.y+tris[j].p3.y)/3.0,
// 					(tris[j].p1.z+tris[j].p2.z+tris[j].p3.z)/3.0};
// 				if ((axis==0 && center_i.x > center_j.x) ||
// 					(axis==1 && center_i.y > center_j.y) ||
// 					(axis==2 && center_i.z > center_j.z))
// 				{
// 					t_triangle tmp = tris[i]; tris[i] = tris[j]; tris[j] = tmp;
// 				}
// 			}
// 		}
// 		int mid = count / 2;
// 		node->left = build_bvh(tris, mid);
// 		node->right = build_bvh(tris + mid, count - mid);
// 		node->triangles = NULL;
// 		node->tri_count = 0;
// 		t_aabb box_left = node->left->box;
// 		t_aabb box_right = node->right->box;
// 		node->box = surrounding_box(box_left, box_right);
// 	}
// 	return node;
// }

int hit_aabb(t_aabb *box, t_ray *ray, double t_min, double t_max)
{
	for (int a = 0; a < 3; a++)
	{
		double invD, t0, t1;
		if (a == 0)
		{
			if (ray->direction.x < 1e-8 && ray->direction.x > -1e-8)
			{
				if (ray->origin.x < box->min.x || ray->origin.x > box->max.x)
					return (0);
			}
			else
				invD = 1.0 / ray->direction.x;
		}
		if (a == 1)
		{
			if (ray->direction.y < 1e-8 && ray->direction.y > -1e-8)
			{
				if (ray->origin.y < box->min.y || ray->origin.y > box->max.y)
					return (0);
			}
			else
				invD = 1.0 / ray->direction.y;
		}
		if (a == 2)
		{
			if (ray->direction.z < 1e-8 && ray->direction.z > -1e-8)
			{
				if (ray->origin.z < box->min.z || ray->origin.z > box->max.z)
					return (0);
			}
			else
				invD = 1.0 / ray->direction.z;
		}

		double origin_axis;
		double min_axis, max_axis;
		if (a == 0) origin_axis = ray->origin.x, min_axis = box->min.x, max_axis = box->max.x;
		if (a == 1) origin_axis = ray->origin.y, min_axis = box->min.y, max_axis = box->max.y;
		if (a == 2) origin_axis = ray->origin.z, min_axis = box->min.z, max_axis = box->max.z;

		t0 = (min_axis - origin_axis) * invD;
		t1 = (max_axis - origin_axis) * invD;
		if (invD < 0.0) { double tmp = t0; t0 = t1; t1 = tmp; }
		t_min = fmax(t0, t_min);
		t_max = fmin(t1, t_max);
		if (t_max < t_min)
			return (0);
	}
	return (1);
}
/*
t_triangle *hit_bvh(t_bvh_node *node, t_ray *ray, double t_min, double t_max, double *hit_t)
{
	t_hit_record temp_rec;

	temp_rec = (t_hit_record){0};
    if (!hit_aabb(&node->box, ray, t_min, t_max))
        return NULL;

    t_triangle *hit_tri = NULL;

    if (node->tri_count > 0)
    {
		(void)temp_rec;
		// printf("BVH leaf node hit with %d triangles\n", node->tri_count);
		return node->triangles;
        // 遍历叶子节点的三角形
        // for (int i=0; i<node->tri_count; i++)
        // {
		// 	t_object tri_obj;
		// 	tri_obj.geo.triangle = node->triangles[i];
		// 	tri_obj.type = TRIANGLE;
		// 	if (triangle_hit(ray, (t_interval){t_min, t_max},
		// 		tri_obj, &temp_rec))
        //     // if (ray_hits_triangle(ray, &node->triangles[i], &t)) // 你已有函数
        //     {
        //         if (temp_rec.t < *hit_t)
        //         {
        //             *hit_t = temp_rec.t;
        //             hit_tri = &node->triangles[i];
        //         }
        //     }
        // }
    }
    else
    {
        double t_left = *hit_t;
        t_triangle *left_hit = hit_bvh(node->left, ray, t_min, t_max, &t_left);
        if (left_hit) { *hit_t = t_left; hit_tri = left_hit; }

        double t_right = *hit_t;
        t_triangle *right_hit = hit_bvh(node->right, ray, t_min, t_max, &t_right);
        if (right_hit) { *hit_t = t_right; hit_tri = right_hit; }
    }
    return hit_tri;
}
*/

t_triangle	*hit_bvh(t_bvh_node *node, t_ray *ray,
				double t_min, double t_max, double *hit_t)
{
	if (!node || !hit_aabb(&node->box, ray, t_min, t_max))
		return (NULL);

	if (node->tri_count > 0)
	{
		// 叶子节点，检查所有三角形
		t_triangle *closest_tri = NULL;
		double closest_t = *hit_t;

		for (int i = 0; i < node->tri_count; i++)
		{
			t_hit_record temp_rec = {0};
			if (triangle_hit2(ray, (t_interval){t_min, closest_t},
				&node->triangles[i], &temp_rec))
			{
				if (temp_rec.t < closest_t)
				{
					closest_t = temp_rec.t;
					closest_tri = &node->triangles[i];
				}
			}
		}
		*hit_t = closest_t;
		return (closest_tri);
	}

	t_triangle *hit_tri = NULL;
	double closest = *hit_t;

	t_triangle *left = hit_bvh(node->left, ray, t_min, closest, &closest);
	if (left) hit_tri = left;

	t_triangle *right = hit_bvh(node->right, ray, t_min, closest, &closest);
	if (right) hit_tri = right;

	*hit_t = closest;
	return (hit_tri);
}

void	init_wld_bvh(t_world *wld)
{
	// 统计三角形总数
	int total_tris = 0;
	for (int i=0; i<wld->num_objects; i++)
	{
		if (wld->objects[i]->type == TRIANGLE)
			total_tris++;
	}
	if (total_tris == 0)
		return;
	// 收集所有三角形
	t_triangle *all_tris = malloc(sizeof(t_triangle) * total_tris);
	if (!all_tris)
		return;
	int index = 0;
	for (int i=0; i<wld->num_objects; i++)
	{
		if (wld->objects[i]->type == TRIANGLE)
		{
			// 计算质心
			t_triangle *tri = &wld->objects[i]->geo.triangle;
			tri->centroid.x = (tri->p1.x + tri->p2.x + tri->p3.x) / 3.0;
			tri->centroid.y = (tri->p1.y + tri->p2.y + tri->p3.y) / 3.0;
			tri->centroid.z = (tri->p1.z + tri->p2.z + tri->p3.z) / 3.0;	
			all_tris[index] = wld->objects[i]->geo.triangle;
			index++;
		}
	}
	// 构建BVH
	wld->bvh_triangle_root = build_bvh(all_tris, total_tris);
	printf("BVH built with %d triangles\n", total_tris);
	// free(all_tris);
}