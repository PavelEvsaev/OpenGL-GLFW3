#pragma once

#include <vector>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP
};

const GLfloat YAW        =  0.0f;
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
    //jumps test
    glm::vec3 gravity;
    GLfloat upSpeed;
    // Eular Angles
    GLfloat yaw;
    GLfloat pitch;
    // Camera options
    GLfloat movement_speed;
    GLfloat mouse_sensitivity;
    GLfloat zoom;

    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), GLfloat yaw = YAW, GLfloat pitch = PITCH) : 
        front(glm::vec3(0.0f, 0.0f, -1.0f)), 
        movement_speed(SPEED), 
        mouse_sensitivity(SENSITIVTY), 
        zoom(ZOOM),
        gravity(glm::vec3(0.0f, -1.0f, 0.0f)),
        upSpeed(0.f)
    {
        this->position = position;
        this->world_up = up;
        this->yaw = yaw;
        this->pitch = pitch;
        this->update_camera_vectors();
    }

    Camera(GLfloat posX, GLfloat posY, GLfloat posZ, GLfloat upX, GLfloat upY, GLfloat upZ, GLfloat yaw, GLfloat pitch) :
        front(glm::vec3(0.0f, 0.0f, -1.0f)),
        movement_speed(SPEED),
        mouse_sensitivity(SENSITIVTY),
        zoom(ZOOM),
        gravity(glm::vec3(0.0f, -1.0f, 0.0f)),
        upSpeed(0.f)
    {
        this->position = glm::vec3(posX, posY, posZ);
        this->world_up = glm::vec3(upX, upY, upZ);
        this->yaw = yaw;
        this->pitch = pitch;
        this->update_camera_vectors();
    }

    glm::mat4 get_view_matrix();
    void update_position(GLfloat deltaTime);

    void process_keyboard(Camera_Movement direction, GLfloat deltaTime);
    void process_mouse_movement(GLfloat xoffset, GLfloat yoffset, GLboolean constrainpitch = true);
    void process_mouse_scroll(GLfloat yoffset);

private:
    void update_camera_vectors();
};