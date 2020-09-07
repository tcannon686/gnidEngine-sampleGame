#include <gnid/gnid.hpp>
#include <gnid/gamebase.hpp>
#include <gnid/matrix/matrix.hpp>
#include <gnid/node.hpp>
#include <gnid/renderernode.hpp>
#include <gnid/scene.hpp>
#include <gnid/phongshader.hpp>
#include <gnid/spatialnode.hpp>
#include <gnid/obj_parser.hpp>
#include <gnid/tmdparser.hpp>
#include <gnid/lightnode.hpp>
#include <gnid/collider.hpp>

#include "game/player.hpp"

#include <memory>
#include <iostream>
#include <fstream>
#include <random>

using namespace tmat;
using namespace gnid;
using namespace std;

class Game : public GameBase {
public:

    Game() : GameBase("Gnid game", 640, 480)
    {
    }
    
    ~Game()
    {
    }

    const shared_ptr<Scene> &currentScene() const override;

    bool loadContent() override;
    bool postLoadContent() override;
    bool init() override;
    void update(float dt) override;

    /*void keyCallback(
            int key,
            int scancode,
            int action,
            int mods) override;

    void charCallback(
            unsigned int character) override; */

    void mouseMoveCallback(
            double x,
            double y) override;

    /*void mouseCallback(
            int button,
            int action,
            int mods) override; */

private:
    shared_ptr<Scene> scene;
    shared_ptr<PhongShader> phongShader;
    shared_ptr<SpatialNode> node1;
    shared_ptr<SpatialNode> node2;
    shared_ptr<Node> obj_node;
    shared_ptr<SpatialNode> trigger;
    std::shared_ptr<gnid::Observer<Collision>> collisionObserver;

    shared_ptr<Player> player;

    double mouseX;
    double mouseY;

    double lastMouseX;
    double lastMouseY;

    bool hasInput;
};

bool Game::init()
{
    phongShader = make_shared<PhongShader>();
    phongShader->init();

    scene = make_shared<Scene>();
    scene->init();

    setCursorEnabled(false);

    return true;
}

bool Game::loadContent()
{
    ifstream s;
    s.open("models/cube.obj");
    ObjParser<istream&> obj(s);

    obj.parse();
    obj_node = obj.buildRendererNode<PhongShader, PhongMaterial>(phongShader);

    return true;
}

bool Game::postLoadContent()
{
    /* First box node. */
    node1 = make_shared<SpatialNode>();

    shared_ptr<Box> box = make_shared<Box>();
    box->add({-1, -1, -1});
    box->add({1, 1, 1});

    node1->add(make_shared<Collider>(box));

    node1->add(obj_node->clone());
    scene->root->add(node1);

    node1->transformLocal(getScaleMatrix(Vector3f { 20, 0.5, 20 }));
    node1->transformLocal(getTranslateMatrix(Vector3f { 0, -3, 0 }));

    node1 = make_shared<SpatialNode>();
    shared_ptr<Collider> colliderNode = make_shared<Collider>(box);
    colliderNode->isTrigger() = true;
    collisionObserver = make_shared<Observer<Collision>>(
                [](Collision collision)
                {
                    cout << "Collision entered!" << endl;
                });
    colliderNode->collisionEntered()->subscribe(collisionObserver);
    node1->add(colliderNode);
    node1->add(obj_node->clone());

    scene->root->add(node1);

    /* Spawn random boxes. */
    default_random_engine gen;
    uniform_real_distribution<float> dist(-10.0f, 10.0f);

    for(int i = 0; i < 10; i ++)
    {

        node1 = make_shared<Rigidbody>();
        node1->add(make_shared<Collider>(box));
        node1->add(obj_node->clone());
        node1->transformWorld(getRotateMatrix(dist(gen), Vector3f::right));
        node1->transformWorld(getRotateMatrix(dist(gen), Vector3f::up));
        node1->transformWorld(getRotateMatrix(dist(gen), Vector3f::forward));
        node1->transformWorld(getTranslateMatrix(
                    Vector3f::right * dist(gen)
                  + Vector3f::up * dist(gen)
                  + Vector3f::forward * dist(gen)));
        scene->root->add(node1);
    }

    auto light = make_shared<LightNode>();
    light->distance() = 1.0f;

    /* Add the player. */
    player = make_shared<Player>();
    player->init();
    player->transformWorld(getTranslateMatrix(-Vector3f::forward * 10));

    scene->root->add(player);

    auto lightNode = make_shared<SpatialNode>();
    lightNode->transformWorld(getTranslateMatrix(Vector3f { -3, 5, -8 }));
    lightNode->add(light);

    scene->root->add(lightNode);

    cout << "player: " << player << endl;
    return true;
}

void Game::update(float dt)
{
    if(hasInput)
    {
        player->lookX() = -dt * (mouseX - lastMouseX) * 0.2;
        player->lookY() = -dt * (mouseY - lastMouseY) * 0.2;
        
        player->moveX() =
            (glfwGetKey(window(), GLFW_KEY_A) == GLFW_PRESS ? -1.0f : 0.0f)
            + (glfwGetKey(window(), GLFW_KEY_D) == GLFW_PRESS ? 1.0f : 0.0f);
        player->moveZ() =
            (glfwGetKey(window(), GLFW_KEY_W) == GLFW_PRESS ? -1.0f : 0.0f)
            + (glfwGetKey(window(), GLFW_KEY_S) == GLFW_PRESS ? 1.0f : 0.0f);
        player->moveY() =
            (glfwGetKey(window(), GLFW_KEY_Q) == GLFW_PRESS ? -1.0f : 0.0f)
            + (glfwGetKey(window(), GLFW_KEY_E) == GLFW_PRESS ? 1.0f : 0.0f);
    }

    lastMouseX = mouseX;
    lastMouseY = mouseY;
}

const shared_ptr<Scene> &Game::currentScene() const
{
    return scene;
}

void Game::mouseMoveCallback(
        double x,
        double y)
{
    mouseX = x;
    mouseY = y;

    if(!hasInput)
    {
        lastMouseX = mouseX;
        lastMouseY = mouseY;
        hasInput = true;
    }
}

int main(int argc, char *argv[])
{
    Game game;
    game.start();
}
