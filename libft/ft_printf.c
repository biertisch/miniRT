/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bliu <bliu@student.42lisboa.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 15:46:16 by bliu              #+#    #+#             */
/*   Updated: 2025/10/03 17:52:42 by bliu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_put_ptr_fd(void *ptr, int fd)
{
	int		count;

	count = 0;
	if (ptr == NULL)
		count += ft_putstr_fd("(nil)", fd);
	else
	{
		count += ft_putstr_fd("0x", fd);
		count += ft_ulnbr_b_fd((unsigned long)ptr, BASE_16, fd);
	}
	return (count);
}

static int	ft_output_format(const char *str, va_list args, int fd)
{
	int	count;

	count = 0;
	if (*str == 'c')
		count += ft_putchar_fd(va_arg(args, int), fd);
	else if (*str == 's')
		count += ft_putstr_fd(va_arg(args, char *), fd);
	else if (*str == 'p')
		count += ft_put_ptr_fd(va_arg(args, void *), fd);
	else if (*str == 'd' || *str == 'i')
		count += ft_lnbr_b_fd(va_arg(args, int), BASE_10, fd);
	else if (*str == 'u')
		count += ft_ulnbr_b_fd(va_arg(args, unsigned int), BASE_10, fd);
	else if (*str == 'x')
		count += ft_lnbr_b_fd(va_arg(args, unsigned int), BASE_16, fd);
	else if (*str == 'X')
		count += ft_lnbr_b_fd(va_arg(args, unsigned int), BASE_16U, fd);
	else if (*str == '%')
		count += ft_putchar_fd('%', fd);
	else
		count += ft_putchar_fd(*str, fd);
	return (count);
}

int	ft_dprintf(const int fd, const char *format, ...)
{
	va_list	args;
	int		count;

	if (fd < 0)
		return (-1);
	if (!format || !*format)
		return (0);
	va_start(args, format);
	count = 0;
	while (*format)
	{
		if (*format == '%' && *(format + 1))
			count += ft_output_format(++ format, args, fd);
		else
			count += ft_putchar_fd(*format, fd);
		format++;
	}
	va_end(args);
	return (count);
}

/**
 * if DEBUG is not defined or set to 0, this function does nothing.
 * if DEBUG is set to a positive integer,
 * this function behaves like ft_dprintf;
 */
int	debug_dprintf(const int fd, const char *format, ...)
{
	va_list	args;
	int		count;

	if (fd < 0)
		return (-1);
	if (!format || !*format || !DEBUG)
		return (0);
	va_start(args, format);
	count = 0;
	while (*format)
	{
		if (*format == '%' && *(format + 1))
			count += ft_output_format(++ format, args, fd);
		else
			count += ft_putchar_fd(*format, fd);
		format++;
	}
	va_end(args);
	return (count);
}

/**
 * Takes a variable number of string arguments and prints them to the given fd
 * Don't forget to put a NULL at the end of the parameters,
 * It is a Stops sign if the NULL argument is encountered
 * Joins all strings together before printing
 * Caller must free the allocated memory if needed
 */
void	ft_nstr_dprint(const int fd, ...)
{
	va_list	args;
	char	*pstr;
	char	*tstr;
	char	*str;

	str = ft_strdup("");
	va_start(args, fd);
	pstr = va_arg(args, char *);
	while (pstr != NULL)
	{
		tstr = str;
		str = ft_strjoin(str, pstr);
		free(tstr);
		if (!str)
		{
			va_end(args);
			return ;
		}
		pstr = va_arg(args, char *);
	}
	va_end(args);
	write(fd, str, ft_strlen(str));
	free(str);
}
