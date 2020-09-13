#include "game/player.hpp"

#include <gnid/box.hpp>
#include <gnid/collider.hpp>
#include <gnid/camera.hpp>
#include <gnid/lightnode.hpp>
#include <gnid/event.hpp>
#include <gnid/collisionevent.hpp>
#include <gnid/capsule.hpp>
#include <gnid/observer.hpp>
#include <gnid/collision.hpp>

using namespace std;
using namespace gnid;
using namespace tmat;

/* Create the player as a rigid body with mass of 1.0. */
Player::Player()
    : Rigidbody::Rigidbody(1.0f)
{
}

bool Player::isOnGround() const
{
    /* If the ground we are standing on exists, we are on the ground. */
    return !ground_.expired();
}

void Player::init()
{
    /* Create the spatial nodes for the player. */
    body = make_shared<SpatialNode>();
    head = make_shared<SpatialNode>();
    /* Create the camera. */
    camera = make_shared<Camera>(
                (float) M_PI / 2.0f,
                800.0f / 600.0f,
                0.1f,
                100.0f);
    head->add(camera);
    body->add(head);
    add(body);
    
    /* Create a capsule collider for the player's body. */
    collider_ = make_shared<Collider>(
            make_shared<Capsule>(
                1.0f,
                Vector3f::zero,
                Vector3f::up * -2.0f));
    add(collider_);

    /* Create an observer for when the player touches something. */
    touchGroundObserver = make_shared<Observer<Collision>>(
            [this](Collision collision)
            {
                /*
                 * This function will be called every frame the player collider
                 * is touching something.
                 */

                /* If we are touching an object facing up, it is the ground. */
                if(collision.overlap().normalized().dot(Vector3f::up)
                        < -0.5f && !collision.colliders()[1]->isTrigger())
                {
                    ground_ = collision.colliders()[1];
                }
            });

    leftGroundObserver = make_shared<Observer<Collision>>(
            [this](Collision collision)
            {
                /* 
                 * This function will be called when the player collider stops
                 * touching something
                 */

                /*
                 * If we stopped touching our "ground" object, we are no longer
                 * on the ground.
                 */
                shared_ptr groundShared = ground_.lock();

                if(collision.colliders()[1] == groundShared)
                {
                    ground_.reset();
                }
            });

    /* Subscribe the observers. */
    collider_->collisionStayed()->subscribe(touchGroundObserver);
    collider_->collisionExited()->subscribe(leftGroundObserver);
}

void Player::update(float dt)
{
    /* Create rotation matrices based on the input. */
    auto rotateX = 
            getRotateMatrix<float>(
                lookY(),
                Vector3f::right);
    auto rotateY =
            getRotateMatrix<float>(
                lookX(),
                Vector3f::up);

    /* Update movement. */
    if(moveX() != 0 || moveZ() != 0 != 0)
    {
        Vector3f dir = body->right() * moveX()
                     + body->forward() * moveZ();
        if(dir.magnitude() > 0.2f)
        {
            dir.normalize();
            dir *= dt * MOVE_SPEED;
            transformWorld(getTranslateMatrix(dir));
        }
    }

    /* Jump if we're on the ground. */
    if(jump_ && isOnGround())
    {
        addImpulse(Vector3f::up * JUMP_FORCE);
    }

    /* Update rotation. */
    head->transformLocal(rotateX);
    body->transformLocal(rotateY);
}

