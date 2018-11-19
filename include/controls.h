#ifndef CONTROLS_H
#define CONTROLS_H

#include <glm/gtc/matrix_transform.hpp>

/**
 * Returns view matrix according to the current camera position and orientation.
 * Pressed keys are taken into consideration.
 * @return View matrix.
 */
glm::mat4 computeViewMatrixFromInputs();

#endif //CONTROLS_H