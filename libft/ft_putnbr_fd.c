/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbuscaro <lbuscaro@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 13:45:58 by lbuscaro          #+#    #+#             */
/*   Updated: 2025/04/29 13:46:11 by lbuscaro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_putnbr_fd(int n, int fd)
{
	if (n >= 0)
	{
		if (n >= 10)
		{
			ft_putnbr_fd(n / 10, fd);
		}
		ft_putchar_fd(n % 10 + '0', fd);
	}
	else if (n == INT_MIN)
	{
		ft_putstr_fd("-2147483648", fd);
	}
	else
	{
		ft_putchar_fd('-', fd);
		ft_putnbr_fd(n * -1, fd);
	}
}
