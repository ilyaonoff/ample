#pragma once

#include <unordered_map>
#include <memory>
#include <string>
#include <filesystem>

#include "WorldLayer2d.h"
#include "Vector2d.h"
#include "Camera.h"
#include "CameraPerspective.h"
#include "GameController.h"
#include "StoredObject.h"
#include "Scene2d.h"

/*
Level structure:
.
├── settings.json                   -- current level settings
├── scenes                          -- all 'slices' of level: foreground, backgrounds
│   └── <...>.json
└── textures                        -- level textures
    └── <...>.[png, jpg, jpeg, bmp]
*/

namespace ample::game
{
class GameController;
} // namespace ample::game

namespace ample::game::game2d
{

class Level : public ControlledObject::ObjectState<GameController>
{
public:
    Level(const std::filesystem::path &path, std::shared_ptr<GameController> controller);
    void save();

    Level(const std::string &name,
          std::shared_ptr<GameController> controller,
          float sliceThikness,
          float physicsLayerPosition,
          const graphics::Vector2d<float> &gravity,
          const std::filesystem::path &destinationPath);

    void onStart() override;
    void onActive() override;
    void onStop() override;

    std::shared_ptr<filing::Scene2d> createSlice(const size_t num, const std::string &name);
    std::shared_ptr<filing::Scene2d> frontSlice() noexcept;
    std::shared_ptr<filing::Scene2d> numberedSlice(const size_t num);

    void addGlobalObject(std::shared_ptr<graphics::GraphicalObject>);
    void removeGlobalObject(std::shared_ptr<graphics::GraphicalObject>);

    std::shared_ptr<graphics::CameraPerspective> camera();

    std::unordered_map<size_t, std::shared_ptr<filing::Scene2d>> &layers() noexcept;

private:
    float _sliceThikness;
    float _physicsLayerPosition;
    graphics::Vector2d<float> _defaultGravity;
    std::unordered_map<size_t, std::shared_ptr<filing::Scene2d>> _sliceByDistance;
    std::shared_ptr<graphics::CameraPerspective> _perspectiveCamera;
    std::shared_ptr<GameController> _controller;
    bool _editingMode = false;
    std::filesystem::path _path;
};
} // namespace ample::game::game2d
