#include<cstdio>
#include<ctime>

#include "parameters.h"
#include "volume.h"

void generatePovFile(const char* povFile) {
    FILE* pfile = fopen(povFile, "w");

    parameters& pars = getParameters();

    vector3 nose = vector3(0.0, 0.0, 1.0);
    vector3 camera = vector3(-0.5 * pars.sizeX, -0.5 * pars.sizeY, 1.5 * pars.sizeZ);
    vector3 lookAt = vector3(pars.sizeX, pars.sizeY, 0.0);
    vector3 lightPos = vector3(-pars.sizeX / 2, pars.sizeY / 2, 1.5 * pars.sizeZ);
    long double FoV = 70;

    fprintf(pfile, "#include \"colors.inc\"\n");
    fprintf(pfile, "camera {\n");
    fprintf(pfile, "sky <%Lf,%Lf,%Lf>\n", nose.x, nose.y, nose.z);
    fprintf(pfile, "location <%Lf,%Lf,%Lf>\n", camera.x, camera.y, camera.z);
    fprintf(pfile, "look_at <%Lf,%Lf,%Lf>\n", lookAt.x, lookAt.y, lookAt.z);
    fprintf(pfile, "angle %Lf\n", FoV);
    fprintf(pfile, "}\n");
    fprintf(pfile, "global_settings { ambient_light Black }\n");
    fprintf(pfile, "light_source { <%Lf,%Lf,%Lf> color 2*White parallel point_at <0.0,0.0,0.0>}\n", lightPos.x, lightPos.y, lightPos.z);
    fprintf(pfile, "background{color Black}\n");

    fprintf(pfile, "#macro L(tail, head)\n");
    fprintf(pfile, "#local r = %Lf;\n", pars.agentSize);
    fprintf(pfile, "cylinder{ tail,head, 0.2*r pigment{ color Gray}}\n");
    fprintf(pfile, "sphere { tail,0.2*r pigment{color Gray}}\n");
    fprintf(pfile, "sphere {head,0.4*r pigment{color Red}}\n");
    fprintf(pfile, "#end\n\n");

    long double r = pars.agentSize;

    for (unsigned int i = 0; i < pars.N; ++i) {
        lipid* agent = &(getAgentAt(i));

        vector3 head = agent->pos + agent->direction * r;
        vector3 tail = agent->pos - agent->direction * r;
        fprintf(pfile, "L(<%Lf,%Lf,%Lf>,<%Lf,%Lf,%Lf>)", tail.x, tail.y, tail.z, head.x, head.y, head.z);
    }

    fclose(pfile);
}
