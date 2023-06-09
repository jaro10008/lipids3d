Lipids 3D - README
Author: Jaroslaw Synak


INTRODUCTION
This package constists of two programs:
a) simulator - console application which performs actual computations. It is fully parallelized.
b) visualizer - OpenGL program used to visualised the results generated by the simulator.

Both programs were written (and tested) for Linux 64-bit System and the author gives no warranty that they will work correctly in any other environment.


PREREQUISITES
Both programs have to be compiled in order to use them. Those tools should be installed first:
1. GCC C++ Compiler
2. Make
3. Program specific tools.
a) for simulator:
    -OpenMP support (GCC should include it by default)
    -POV-Ray with includes(recommended)
b) for visualizer:
    -OpenGL support and libraries: GLFW, GLM, GLUT


In Ubuntu, all the tools needed can be installed by running the command:

sudo apt install make g++ libglfw3-dev libglm-dev freeglut3-dev povray povray-includes


BUILDING
All the commands presented in this chapter have to be run in this directory. Before compiling anything always run:
    make clean
This command cleans all the compiled files. It can be used after the compilation and definately should be used just after downloading files from the repository.

To compile both programs, simply run the command:
    make

To compile only one of the two, execute:
1. For simulator:
    make simulator
2. For visualizer:
    make visualizer


USAGE

During every simulation state files are generated, single state file contain all the information about the simulation state and the parameters used. They have ".bin" suffix. State files are imprtant source of data for both programs.

Below the detailed usage of both programs is described.

1. Simulator
./simulator [flag] [args]

There are four flags defined currently:

a) -n -> starting new simulation from a parameter file. An example of a parameter file is included in this package (test_parameters.txt). Command:

./simulator -n [parameter file] [result directory] [number of steps]

Result directory is a directory where all the generated files will be saved to. This directory has to exist when starting the simulation. The last argument is the number of steps to simulate.


b) -r -> resuming the simulation. Command:

./simulator -r [state file] [result directory] [number of steps]

To resume the simulation, the appropriate state file should be given. "Result directory" and "number of steps" are the same as in the "-n" flag description.


c) -g -> generate POV file. This flag takes as the first argument simulation state file and outputs POV file with the name specified as the second argument (it should have ".pov" suffix). Command:

./simulator -g [state file] [POV file]

POV files can be later used to generate images (of the simulation volume) using POVRay.

d) -p -> extract parameters. In this mode the state file given is loaded and using the data, a valid parameter file is generated. The new file contains the parameters used in the simulation.

./simulator -p [state file] [new parameter file]


2. Vizualizer

./vizualizer [state files directory] [updates per second]

Visualizer is a tool used to visualize simulation state in 3D, the position and the direction of the virtual camera can be modified freely. WSAD buttons are used to move through the simulation volume, while arrows and QE buttons are used to rotate.

The first argument is the name of a directory with simulation state files. They are sorted alphabetically and read in this order. This directory can be the result of the "simulator" program as in this case, files are automatically named to be sorted in the correct order (chronologically). Currently displayed simulation state is automatically replaced by the next one after a certain amount of time. This amount of time is defined by the second parameter - number of updates per second. This parameter doesn't have to be an integer. 0 is a special value and means no updates.

Because of the functionality described above, images displayed by this program can be treated as an interactive movie.


3. convert.sh

./convert.sh [state files directory] [PNG result directory]

"convert.sh" is an additional script file included in the package. It requires POV-Ray (with includes) in order to work correctly. It takes state files from the directory given, converts them to PNG images (using POV-Ray) and save them in the directory given as the second argument. The second directory has to exist before an execution of this script.

The resolution of PNG images generated is 1024x1024.


SIMULATION PARAMETERS DESCRIPTION
N 	  	            -> number of agents
logTime   	        -> how often simulation state should be saved, it is saved only for steps being a multiple of this number
r 	 	            -> interaction radius
agentSize 	        -> radius of a single agent
m 	  	            -> mass of one point, total agent mass is 3m
dt 	  	            -> time step length
sizeX, sizeY, sizeZ -> dimensions of the simulation volume
friction  	        -> friction constant
D         	        -> diffusion constant
epsilonHH, rmHH     -> LJ force parameters between two heads
epsilonHT, rmHT     -> LJ force parameters between head and tail
epsilonTT, rmTT     -> LJ force parameters between two tails
toroidal 	        -> T/F, boolean informing if the simulation space should be toroidal or not
