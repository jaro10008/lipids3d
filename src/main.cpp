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

using namespace std;

static const int version = 1;

static lipid** agents;
static int noOfAgents;
parameters pars;
static lipid** sectors;
int sectorsX;
int sectorsY;
int sectorsZ;

static unsigned int step = 0;
static unsigned int maxSteps = 0;

void updateSectors(lipid*);

void initializeAgents(){
	srand(time(0));
	for(unsigned int i = 0; i < pars.N; ++i){
		lipid* agent = new lipid;
		agent->pos.x = randomDoubleWithout1() * pars.sizeX;
		agent->pos.y = randomDoubleWithout1() * pars.sizeY;
		agent->pos.z = randomDoubleWithout1() * pars.sizeZ;

		agent->v = vector3(0.0, 0.0, 0.0);
		agent->angularV = vector3(0.0,0.0,0.0);

		agent->direction = randomUnitVector();

		agent->pre = NULL;
		agent->next = NULL;
		agent->x = -1;

		updateSectors(agent);

		agents[i] = agent;
	}
	noOfAgents = pars.N;

}

static void printTime(time_t t){
	tm* timeInfo;
	timeInfo = localtime(&t);
	char buffer[128];
	strftime(buffer, 128, "%F %T", timeInfo);
	printf("%s", buffer);
}

int res;

static void init(const char* parFile, bool newSim){
	FILE* file;
	if(newSim){	
		puts("Loading parameters...");
		if(!pars.readParametersFromFile(parFile)){
			fprintf(stderr, "ERROR: Can't read parameters from file: %s\n", parFile);
			exit(1);
		}
	}
	else{
		puts("Loading simulation state...");
		file = fopen(parFile, "rb");
		
		int vers;
		res = fread((void*) &vers, sizeof(vers), 1, file);
		if(vers != version){
			fprintf(stderr, "File version incorrect!\nFile format version: %i\nCurrent version: %i\n", vers, version);
			exit(1);;
		}
		else
			printf("File format version: %i\n", version);
		time_t curr;
		res = fread((void*) &curr, sizeof(curr), 1, file);
		printf("Time stamp: ");
		printTime(curr);
		puts("");

		res = fread((void*) &pars, sizeof(pars), 1, file);
		res = fread((void*) &step, sizeof(step), 1, file);
		printf("Current step: %i\n", step);
		res = fread((void*) &noOfAgents, sizeof(noOfAgents), 1, file);
		printf("Loading %i agents\n", noOfAgents);
	}
	agents = new lipid*[pars.initialTabSize];

	sectorsX = int(pars.sizeX / pars.r) + 1;
	sectorsY = int(pars.sizeY / pars.r) + 1;
	sectorsZ = int(pars.sizeZ / pars.r) + 1;

	pars.sizeX = sectorsX * pars.r;
	pars.sizeY = sectorsY * pars.r;
	pars.sizeZ = sectorsZ * pars.r;

	sectors = new lipid*[sectorsX * sectorsY * sectorsZ];

	for(int i = 0; i < sectorsX * sectorsY * sectorsZ; ++i){
		sectors[i] = NULL;
	}

	if(newSim)
		initializeAgents();
	else{
		for(int i = 0; i < noOfAgents; ++i){
			agents[i] = new lipid;
			lipid* agent = agents[i];
			res = fread((void*) &(agent->pos), sizeof(vector3), 1, file);
			res = fread((void*) &(agent->v), sizeof(vector3), 1, file);
			res = fread((void*) &(agent->direction), sizeof(vector3), 1, file);
			res = fread((void*) &(agent->angularV), sizeof(vector3), 1, file);

			agent->pre = NULL;
			agent->next = NULL;
			agent->x = -1;
			updateSectors(agent);
		}

		puts("Loading finished successfully\nResuming simulation...\n\n");
	}
}

int getSectorIndex(int x, int y, int z){
	if(pars.toroidal){
		if(x < 0)
			x += sectorsX;
		else if(x >= sectorsX)
			x -= sectorsX;
		if(y < 0)
			y += sectorsY;
		else if(y >= sectorsY)
			y -= sectorsY;
		if(z < 0)
			z += sectorsZ;
		else if(z >= sectorsZ)
			z -= sectorsZ;
	}
	else{
		if(x < 0 || x >= sectorsX)
			return -1;
		if(y < 0 || y >= sectorsY)
			return -1;
		if(z < 0 || z >= sectorsZ)
			return -1;
	}

	return z * sectorsY * sectorsX + y * sectorsX + x;

}

void correctCoordinate(long double& x, long double sizeX){
	if(pars.toroidal){
		if(x < 0){
			x = fmod(x, sizeX) + sizeX;
		}
		else if(x >= sizeX)
			x = fmod(x, sizeX);
	}
	else{
		if(x < 0){
			x = fmod(x, 2 * sizeX);
			if(x > -sizeX)
                		x = -x;
            		else
                		x += 2 * sizeX;
		}
		else if(x >= sizeX){
			x = fmod(x, 2 * sizeX);
			if(x >= sizeX)
                		x = 2 * sizeX - x;
		}

	}
}

void correctPosition(long double& x, long double& y, long double& z){
	correctCoordinate(x, pars.sizeX);
	correctCoordinate(y, pars.sizeY);
	correctCoordinate(z, pars.sizeZ);
}

lipid* getFirstInSector(int x, int y, int z){

	int in = getSectorIndex(x, y, z);

	if(in == -1)
		return NULL;
	
	return sectors[in];
}

void setFirstInSector(int x, int y, int z, lipid* ptr){

	int in = getSectorIndex(x, y, z);

	if(in != -1)
		sectors[in] = ptr;;
}



void updateSectors(lipid* l){
	correctPosition(l->pos.x, l->pos.y, l->pos.z);

	if(l->x == l->getX() && l->y == l->getY() && l->z == l->getZ())
		return;
	if(l->pre != NULL)
		l -> pre -> next = l -> next;
	else if(l->x >= 0)
		setFirstInSector(l->x, l->y, l->z, l->next);

	if(l->next != NULL)
		l -> next -> pre = l -> pre;

	l->pre = NULL;
	l->x = l->getX();
	l->y = l->getY();
	l->z = l->getZ();

	lipid* first = getFirstInSector(l->x, l->y, l->z);

	if(first != NULL)
		first -> pre = l;
	l->next = first;
	setFirstInSector(l->x, l->y, l->z, l);
}


static void saveSimDataToFile(const char* fileName){
	FILE* file = fopen(fileName, "wb");

	fwrite((const void*) &version, sizeof(version), 1, file);
	time_t curr = time(NULL);
	fwrite((const void*) &curr, sizeof(curr), 1, file);
	fwrite((const void*) &pars, sizeof(pars), 1, file);
	fwrite((const void*) &step, sizeof(step), 1, file);
	fwrite((const void*) &noOfAgents, sizeof(noOfAgents), 1, file);
	for(int i = 0; i < noOfAgents; ++i){
		lipid* agent = agents[i];
		fwrite((const void*) &(agent->pos), sizeof(vector3), 1, file);
		fwrite((const void*) &(agent->v), sizeof(vector3), 1, file);
		fwrite((const void*) &(agent->direction), sizeof(vector3), 1, file);
		fwrite((const void*) &(agent->angularV), sizeof(vector3), 1, file);
	}
	fclose(file);
}

static void extractParameters(const char* inFile, const char* parFile){
	FILE* file = fopen(inFile, "rb");
		
	int vers;
	res = fread((void*) &vers, sizeof(vers), 1, file);
	if(vers != version){
		fprintf(stderr, "File version incorrect!\nFile format version: %i\nCurrent version: %i\n", vers, version);
		exit(1);;
	}
	else
		printf("File format version: %i\n", version);
	time_t curr;
	res = fread((void*) &curr, sizeof(curr), 1, file);
	printf("Time stamp: ");
	printTime(curr);
	puts("");

	res = fread((void*) &pars, sizeof(pars), 1, file);

	res = fread((void*) &step, sizeof(step), 1, file);
	printf("Current step: %i\n", step);
	res = fread((void*) &noOfAgents, sizeof(noOfAgents), 1, file);
	printf("%i agents detected\n", noOfAgents);
	puts("Generating parameters file");
	pars.saveParameters(parFile);
	fclose(file);	


}

static void generatePovFile(const char* inFile, const char* povFile){
	FILE* file = fopen(inFile, "rb");
	FILE* pfile = fopen(povFile, "w");
		
	int vers;
	res = fread((void*) &vers, sizeof(vers), 1, file);
	if(vers != version){
		fprintf(stderr, "File version incorrect!\nFile format version: %i\nCurrent version: %i\n", vers, version);
		exit(1);;
	}
	else
		printf("File format version: %i\n", version);
	time_t curr;
	res = fread((void*) &curr, sizeof(curr), 1, file);
	printf("Time stamp: ");
	printTime(curr);
	puts("");

	res = fread((void*) &pars, sizeof(pars), 1, file);

	res = fread((void*) &step, sizeof(step), 1, file);
	printf("Current step: %i\n", step);
	res = fread((void*) &noOfAgents, sizeof(noOfAgents), 1, file);
	printf("%i agents detected\n", noOfAgents);

	fprintf(pfile,"#include \"colors.inc\"\n");

	vector3 nose = vector3(0.0,0.0,1.0);
	vector3 camera = vector3(-0.5 * pars.sizeX,-0.5*pars.sizeY, 1.5*pars.sizeZ);
	vector3 lookAt = vector3(pars.sizeX, pars.sizeY, 0.0);
	vector3 lightPos = vector3(-pars.sizeX / 2,pars.sizeY / 2,1.5 * pars.sizeZ);
	long double FoV = 70;

	fprintf(pfile,"camera {\n");
	fprintf(pfile,"sky <%Lf,%Lf,%Lf>\n", nose.x, nose.y, nose.z);
	fprintf(pfile,"location <%Lf,%Lf,%Lf>\n", camera.x, camera.y, camera.z);
	fprintf(pfile,"look_at <%Lf,%Lf,%Lf>\n", lookAt.x, lookAt.y, lookAt.z);
	fprintf(pfile,"angle %Lf\n", FoV);
	fprintf(pfile,"}\n");
	fprintf(pfile,"global_settings { ambient_light Black }\n");
	fprintf(pfile,"light_source { <%Lf,%Lf,%Lf> color 2*White parallel point_at <0.0,0.0,0.0>}\n", lightPos.x, lightPos.y, lightPos.z);
	fprintf(pfile,"background{color Pink}\n");

	fprintf(pfile,"#macro L(tail, head)\n");
	fprintf(pfile,"#local r = %Lf;\n", pars.agentSize);
	fprintf(pfile,"cylinder{ tail,head, 0.2*r pigment{ color Gray}}\n");
	fprintf(pfile,"sphere { tail,0.2*r pigment{color Gray}}\n");
	fprintf(pfile,"sphere {head,0.4*r pigment{color Red}}\n");
	fprintf(pfile,"#end\n\n");

	for(int i = 0; i < noOfAgents; ++i){
		lipid lip;
		lipid* agent = &lip;
		res = fread((void*) &(agent->pos), sizeof(vector3), 1, file);
		res = fread((void*) &(agent->v), sizeof(vector3), 1, file);
		res = fread((void*) &(agent->direction), sizeof(vector3), 1, file);
		res = fread((void*) &(agent->angularV), sizeof(vector3), 1, file);
		long double r = pars.agentSize;
		vector3 head = agent->pos + agent->direction * r;
		vector3 tail = agent->pos - agent->direction * r;
		fprintf(pfile,"L(<%Lf,%Lf,%Lf>,<%Lf,%Lf,%Lf>)", tail.x, tail.y, tail.z, head.x, head.y, head.z);
	}


	fclose(pfile);
	fclose(file);
}

int main(int argc, char** argv){
	if(argc < 4){
		fprintf(stderr, "Too few arguments!\n");
		return 1;
	}
	
	if(argv[1][0]!='-'){
		fprintf(stderr, "Unrecognized flag!\n");
		return 1;
	}
	switch(argv[1][1]){
	case 'n':
		if(argc < 5){
			fprintf(stderr, "Too few arguments!\n");
			return 1;
		}
		init(argv[2], true);
		maxSteps = atoi(argv[4]);
		break;
	case 'r':
		if(argc < 5){
			fprintf(stderr, "Too few arguments!\n");
			return 1;
		}
		init(argv[2], false);
		maxSteps = step + atoi(argv[4]);
		break;
	case 'p':
		extractParameters(argv[2], argv[3]);
		return 0;
	case 'g':
		generatePovFile(argv[2], argv[3]);
		return 0;

	}

	//omp_set_num_threads(8);

	printf("\nNumber of threads: %i\n\n", omp_get_max_threads());

	for(; step < maxSteps; ++step){
		if(step % pars.logTime == 0){
			printf("Computing step %d\n", step);
			char fileName[128];
			sprintf(fileName, "%s/step_%09d.bin", argv[3], step);
			saveSimDataToFile(fileName);
			//printf("%f %f %f\n", (float)agents[0]->pos.x,  (float)agents[0]->pos.y,  (float)agents[0]->pos.z);
		}

		#pragma omp parallel for
		for(int i = 0; i < noOfAgents; ++i){
			srand(omp_get_thread_num() ^ time(0) ^ step ^ i);
			lipid* agent = agents[i];

			int x = agent->getX();
			int y = agent->getY();
			int z = agent->getZ();
		
	
			agent->FH = vector3(0.0, 0.0, 0.0);
			agent->FM = vector3(0.0, 0.0, 0.0);
			agent->FT = vector3(0.0, 0.0, 0.0);
			
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

						if(d.length2() <= pars.r * pars.r){

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
			agents[i]->update();
		}
		for(int i = 0; i < noOfAgents; ++i)
			updateSectors(agents[i]);

	}


	

	return 0;
}
