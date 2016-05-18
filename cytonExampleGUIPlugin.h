#ifndef cytonExampleGUIPlugin_H_
#define cytonExampleGUIPlugin_H_
//------------------------------------------------------------------------------
// Copyright (c) 2012-2013 Energid Technologies. All rights reserved.
//
/// @file cytonExampleGUIPlugin.h
/// @class cytonExampleGUIPlugin
/// @brief A Qt-based GUI plugin example for creating code that will
///        interface with Energid's viewer.
/// @details It utilizes the Qt windowing toolkit for the UI controls. Since most
///          Qt window-specific calls need to be issued in the main application
///          thread, a separate process is not spawned by default.  Actions are
///          typically handled through Qt's signal/slot mechanism.
//
//------------------------------------------------------------------------------
#include <viewerCore/ecPluginGUI.h>

class QDockWidget;
class manipulationActionExecPlugin;
class EcManipulationActionManager;
class exampleWidget;

/// This plugin is mostly an output plugin, but it does set an initial
/// state on startup to initialize the joint angles.
class cytonExampleGUIPlugin : public Ec::PluginGUI
{
   Q_OBJECT
public:
   void runManipulationAction
   (
   const EcString &actionName
   );
   const QStringList getManipActionList
      (
      );
   EcBoolean loadManipulationActionManager
      (
      const EcString &manipActionFile
      );
   void runManipulationActionSeries
      (
      );
   void stopAction
      (
      );
protected:
   /// Constructor.
   cytonExampleGUIPlugin
      (
      );

   /// Destructor.
   ~cytonExampleGUIPlugin
      (
      );

   /// Initialize plugin.  Called before first use.
   /// @return EcBoolean Success or failure of reset command.
   EcBoolean init
      (
      );
   EcBoolean startAction
      (
      );
   void actionSeriesCompletionCallback
      (
      EcBoolean success
      );
   void actionCompletionCallback
      (
      EcBoolean success
      );
Q_SIGNALS:
   void manipActionCompleted();
   void actionSeriesCompleted(EcBoolean success);
   void actionCompleted(EcBoolean success);
   void seriesActionCompleted(EcBoolean success);

public Q_SLOTS:
   void exampleSlot();

protected Q_SLOTS:
   void onActionButtonPressed(bool checked);
   void closeWindow();
   void onSeriesActionCompleted(EcBoolean success);
protected:
      QDockWidget* m_pSampleDockWidget;
      exampleWidget* m_pMainWidget;
      QAction*     m_pAction;
      EcString     m_manipActionFile;
      manipulationActionExecPlugin* m_ManipActionExecPlugin;
      // Action manager object
      EcManipulationActionManager *m_pManipActionMgr;
      EcU32       m_CurrentActionIndex;
      EcBoolean   m_RunSeriesInProgress;

};

#endif // exampleGUIPlugin_H_
