#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

namespace qts {

struct camera {
    glm::vec3 eye;

    glm::vec3 up;
    glm::vec3 forward;

    glm::vec3 translational_velocity;
    glm::vec3 angular_velocity;

    glm::quat rotation;

    camera()
        : camera(glm::vec3(0.0f), glm::quat(1.0f, glm::vec3(0.0f)), glm::vec3(0.0f, 1.0f, 0.0f))
    {}

    camera(const glm::vec3& translation, const glm::quat& rotation, const glm::vec3& up)
        : up(up)
        , eye(translation)
        , rotation(rotation)
    	, forward(glm::vec3(0.0f, 0.0f, -1.0f))
    {}
};

}

#endif // CAMERA_HPP
