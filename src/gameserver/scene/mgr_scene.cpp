#include "src/gameserver/scene/mgr_scene.h"

#include "src/gameserver/scene/scene.h"

MgrScene::MgrScene()
    : m_pSceneCapital(nullptr)
{
}

MgrScene::~MgrScene()
{

}

bool MgrScene::Init()
{
    m_pSceneCapital = std::make_unique<Scene>();
    return true;
}
