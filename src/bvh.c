
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

t_bvh_node *build_bvh(t_triangle *tris, int count)
{
	t_bvh_node *node = malloc(sizeof(t_bvh_node));
	if (!node) return NULL;

	if (count == 1)
	{
		node->triangles = tris;
		node->tri_count = 1;
		node->left = node->right = NULL;
		node->box = triangle_get_aabb(&tris[0]);
	}
	else
	{
		// 随机选择轴：0=x,1=y,2=z
		int axis = rand() % 3;
		// 简单排序
		for (int i = 0; i < count-1; i++)
		{
			for (int j = i+1; j < count; j++)
			{
				t_vec3 center_i = {(tris[i].p1.x+tris[i].p2.x+tris[i].p3.x)/3.0,
					(tris[i].p1.y+tris[i].p2.y+tris[i].p3.y)/3.0,
					(tris[i].p1.z+tris[i].p2.z+tris[i].p3.z)/3.0};
				t_vec3 center_j = {(tris[j].p1.x+tris[j].p2.x+tris[j].p3.x)/3.0,
					(tris[j].p1.y+tris[j].p2.y+tris[j].p3.y)/3.0,
					(tris[j].p1.z+tris[j].p2.z+tris[j].p3.z)/3.0};
				if ((axis==0 && center_i.x > center_j.x) ||
					(axis==1 && center_i.y > center_j.y) ||
					(axis==2 && center_i.z > center_j.z))
				{
					t_triangle tmp = tris[i]; tris[i] = tris[j]; tris[j] = tmp;
				}
			}
		}
		int mid = count / 2;
		node->left = build_bvh(tris, mid);
		node->right = build_bvh(tris + mid, count - mid);
		node->triangles = NULL;
		node->tri_count = 0;
		t_aabb box_left = node->left->box;
		t_aabb box_right = node->right->box;
		node->box = surrounding_box(box_left, box_right);
	}
	return node;
}

int hit_aabb(t_aabb *box, t_ray *ray, double t_min, double t_max)
{
    for (int a = 0; a < 3; a++)
    {
        double invD, t0, t1;
        if (a == 0) invD = 1.0 / ray->direction.x;
        if (a == 1) invD = 1.0 / ray->direction.y;
        if (a == 2) invD = 1.0 / ray->direction.z;

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
        if (t_max <= t_min)
            return 0;
    }
    return 1;
}

t_triangle *hit_bvh(t_bvh_node *node, t_ray *ray, double t_min, double t_max, double *hit_t)
{
	t_hit_record temp_rec;

	temp_rec = (t_hit_record){0};
    if (!hit_aabb(&node->box, ray, t_min, t_max))
        return NULL;

    t_triangle *hit_tri = NULL;

    if (node->tri_count > 0)
    {
        // 遍历叶子节点的三角形
        for (int i=0; i<node->tri_count; i++)
        {
			t_object tri_obj;
			tri_obj.geo.triangle = node->triangles[i];
			tri_obj.type = TRIANGLE;
			if (triangle_hit(ray, (t_interval){t_min, t_max},
				tri_obj, &temp_rec))
            // if (ray_hits_triangle(ray, &node->triangles[i], &t)) // 你已有函数
            {
                if (temp_rec.t < *hit_t)
                {
                    *hit_t = temp_rec.t;
                    hit_tri = &node->triangles[i];
                }
            }
        }
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
			all_tris[index] = wld->objects[i]->geo.triangle;
			index++;
		}
	}
	// 构建BVH
	wld->bvh_triangle_root = build_bvh(all_tris, total_tris);
	free(all_tris);
}