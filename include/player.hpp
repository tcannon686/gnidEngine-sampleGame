#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <memory>
#include "spatialnode.hpp"
#include "matrix/matrix.hpp"

namespace gnid
{
    class Camera;
}

class Player : public gnid::SpatialNode
{
public:
    const float MOVE_SPEED = 4;

    Player();
    float &lookX() { return lookX_; }
    float &lookY() { return lookY_; }

    float &moveX() { return moveX_; }
    float &moveZ() { return moveZ_; }

    void update(float dt) override;
    void init();
private:
    float lookX_ = 0.0f;
    float lookY_ = 0.0f;
    
    float moveX_ = 0.0f;
    float moveZ_ = 0.0f;

    tmat::Vector3f position;

    std::shared_ptr<gnid::Camera> camera;
};

#endif /* ifndef PLAYER_HPP */
