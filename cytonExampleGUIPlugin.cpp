//------------------------------------------------------------------------------
// Copyright (c) 2012-2013 Energid Technologies. All rights reserved.
//
/// @file cytonExampleGUIPlugin.cpp
//
//------------------------------------------------------------------------------
#include "cytonExampleGUIPlugin.h"
#include "cytonIncludes.h"
#include "exampleWidget.h"

#include <foundCore/ecPluginManager.h>
#include <manipulation/ecManipulationActionManager.h>
#include <manipulationActionExecPlugin/manipulationActionExecPlugin.h>
#include <plugins/ecIOParams.h>
#include <slotManager/ecSlotManager.h>
#include <xmlReaderWriter/ecXmlObjectReaderWriter.h>

#include <QtCore/QString>
#include <QtWidgets/QAction>
#include <QtWidgets/QDockWidget>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QPushButton>

// Required declaration to define entry point and parameters for this plugin.
EC_PLUGIN_STUB_DEFAULT(cytonExampleGUIPlugin);

//------------------------------------------------------------------------------
cytonExampleGUIPlugin::cytonExampleGUIPlugin
   (
   ):
Ec::PluginGUI()
{
   m_pSampleDockWidget = EcNULL;
   m_pManipActionMgr = EcNULL;
   m_RunSeriesInProgress = EcFalse;
   m_RequiredPlugins.push_back("manipulationActionExecPlugin"); //this method makes sure the required plugins are loaded first.
   m_RequiredPlugins.push_back("poseLibraryPlugin");
}


//------------------------------------------------------------------------------
cytonExampleGUIPlugin::~cytonExampleGUIPlugin
   ( 
   )
{
   // As long as proper Qt parentage is applied to objects, everything
   // should be cleaned up when this plugin is destroyed and we shouldn't
   // need to explicitly delete anything.  Objects that don't have this plugin
   // as their parent (or no parent) can be added to the auto-cleanup list by
   // using the following call:
   //
   // addAutoCleanup(QtObject*);
   //
   // These objects will be destroyed as part of the base-class destructor. Do
   // not add objects to this list that will handled elsewhere, otherwise
   // double-deletion may occur.
   //
   // QDockWidgets and QToolBars will be automatically handled when added with
   // the PluginGUI methods (addToolBar, addDockWidget).
}


//------------------------------------------------------------------------------
// Initializes plugin before first use. Called right after the constructor.
//------------------------------------------------------------------------------
EcBoolean
cytonExampleGUIPlugin::init
   (
   )
{
   // Create a GUI button
   m_pAction = new QAction(this);
   m_pAction->setCheckable(true);
   m_pAction->setObjectName(QString::fromUtf8("actionExample"));
   m_pAction->setText(QString::fromUtf8("Show Example GUI"));
   m_ManipActionExecPlugin = EcPLUGIN_FIND_AND_CAST(manipulationActionExecPlugin);

   setObjectName("cytonExampleGUIPlugin");   // required for the slot to register properly
   Ec::slotManager* sm = Ec::slotManager::instance();
   sm->addSlot(this, "exampleSlot",  "Example Slot");

   connect (this, SIGNAL(seriesActionCompleted(EcBoolean)), this, SLOT(onSeriesActionCompleted(EcBoolean)));
   return addToToolBar(m_pAction) &&
          connect(m_pAction, SIGNAL(toggled(bool)), SLOT(onActionButtonPressed(bool)));
}


//------------------------------------------------------------------------------
// Called when user presses the toolbar button
//------------------------------------------------------------------------------
void
cytonExampleGUIPlugin::onActionButtonPressed
   (
   bool checked
   )
{
   QAction* action = qobject_cast<QAction*>(sender());
   if(action)
   {
      action->setText(checked ? tr("Hide Example GUI Plugin") : tr("Show Example GUI"));
      if (checked)
      {
         if(!m_pSampleDockWidget)
         {
            m_pSampleDockWidget = new QDockWidget; 
            m_pSampleDockWidget->setAllowedAreas(Qt::AllDockWidgetAreas);
            m_pMainWidget = new exampleWidget(this);
            m_pSampleDockWidget->setWidget(m_pMainWidget);
            addDockWidget(Qt::RightDockWidgetArea, m_pSampleDockWidget);
         }
         else
            m_pSampleDockWidget->setVisible(EcTrue);
      }
      else
      {
         m_pSampleDockWidget->setVisible(EcFalse);
      }

   }

}

//------------------------------------------------------------------------------
// Called when user closes the dock widget
//------------------------------------------------------------------------------
void cytonExampleGUIPlugin::closeWindow
   (
   )
{
   if(m_pAction)
   {
      m_pAction->setChecked(EcFalse);
      m_pAction->setText(tr("Press me"));
   }
}

void cytonExampleGUIPlugin::runManipulationAction
   (
   const EcString& actionName
   )
{
   m_ManipActionExecPlugin->setManipulationAction(actionName);
   startAction();
}

EcBoolean cytonExampleGUIPlugin::loadManipulationActionManager
   (
   const EcString &manipActionFile
   )
{
   m_manipActionFile = manipActionFile;
   EcRealVector jointAnglesVectorOrg, jointAnglesVectorMod;

   if (m_ManipActionExecPlugin != EcNULL)
   {
      m_pManipActionMgr = new EcManipulationActionManager;
      if(!EcXmlObjectReaderWriter::readFromFile(*m_pManipActionMgr, m_manipActionFile))
      {
         QMessageBox::information(this, tr("Error"), tr("Unable to read from file"));
         EcPrint(Fatal) << "Unable to load action manager from file (try testRemoteCommandActions.xml int he bin directory)" << m_manipActionFile << "'" << std::endl;
         return EcFalse;
      }
      m_ManipActionExecPlugin->setManipulationActionManager(*m_pManipActionMgr);
   }
   return EcTrue;
}

EcBoolean cytonExampleGUIPlugin::startAction()
{
   m_ManipActionExecPlugin->setManipIndex(0);
   m_ManipActionExecPlugin->setManipulationCompletionCallback
      (
      boost::bind(&cytonExampleGUIPlugin::actionCompletionCallback, this, _1)
      );
   if(!m_ManipActionExecPlugin->start())
   {
      EcPrint(Fatal) << "Failed to start action execution" << std::endl;
      setParam<Ec::SimRunState>(Ec::SimulationStopped);
      return EcFalse;
   }
   setParam<Ec::SimRunState>(Ec::SimulationRunning);
   return EcTrue;
}

//----------------------------------------------------------------------------
// This method gets a list of all actions in the manipulation action file that has been loaded
//----------------------------------------------------------------------------
const QStringList cytonExampleGUIPlugin::getManipActionList()
{
   QStringList actionList;

   EcXmlStringVector actionOrderList;
   if (m_pManipActionMgr != EcNULL)
   {
      actionOrderList = m_pManipActionMgr->actionOrder();
      EcU32 numActions = (EcU32)actionOrderList.size();
      for (EcU32 ii = 0; ii < numActions; ++ii)
      {
         actionList.push_back(QString::fromStdString(actionOrderList[ii].value()));
      }
   }
   return actionList;
}

//----------------------------------------------------------------------------
// This method runs all the actions in the manipulation action file as a series in the order in which
// it had been added while saving the file
//----------------------------------------------------------------------------
void cytonExampleGUIPlugin::runManipulationActionSeries
   (
   )
{
   if (m_pManipActionMgr == EcNULL || m_ManipActionExecPlugin == EcNULL)
   {
      return;
   }
   m_ManipActionExecPlugin->setManipIndex(MANIP_INDEX);
   m_ManipActionExecPlugin->setObjectIndex(0);
   //m_pExecPlugin->setPoseSetPtr(m_pPosePlugin->currentPoseSetPtr());
   m_ManipActionExecPlugin->setManipulationCompletionCallback
            (
            boost::bind(&cytonExampleGUIPlugin::actionSeriesCompletionCallback, this, _1)
            );
   m_CurrentActionIndex = 0;
   const EcManipulationAction* action = m_pManipActionMgr->manipulationAction(m_CurrentActionIndex);
   if(action)
   {
      m_pMainWidget->updateManipAction(m_CurrentActionIndex);
      m_ManipActionExecPlugin->setManipulationAction(*action);
      m_ManipActionExecPlugin->start();
      setParam<Ec::SimRunState>(Ec::SimulationRunning);
   }
}

//----------------------------------------------------------------------------
// This is a callback method that is called when each action in a manipulation series is completed, 
// when the series is being executed
//----------------------------------------------------------------------------
void cytonExampleGUIPlugin::actionSeriesCompletionCallback
   (
   EcBoolean success
   )
{
   seriesActionCompleted(success);
}

//----------------------------------------------------------------------------
// This method is a callback and is called when Run Action is selected and the action is completed
//----------------------------------------------------------------------------
void cytonExampleGUIPlugin::actionCompletionCallback
   (
   EcBoolean success
   )
{
   actionCompleted(success);
}

void cytonExampleGUIPlugin::stopAction
   (
   )
{
   m_ManipActionExecPlugin->setManipulationCompletionCallback(0);
   setParam<Ec::SimRunState>(Ec::SimulationStopped);
}

void cytonExampleGUIPlugin::onSeriesActionCompleted
   (
   EcBoolean success
   )
{
   EcXmlStringVector actionList = m_pManipActionMgr->actionOrder();
   EcU32 numActions = (EcU32)actionList.size();

   //If Run series was started and now it has completed then inform the clients about it
   if (!success || m_CurrentActionIndex >= numActions-1)
   {
      m_ManipActionExecPlugin->setManipulationCompletionCallback(0);
      actionSeriesCompleted(success);
   }
   else
   {
      //If the actions in the series has been completed then start the next action in the order
      m_CurrentActionIndex++;
      m_pMainWidget->updateManipAction(m_CurrentActionIndex);
      m_ManipActionExecPlugin->setManipulationAction(actionList[m_CurrentActionIndex].value());
      m_ManipActionExecPlugin->start();
   }
}

void cytonExampleGUIPlugin::exampleSlot
   (
   )
{
   QMessageBox::information(this, tr("cytonExampleGUIPlugin"), tr("example slot is executed")); 
}
