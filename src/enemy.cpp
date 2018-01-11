#include "enemy.h"
#include <irrlicht.h>

Enemy::Enemy(is::ISceneManager *smgr_param,
             irr::IRandomizer *random_generator_param)
{
    smgr = smgr_param;
    random_generator = random_generator_param;
    mesh = smgr->getMesh("../data/tris.md2");
    waiting_position_center = {0.0f,0.0f,0.0f};
}

void Enemy::addEnemyMeshToScene(){
    node = smgr->addAnimatedMeshSceneNode(mesh);
    node -> setMaterialFlag(irr::video::EMF_LIGHTING,false);
    node -> setMD2Animation(irr::scene::EMAT_STAND);
    is::ITriangleSelector* selector = smgr->createTriangleSelector(node);
    node->setTriangleSelector(selector);
    selector->drop(); // We're done with this selector, so drop it now.
}

void Enemy::setPosition(ic::vector3df vec3){
    node -> setPosition(vec3);
}


void Enemy::setID(s32 id){
    node -> setID(id);
}

void Enemy::setTexture(iv::ITexture *tex){
    assert(node != NULL);
    node->setMaterialTexture(0, tex);
}

void Enemy::being_hit(iv::ITexture* texture_hit){
    if (health_point > 0){
        std::cout<<health_point<<std::endl;
        health_point--;
        blink_frame = 15;
        node->setMaterialTexture(0, texture_hit);
    }
    if (health_point == 0){
        node->setVisible(false);
       /** A MODIFIER ! FAIRE MOURIR LES ENEMIS PLUTOT **/
        //node->setMD2Animation(is::EMAT_DEATH_FALLBACKSLOW);
    }

}


void Enemy::move_randomely_arround_waiting_position()
{

    f32 radius = 100.f;
    ic::array<ic::vector3df> points;
    for(int i = 0; i< 10; i++){
        f32 r = random_generator->frand() * radius;
        f32 teta = random_generator->frand() * M_PI * 2.0f;
        f32 test = random_generator->frand();
        ic::vector3df position = waiting_position_center;
        position.X += r*cos(teta);
        position.Z += r*sin(teta);
        points.push_back(position);
    }

    is::ISceneNodeAnimator *anim = smgr->createFollowSplineAnimator(0.0f,
                                                                    points,0.5f);
    node ->setMD2Animation(is::EMAT_RUN);
    node->addAnimator(anim);
}

void Enemy::make_blink(video::ITexture *texture)
{
    if(blink_frame>0){
        blink_frame--;
    }
    else{
        node->setMaterialTexture(0, texture);
    }
}

void Enemy::attack(Character *perso)
{
    f32 distance_from_player;
    ic::vector3df pos_player = perso->body->getPosition();

    if (pos_player.getDistanceFrom(node->getPosition())<20){
        std::cout<<"touché"<<std::endl;
        perso->take_damage();
        node ->setMD2Animation(is::EMAT_ATTACK);

    }


}

void Enemy::create_collision_with_map(is::ITriangleSelector *world)
{
    const ic::aabbox3d<f32>& box = node->getBoundingBox();
    ic::vector3df radius = box.MaxEdge - box.getCenter();
    is::ISceneNodeAnimatorCollisionResponse *world_collision_anim_response = smgr
            ->createCollisionResponseAnimator(world,node,radius,
                                              ic::vector3df(0,-10,0));

    world_collision_anim_response->setCollisionCallback(&world_collision_response);
    node->addAnimator(world_collision_anim_response);

}


/*
bool Enemy::getCollision()
{
        ic::list< scene::ISceneNodeAnimator*>::ConstIterator begin = mesh->getAnimators().begin();
        ic::list< scene::ISceneNodeAnimator*>::ConstIterator end   = mesh->getAnimators().end();

        for(; begin != end; ++begin)
        {
                 scene::ISceneNodeAnimator* anm = *begin;

                if(anm->getType() == ESNAT_COLLISION_RESPONSE)
                {
                        if( ((scene::ISceneNodeAnimatorCollisionResponse*)anm)->collisionOccurred())
                                return true;
                }
        }

        return false;
}*/

