#include "room_light.h"

#include <string>

#include <ignition/math/Pose3.hh>

#include "gazebo/physics/Entity.hh"
#include "gazebo/physics/World.hh"

namespace gazebo {

void RoomLight::Load(physics::WorldPtr _world, sdf::ElementPtr _sdf) {

  gzmsg << "Loading Example plugin\n";
  // Transport initialization
  this->gzNode_ = transport::NodePtr(new transport::Node());
  this->gzNode_->Init(_world->Name());
  this->world_ = _world;

  // Entity name
  if (!_sdf->HasElement("entity")) {
    gzerr << "Missing required parameter <entity>, plugin will not be "
          << "initialized." << std::endl;
    return;
  }
  this->entityName_ = _sdf->Get<std::string>("entity");

  // Light name
  if (!_sdf->HasElement("light_name")) {
    gzerr << "Missing required parameter <light_name>, plugin will not be "
          << "initialized." << std::endl;
    return;
  }
  this->light_name_ = _sdf->Get<std::string>("light_name");

  // Pose
  if (!_sdf->HasElement("pose")) {
    gzerr << "Missing required parameter <pose>, plugin will not be "
          << "initialized." << std::endl;
    return;
  }
  auto pose = _sdf->Get<ignition::math::Pose3d>("pose");
  sdf::ParamPtr frameParam = _sdf->GetElement("pose")->GetAttribute("frame");
  if (frameParam) {
    this->containerEntityName_ = frameParam->GetAsString();
  }

  // Geometry
  if (!_sdf->HasElement("geometry")) {
    gzerr << "Missing required parameter <geometry>, plugin will not be "
          << "initialized." << std::endl;
    return;
  }
  auto geometryElem = _sdf->GetElement("geometry");

  // Only box for now
  if (!geometryElem->HasElement("box")) {
    gzerr << "Missing required parameter <box>, plugin will not be "
          << "initialized." << std::endl;
    return;
  }
  auto boxElem = geometryElem->GetElement("box");

  if (!boxElem->HasElement("size")) {
    gzerr << "Missing required parameter <size>, plugin will not be "
          << "initialized." << std::endl;
    return;
  }
  auto size = boxElem->Get<ignition::math::Vector3d>("size");

  this->box_ = ignition::math::OrientedBoxd(size, pose);
  this->lightPub_ = this->gzNode_->Advertise<msgs::Light>("~/light/modify");

  this->Start();
}

void RoomLight::Start() {
  // Start update
  this->updateConnection_ = event::Events::ConnectWorldUpdateBegin(
      std::bind(&RoomLight::OnUpdate, this, std::placeholders::_1));

  gzmsg << "Started room_light plugin [" << this->light_name_ << "]"
        << std::endl;
}

void RoomLight::OnUpdate(const common::UpdateInfo &/*_info*/) {
  // Only get the entity once
  physics::EntityPtr entity = this->entity_.lock();
  if (!entity) {
    this->entity_ = this->world_->EntityByName(
        this->entityName_);
    entity = this->entity_.lock();
    if (!entity)
    {
      // Could not find entity being tested.
      this->Publish(false);
      return;
    }
  }

  ignition::math::Vector3d entityInWorldFrame =
    entity->WorldPose().Pos();

  ignition::math::Vector3d entityInBoxFrame;
  if (!this->containerEntityName_.empty()) {
    physics::EntityPtr referenceEntity = this->containerEntity_.lock();
    // box is in a potentially moving reference frame.
    if (!referenceEntity) {
      this->containerEntity_ = this->world_->EntityByName(
        this->containerEntityName_);
      referenceEntity = this->containerEntity_.lock();
      if (!referenceEntity) {
        // Could not find reference entity.
        this->Publish(false);
        return;
      }
    }

    auto boxToWorld = referenceEntity->WorldPose().Inverse();
    // Transform the entity vector from world frame to the frame the box is in
    entityInBoxFrame = (boxToWorld.Rot() * entityInWorldFrame)
      + boxToWorld.Pos();
  }
  else {
    // box frame is world frame
    entityInBoxFrame = entityInWorldFrame;
  }

  this->Publish(this->box_.Contains(entityInBoxFrame));
}

void RoomLight::Publish(const bool _contains) {
  msgs::Light lightMsg;
  lightMsg.set_name(this->light_name_);
  // Turn light on when the entity enters the box, and off when it leaves.
  if (_contains) {
    lightMsg.set_range(15.0);
  }
  else {
    lightMsg.set_range(0.0);
  }
  this->lightPub_->Publish(lightMsg);
}

} // namespace gazebo
