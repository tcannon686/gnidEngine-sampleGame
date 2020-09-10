#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <memory>
#include <gnid/collider.hpp>
#include <gnid/rigidbody.hpp>
#include <gnid/matrix/matrix.hpp>
#include <gnid/observer.hpp>
#include <gnid/collision.hpp>

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

    bool &jump() { return jump_; }

    bool isOnGround() const;

    void update(float dt) override;
    void init();

private:
    float lookX_ = 0.0f;
    float lookY_ = 0.0f;
    
    float moveX_ = 0.0f;
    float moveZ_ = 0.0f;
    float moveY_ = 0.0f;

    const float jumpForce = 4.0;

    bool jump_ = false;

    std::shared_ptr<gnid::SpatialNode> body;
    std::shared_ptr<gnid::SpatialNode> head;
    std::shared_ptr<gnid::Camera> camera;

    std::shared_ptr<gnid::Collider> collider_;

    std::shared_ptr<gnid::Observer<Collision>> touchGroundObserver;
    std::shared_ptr<gnid::Observer<Collision>> leftGroundObserver;

    std::weak_ptr<gnid::Collider> ground_;
};

#endif /* ifndef PLAYER_HPP */
