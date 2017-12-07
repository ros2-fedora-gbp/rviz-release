/*
 * Copyright (c) 2013, Willow Garage, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the Willow Garage, Inc. nor the names of its
 *       contributors may be used to endorse or promote products derived from
 *       this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef RVIZ_COMMON__TEMP__ROBOT__ROBOT_JOINT_HPP_
#define RVIZ_COMMON__TEMP__ROBOT__ROBOT_JOINT_HPP_

#include <map>
#include <string>

#ifndef Q_MOC_RUN
#include <OgreVector3.h>
#include <OgreQuaternion.h>
#include <OgreAny.h>
#include <OgreMaterial.h>
#endif

#include <QObject>

#include "urdf/model.h"
#include "urdf_model/pose.h"

#include "rviz_rendering/object.hpp"
#include "rviz_common/selection/forwards.hpp"

namespace Ogre
{
class SceneManager;
class Entity;
class SubEntity;
class SceneNode;
class Vector3;
class Quaternion;
class Any;
class RibbonTrail;
}  // namespace Ogre

namespace rviz_rendering
{
class Shape;
class Arrow;
class Axes;
}  // namespace rviz_rendering

namespace rviz_common
{

namespace properties
{
class BoolProperty;
class FloatProperty;
class Property;
class QuaternionProperty;
class StringProperty;
class VectorProperty;
}  // namespace properties

class DisplayContext;
class Robot;
class RobotLinkSelectionHandler;
class RobotJoint;


/**
 * \struct RobotJoint
 * \brief Contains any data we need from a joint in the robot.
 */
class RobotJoint : public QObject
{
  Q_OBJECT

public:
  RobotJoint(Robot * robot, const urdf::JointConstSharedPtr & joint);
  virtual ~RobotJoint();


  void setTransforms(
    const Ogre::Vector3 & parent_link_position,
    const Ogre::Quaternion & parent_link_orientation);

  const std::string & getName() const {return name_;}
  const std::string & getParentLinkName() const {return parent_link_name_;}
  const std::string & getChildLinkName() const {return child_link_name_;}
  const properties::Property * getJointProperty() const {return joint_property_;}
  properties::Property * getJointProperty() {return joint_property_;}
  RobotJoint * getParentJoint();
  void hideSubProperties(bool hide);

  // Remove joint_property_ from its old parent and add to new_parent.
  // If new_parent==NULL then leav unparented.
  void setParentProperty(properties::Property * new_parent);

  Ogre::Vector3 getPosition();
  Ogre::Quaternion getOrientation();

  void setRobotAlpha(float a) {(void) a;}

  bool hasDescendentLinksWithGeometry() const {return has_decendent_links_with_geometry_;}

  // place subproperties as children of details_ or joint_property_
  void useDetailProperty(bool use_detail);

  // expand all sub properties
  void expandDetails(bool expand);

  // Set the description for the joint.
  // Also sets the checkbox.
  // Also sets has_decendent_links_with_geometry_.
  // Called when the link_tree style changes.
  void setJointPropertyDescription();

  // set checkboxes based on state of descendent link enables
  // Should only be called by Robot::calculateJointCheckboxes()
  void calculateJointCheckboxesRecursive(
    int & links_with_geom,              // returns # of children with geometry
    int & links_with_geom_checked,      // returns # of enabled children with geometry
    int & links_with_geom_unchecked);   // returns # of disabled children with geometry

private Q_SLOTS:
  void updateAxes();
  void updateAxis();
  void updateChildVisibility();

private:
  bool getEnabled() const;

  // true if displaying in a tree style.  False if list style.
  bool styleIsTree() const;

  // determine the state of child link(s)
  void getChildLinkState(
    int & links_with_geom,              // returns # of children with geometry
    int & links_with_geom_checked,      // returns # of enabled children with geometry
    int & links_with_geom_unchecked,    // returns # of disabled children with geometry
    bool recursive) const;              // True: all descendant links.
  // False: just single child link.

  // set the value of the enable checkbox without touching child joints/links
  void setJointCheckbox(QVariant val);

protected:
  Robot * robot_;
  std::string name_;                          ///< Name of this joint
  std::string parent_link_name_;
  std::string child_link_name_;

  // properties
  properties::Property * joint_property_;
  properties::Property * details_;
  properties::VectorProperty * position_property_;
  properties::QuaternionProperty * orientation_property_;
  properties::Property * axes_property_;
  // The joint axis if any, as opposed to the frame in which the joint exists above
  properties::VectorProperty * axis_property_;
  properties::Property * show_axis_property_;
  properties::StringProperty * type_property_;
  properties::FloatProperty * lower_limit_property_;
  properties::FloatProperty * upper_limit_property_;

private:
  Ogre::Vector3 joint_origin_pos_;
  Ogre::Quaternion joint_origin_rot_;
  bool has_decendent_links_with_geometry_;

  bool doing_set_checkbox_;   // prevents updateChildVisibility() from  touching children

  rviz_rendering::Axes * axes_;
  rviz_rendering::Arrow * axis_;
};

}  // namespace rviz_common

#endif  // RVIZ_COMMON__TEMP__ROBOT__ROBOT_JOINT_HPP_
