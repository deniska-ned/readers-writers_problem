OBJECT_FLAGS := -Wall -Werror -Wpedantic -Wvla \
-Wextra -Wcast-align -Wcast-qual -Wconversion -Wenum-compare \
-Wfloat-equal -Wredundant-decls -Wsign-conversion

app: main.o
	$(CC) -o $@ $^ -lpthread


main.o: main.c
	$(CC) $(OBJECT_FLAGS) -o $@ -c $<
