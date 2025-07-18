/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbuscaro <lbuscaro@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 14:49:22 by lbuscaro          #+#    #+#             */
/*   Updated: 2025/05/06 14:49:23 by lbuscaro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H
# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 10000
# endif
# include <unistd.h>
# include <stdlib.h>
# include <stddef.h>

typedef struct s_listg
{
	char			buffer[BUFFER_SIZE];
	int				line_len;
	struct s_listg	*next;	
}	t_listg;

void	*ft_memcpy(void *dest, const void *src, size_t n);
t_listg	*new_list(char *buffer, int r_buffer);
t_listg	*list_last(t_listg *buffer_list);
int		list_add(t_listg **buffer_list, t_listg *new);
void	list_clear(t_listg **buffer_list);
char	*get_next_line(int fd);

#endif
