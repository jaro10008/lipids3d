#include"controls.h"

#include <cmath>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>

using namespace glm;

extern GLFWwindow* window;

static vec3 direction = vec3(0.0f, 0.0f, -1.0f);
static vec3 playerPos = vec3(0.0f, 0.0f, 0.0f);
static vec3 upVector = vec3(0.0f, 1.0f, 0.0f);
static float speed = 1.2;
static float rotationSpeed = 20.0f;

static double lastTime;

mat4 computeViewMatrixFromInputs(){

	double currentTime = glfwGetTime();
	float deltaTime = float(currentTime - lastTime);
	lastTime = currentTime;

	float rotSpeed = tan(M_PI*rotationSpeed*deltaTime / 180.0f); //rotacje realizujemy w ten sposob, ze do wektora patrzenia dodajemy maly wektor zmiany, tutaj przeliczamy kat obrotu na dlugosc wektora zmiany

	vec3 right = normalize(cross(direction, upVector));  //wyliczamy wektor pokazujacy w prawo, normalizujemy wynik dla pewnosci

	// Do przodu
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
		playerPos += direction * deltaTime * speed; //przesuwamy gracza w kierunku patrzenia o okreslona ilosc jednostek
	}
	// Do tylu
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
		playerPos -= direction * deltaTime * speed;
	}
	// W lewo
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
		playerPos -= right * deltaTime * speed;
	}
	// W prawo
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
		playerPos += right * deltaTime * speed;
	}
	// W gore
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS){
		playerPos += upVector * deltaTime * speed;
	}
	// W dol
	if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS){
		playerPos -= upVector * deltaTime * speed;
	}

	// Rotacja w gore
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS){ //jesli wcisnieto strzalke w gore...
		direction = normalize(direction + upVector*rotSpeed); //wyliczamy nowy kierunek patrzenia (patrzymy minimalnie bardziej w gore)
		upVector = normalize(cross(right, direction)); //wyliczamy, gdzie jest teraz gora
	}
	// Rotacja w dol
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS){
		direction = normalize(direction - upVector*rotSpeed);
		upVector = normalize(cross(right, direction));
	}
	// Rotacja w prawo
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS){
		direction = normalize(direction + right*rotSpeed);
		right = normalize(cross(direction, upVector));
	}
	// Rotacja w lewo
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS){
		direction = normalize(direction - right*rotSpeed);
		right = normalize(cross(direction, upVector));
	}

	// W prawo
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS){
		upVector = normalize(upVector - right*rotSpeed);
		right = normalize(cross(direction, upVector));
	}
	// W lewo
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS){
		upVector = normalize(upVector + right*rotSpeed);
		//right = normalize(cross(direction, upVector)); //nie ma potrzeby aktualizowac tutaj wektora right, gdyz nie bedzie on dluzwej potrzebny
	}


	return lookAt(playerPos, playerPos + direction, upVector);
}
