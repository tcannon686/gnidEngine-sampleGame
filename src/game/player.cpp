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

Player::Player()
    : Rigidbody::Rigidbody(1.0f)
{
}

bool Player::isOnGround() const
{
    return !ground_.expired();
}

void Player::init()
{
    body = make_shared<SpatialNode>();
    head = make_shared<SpatialNode>();
    camera = make_shared<Camera>(
                (float) M_PI / 2.0f,
                640.0f / 480.0f,
                0.1f,
                100.0f);
    head->add(camera);
    body->add(head);
    add(body);
    
    shared_ptr<Box> box = make_shared<Box>();
    box->add({-1, -1, -1});
    box->add({1, 1, 1});

    collider_ = make_shared<Collider>(make_shared<Capsule>(
                1.0f,
                Vector3f::zero,
                Vector3f::up * -2.0f));
    add(collider_);

    touchGroundObserver = make_shared<Observer<Collision>>(
            [this](Collision collision)
            {
                std::cout << collision.overlap().normalized().dot(Vector3f::up) << std::endl;
                if(collision.colliders()[0] == collider_)
                {
                    if(collision.overlap().normalized().dot(Vector3f::up)
                            > 0.25f && !collision.colliders()[1]->isTrigger())
                    {
                        ground_ = collision.colliders()[1];
                    }
                }
                else
                {
                    if(collision.overlap().normalized().dot(Vector3f::up)
                            < -0.25f && !collision.colliders()[0]->isTrigger())
                    {
                        ground_ = collision.colliders()[0];
                    }
                }
            });

    leftGroundObserver = make_shared<Observer<Collision>>(
            [this](Collision collision)
            {
                auto groundShared = ground_.lock();
                if(groundShared
                        && (collision.colliders()[0] == groundShared
                        || collision.colliders()[1] == groundShared))
                {
                    ground_.reset();
                }
            });

    collider_->collisionEntered()->subscribe(touchGroundObserver);
    collider_->collisionExited()->subscribe(leftGroundObserver);
}

void Player::update(float dt)
{
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

    if(jump_ && isOnGround())
    {
        cout << "jumped!" << endl;
        addImpulse(Vector3f::up * jumpForce);
    }

    /* Update position. */
    head->transformLocal(rotateX);
    body->transformLocal(rotateY);
}

