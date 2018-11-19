#include <cmath>
#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <stdio.h>
#include "controls.h"
#include "drawing.h"
#include <GL/glut.h>
#include <GL/glu.h>
#include <dirent.h>
#include <limits>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

/**
 * Prints an error description.
 * @param error Error ID.
 * @param description Error description.
 */
static void error_callback(int error, const char* description) {
    fputs(description, stderr);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}

GLFWwindow* window;

int main(int argc, char** argv) {
    if (argc < 2) {
        fprintf(stderr, "ERROR: File name not given\n");
        return 1;
    }
    if (argc < 3) {
        fprintf(stderr, "ERROR: Undefined number of FPS\n");
        return 1;
    }


    const float fov = 60.0f;
    double fps = atof(argv[2]);
    double tbf; //time between files

    if (fps == 0.0)
        tbf = std::numeric_limits<double>::infinity();
    else
        tbf = 1 / fps;

    DIR *dir;
    if ((dir = opendir(argv[1])) == NULL) {
        fprintf(stderr, "ERROR: Can't open the directory given: %s\n", argv[1]);
        return 1;
    }

    vector<string> sortVector;

    struct dirent *ent;

    while ((ent = readdir(dir)) != NULL) {
        if (ent->d_name[0] == '.')
            continue;

        char buffer[522];
        sprintf(buffer, "%s/%s", argv[1], ent->d_name);
        sortVector.push_back(string(buffer));
    }

    if (sortVector.empty()) {
        fprintf(stderr, "ERROR: No files found\n");
        return 1;
    }

    sort(sortVector.begin(), sortVector.end());



    vector<MoleculeData> frames;

    for (unsigned int i = 0; i < sortVector.size(); ++i) {
        MoleculeData res = readFile(sortVector[i].c_str());
        if (res.tab == NULL) {
            fprintf(stderr, "ERROR: Can't load simulation state: %s\n", sortVector[i].c_str());
            continue;
        }
        frames.push_back(res);
    }

    if (frames.empty()) {
        fprintf(stderr, "ERROR: No proper files found\n");
        return 1;
    }

    glfwSetErrorCallback(error_callback);
    if (!glfwInit())
        exit(EXIT_FAILURE);

    glutInit(&argc, argv);

    window = glfwCreateWindow(640, 480, "Lipids 3D", NULL, NULL);
    if (!window) {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_callback);


    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH);

    GLfloat light_position[] = {1.0, 1.0, 1.0, 0.0};
    float light_ambient[] = {1.0f, 1.0f, 1.0f, 1.0f};
    float light_diffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};

    float mat_ambient[] = {0.0f, 0.0f, 0.0f, 1.0f};
    float mat_diffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};

    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);

    double lastTime = glfwGetTime() - tbf;

    unsigned int ptr = 0;

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        float ratio;
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        ratio = width / (float) height;
        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT);
        glMatrixMode(GL_PROJECTION);
        //glLoadIdentity();
        //glOrtho(-ratio, ratio, -1.f, 1.f, 1.f, -1.f);

        glm::mat4 projection = glm::perspective(float(M_PI) * fov / 180.0f, ratio, 0.1f, 100.0f);

        glLoadMatrixf(&projection[0][0]);

        glm::mat4 viewMatrix = computeViewMatrixFromInputs();

        glMatrixMode(GL_MODELVIEW);
        glLoadMatrixf(&viewMatrix[0][0]);
        glLightfv(GL_LIGHT0, GL_POSITION, light_position);
        glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);

        double currentTime = glfwGetTime();
        double deltaTime = currentTime - lastTime;
        if (deltaTime > tbf || !ptr) {
            if (ptr < frames.size()) {
                setMoleculeData(frames[ptr]);
                ++ptr;
            }
            lastTime = currentTime;
        }

        /*
        glBegin(GL_TRIANGLES);
		
        glColor3f(1.f, 0.f, 0.f);
        glVertex3f(-0.6f, -0.4f, 0.f);
        glColor3f(0.f, 1.f, 0.f);
        glVertex3f(0.6f, -0.4f, 0.f);
        glColor3f(0.f, 0.f, 1.f);
        glVertex3f(0.f, 0.6f, 0.f);
        glEnd();*/
        drawMolecules();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}
