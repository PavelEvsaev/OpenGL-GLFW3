#include "Camera.h"

// Returns the view matrix calculated using Eular Angles and the LookAt Matrix
glm::mat4 Camera::get_view_matrix()
{
    return glm::lookAt(this->position, this->position + this->front, this->up);
}

// Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
void Camera::process_keyboard(Camera_Movement direction, GLfloat deltaTime)
{
    GLfloat velocity = this->movement_speed * deltaTime;

    if (direction == FORWARD)
        this->position += this->front * velocity;
    if (direction == BACKWARD)
        this->position -= this->front * velocity;
    if (direction == LEFT)
        this->position -= this->right * velocity;
    if (direction == RIGHT)
        this->position += this->right * velocity;

    //jumps test
    if (direction == UP && this->upSpeed == 0.f)
        this->upSpeed = 1.f;
}

void Camera::update_position(GLfloat deltaTime)
{   
    GLfloat velocity = this->movement_speed * deltaTime;

    this->position.y += this->upSpeed * velocity;

    if (this->position.y > 0.f)
    {
        this->upSpeed += this->gravity.y * velocity;
    }
    else 
    {
        this->upSpeed = 0;
    }
}

// Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
void Camera::process_mouse_movement(GLfloat xoffset, GLfloat yoffset, GLboolean constrainpitch)
{
    xoffset *= this->mouse_sensitivity;
    yoffset *= this->mouse_sensitivity;

    this->yaw += xoffset;
    this->pitch += yoffset;

    // Make sure that when pitch is out of bounds, screen doesn't get flipped
    if (constrainpitch)
    {
        if (this->pitch > 89.0f)
            this->pitch = 89.0f;
        if (this->pitch < -89.0f)
            this->pitch = -89.0f;
    }

    // update front, right and up Vectors using the updated Eular angles
    this->update_camera_vectors();
}

void Camera::process_mouse_scroll(GLfloat yoffset)
{
    if (this->zoom >= 1.0f && this->zoom <= 45.0f)
        this->zoom -= yoffset;
    if (this->zoom <= 1.0f)
        this->zoom = 1.0f;
    if (this->zoom >= 45.0f)
        this->zoom = 45.0f;
}

void Camera::update_camera_vectors()
{
    // Calculate the new front vector
    glm::vec3 front;
    front.x = sin(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
    front.y = sin(glm::radians(this->pitch));
    front.z = -cos(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
    this->front = glm::normalize(front);
    // Also re-calculate the right and up vector
    this->right = glm::normalize(glm::cross(this->front, this->world_up));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
    this->up    = glm::normalize(glm::cross(this->right, this->front));
}