#pragma once

#include <vector>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

const GLfloat YAW        = -90.0f;
const GLfloat PITCH      =  0.0f;
const GLfloat SPEED      =  3.0f;
const GLfloat SENSITIVTY =  0.25f;
const GLfloat ZOOM       =  45.0f;

class Camera
{
public:
    // Camera Attributes
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 world_up;
    // Eular Angles
    GLfloat yaw;
    GLfloat pitch;
    // Camera options
    GLfloat movement_speed;
    GLfloat mouse_sensitivity;
    GLfloat zoom;

    // Constructor with vectors
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), GLfloat yaw = YAW, GLfloat pitch = PITCH) : 
        front(glm::vec3(0.0f, 0.0f, -1.0f)), 
        movement_speed(SPEED), 
        mouse_sensitivity(SENSITIVTY), 
        zoom(ZOOM)
    {
        this->position = position;
        this->world_up = up;
        this->yaw = yaw;
        this->pitch = pitch;
        this->update_camera_vectors();
    }

    // Constructor with scalar values
    Camera(GLfloat posX, GLfloat posY, GLfloat posZ, GLfloat upX, GLfloat upY, GLfloat upZ, GLfloat yaw, GLfloat pitch) :
        front(glm::vec3(0.0f, 0.0f, -1.0f)),
        movement_speed(SPEED),
        mouse_sensitivity(SENSITIVTY),
        zoom(ZOOM)
    {
        this->position = glm::vec3(posX, posY, posZ);
        this->world_up = glm::vec3(upX, upY, upZ);
        this->yaw = yaw;
        this->pitch = pitch;
        this->update_camera_vectors();
    }

    // Returns the view matrix calculated using Eular Angles and the LookAt Matrix
    glm::mat4 get_view_matrix();
    // Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
    void process_keyboard(Camera_Movement direction, GLfloat deltaTime);
    // Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
    void process_mouse_movement(GLfloat xoffset, GLfloat yoffset, GLboolean constrainpitch = true);
    // Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
    void process_mouse_scroll(GLfloat yoffset);

private:
    // Calculates the front vector from the Camera's (updated) Eular Angles
    void update_camera_vectors();
};