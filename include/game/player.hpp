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

/**
 * \brief A simple FPS player node
 */
class Player : public Rigidbody
{
public:
    /**
     * \brief Controls how fast the player moves
     */
    const float MOVE_SPEED = 4;

    /**
     * \brief Controls how much force is applied when the player jumps
     */
    const float JUMP_FORCE = 8.0;

    Player();
    /**
     * \brief Controls the direction the player is looking
     */
    float &lookX() { return lookX_; }
    /**
     * \brief Controls the direction the player is looking
     */
    float &lookY() { return lookY_; }

    /**
     * \brief Controls the player's movement
     */
    float &moveX() { return moveX_; }
    /**
     * \brief Controls the player's movement
     */
    float &moveZ() { return moveZ_; }
    /**
     * \brief Controls the player's movement
     */
    float &moveY() { return moveY_; }

    /**
     * \brief The player jumps if this is true
     */
    bool &jump() { return jump_; }

    /**
     * \brief Returns true if the player is currently on the ground
     */
    bool isOnGround() const;

    void update(float dt) override;
    void init();

private:
    float lookX_ = 0.0f;
    float lookY_ = 0.0f;
    
    float moveX_ = 0.0f;
    float moveZ_ = 0.0f;
    float moveY_ = 0.0f;

    bool jump_ = false;

    std::shared_ptr<gnid::SpatialNode> body;
    std::shared_ptr<gnid::SpatialNode> head;
    std::shared_ptr<gnid::Camera> camera;

    std::shared_ptr<gnid::Collider> collider_;

    /*
     * Observers that will be used to tell when the player touches the ground.
     */
    std::shared_ptr<gnid::Observer<Collision>> touchGroundObserver;
    std::shared_ptr<gnid::Observer<Collision>> leftGroundObserver;

    /* A pointer to the last "ground-like" object the player stepped on. */
    std::weak_ptr<gnid::Collider> ground_;
};

#endif /* ifndef PLAYER_HPP */
