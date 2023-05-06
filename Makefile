CC= gcc
CFLAGS= -Wall -pedantic -std=c99 -O2
LIBS= -lm
NAME= filetopng

$(NAME): main.c
	$(CC) -o $(NAME) main.c png_wrapper.c $(LIBS) $(CFLAGS)
