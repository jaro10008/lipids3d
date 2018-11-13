#include<ctime>
#include<cstdio>

#include "parameters.h"
#include "volume.h"

static const unsigned int version = 3;

static void printTime(time_t t){
	tm* timeInfo;
	timeInfo = localtime(&t);
	char buffer[128];
	strftime(buffer, 128, "%F %T", timeInfo);
	printf("%s", buffer);
}

bool loadSimDataFromFile(const char* fileName){
    FILE* file = fopen(fileName, "rb");
		puts("Loading simulation state...");
		
		int vers;
		(void)fread((void*) &vers, sizeof(vers), 1, file);
		if(vers != version){
			fprintf(stderr, "File version incorrect!\nFile format version: %i\nCurrent version: %i\n", vers, version);
			return false;
		}
		printf("File format version: %i\n", version);

		time_t curr;
		fread((void*) &curr, sizeof(curr), 1, file);
		printf("Time stamp: ");
		printTime(curr);
		puts("");
        
        parameters& pars = getParameters();

		fread((void*) &pars, sizeof(pars), 1, file);

        initializeSimulation(false);

        unsigned int steps;
		fread((void*) &steps, sizeof(steps), 1, file);
        setStepCount(steps);

		printf("Current step: %u\n", steps);

        int noOfAgents = pars.N;
		fread((void*) &noOfAgents, sizeof(noOfAgents), 1, file);
		printf("Loading %i agents\n", noOfAgents);
		for(int i = 0; i < noOfAgents; ++i){
            lipid* agent = &(getAgentAt(i));
			fread((void*) &(agent->pos), sizeof(vector3), 1, file);
			fread((void*) &(agent->v), sizeof(vector3), 1, file);
			fread((void*) &(agent->direction), sizeof(vector3), 1, file);
			fread((void*) &(agent->angularV), sizeof(vector3), 1, file);

			agent->pre = NULL;
			agent->next = NULL;
			agent->x = -1;
			updateSectors(agent);
		}
    puts("Loading finished successfully\n");
    fclose(file);
    return true;
}

void saveSimDataToFile(const char* fileName){
	FILE* file = fopen(fileName, "wb");

	fwrite((const void*) &version, sizeof(version), 1, file);
	time_t curr = time(NULL);
	fwrite((const void*) &curr, sizeof(curr), 1, file);

    parameters& pars = getParameters();
	fwrite((const void*) &pars, sizeof(pars), 1, file);

    int step = getStepCount();
	fwrite((const void*) &step, sizeof(step), 1, file);
    int noOfAgents = pars.N;
	fwrite((const void*) &noOfAgents, sizeof(noOfAgents), 1, file);

	for(int i = 0; i < noOfAgents; ++i){
		fwrite((const void*) &(getAgentAt(i).pos), sizeof(vector3), 1, file);
		fwrite((const void*) &(getAgentAt(i).v), sizeof(vector3), 1, file);
		fwrite((const void*) &(getAgentAt(i).direction), sizeof(vector3), 1, file);
		fwrite((const void*) &(getAgentAt(i).angularV), sizeof(vector3), 1, file);
	}
	fclose(file);
}

void extractParameters(const char* inFile, const char* parFile){
	FILE* file = fopen(inFile, "rb");
		
	int vers;
	fread((void*) &vers, sizeof(vers), 1, file);
	if(vers != version){
		fprintf(stderr, "File version incorrect!\nFile format version: %i\nCurrent version: %i\n", vers, version);
		exit(1);;
	}
	else
		printf("File format version: %i\n", version);
	time_t curr;
	fread((void*) &curr, sizeof(curr), 1, file);
	printf("Time stamp: ");
	printTime(curr);
	puts("");


    parameters& pars = getParameters();
	fread((void*) &pars, sizeof(pars), 1, file);

    unsigned int step = getStepCount();
	fread((void*) &step, sizeof(step), 1, file);
	printf("Current step: %u\n", step);
    int noOfAgents = pars.N;
	fread((void*) &noOfAgents, sizeof(noOfAgents), 1, file);
	printf("%i agents detected\n", noOfAgents);
	puts("Generating parameters file");
	pars.saveParameters(parFile);
	fclose(file);	


}

bool readRawInfo(const char* inFile, parameters* pars, lipid** tab){
    FILE* file = fopen(inFile, "rb");
		puts("Loading simulation state...");
		
        if(!file)
            return false;

		int vers;
		(void)fread((void*) &vers, sizeof(vers), 1, file);
		if(vers != version){
			fprintf(stderr, "File version incorrect!\nFile format version: %i\nCurrent version: %i\n", vers, version);
			return false;
		}
		printf("File format version: %i\n", version);

		time_t curr;
		fread((void*) &curr, sizeof(curr), 1, file);
		printf("Time stamp: ");
		printTime(curr);
		puts("");

		fread((void*) pars, sizeof(*pars), 1, file);

        unsigned int steps;
		fread((void*) &steps, sizeof(steps), 1, file);

		printf("Current step: %u\n", steps);

        unsigned int noOfAgents = pars->N;
		fread((void*) &noOfAgents, sizeof(noOfAgents), 1, file);
		printf("Loading %i agents\n", noOfAgents);

        if(!(*tab)){
            *tab = new lipid[noOfAgents];
        }
        lipid* tab2 = *tab;

		for(unsigned int i = 0; i < noOfAgents; ++i){
			fread((void*) &(tab2[i].pos), sizeof(vector3), 1, file);
			fread((void*) &(tab2[i].v), sizeof(vector3), 1, file);
			fread((void*) &(tab2[i].direction), sizeof(vector3), 1, file);
			fread((void*) &(tab2[i].angularV), sizeof(vector3), 1, file);
		}
    puts("Loading finished successfully\n");
    fclose(file);
    return true;
}
