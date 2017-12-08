#ifndef ENEMY_H
#define ENEMY_H
#include <irrlicht.h>
#include <string>
#include <assert.h>
#include <iostream>


using namespace irr;
namespace is = irr::scene;
namespace iv = irr::video;
namespace ic = irr::core;
class Enemy
{
private:
    //Mesh associated to the enemy
    is::IAnimatedMesh * mesh;
    //Noeud qui mermet de manipuler le maillage
    is::IAnimatedMeshSceneNode *node;

public:

    void addEnemyMeshToScene(is::ISceneManager *smgr);
    void setPosition(ic::vector3df vec3);
    /**
     * Function setTexture
     * *******************
     * Set a texture for the node of the enemy
     * require:
     * *******
     * - a path for the texture of type string
     * - the video driver of type driver
     * ensure:
     * ******
     * - texture set to the node
     * */
    void setTexture(io::path path, iv::IVideoDriver *driver);
    Enemy(is::ISceneManager *smgr);
};

#endif // ENEMY_H