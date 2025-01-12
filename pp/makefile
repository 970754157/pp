# 定义编译器和编译选项
CC = gcc
CFLAGS = -Wall -Wextra -g
LDFLAGS =

# 定义目标文件和源文件
TARGET = film_tool
SRCS = main.c FilmMaster2000.c
OBJS = $(SRCS:.c=.o)
HEADER = FilmMaster2000.h

# 默认目标
all: $(TARGET)

# 链接目标文件
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LDFLAGS)

# 编译源文件
%.o: %.c $(HEADER)
	$(CC) $(CFLAGS) -c $< -o $@

# 清理生成的文件
clean:
	rm -f $(OBJS) $(TARGET)

# 运行测试（可选）
test: $(TARGET)
	./$(TARGET) info example.film
	./$(TARGET) reverse example.film reversed.film
	./$(TARGET) swap example.film swapped.film 0 2
	./$(TARGET) clip example.film clipped.film 1 50 200
	./$(TARGET) scale example.film scaled.film 0 1.5

.PHONY: all clean test
