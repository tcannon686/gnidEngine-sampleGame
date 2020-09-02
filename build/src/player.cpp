#include "player.hpp"
#include <gnid/box.hpp>
#include <gnid/collider.hpp>
#include <gnid/camera.hpp>
#include <gnid/lightnode.hpp>
#include <gnid/event.hpp>
#include <gnid/collisionevent.hpp>

using namespace std;
using namespace gnid;
using namespace tmat;

Player::Player()
{
}

void Player::init()
{
    body = make_shared<SpatialNode>();
    head = make_shared<SpatialNode>();
    camera = make_shared<Camera>(
                (float) M_PI / 3.0f,
                640.0f / 480.0f,
                0.1f,
                100.0f);
    head->add(camera);
    body->add(head);
    add(body);
    
    shared_ptr<Box> box = make_shared<Box>();
    box->add({-1, -1, -1});
    box->add({1, 1, 1});

    shared_ptr<Collider> collider = make_shared<Collider>(box);
    add(collider);
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
    if(moveX() != 0 || moveZ() != 0 || moveY() != 0)
    {
        Vector3f dir = body->right() * moveX()
                     + body->forward() * moveZ()
                     + body->up() * moveY();
        if(dir.magnitude() > .2)
        {
            dir.normalize();
            dir *= dt * MOVE_SPEED;
            transformWorld(getTranslateMatrix(dir));
        }
    }

    /* Update position. */
    head->transformLocal(rotateX);
    body->transformLocal(rotateY);
}

