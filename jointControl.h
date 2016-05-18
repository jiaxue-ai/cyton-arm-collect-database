#ifndef jointControl_H_
#define jointControl_H_
//------------------------------------------------------------------------------
// Copyright (c) 2005-2013 Energid Technologies. All rights reserved.
//
/// @file jointControl.h
/// @class jointControl
/// @brief class that contains methods for setting joint positions
//
//------------------------------------------------------------------------------
#include <control/ecCoordinatedJointEndEffectorTool.h>
#include <control/ecPosContSystem.h>
#include <manipulator/ecManipSysState.h>
#include <xml/ecXmlBasicType.h>

// forward declarations
namespace Ec { class PluginGUI; }

class jointControl
{
public:
   jointControl();
   jointControl
      (
      Ec::PluginGUI *plugin
      );
public:
   EcBoolean setJointPosition 
      (
      EcReal delta
      );
   const EcXmlRealVector& getJointPosition
      (
      );
   EcBoolean configure
      (
      );
protected:
   Ec::PluginGUI *m_pPlugin;
   /// the vector of desired placements used in joint control
   EcEndEffectorPlacementVector  m_DesiredJointPlacements;
   EcCoordinatedJointEndEffectorTool   m_JointEeTool;
   /// a copy of the system state
   EcManipulatorSystemState   m_State;

};

#endif