CC= gcc
CFLAGS= -Wall -pedantic -std=c99 -O2
LIBS= -lpng -lm
NAME= filetopng

$(NAME): main.c
	$(CC) $(CFLAGS) $(LIBS) -o $(NAME) main.c png_wrapper.c
