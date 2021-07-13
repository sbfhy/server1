#include "src/base/common/singleton.h"
#include "src/base/common/mgr_base.h"

#include <memory>

class MgrScene : public Singleton<MgrScene>, public MgrBase
{
public:
    virtual void Start() {}

private:
    std::unique_ptr<class Scene> m_pSceneCapital{nullptr};
}
