//------------------------------------------------------------------------------
// Copyright (c) 2012-2013 Energid Technologies. All rights reserved.
//
/// @file jointControl.cpp
//
//------------------------------------------------------------------------------

#include "jointControl.h"
#include "cytonIncludes.h"

#include <plugins/ecIOParams.h>
#include <viewerCore/ecPluginGUI.h>
#include <QtWidgets/QMessageBox>


//--------------------------------------------------------------------------------
jointControl::jointControl
   (
   Ec::PluginGUI  *plugin
   )
{
   m_pPlugin = plugin;
}

//--------------------------------------------------------------------------------
EcBoolean jointControl::configure
   (
   )
{
	// get the current state
	m_State = m_pPlugin->param<Ec::State,EcManipulatorSystemState>();
	const EcIndividualManipulatorVector* manips = m_pPlugin->paramPtr<Ec::Manipulator,EcIndividualManipulatorVector>();
	const EcPositionControlSystem* pPosCont = m_pPlugin->paramPtr<Ec::ControlSystem, EcPositionControlSystem>();
	if(!manips || !pPosCont)
	{
		EcPrint(Error) << "Invalid manipulator" << std::endl;
		// null pointers, bail
		return EcFalse;
	}
	if(!m_JointEeTool.configure(*manips, pPosCont->positionControllers()))
	{
		EcPrint(Warning) << "Failed to configure joint controller tool." << std::endl;
      return EcFalse;
	}

   return EcTrue;
}

//--------------------------------------------------------------------------------
const EcXmlRealVector& jointControl::getJointPosition
   (
   )
{
   // set the joint control
   m_pPlugin->setParam<Ec::EndEffectorSet>(MANIP_INDEX, EcU32(EcVelocityController::JOINT_CONTROL_INDEX));
	const EcPositionStateVector& positions=m_State.positionStates();
			
   // prepare the desired joint placement
   m_DesiredJointPlacements.resize(MANIP_INDEX+1);

   // now populate the current joint positions
   const EcPositionState& posState = m_State.positionStates()[MANIP_INDEX];
   m_JointEeTool.setEndEffectorPlacement(MANIP_INDEX, posState.jointPositions(),
	   m_DesiredJointPlacements[MANIP_INDEX]);
   return (positions[MANIP_INDEX].jointPositions());
}

//--------------------------------------------------------------------------------
EcBoolean jointControl::setJointPosition
   (
   EcReal delta
   )
{
   EcXmlRealVector jointPosVector;
	m_pPlugin->setParam<Ec::ExternalControl>(EcFalse);
	m_pPlugin->setParam<Ec::SimRunState>(Ec::SimulationRunning);
   jointPosVector = getJointPosition ();

   EcU32 numJoints = (EcU32)jointPosVector.size();
   for (EcU32 ii = 0; ii < (numJoints - 2); ii++)//increment all joints but the gripper joints.
   {
		// update the state
		m_State.positionStates()[MANIP_INDEX].jointPositions()[ii]=jointPosVector[ii] + delta;
	   // sometime linkIndex is not the same index in the EcCoordinatedJointEndEffector due to constrained joints
	   // need to account for this
	   m_JointEeTool.setEndEffectorPlacement(MANIP_INDEX, m_State.positionStates()[MANIP_INDEX].jointPositions(), 
		   m_DesiredJointPlacements[MANIP_INDEX]);

	   m_pPlugin->setParam<Ec::DesiredEndEffector>(MANIP_INDEX, 0, m_DesiredJointPlacements[MANIP_INDEX]);
   }
   return EcTrue;
}