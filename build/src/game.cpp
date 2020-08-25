#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "gnid.hpp"
#include "gamebase.hpp"
#include "matrix/matrix.hpp"
#include "node.hpp"
#include "renderernode.hpp"
#include "scene.hpp"
#include "phongshader.hpp"
#include "spatialnode.hpp"
#include "obj_parser.hpp"
#include "lightnode.hpp"
#include "collider.hpp"

#include "player.hpp"

#include <memory>
#include <iostream>
#include <fstream>

using namespace gnid;
using namespace std;

class Game : public GameBase {
public:

    Game()
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

    Gnid::setCursorEnabled(false);

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

    /* Second box node. */
    node1 = make_shared<SpatialNode>();

    node1->add(make_shared<Collider>(box));

    node1->add(obj_node->clone());
    node1->transform(getTranslateMatrix(Vector3f::right * 1 + Vector3f::up * 2.1 + Vector3f::forward * 1.25));
    scene->root->add(node1);

    auto light = make_shared<LightNode>();
    light->distance() = 1.0f;

    /* Add the player. */
    player = make_shared<Player>();
    player->init();
    player->transform(getTranslateMatrix(-Vector3f::forward * 10));

    scene->root->add(player);

    auto lightNode = make_shared<SpatialNode>();
    lightNode->transform(getTranslateMatrix(Vector3f { -3, 5, -8 }));
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
            (glfwGetKey(window(), GLFW_KEY_E) == GLFW_PRESS ? -1.0f : 0.0f)
            + (glfwGetKey(window(), GLFW_KEY_Q) == GLFW_PRESS ? 1.0f : 0.0f);
    }

    node1->transform(getRotateMatrix(dt * 0.1f, Vector3f::right));
    node1->transform(getRotateMatrix(dt * 0.0123f, Vector3f::up));
    node1->transform(
            getTranslateMatrix(
                transformDirection(
                node1->getWorldMatrix().inverse(),
                    -Vector3f::up * dt * 0.1f)));

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
    Gnid::start(game);
}
