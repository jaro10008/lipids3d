#include<cstdio>
#include<cstdlib>
#include<ctime>
#include<omp.h>
#include<cmath>
#include<string>
#include"vector3.h"
#include"parameters.h"
#include"lipid.h"
#include"utils.h"
#include"povray.h"
#include"statefile.h"
#include"volume.h"

using namespace std;

static unsigned int maxSteps = 0;

int main(int argc, char** argv){
    srand(time(0));


	if(argc < 4){
		fprintf(stderr, "Too few arguments!\n");
		return 1;
	}
	
	if(argv[1][0]!='-' || !argv[1][1] || argv[1][2]){
		fprintf(stderr, "Unrecognized flag!\n");
		return 1;
	}
	switch(argv[1][1]){
	case 'n':
		if(argc < 5){
			fprintf(stderr, "Too few arguments!\n");
			return 1;
		}
        if(!getParameters().readParametersFromFile(argv[2])){
            fprintf(stderr, "Can't read the parameter file: %s\n", argv[2]);
            return 1;
        }
		initializeSimulation();
		maxSteps = atoi(argv[4]);
		break;
	case 'r':
		if(argc < 5){
			fprintf(stderr, "Too few arguments!\n");
			return 1;
		}
        loadSimDataFromFile(argv[2]);
		maxSteps = getStepCount() + atoi(argv[4]);
        puts("Resuming simulation...\n\n");
		break;
	case 'p':
		extractParameters(argv[2], argv[3]);
		return 0;
	case 'g':
        loadSimDataFromFile(argv[2]);
		generatePovFile(argv[3]);
		return 0;

	}

    parameters& pars = getParameters();
    int noOfAgents = pars.N;


	//omp_set_num_threads(1);

	printf("\nNumber of threads: %i\n\n", omp_get_max_threads());

    
    char fileName[128];
	sprintf(fileName, "%s/step_%09d.bin", argv[3], getStepCount());
	saveSimDataToFile(fileName);
    incrementStepCount();

	for(; getStepCount() <= maxSteps; incrementStepCount()){
		if(getStepCount() % pars.logTime == 0){
			printf("Computing step %d\n", getStepCount());
		}

		#pragma omp parallel for
		for(int i = 0; i < noOfAgents; ++i){
			srand(omp_get_thread_num() ^ time(0) ^ getStepCount() ^ i);
			lipid* agent = &(getAgentAt(i));

			int x = agent->getX();
			int y = agent->getY();
			int z = agent->getZ();
	
			agent->F[0] = vector3(0.0, 0.0, 0.0);
			agent->F[1] = vector3(0.0, 0.0, 0.0);
			agent->F[2] = vector3(0.0, 0.0, 0.0);
			
			for(int j = -1; j < 2; ++j)
			for(int k = -1; k < 2; ++k)
			for(int l = -1; l < 2; ++l){
				lipid* ptr = getFirstInSector(x + j, y + k, z + l);
				
				while(ptr != NULL){
					if(ptr != agent){
						vector3 d = agent->pos - ptr->pos;
						if(pars.toroidal){
							d = distanceInToroidalSpace(d, vector3(pars.sizeX, pars.sizeY, pars.sizeZ));
						}

                        long double width = pars.r + pars.agentSize;

						if(d.length2() <= width * width){

							calculateForces(agent, ptr);
						}
					}
					ptr = ptr-> next;
				}

			}
			
			friction(agent);
			diffusion(agent);

		}
		#pragma omp parallel for
		for(int i = 0; i < noOfAgents; ++i){
			getAgentAt(i).update();
		}
		for(int i = 0; i < noOfAgents; ++i)
			updateSectors(&(getAgentAt(i)));

        if(getStepCount() % pars.logTime == 0){
			sprintf(fileName, "%s/step_%09d.bin", argv[3], getStepCount());
			saveSimDataToFile(fileName);
        }

	}


	

	return 0;
}
