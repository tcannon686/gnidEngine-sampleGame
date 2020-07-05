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
    shared_ptr<SpatialNode> node;
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
    cout << "init" << endl;

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
    s.open("models/firstScene.obj");
    ObjParser<istream&> obj(s);

    obj.parse();
    obj_node = obj.buildRendererNode<PhongShader, PhongMaterial>(phongShader);

    return true;
}

bool Game::postLoadContent()
{
    node = make_shared<SpatialNode>();

    node->add(obj_node->clone());
    scene->root->add(node);

    auto light = make_shared<LightNode>();
    light->distance() = 1.0f;
    
    /* Add the player. */
    player = make_shared<Player>();
    player->init();
    Box3f box;
    box.add({ -1, -1, -1 });
    box.add({ 1, 1, 1 });
    player->add<Collider<Box3f>>(box);
    scene->root->add(player);


    scene->root->add(
        make_shared<SpatialNode>()
            ->transform(getTranslateMatrix(Vector3f { -8, 5, -8 }))
            ->add(light));

    return true;
}

void Game::update(float dt)
{
    if(hasInput)
    {
        player->lookX() -= dt * (mouseX - lastMouseX) * 0.2;
        player->lookY() -= dt * (mouseY - lastMouseY) * 0.2;
        
        player->moveX() =
            (glfwGetKey(window(), GLFW_KEY_A) == GLFW_PRESS ? -1.0f : 0.0f)
            + (glfwGetKey(window(), GLFW_KEY_D) == GLFW_PRESS ? 1.0f : 0.0f);
        player->moveZ() =
            (glfwGetKey(window(), GLFW_KEY_W) == GLFW_PRESS ? -1.0f : 0.0f)
            + (glfwGetKey(window(), GLFW_KEY_S) == GLFW_PRESS ? 1.0f : 0.0f);
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
    Gnid::start(game);
}
