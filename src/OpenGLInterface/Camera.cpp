#include "OpenGLInterface\Camera.h"

void Camera::ProcessKeyboard(Camera_Movement direction, float deltaTime)
{
    float velocity = MovementSpeed * deltaTime;
    if (direction == Camera_Movement::FORWARD)
        Position += Front * velocity;
    if (direction == Camera_Movement::BACKWARD)
        Position -= Front * velocity;
    if (direction == Camera_Movement::LEFT)
        Position -= Right * velocity;
    if (direction == Camera_Movement::RIGHT)
        Position += Right * velocity;
    if(direction==Camera_Movement::UP)
        Position += WorldUp * velocity;
    if(direction==Camera_Movement::DOWN)
        Position -= WorldUp * velocity;
}

void Camera::ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch)
{
    xoffset *= MouseSensitivity;
    yoffset *= MouseSensitivity;

    Yaw += xoffset;
    Pitch += yoffset;

    if (constrainPitch)
    {
        if (Pitch > 89.0f)
            Pitch = 89.0f;
        if (Pitch < -89.0f)
            Pitch = -89.0f;
    }

    updateCameraVectors();
}

void Camera::ProcessMouseScroll(double yoffset)
{
    Zoom -= (float)yoffset;
    if (Zoom < 1.0f)
        Zoom = 1.0f;
    if (Zoom > 45.0f)
        Zoom = 45.0f;
}

//void Camera::ProcessArcBall(float x, float y)//���Ǻ�˿�� 
//{
//    x /= SCREEN_WIDTH * 0.1f;
//    y /= SCREEN_HEIGHT * 0.1f;
//    float zx = sqrt(1 - x * x);
//    float zy = sqrt(1 - y * y);
//
//    glm::vec3 start{0.f,0.f,1.f};
//    //�ѽǶȷֳ��������������x���y�� Ȼ��ֱ����
//    glm::vec3 endX = glm::vec3{ x,0,zx };
//    glm::vec3 endY = glm::vec3{ 0,y,zy };
//
//    float Xangle = 10.f * acos(glm::dot(start, endX));//��10����Ϊ֮ǰ�ı仯̫С
//    float Yangle = 10.f * acos(glm::dot(start, endY));
//
//    //��Ϊ��˲�������� ���Զ�x��y�����������ж�
//    if (x < 0) {
//        Yaw += Xangle;
//        Position -= sin(glm::radians(Xangle)) *5.f* Right;
//    }
//    else {
//        Yaw -= Xangle;
//        Position += sin(glm::radians(Xangle)) * 5.f * Right;
//    }
//
//    if (y < 0) {
//        Pitch += Yangle;
//        Position -= sin(glm::radians(Yangle)) * 5.f * Up;
//        Position += (1.0f-cos(glm::radians(Yangle))) * 5.f * Front;
//    }
//    else {
//        Pitch -= Yangle;
//        Position += sin(glm::radians(Yangle)) * 5.f * Up;
//        Position += (1.0f - cos(glm::radians(Yangle))) * 5.f * Front;
//    }
//
//    //��������ĳ���
//    updateCameraVectors();
//}

glm::mat4 Camera::GetViewMatrix()
{
    return glm::lookAt(Position, Position + Front, Up);
}

glm::mat4 Camera::GetProjectionMatrix()
{
    return glm::perspective(glm::radians(Zoom), SCREEN_ASPECT_RATIO, 0.1f, 100.0f);
}

glm::mat4 Camera::GetInvViewMatrix()//��͵�� ������ ������дһ�����������
{
    return glm::inverse(GetViewMatrix());
}

glm::mat4 Camera::GetInvProjectionMatrix()
{
    return  glm::inverse(GetProjectionMatrix());
}

// update Front, Right and Up Vectors using the updated Euler angles�����ı��˵�ʵ�֣�
void Camera::updateCameraVectors()
{
    // calculate the new Front vector
    glm::vec3 front;
    front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    front.y = sin(glm::radians(Pitch));
    front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    Front = glm::normalize(front);
    // also re-calculate the Right and Up vector
    Right = glm::normalize(glm::cross(Front, WorldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
    Up = glm::normalize(glm::cross(Right, Front));
}
