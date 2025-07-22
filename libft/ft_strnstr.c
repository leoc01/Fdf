/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbuscaro <lbuscaro@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 17:47:39 by lbuscaro          #+#    #+#             */
/*   Updated: 2025/04/18 17:52:29 by lbuscaro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *big, const char *little, size_t len)
{
	size_t	i;
	size_t	j;

	if (little[0] == '\0')
		return ((char *)big);
	i = 0;
	j = 0;
	while (big[i] != '\0' && i < len)
	{
		while (big[i + j] == little[j])
		{
			j++;
			if (i + j > len)
				break ;
			if (little[j] == '\0')
				return (&((char *)big)[i]);
		}
		j = 0;
		i++;
	}
	return (NULL);
}
