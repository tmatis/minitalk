# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: tmatis <tmatis@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2020/10/08 16:38:19 by tmatis            #+#    #+#              #
#    Updated: 2021/05/26 23:58:54 by tmatis           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

################################################################################
#                                     CONFIG                                   #
################################################################################


NAME	= minitalk
CC 		= clang
CFLAGS	= -Wall -Werror -Wextra
LIBFT	= libft/libft.a
AUTHOR	= tmatis
DATE	= 26/05/2021

################################################################################
#                                 PROGRAM'S SRCS                               #
################################################################################

SRCS		=

MAIN_CLIENT		= client/main_client.c
MAIN_SERVER		= server/main_server.c

################################################################################
#                                     TESTS's SRCS                             #
################################################################################

#SRCS_TESTS	= main.c

################################################################################
#                                 Makefile logic                               #
################################################################################


OBJ_MAIN_CLIENT	= $(addprefix objs/, ${MAIN_CLIENT:.c=.o})
OBJ_MAIN_SERVER	= $(addprefix objs/, ${MAIN_SERVER:.c=.o})

OBJS		= $(addprefix objs/, ${SRCS:.c=.o})
#OBJS_TESTS	= $(addprefix objs_tests/, ${SRCS_TESTS:.c=.o})

COM_COLOR   = \033[0;34m
OBJ_COLOR   = \033[0;36m
OK_COLOR    = \033[0;32m
ERROR_COLOR = \033[0;31m
WARN_COLOR  = \033[0;33m
NO_COLOR    = \033[m

OK_STRING    = "[OK]"
ERROR_STRING = "[KO]"
WARN_STRING  = "[WARN]"
COM_STRING   = "Compiling"

define run_and_test
printf "%b" "$(COM_COLOR)$(COM_STRING) $(OBJ_COLOR)$(@F)$(NO_COLOR)\r"; \
$(1) 2> $@.log; \
RESULT=$$?; \
if [ $$RESULT -ne 0 ]; then \
  printf "%-60b%b" "$(COM_COLOR)$(COM_STRING)$(OBJ_COLOR) $@" "$(ERROR_COLOR)$(ERROR_STRING)$(NO_COLOR)\n"   ; \
elif [ -s $@.log ]; then \
  printf "%-60b%b" "$(COM_COLOR)$(COM_STRING)$(OBJ_COLOR) $@" "$(WARN_COLOR)$(WARN_STRING)$(NO_COLOR)\n"   ; \
else  \
  printf "%-60b%b" "$(COM_COLOR)$(COM_STRING)$(OBJ_COLOR) $(@F)" "$(OK_COLOR)$(OK_STRING)$(NO_COLOR)\n"   ; \
fi; \
cat $@.log; \
rm -f $@.log; \
exit $$RESULT
endef

all:	header client server

header:
	@printf "%b" "$(OK_COLOR)"
	@echo "        ___  _____ ___  ___      _        "
	@echo "       /   |/ __  \|  \/  |     | |       "
	@echo "      / /| |\`' / /'| .  . | __ _| | _____ "
	@echo "     / /_| |  / /  | |\/| |/ _\` | |/ / _ \\"
	@echo "     \___  |./ /___| |  | | (_| |   <  __/"
	@echo "         |_/\_____/\_|  |_/\__,_|_|\_\___|"
	@echo
	@printf "%b" "$(OBJ_COLOR)Name:	$(WARN_COLOR)$(NAME)\n"
	@printf "%b" "$(OBJ_COLOR)Author:	$(WARN_COLOR)$(AUTHOR)\n"
	@printf "%b" "$(OBJ_COLOR)Date: 	$(WARN_COLOR)$(DATE)\n\033[m"
	@echo

client: 	$(LIBFT) ${OBJS} ${OBJ_MAIN_CLIENT}
			@$(call run_and_test,$(CC) $(CFLAGS) -o $@ ${OBJS} ${OBJ_MAIN_CLIENT} -L./libft -lft)

server: 	$(LIBFT) ${OBJS} ${OBJ_MAIN_SERVER}
			@$(call run_and_test,$(CC) $(CFLAGS) -o $@ ${OBJS} ${OBJ_MAIN_SERVER} -L./libft -lft)

objs/%.o: 	srcs/%.c
			@mkdir -p $(dir $@)
			@$(call run_and_test,$(CC) $(CFLAGS) -c $< -o $@)

#objs_tests/%.o: tests/%.c
#			@mkdir -p $(dir $@)
#			@$(call run_and_test,$(CC) $(CFLAGS) -c $< -o $@)

$(LIBFT):
			@$(call run_and_test,make -sC ./libft libft.a)

clean:		header
			@rm -rf objs objs_tests tests/logs tests/outs tests/diffs
			@make -sC ./libft clean
			@printf "%-53b%b" "$(COM_COLOR)clean:" "$(OK_COLOR)$(OK_STRING)$(NO_COLOR)\n"

fclean:		header clean
			@rm -rf client server
			@make -sC ./libft fclean
			@printf "%-53b%b" "$(COM_COLOR)fclean:" "$(OK_COLOR)$(OK_STRING)$(NO_COLOR)\n"

re:			fclean all

bonus:		all

.PHONY:		all clean fclean re libft header bonus
