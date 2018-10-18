CC = g++
CFLAGS = -O3 -fopenmp -Wall
TARGET = simulator
RM = rm
RMFLAGS = -rf
SRCDIR = ./src
SRCS = $(wildcard $(SRCDIR)/*.cpp)
OBJDIR = ./obj

OBJS = $(SRCS:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)

.PHONY: all clean

all: $(TARGET)

$(TARGET):$(OBJS)
	@$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p ./obj
	@$(CC) $(CFLAGS) -c $< -o $@



clean:
	@$(RM) $(RMFLAGS) $(OBJDIR) $(TARGET)

