CC= gcc
CFLAGS= -Wall -pedantic -std=c99 -fsanitize=address -g
LIBS= -lpng -lm
NAME= filetopng

$(NAME): main.c
	$(CC) $(CFLAGS) $(LIBS) -o $(NAME) main.c png_wrapper.c
