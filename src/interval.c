/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interval.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bliu <bliu@student.42lisboa.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/21 16:44:00 by bliu              #+#    #+#             */
/*   Updated: 2025/12/21 16:46:50 by bliu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

t_interval	new_interval(double min, double max)
{
	t_interval	interval;

	interval.min = min;
	interval.max = max;
	return (interval);
}

int	interval_contains(t_interval *interval, double value)
{
	return (value >= interval->min && value <= interval->max);
}

int	interval_surrounds(t_interval *interval, double value)
{
	return (value > interval->min && value < interval->max);
}

double	interval_clamp(t_interval *interval, double value)
{
	if (value < interval->min)
		return (interval->min);
	if (value > interval->max)
		return (interval->max);
	return (value);
}

/*
t_interval	interval_union(t_interval *a, t_interval *b)
{
	double	min;
	double	max;

	min = fmin(a->min, b->min);
	max = fmax(a->max, b->max);
	return (new_interval(min, max));
}

t_interval	empty_interval(void)
{
	return (new_interval(RT_INFINITY, -RT_INFINITY));
}
t_interval	universal_interval(void)
{
	return (new_interval(-RT_INFINITY, RT_INFINITY));
}

double	interval_size(t_interval *interval)
{
	return (interval->max - interval->min);
}

t_interval	interval_expand(t_interval *interval, double delta)
{
	return (new_interval(interval->min - delta / 2, interval->max + delta / 2));
}
*/