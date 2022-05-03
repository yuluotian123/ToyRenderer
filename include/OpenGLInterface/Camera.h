#pragma once
#include "Base\Common.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

enum class Camera_Movement{
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN
};

constexpr float YAW = -90.0f;
constexpr float PITCH = 0.0f;
constexpr float SPEED = 5.0f;
constexpr float SENSITIVITY = 0.1f;
constexpr float ZOOM = 45.0f;
constexpr float NEARP = 0.1f;
constexpr float FARP = 100.0f;

//定义视锥体（TO DO）
class Camera
{
public:
    void ProcessKeyboard(Camera_Movement direction, float deltaTime);
    void ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true);
    void ProcessMouseScroll(double yoffset);
    //void ProcessArcBall(float xoffset, float yoffset);

public:
    Camera(glm::vec3 position = glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH)
        : Front(glm::vec3(0.0f, 0.0f, -1.0f)),
        MovementSpeed(SPEED),
        MouseSensitivity(SENSITIVITY),
        Zoom(ZOOM),
        Position(position),
        WorldUp(up),
        Yaw(yaw),
        Pitch(pitch) 
    {
        updateCameraVectors();
    }

    Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch)
        : Front(glm::vec3(0.0f, 0.0f, -1.0f)),
        MovementSpeed(SPEED),
        MouseSensitivity(SENSITIVITY),
        Zoom(ZOOM),
        Position(glm::vec3(posX, posY, posZ)),
        WorldUp(glm::vec3(upX, upY, upZ)),
        Yaw(yaw),
        Pitch(pitch) 
    {
        updateCameraVectors();
    }

    glm::mat4 GetViewMatrix();
    glm::mat4 GetProjectionMatrix();

    glm::mat4 GetInvViewMatrix();
    glm::mat4 GetInvProjectionMatrix();

    glm::vec3* GetPositionp() { return &Position; };
    float* getSpeedp() { return &MovementSpeed; };

    const glm::vec3& GetPosition() { return Position; };
private:
    void updateCameraVectors();

private:
    // camera Attributes
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;
    // euler Angles
    float Yaw;
    float Pitch;
    // camera options
    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;
};

