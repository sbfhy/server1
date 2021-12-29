#pragma once 

#include <memory>

class Scene;
typedef std::shared_ptr<Scene> ScenePtr;
typedef std::weak_ptr<Scene> SceneWPtr;
