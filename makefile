SRCDIR = ./src
INCDIR = ./include
OBJDIR = ./obj
CC = g++
CFLAGS = -O3 -Wall -Wno-unused-result -I $(INCDIR) -fopenmp
SIM_CFLAGS = 
VIS_CFLAGS = -lGL -lglfw -lglut -lGLU
RM = rm
RMFLAGS = -rf

SIM_OBJS = $(addprefix $(OBJDIR)/, simulator.o statefile.o volume.o povray.o lipid.o parameters.o utils.o vector3.o)
VIS_OBJS = $(addprefix $(OBJDIR)/, visualizer.o controls.o drawing.o parameters.o statefile.o lipid.o volume.o utils.o vector3.o)

.PHONY: all clean

all: simulator visualizer

simulator:$(SIM_OBJS)
	@$(CC) $(CFLAGS) -o simulator $(SIM_OBJS) $(SIM_CFLAGS)

visualizer:$(VIS_OBJS)
	@$(CC) $(CFLAGS) -o visualizer $(VIS_OBJS) $(VIS_CFLAGS)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p ./obj
	@$(CC) $(CFLAGS) -c $< -o $@



clean:
	@$(RM) $(RMFLAGS) $(OBJDIR) ./simulator ./visualizer

