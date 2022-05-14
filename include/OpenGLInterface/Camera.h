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

enum class Camera_Rolling {
    LEFT,
    RIGHT,
    UP,
    DOWN
};

constexpr float YAW = -90.0f;
constexpr float PITCH = 0.0f;
constexpr float SPEED = 15.0f;
constexpr float SENSITIVITY = 0.1f;
constexpr float NEARP = 0.1f;
constexpr float FARP = 1000.0f;
constexpr float AR = SCREEN_ASPECT_RATIO;
constexpr float FOV = 45.0f;

class Camera
{
public:
    void ProcessKeyboard(Camera_Movement direction, float deltaTime);
    void ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true);
    void ProcessMouseScroll(double yoffset);

public:
    Camera(glm::vec3 position = glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), 
        float yaw = YAW, float pitch = PITCH,float nearp = NEARP,float farp = FARP,float fov = FOV )
        : Front(glm::vec3(0.0f, 0.0f, -1.0f)),
        MovementSpeed(SPEED),
        MouseSensitivity(SENSITIVITY),
        Ar(AR),
        Position(position),
        WorldUp(up),
        Yaw(yaw),
        Pitch(pitch),
        NearPlane(nearp),
        FarPlane(farp),
        Fov(fov)
    {
        Distance = glm::distance(Position, glm::vec3(0, -2, -1));
        updateCameraVectors();
    }

    Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, 
        float yaw, float pitch, float nearp, float farp, float fov)
        : Front(glm::vec3(0.0f, 0.0f, -1.0f)),
        MovementSpeed(SPEED),
        MouseSensitivity(SENSITIVITY),
        Ar(AR),
        Position(glm::vec3(posX, posY, posZ)),
        WorldUp(glm::vec3(upX, upY, upZ)),
        Yaw(yaw),
        Pitch(pitch),
        NearPlane(nearp),
        FarPlane(farp),
        Fov(fov)
    {
        Distance = glm::distance(Position, glm::vec3(0, -2, -1));
        updateCameraVectors();
    }

    glm::mat4 GetViewMatrix();
    glm::mat4 GetProjectionMatrix();
    glm::mat4 GetInvViewMatrix();
    glm::mat4 GetInvProjectionMatrix();

    glm::vec3* GetPositionp() { return &Position; };
    float* GetSpeedp() { return &MovementSpeed; };
    float* GetNearPlanep() { return &NearPlane; };
    float* GetFarPlanep() { return &FarPlane; };

    const glm::vec3 GetPosition() const { return Position; };
    const float GetNearPlane()const { return NearPlane; };
    const float GetFarPlane()const { return FarPlane; };
    const float GetFoV() const { return Fov; };
    const glm::vec3 GetFront() const { return Front; };
    //const glm::vec3 GetFront() const { return Front; };
private:
    void updateCameraVectors();
private:
    // camera Attributes
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;
    float Distance;
    // euler Angles
    float Yaw;
    float Pitch;
    //camera frustum
    float Fov;
    float Ar;
    float NearPlane;
    float FarPlane;
    // camera options
    float MovementSpeed;
    float MouseSensitivity;
};

