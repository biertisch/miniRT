/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pdf.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bliu <bliu@student.42lisboa.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/21 16:51:27 by bliu              #+#    #+#             */
/*   Updated: 2025/12/21 23:22:53 by bliu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
/*
double	sphere_pdf_value(t_pdf *self, t_vec3 direction)
{
	(void)self;
	(void)direction;
	return (1.0 / (4.0 * M_PI));
}
t_vec3	sphere_pdf_generate(t_pdf *self)
{
	(void)self;
	return (random_unit_vec3());
}

t_cosine_pdf	cosine_pdf_new(t_vec3 w)
{
	t_cosine_pdf	cpdf;

	cpdf.uvw = onb_new(w);
	cpdf.base.value = &cosine_pdf_value;
	cpdf.base.generate = &cosine_pdf_generate;
	return (cpdf);
}

t_vec3	cosine_pdf_generate(t_pdf *self)
{
	t_cosine_pdf	*cpdf;

	cpdf = (t_cosine_pdf *)self;
	return (onb_transform(cpdf->uvw, random_cosine_direction()));
}
*/

double	cosine_pdf_value(t_pdf *self, t_vec3 direction)
{
	double			cosine;
	t_cosine_pdf	*cpdf;

	cpdf = (t_cosine_pdf *)self;
	cosine = vec3_dot(unit_vector(direction), onb_w(cpdf->uvw));
	return (fmax(cosine / M_PI, 0.0));
}

double	hitable_pdf_value(t_pdf *self, t_vec3 dir)
{
	t_hitable_pdf	*hpdf;

	hpdf = (t_hitable_pdf *)self;
	return (hpdf->objects->pdf_value(*(hpdf->objects), hpdf->origin, dir));
}

t_vec3	hitable_pdf_generate(t_pdf *self)
{
	t_hitable_pdf	*hpdf;

	hpdf = (t_hitable_pdf *)self;
	return (hpdf->objects->random(*(hpdf->objects), hpdf->origin));
}

t_hitable_pdf	hitable_pdf_new(t_object *obj, t_vec3 origin)
{
	t_hitable_pdf	hpdf;

	hpdf.objects = obj;
	hpdf.origin = origin;
	hpdf.base.value = &hitable_pdf_value;
	hpdf.base.generate = &hitable_pdf_generate;
	return (hpdf);
}
