#ifndef exampleWidget_H_
#define exampleWidget_H_
//------------------------------------------------------------------------------
// Copyright (c) 2005-2013 Energid Technologies. All rights reserved.
//
/// @file exampleWidget.h
/// @class exampleWidget
/// @brief UI that enables setting joint values and other simple functions to the simulation/hardware if connected
//
//------------------------------------------------------------------------------
#include <foundCore/ecTypes.h>

#include <boost/scoped_ptr.hpp>

#include <QtWidgets/QComboBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QWidget>

// forward declarations

namespace Ui
{
   class exampleWidget;
}
namespace Ec { class PluginGUI; }

class cytonExampleGUIPlugin;


class exampleWidget : public QWidget
{
   Q_OBJECT


public:
   /// Constructor
   explicit exampleWidget
      (
      cytonExampleGUIPlugin* plugin
      );

   /// Destructor
   ~exampleWidget
      (
      );

   void updateManipAction
      (
      EcU32 index
      );
protected Q_SLOTS:
   void onRunManipAction();
   void onSelectManipActionFile();
   void onManipActionActionCompleted(EcBoolean success);
   void onLoadManipAction();
   void onActionSeriesCompleted(EcBoolean success);
   void onRunManipSeries();
   void onRunProgramCode();
protected:

   void init 
      (
      );

   void initLayout
      (
      );

   void connectSignal
      (
      );
   void setButtonStates
      (
      EcBoolean enable
      );

   boost::scoped_ptr<QPushButton>      m_RunManipActionButtonPtr;
   boost::scoped_ptr<QLabel>           m_LoadManipActionLabelPtr;
   boost::scoped_ptr<QLineEdit>        m_ManipActionFileEditPtr;
   boost::scoped_ptr<QComboBox>        m_ManipActionComboBoxPtr;
   boost::scoped_ptr<QPushButton>      m_LoadManipActionButtonPtr;
   boost::scoped_ptr<QPushButton>      m_RunManipSeriesButtonPtr;

   boost::scoped_ptr<QPushButton>      m_RunProgramButtonPtr;

   ///< Handle to owning plugin
   cytonExampleGUIPlugin* m_pPlugin;

};

#endif
