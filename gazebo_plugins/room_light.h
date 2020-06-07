#include "gazebo/common/Events.hh"
#include "gazebo/common/UpdateInfo.hh"

#include "gazebo/common/Plugin.hh"
#include "gazebo/transport/Node.hh"

namespace gazebo {

  /// \brief Plugin which emits Ignition light message according to whether
  /// an entity's origin is inside or outside a given volume.
  ///
  /// Example usage:
  ///
  ///  <plugin name="LivingLight" filename="libroom_light.so">
  ///
  ///    <!-- Scoped name of entity to check -->
  ///    <entity>car::chassis</entity>
  ///
  ///    <!-- Light entity's name -->
  ///    <light_name>living_light</light_name>
  ///
  ///    <!-- Pose of the geometry's center point in world coordinates -->
  ///    <pose>10 10 2 0 0 1.57</pose>
  ///
  ///    <!-- OR Pose of the geometry's center in link frame coordinates -->
  ///    <pose frame="model_name::link_name">10 10 2 0 0 1.57</pose>
  ///
  ///    <!-- Geometry of the volume to check, uses the SDF geometry spec:
  ///         http://sdformat.org/spec?ver=1.6&elem=geometry
  ///         currently <box> is supported -->
  ///    <geometry>
  ///      <box>
  ///        <size>1 1 4</size>
  ///      </box>
  ///    </geometry>
  ///
  ///  </plugin>
  ///
  class RoomLight : public WorldPlugin {
   public:
    void Load(physics::WorldPtr _world, sdf::ElementPtr _sdf) override;

   private:
    /// \brief Called every world iteration on world update begin.
    /// \param[in] _info Update info.
    void OnUpdate(const common::UpdateInfo &_info);

    /// \brief Starts the plugin.
    void Start();

    /// \brief Publish a message to turn on or off the light.
    /// \param[in] on True if the geometry contains the target and the light must be turn on.
    void Publish(bool on);

    // Connection to world update.
    event::ConnectionPtr updateConnection_;
    // Pointer to the world.
    physics::WorldPtr world_;
    // Scoped name of the entity we're checking.
    std::string entityName_;
    // Pointer to the entity we're checking.
    boost::weak_ptr<physics::Entity> entity_;
    // Box representing the volume to check.
    ignition::math::OrientedBoxd box_;
    // pointer to an entity whose pose the geometry will track
    boost::weak_ptr<physics::Entity> containerEntity_;
    // scoped name of entity to track
    std::string containerEntityName_;
    // Name of the light.
    std::string light_name_;

    transport::NodePtr gzNode_;

    transport::PublisherPtr lightPub_;

  };
GZ_REGISTER_WORLD_PLUGIN(RoomLight);

} // namespace gazebo
