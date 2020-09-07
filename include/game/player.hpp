#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <memory>
#include <gnid/collider.hpp>
#include <gnid/rigidbody.hpp>
#include <gnid/matrix/matrix.hpp>

using namespace gnid;

namespace gnid
{
    class Camera;
    class Event;
}

class Player : public Rigidbody
{
public:
    const float MOVE_SPEED = 4;

    Player();
    float &lookX() { return lookX_; }
    float &lookY() { return lookY_; }

    float &moveX() { return moveX_; }
    float &moveZ() { return moveZ_; }
    float &moveY() { return moveY_; }

    void update(float dt) override;
    void init();

private:
    float lookX_ = 0.0f;
    float lookY_ = 0.0f;
    
    float moveX_ = 0.0f;
    float moveZ_ = 0.0f;
    float moveY_ = 0.0f;

    std::shared_ptr<gnid::SpatialNode> body;
    std::shared_ptr<gnid::SpatialNode> head;
    std::shared_ptr<gnid::Camera> camera;
};

#endif /* ifndef PLAYER_HPP */
