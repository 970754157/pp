# 编译器和选项
CC = gcc
AR = ar
CFLAGS = -Wall -Wextra -g
ARFLAGS = rcs

# 文件定义
LIB_NAME = libFilmMaster2000.a
LIB_SRCS = FilmMaster2000.c
LIB_OBJS = $(LIB_SRCS:.c=.o)
MAIN_SRCS = main.c
MAIN_OBJS = $(MAIN_SRCS:.c=.o)
TARGET = film_tool

# 默认目标
all: $(LIB_NAME) $(TARGET)

# 生成静态库
$(LIB_NAME): $(LIB_OBJS)
	$(AR) $(ARFLAGS) $@ $^

# 生成可执行文件
$(TARGET): $(MAIN_OBJS) $(LIB_NAME)
	$(CC) $(CFLAGS) -o $@ $^

# 编译规则
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# 清理生成文件
clean:
	rm -f $(LIB_OBJS) $(MAIN_OBJS) $(LIB_NAME) $(TARGET)

# 测试运行
test: $(TARGET)
	./$(TARGET) info example.film
	./$(TARGET) reverse example.film reversed.film

.PHONY: all clean test

