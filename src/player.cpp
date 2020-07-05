#include "camera.hpp"
#include "lightnode.hpp"
#include "player.hpp"

using namespace std;
using namespace gnid;
using namespace tmat;

Player::Player()
    : position(Vector3f::up)
{
}

void Player::init()
{
    camera = make_shared<Camera>(
                (float) M_PI / 3.0f,
                640.0f / 480.0f,
                0.1f,
                100.0f);
    add(camera);

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
    if(moveX() != 0 || moveZ() != 0)
    {
        Vector3f dir = Vector3f::right * moveX() + Vector3f::forward * moveZ();
        if(dir.magnitude() > .2)
        {
            dir.normalize();
            dir *= dt * MOVE_SPEED;
            position += (rotateY * dir.homo()).cut();
        }
    }

    /* Update position. */
    setLocalMatrix(
            getTranslateMatrix<float>(position)
            * rotateY
            * rotateX
            );
}

