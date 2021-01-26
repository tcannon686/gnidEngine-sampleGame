#include "game/game.hpp"

#include <gnid/gnid.hpp>
#include <gnid/matrix/matrix.hpp>
#include <gnid/node.hpp>
#include <gnid/renderernode.hpp>
#include <gnid/scene.hpp>
#include <gnid/phongshader.hpp>
#include <gnid/spatialnode.hpp>
#include <gnid/tmdparser.hpp>
#include <gnid/directionallight.hpp>
#include <gnid/collider.hpp>

#include "game/player.hpp"

#include <memory>
#include <iostream>
#include <fstream>
#include <random>

using namespace tmat;
using namespace gnid;
using namespace std;

Game::Game()
    : GameBase("Sample Game", 800, 600)
{
}

std::shared_ptr<Node> Game::loadTmdModel(std::string path) const
{
    ifstream s(path);

    /* Load a TMD model from the ifstream. */
    TmdParser<istream&> tmd(s);
    tmd.parse();
    assert(phongShader);
    return tmd.buildRendererNode<PhongShader, PhongMaterial>(
            materialMappings);
}

bool Game::init()
{
    /* Create a phong shader.  */
    phongShader = make_shared<PhongShader>();
    phongShader->init();

    /* Create the scene. */
    scene = make_shared<Scene>();
    scene->init();

    /* Hide the cursor. */
    setCursorEnabled(false);

    return true;
}

bool Game::loadContent()
{
    /* Specify the names of the materials in the models being imported. */
    materialMappings = {
        {
            "Material",
            make_shared<PhongMaterial>(
                    phongShader,
                    Vector3f { 1, 1, 1 },
                    Vector3f { 0.1, 0.1, 0.1 },
                    50.0f)
        }
    };

    /* Load the cube model. */
    cubeNode = loadTmdModel("models/Cube.tmd");

    return true;
}

bool Game::postLoadContent()
{
    /* Create a unit box. */
    auto box = make_shared<Box>();
    box->add({-1, -1, -1});
    box->add({1, 1, 1});

    /* Create the floor node. */
    const auto floorNode = make_shared<SpatialNode>();

    /* Add a collider to the floor node. */
    floorNode->add(make_shared<Collider>(box));
    
    /*
     * Add the cube mesh to the floor node. We clone it because otherwise the
     * node would be removed from its previous parent.
     */
    floorNode->add(cubeNode->clone());

    /* Add the floor node to the scene. */
    scene->root->add(floorNode);

    /* Set its scale and place it 10 meters below us. */
    floorNode->transformLocal(getScaleMatrix(Vector3f { 200, 0.5, 200 }));
    floorNode->transformLocal(getTranslateMatrix(Vector3f { 0, -10, 0 }));

    /* Spawn random boxes. */
    default_random_engine gen;
    uniform_real_distribution<float> dist(-50.0f, 50.0f);
    uniform_real_distribution<float> distY(-10.0f, 50.0f);

    for(int i = 0; i < 1000; i ++)
    {
        /* Create the box node. */
        const auto node = make_shared<SpatialNode>();
        /* Add a collider to it. */
        node->add(make_shared<Collider>(box));
        /* Add the mesh. */
        node->add(cubeNode->clone());
        /* Rotate it a random amount around the Y axis. */
        node->transformWorld(getRotateMatrix(dist(gen), Vector3f::up));
        /* Translate it to a random position. */
        node->transformWorld(getTranslateMatrix(
                    Vector3f::right * dist(gen)
                  + Vector3f::up * distY(gen)
                  + Vector3f::forward * dist(gen)));
        /* Add it to the scene. */
        scene->root->add(node);
    }

    /* Add a light to the scene. */
    /* Create the light and set its properties. */
    const auto lightNode = make_shared<DirectionalLight>();
    lightNode->direction() = Vector3f {1.0f, 0.75f, 0.5f};

    /* Create a spatial node to hold the light. */
    const auto lightNodeSpatial = make_shared<SpatialNode>();
    /* Move it to the specified position. */
    lightNodeSpatial->transformWorld(
            getTranslateMatrix(Vector3f { -3, 5, -8 }));
    /* Attach the light to it. */
    lightNodeSpatial->add(lightNode);

    /* Add the light to the scene. */
    scene->root->add(lightNodeSpatial);

    /* Add the player. */
    player = make_shared<Player>();
    /* Initialize its connections. */
    player->init();
    /* Add it to the scene. */
    scene->root->add(player);

    return true;
}

void Game::update(float dt)
{
    if(hasInput)
    {
        player->lookX() = -dt * (mouseX - lastMouseX) * 0.2;
        player->lookY() = -dt * (mouseY - lastMouseY) * 0.2;
    }

    /* Move the player depending on the key inputs. */
    player->moveX() =
        (glfwGetKey(window(), GLFW_KEY_A) == GLFW_PRESS ? -1.0f : 0.0f)
        + (glfwGetKey(window(), GLFW_KEY_D) == GLFW_PRESS ? 1.0f : 0.0f);
    player->moveZ() =
        (glfwGetKey(window(), GLFW_KEY_W) == GLFW_PRESS ? -1.0f : 0.0f)
        + (glfwGetKey(window(), GLFW_KEY_S) == GLFW_PRESS ? 1.0f : 0.0f);
    player->moveY() =
        (glfwGetKey(window(), GLFW_KEY_Q) == GLFW_PRESS ? -1.0f : 0.0f)
        + (glfwGetKey(window(), GLFW_KEY_E) == GLFW_PRESS ? 1.0f : 0.0f);

    player->jump() = glfwGetKey(window(), GLFW_KEY_SPACE) == GLFW_PRESS;

    lastMouseX = mouseX;
    lastMouseY = mouseY;
}

const shared_ptr<Scene> &Game::currentScene() const
{
    /* Let the game know which scene to render/update. */
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

/* The entry point to the application. */
int main(int argc, char *argv[])
{
    Game game;
    game.start();
}

