#include <memory>

#include "ImguiActivity.h"
#include "AmpleGui.h"
#include "ample/RegularPolygon.h"
#include "ample/WorldObject2d.h"

#include "Editor.h"
#include "SliceManager.h"
#include "TextureManager.h"

namespace ample::gui
{
AmpleGui::AmpleGui(ample::window::Window &window)
    : ImguiActivity(window),
      _observer(std::make_shared<Observer>(eventManager()))
{
    auto level = createLevel(1, 10.0f, 0.5f);
    setCurrentLevel(1);
    addBehavior(std::static_pointer_cast<Behavior>(_observer));
    level->camera()->setVisibility(false);
    level->frontSlice()->addCamera(std::static_pointer_cast<graphics::Camera>(_observer->getCamera()));
    level->addGlobalObject(_observer->getLamp());
    Editor::instance().setCurrentLayer(level->frontSlice());
    SliceManager::instance().setLevel(level);
}

void AmpleGui::onResize()
{
    ImguiActivity::onResize();
    _observer->onWindowResized({static_cast<int>(getWidth()), static_cast<int>(getHeight())});
}

void AmpleGui::drawInterface()
{
    // ImguiActivity::drawInterface();
    Editor::instance().drawInterface();
    SliceManager::instance().drawInterface();
    TextureManager::instance().drawInterface();
}
} // namespace ample::gui
