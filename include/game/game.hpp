#ifndef GAME_HPP
#define GAME_HPP

#include <memory>
#include <map>
#include <gnid/gamebase.hpp>
#include <gnid/observer.hpp>

/* Forward declarations */
namespace gnid
{
    class Scene;
    class Node;
    class SpatialNode;
    class PhongShader;
    class PhongMaterial;
    class Collision;
}

class Player;

/**
 * \brief
 *     The game class, inherits from gnid::GameBase, essentially the entry point
 *     for the game
 */
class Game : public gnid::GameBase
{
public:

    Game();

    bool loadContent() override;
    bool postLoadContent() override;
    bool init() override;
    void update(float dt) override;

    const std::shared_ptr<gnid::Scene> &currentScene() const override;

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

    /**
     * \brief Loads a TMD model from the given path
     */
    std::shared_ptr<gnid::Node> loadTmdModel(std::string path) const;
private:

    std::shared_ptr<gnid::Scene> scene;
    std::shared_ptr<gnid::PhongShader> phongShader;
    std::shared_ptr<gnid::SpatialNode> trigger;
    std::shared_ptr<gnid::Observer<gnid::Collision>> collisionObserver;
    std::map<std::string, std::shared_ptr<gnid::PhongMaterial>>
        materialMappings;

    /* Models */
    std::shared_ptr<gnid::Node> cubeNode;

    std::shared_ptr<Player> player;

    double mouseX;
    double mouseY;

    double lastMouseX;
    double lastMouseY;

    bool hasInput;
};

#endif
