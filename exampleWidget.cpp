//------------------------------------------------------------------------------
// Copyright (c) 2013 Energid Technologies. All rights reserved.
//
/// @file ecCollisionExclusionConfigWidget.cpp
//
//------------------------------------------------------------------------------

#include "cytonExampleIncludes.h"
#include "exampleWidget.h"
#include "cytonExampleGUIPlugin.h"
#include "jointControl.h"

#include <control/ecPointEndEffector.h>
#include <cytonPlugin/cytonPlugin.h>
#include <plugins/ecIOParams.h>
#include <foundCore/ecApplication.h>
#include <osg/Matrix>

#include <QtCore/QSettings>

#include <QtWidgets/QButtonGroup>
#include <QtGui/QCloseEvent>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QInputDialog>
#include <string>
#include <iomanip>
#include <pylon/PylonIncludes.h>
#ifdef PYLON_WIN_BUILD
#    include <pylon/PylonGUI.h>
#endif
#define _CRT_SECURE_NO_WARNINGS
using namespace Pylon;
using namespace std;

// Settings for using  Basler USB cameras.
#include <pylon/usb/BaslerUsbInstantCamera.h>
typedef Pylon::CBaslerUsbInstantCamera Camera_t;
using namespace Basler_UsbCameraParams;

// The camera specific grab result smart pointer.
typedef Camera_t::GrabResultPtr_t GrabResultPtr_t;

void AutoGainOnce(Camera_t& camera);
void AutoWhiteBalance(Camera_t& camera);
void AutoExposureOnce(Camera_t& camera);
void AutoGainContinuous(Camera_t& camera);
void AutoExposureContinuous(Camera_t& camera);

string setWitThree(int number)
{
	std::ostringstream ss;
	ss << std::setw(3) << std::setfill('0') << number;
	std::string s2(ss.str());
	return s2;
}

string setWitTwo(int number)
{
	std::ostringstream ss;
	ss << std::setw(2) << std::setfill('0') << number;
	std::string s2(ss.str());
	return s2;
}
//------------------------------------------------------------------------------
exampleWidget::exampleWidget
   (
   cytonExampleGUIPlugin* plugin
   ) :
QWidget(plugin),
m_pPlugin(plugin)
{
   init();
   connectSignal();
}

//------------------------------------------------------------------------------
exampleWidget::~exampleWidget
   (
   )
{
}

//------------------------------------------------------------------------------
// This method creates all the UI controls for the plugin and performs
// the layouting required
//------------------------------------------------------------------------------
void exampleWidget::init
   (
   )
{
   //Create two grid layouts, manipLayout for performing manipulation action manager related operations
   //and setting of the joints

   //Then add the two grid layouts to a vertical layout and set it to the widget
   QVBoxLayout *vBoxLayout = new QVBoxLayout();

   QGroupBox *pManipGroupBox = new QGroupBox(tr("Manipulation Actions"), this);
   QGridLayout *manipLayout = new QGridLayout();
 
   QLabel *label = new QLabel(tr("Load Manip Action: "), this);
   m_LoadManipActionButtonPtr.reset(new QPushButton(tr("Load"), this));
   m_ManipActionFileEditPtr.reset(new QLineEdit(this));
   m_LoadManipActionButtonPtr.get()->setToolTip(tr("Load a manipulation action file"));

   QPushButton *selectManipFileButton = new QPushButton("...", this);
   connect (selectManipFileButton, SIGNAL(clicked()), this, SLOT(onSelectManipActionFile()));
   manipLayout->addWidget(label, 1, 0, Qt::AlignRight);
   manipLayout->addWidget(m_ManipActionFileEditPtr.get(), 1, 1, Qt::AlignLeft);
   manipLayout->addWidget(selectManipFileButton, 1, 2, Qt::AlignCenter);
   manipLayout->addWidget(m_LoadManipActionButtonPtr.get(), 1, 3, Qt::AlignCenter);

   label = new QLabel (tr("Select Action: "), this);
   m_ManipActionComboBoxPtr.reset(new QComboBox(this));
   m_RunManipActionButtonPtr.reset(new QPushButton(tr("Run Action"), this));
   m_RunManipActionButtonPtr.get()->setToolTip(tr("Run Currently Selected Manipulation Action"));
   m_RunManipSeriesButtonPtr.reset(new QPushButton(tr("Run Series"), this));
   m_RunManipSeriesButtonPtr.get()->setToolTip(tr("Run All actions as a series"));
   m_ManipActionComboBoxPtr.get()->setMinimumWidth(150);
   manipLayout->addWidget(label, 2, 0, Qt::AlignRight);
   manipLayout->addWidget(m_ManipActionComboBoxPtr.get(), 2, 1, Qt::AlignLeft);
   manipLayout->addWidget(m_RunManipActionButtonPtr.get(), 2, 2, Qt::AlignCenter);
   manipLayout->addWidget(m_RunManipSeriesButtonPtr.get(), 2, 3, Qt::AlignCenter);

   manipLayout->setHorizontalSpacing(12);
   manipLayout->setVerticalSpacing(5);
   manipLayout->setMargin(0);

   manipLayout->setColumnStretch(0, 2);
   manipLayout->setColumnStretch(1, 3);
   manipLayout->setColumnStretch(2, 1);
   manipLayout->setColumnStretch(3, 1);

   manipLayout->setRowStretch(0, 1);
   manipLayout->setRowStretch(1, 1);
   manipLayout->setRowStretch(2, 1);
   manipLayout->setSpacing(2);
   manipLayout->setContentsMargins(0, 0, 4, 1);

   pManipGroupBox->setLayout(manipLayout);

   //ADD A BUTTON TO ALLOW CODE TO BE EXPANDED
   m_RunProgramButtonPtr.reset(new QPushButton(tr("Run Program"), this));
   
   vBoxLayout->addWidget(pManipGroupBox);
   vBoxLayout->setSpacing(30);
   vBoxLayout->addWidget(m_RunProgramButtonPtr.get());
   vBoxLayout->addStretch(1);
   
   setLayout(vBoxLayout);

   setButtonStates(EcFalse);

}


//------------------------------------------------------------------------------
void exampleWidget::connectSignal
   (
   )
{
   connect(m_LoadManipActionButtonPtr.get(), SIGNAL(pressed()), this, SLOT(onLoadManipAction()));
   connect(m_RunManipActionButtonPtr.get(), SIGNAL(pressed()), this, SLOT(onRunManipAction()));
   connect(m_pPlugin, SIGNAL(actionCompleted(EcBoolean)), this, SLOT(onManipActionActionCompleted(EcBoolean)));
   connect(m_RunManipSeriesButtonPtr.get(), SIGNAL(pressed()), this, SLOT(onRunManipSeries()));
   connect (m_pPlugin, SIGNAL(actionSeriesCompleted(EcBoolean)), this, SLOT(onActionSeriesCompleted(EcBoolean)));
   connect (m_RunProgramButtonPtr.get(), SIGNAL(pressed()), this, SLOT(onRunProgramCode()));
}


//------------------------------------------------------------------------------
// This method loads a manipulation action file and runs a particular action
//------------------------------------------------------------------------------

void exampleWidget::onRunManipAction
   (
   )
{
   m_RunManipActionButtonPtr.get()->setEnabled(EcFalse);
   m_RunManipSeriesButtonPtr->setEnabled(EcFalse);
   m_pPlugin->runManipulationAction(m_ManipActionComboBoxPtr->currentText().toStdString());
}

//------------------------------------------------------------------------------
// This method is called when user clicks the button to select a manipulation ation file to load
//------------------------------------------------------------------------------
void exampleWidget::onSelectManipActionFile
   (
   )
{

   QString filename = QFileDialog::getOpenFileName(this, tr("Load Manipulation Action File"),
                                                   "",
                                                   QString("Ecam File (*.ecam)"));
   if (!filename.isEmpty())
   {
      m_ManipActionFileEditPtr.get()->setText(filename);
      m_LoadManipActionButtonPtr.get()->setEnabled(EcTrue);
   }
}

//------------------------------------------------------------------------------
// This method is called when the plugin sends a signal on completion of running manipulation action
//------------------------------------------------------------------------------
void exampleWidget::onManipActionActionCompleted
   (
   EcBoolean success
   )
{
   //m_pPlugin->stopAction();
   setButtonStates (EcTrue);
}

//------------------------------------------------------------------------------
// This method is called when a manipulation action file is selcted and the load button is pressed
//------------------------------------------------------------------------------
void exampleWidget::onLoadManipAction
   (
   )
{
   if (!m_pPlugin->loadManipulationActionManager (m_ManipActionFileEditPtr.get()->text().toStdString()))
   {
      setButtonStates(EcFalse);
      return;
   }
   setButtonStates(EcTrue);
   const QStringList manipActionList = m_pPlugin->getManipActionList();
   EcU32 numActions = (EcU32)manipActionList.size();
   for (EcU32 ii = 0; ii < numActions; ++ii)
   {
      m_ManipActionComboBoxPtr->addItem(manipActionList[ii]);
   }
}

//------------------------------------------------------------------------------
// This is a callback method and is called when the manipulation action series is completed
//------------------------------------------------------------------------------
void exampleWidget::onActionSeriesCompleted
   (
   EcBoolean success
   )
{
   setButtonStates (EcTrue);
   
   if (success)
   {
      QMessageBox::information(this, tr("Info"), tr("Manipulation Series completed successfully"));
   }
   else
   {
      QMessageBox::information(this, tr("Info"), tr("Manipulation Series could not be completed successfully"));
   }
}

//------------------------------------------------------------------------------
// This is method is called when the manipulation action series button is clicked on the UI
// This method can be called only after loading a manipulation action
//------------------------------------------------------------------------------
void exampleWidget::onRunManipSeries
   (
   )
{
   setButtonStates (EcFalse);
   m_pPlugin->runManipulationActionSeries();
}

//----------------------------------------------------------------------------
// This method enables/disables the manipulation action related buttons
//----------------------------------------------------------------------------

void exampleWidget::setButtonStates
   (
   EcBoolean enable
   )
{
   m_RunManipActionButtonPtr.get()->setEnabled(enable);
   m_RunManipSeriesButtonPtr.get()->setEnabled(enable);
   m_LoadManipActionButtonPtr.get()->setEnabled(enable);
}

//------------------------------------------------------------------------------
// This method updates the Action combo box to reflect the current action being executed in the series
//------------------------------------------------------------------------------
void exampleWidget::updateManipAction
   (
   EcU32 index
   )
{
   blockSignals(EcTrue);
   m_ManipActionComboBoxPtr.get()->blockSignals(EcTrue);
   m_ManipActionComboBoxPtr.get()->setCurrentIndex(index);
   m_ManipActionComboBoxPtr.get()->blockSignals(EcFalse);
   blockSignals(EcFalse);
}

//------------------------------------------------------------------------------
// This method is called when "Add Your Code" button is clicked
//------------------------------------------------------------------------------
void exampleWidget::onRunProgramCode
   (
   )
{
	// Automagically call PylonInitialize and PylonTerminate to ensure the pylon runtime system
	// is initialized during the lifetime of this object.
	Pylon::PylonAutoInitTerm autoInitTerm;

	try
	{
		// Only look for cameras supported by Camera_t. 
		CDeviceInfo info;
		info.SetDeviceClass(Camera_t::DeviceClass());

		// Create an instant camera object with the first found camera device that matches the specified device class.
		Camera_t camera(CTlFactory::GetInstance().CreateFirstDevice(info));

		QMessageBox::information(this, tr("Info"), tr(camera.GetDeviceInfo().GetModelName()));

		// Open the camera to allow parameter changes
		camera.Open();

		// Turn test image off.
		camera.TestImageSelector = TestImageSelector_Off;
		// This smart pointer will receive the grab result data.
		CGrabResultPtr ptrGrabResult;
		String_t filename;
		QMessageBox::information(this, tr("Info"), tr("Put on calibration card, image for calibration card"));

		AutoGainContinuous(camera);
		AutoExposureContinuous(camera);
		
		// Carry out white balance using the balance white auto function.
		AutoWhiteBalance(camera);

		// Carry out luminance control by using the "once" gain auto function.
		AutoGainOnce(camera);
		// Carry out luminance control by using the "once" exposure auto function.
		AutoExposureOnce(camera);
		// Carry out white balance using the balance white auto function.
		AutoWhiteBalance(camera);
		bool ok;
		int numSample = QInputDialog::getInt(this, tr("Number of Sample"), tr("input the number of sample:"), 25, 0, 1000, 1, &ok);
		int numSession = QInputDialog::getInt(this, tr("Number of Session"), tr("input the number of session:"), 25, 0, 100, 1, &ok);

		char *buffer= new char[500];
		int n = sprintf(buffer, "C:\\RunningSample\\Sample%s_%s_parameters.txt", setWitThree(numSample).c_str(), setWitTwo(numSession).c_str());
		//int n = sprintf(buffer, "C:\\RunningSample\\Sampleparameters.txt");
		string fileNa = buffer;
		std::ofstream file(fileNa.c_str(), std::ios::out | std::ios::trunc);
		file << "Gain: " << camera.Gain.GetValue() << endl;

		camera.BalanceRatioSelector.SetValue(BalanceRatioSelector_Red);
		file << "R = " << camera.BalanceRatio.GetValue() << "   ";
		camera.BalanceRatioSelector.SetValue(BalanceRatioSelector_Green);
		file << "G = " << camera.BalanceRatio.GetValue() << "   ";
		camera.BalanceRatioSelector.SetValue(BalanceRatioSelector_Blue);
		file << "B = " << camera.BalanceRatio.GetValue() << endl;

		file << "Exposure: " << camera.ExposureTime.GetValue() << " us" << endl << endl;

		file.close();

		// Get the camera's nodemap to adjust parameters via string
		GenApi::INodeMap& nodemap = camera.GetNodeMap();

		// Get any necessary camera nodes
		GenApi::CEnumerationPtr triggerSelector(nodemap.GetNode("TriggerSelector"));
		GenApi::CEnumerationPtr triggerSource(nodemap.GetNode("TriggerSource"));
		GenApi::CEnumerationPtr triggerMode(nodemap.GetNode("TriggerMode"));

		// Set the camera up for software triggering
		triggerSelector->FromString("FrameStart");
		triggerMode->FromString("On");
		triggerSource->FromString("Software");
		

		//QMessageBox::information(this, tr("Info"), tr("Triggers Set"));

		

		const QStringList manipActionList = m_pPlugin->getManipActionList();
		int numActions = (int)manipActionList.size();
		int exp = camera.ExposureTime.GetValue();


		//Image for calibration card
		// Set up the stream grabber to start acquisition
		camera.StartGrabbing();

		// Execute a trigger so the camera can acquire
		camera.ExecuteSoftwareTrigger();

		// Wait for an image and then retrieve it. A timeout of 5000ms is used
		camera.RetrieveResult(15000, ptrGrabResult, TimeoutHandling_ThrowException);

		// Image grabbed successfully?
		if (ptrGrabResult->GrabSucceeded())
		{
			//QMessageBox::information(this, tr("Info"), tr("Grab Succesfull"));

			/* The pylon grab result smart pointer classes provide a cast operator to the IImage
			// interface. This makes it possible to pass a grab result directly to the
			// function that saves an image to disk*/
			n = sprintf(buffer, "C:\\RunningSample\\sample%s_%s_calibrationcard_n.bmp", setWitThree(numSample).c_str(), setWitTwo(numSession).c_str());
			/**********************************************************/

			//QMessageBox::information(this, tr("Info"), tr(filename));
			//CImagePersistence::Save(ImageFileFormat_Bmp, filename, ptrGrabResult);
			filename = buffer;
			CImagePersistence::Save(ImageFileFormat_Bmp, filename, ptrGrabResult);
		}
		else
		{
			QMessageBox::information(this, tr("Info"), tr("Error: " + ptrGrabResult->GetErrorCode() + ' ' + ptrGrabResult->GetErrorDescription()));
		}

		// end the command for the stream grabber to stop acquisition
		camera.StopGrabbing();

		Sleep(200);


		//High exposure image
		camera.ExposureTime.SetValue(exp * 2);
		// Set up the stream grabber to start acquisition
		camera.StartGrabbing();

		// Execute a trigger so the camera can acquire
		camera.ExecuteSoftwareTrigger();

		// Wait for an image and then retrieve it. A timeout of 5000ms is used
		camera.RetrieveResult(15000, ptrGrabResult, TimeoutHandling_ThrowException);

		// Image grabbed successfully?
		if (ptrGrabResult->GrabSucceeded())
		{
			//QMessageBox::information(this, tr("Info"), tr("Grab Succesfull"));

			/* The pylon grab result smart pointer classes provide a cast operator to the IImage
			// interface. This makes it possible to pass a grab result directly to the
			// function that saves an image to disk*/
			n = sprintf(buffer, "C:\\RunningSample\\sample%s_%s_calibrationcard_h.bmp", setWitThree(numSample).c_str(), setWitTwo(numSession).c_str());
			/**********************************************************/

			//QMessageBox::information(this, tr("Info"), tr(filename));
			//CImagePersistence::Save(ImageFileFormat_Bmp, filename, ptrGrabResult);
			filename = buffer;
			CImagePersistence::Save(ImageFileFormat_Bmp, filename, ptrGrabResult);
		}
		else
		{
			QMessageBox::information(this, tr("Info"), tr("Error: " + ptrGrabResult->GetErrorCode() + ' ' + ptrGrabResult->GetErrorDescription()));
		}

		// end the command for the stream grabber to stop acquisition
		camera.StopGrabbing();

		Sleep(200);

		//low exposure image
		camera.ExposureTime.SetValue(exp / 2);
		// Set up the stream grabber to start acquisition
		camera.StartGrabbing();

		// Execute a trigger so the camera can acquire
		camera.ExecuteSoftwareTrigger();

		// Wait for an image and then retrieve it. A timeout of 5000ms is used
		camera.RetrieveResult(15000, ptrGrabResult, TimeoutHandling_ThrowException);

		// Image grabbed successfully?
		if (ptrGrabResult->GrabSucceeded())
		{
			//QMessageBox::information(this, tr("Info"), tr("Grab Succesfull"));

			/* The pylon grab result smart pointer classes provide a cast operator to the IImage
			// interface. This makes it possible to pass a grab result directly to the
			// function that saves an image to disk*/
			n = sprintf(buffer, "C:\\RunningSample\\sample%s_%s_calibrationcard_l.bmp", setWitThree(numSample).c_str(), setWitTwo(numSession).c_str());
			/**********************************************************/

			//QMessageBox::information(this, tr("Info"), tr(filename));
			//CImagePersistence::Save(ImageFileFormat_Bmp, filename, ptrGrabResult);
			filename = buffer;
			CImagePersistence::Save(ImageFileFormat_Bmp, filename, ptrGrabResult);
		}
		else
		{
			QMessageBox::information(this, tr("Info"), tr("Error: " + ptrGrabResult->GetErrorCode() + ' ' + ptrGrabResult->GetErrorDescription()));
		}

		// end the command for the stream grabber to stop acquisition
		camera.StopGrabbing();

		camera.ExposureTime.SetValue(exp);

		QMessageBox::information(this, tr("Info"), tr("Image for object, take away calibration card"));
		for (int ii = 0; ii < numActions; ++ii)
		{
			Sleep(2000);
			m_pPlugin->runManipulationAction(manipActionList[ii].toStdString());
			Sleep(2500);
			//Wait for arm to settle
			//Sleep(3000);
			if (ii == 1||ii==8||ii==10||ii==14||ii==16||ii==17||ii==18||ii==20)
			{
				QMessageBox::information(this, tr("Info"), tr("In position"));
			}


			if (ii == 19)
			{
				QMessageBox::information(this, tr("Info"), tr("Image for the ball, put on the ball"));
				for (int i = 0; i < 40; i++)
				{
					// Set up the stream grabber to start acquisition
					camera.StartGrabbing();
					// Execute a trigger so the camera can acquire
					camera.ExecuteSoftwareTrigger();

					// Wait for an image and then retrieve it. A timeout of 5000ms is used
					camera.RetrieveResult(5000, ptrGrabResult, TimeoutHandling_ThrowException);
					//camera.GrabOne(5000, ptrGrabResult);
						Pylon::DisplayImage(1, ptrGrabResult);
					++n;

					//For demonstration purposes only. Wait until the image is shown.
						::Sleep(200);
						// end the command for the stream grabber to stop acquisition
						camera.StopGrabbing();
				}
				QMessageBox::information(this, tr("Info"), tr("Image for the ball"));
			}
			
			// Set up the stream grabber to start acquisition
			camera.StartGrabbing();

			// Execute a trigger so the camera can acquire
			camera.ExecuteSoftwareTrigger();

			// Wait for an image and then retrieve it. A timeout of 5000ms is used
			camera.RetrieveResult(15000, ptrGrabResult, TimeoutHandling_ThrowException);

			// Image grabbed successfully?
			if (ptrGrabResult->GrabSucceeded())
			{
				//QMessageBox::information(this, tr("Info"), tr("Grab Succesfull"));

				/* The pylon grab result smart pointer classes provide a cast operator to the IImage
				// interface. This makes it possible to pass a grab result directly to the
				// function that saves an image to disk*/
				n = sprintf(buffer, "C:\\RunningSample\\sample%s_%s_i%sn.bmp", setWitThree(numSample).c_str(), setWitTwo(numSession).c_str(), setWitTwo(ii).c_str());

				if (ii == 19)
				{
					n = sprintf(buffer, "C:\\RunningSample\\sample%s_%s_ball1n.bmp", setWitThree(numSample).c_str(), setWitTwo(numSession).c_str());
				}
				if (ii == 20)
				{
					n = sprintf(buffer, "C:\\RunningSample\\sample%s_%s_ball2n.bmp", setWitThree(numSample).c_str(), setWitTwo(numSession).c_str());
				}
				/**********************************************************/

				//QMessageBox::information(this, tr("Info"), tr(filename));
				//CImagePersistence::Save(ImageFileFormat_Bmp, filename, ptrGrabResult);
				filename = buffer;
				CImagePersistence::Save(ImageFileFormat_Bmp, filename, ptrGrabResult);
			}
			else
			{
				QMessageBox::information(this, tr("Info"), tr("Error: " + ptrGrabResult->GetErrorCode() + ' ' + ptrGrabResult->GetErrorDescription()));
			}

			// end the command for the stream grabber to stop acquisition
			camera.StopGrabbing();

			Sleep(200);


			//High exposure image
			camera.ExposureTime.SetValue(exp*2);
			// Set up the stream grabber to start acquisition
			camera.StartGrabbing();

			// Execute a trigger so the camera can acquire
			camera.ExecuteSoftwareTrigger();

			// Wait for an image and then retrieve it. A timeout of 5000ms is used
			camera.RetrieveResult(15000, ptrGrabResult, TimeoutHandling_ThrowException);

			// Image grabbed successfully?
			if (ptrGrabResult->GrabSucceeded())
			{
				//QMessageBox::information(this, tr("Info"), tr("Grab Succesfull"));

				/* The pylon grab result smart pointer classes provide a cast operator to the IImage
				// interface. This makes it possible to pass a grab result directly to the
				// function that saves an image to disk*/
				n = sprintf(buffer, "C:\\RunningSample\\sample%s_%s_i%sh.bmp", setWitThree(numSample).c_str(), setWitTwo(numSession).c_str(), setWitTwo(ii).c_str());
				/**********************************************************/
				if (ii == 19)
				{
					n = sprintf(buffer, "C:\\RunningSample\\sample%s_%s_ball1h.bmp", setWitThree(numSample).c_str(), setWitTwo(numSession).c_str());
				}
				if (ii == 20)
				{
					n = sprintf(buffer, "C:\\RunningSample\\sample%s_%s_ball2h.bmp", setWitThree(numSample).c_str(), setWitTwo(numSession).c_str());
				}
				//QMessageBox::information(this, tr("Info"), tr(filename));
				//CImagePersistence::Save(ImageFileFormat_Bmp, filename, ptrGrabResult);
				filename = buffer;
				CImagePersistence::Save(ImageFileFormat_Bmp, filename, ptrGrabResult);
			}
			else
			{
				QMessageBox::information(this, tr("Info"), tr("Error: " + ptrGrabResult->GetErrorCode() + ' ' + ptrGrabResult->GetErrorDescription()));
			}

			// end the command for the stream grabber to stop acquisition
			camera.StopGrabbing();

			Sleep(200);

			//low exposure image
			camera.ExposureTime.SetValue(exp / 2);
			// Set up the stream grabber to start acquisition
			camera.StartGrabbing();

			// Execute a trigger so the camera can acquire
			camera.ExecuteSoftwareTrigger();

			// Wait for an image and then retrieve it. A timeout of 5000ms is used
			camera.RetrieveResult(15000, ptrGrabResult, TimeoutHandling_ThrowException);

			// Image grabbed successfully?
			if (ptrGrabResult->GrabSucceeded())
			{
				//QMessageBox::information(this, tr("Info"), tr("Grab Succesfull"));

				/* The pylon grab result smart pointer classes provide a cast operator to the IImage
				// interface. This makes it possible to pass a grab result directly to the
				// function that saves an image to disk*/
				n = sprintf(buffer, "C:\\RunningSample\\sample%s_%s_i%sl.bmp", setWitThree(numSample).c_str(), setWitTwo(numSession).c_str(), setWitTwo(ii).c_str());
				/**********************************************************/
				if (ii == 19)
				{
					n = sprintf(buffer, "C:\\RunningSample\\sample%s_%s_ball1l.bmp", setWitThree(numSample).c_str(), setWitTwo(numSession).c_str());
				}
				if (ii == 20)
				{
					n = sprintf(buffer, "C:\\RunningSample\\sample%s_%s_ball2l.bmp", setWitThree(numSample).c_str(), setWitTwo(numSession).c_str());
				}
				//QMessageBox::information(this, tr("Info"), tr(filename));
				//CImagePersistence::Save(ImageFileFormat_Bmp, filename, ptrGrabResult);
				filename = buffer;
				CImagePersistence::Save(ImageFileFormat_Bmp, filename, ptrGrabResult);
			}
			else
			{
				QMessageBox::information(this, tr("Info"), tr("Error: " + ptrGrabResult->GetErrorCode() + ' ' + ptrGrabResult->GetErrorDescription()));
			}

			// end the command for the stream grabber to stop acquisition
			camera.StopGrabbing();

			camera.ExposureTime.SetValue(exp);
		}

		camera.TriggerMode.SetValue(TriggerMode_Off);
		camera.Close();
		QMessageBox::information(this, tr("Info"), tr("Sample Completed"));


	}
	catch (GenICam::GenericException &e) {
		QMessageBox::information(this, tr("Info"), tr(e.GetDescription()));
	}
	
   
}

void AutoGainOnce(Camera_t& camera)
{
	// Check whether the gain auto function is available.
	if (!IsWritable(camera.GainAuto))
	{
		cout << "The camera does not support Gain Auto." << endl << endl;
		return;
	}

	// Maximize the grabbed image area of interest (Image AOI).
	if (IsWritable(camera.OffsetX))
	{
		camera.OffsetX.SetValue(camera.OffsetX.GetMin());
	}
	if (IsWritable(camera.OffsetY))
	{
		camera.OffsetY.SetValue(camera.OffsetY.GetMin());
	}
	camera.Width.SetValue(camera.Width.GetMax());
	camera.Height.SetValue(camera.Height.GetMax());

	// Set the Auto Function AOI for luminance statistics.
	// Currently, AutoFunctionAOISelector_AOI1 is predefined to gather
	// luminance statistics.
	camera.AutoFunctionAOISelector.SetValue(AutoFunctionAOISelector_AOI1);
	camera.AutoFunctionAOIOffsetX.SetValue(0);
	camera.AutoFunctionAOIOffsetY.SetValue(0);
	camera.AutoFunctionAOIWidth.SetValue(camera.Width.GetMax());
	camera.AutoFunctionAOIHeight.SetValue(camera.Height.GetMax());

	// Set the target value for luminance control.
	// A value of 0.3 means that the target brightness is 30 % of the maximum brightness of the raw pixel value read out from the sensor.
	// A value of 0.4 means 40 % and so forth.
	camera.AutoTargetBrightness.SetValue(0.3);

	// We are going to try GainAuto = Once.

	//cout << "Trying 'GainAuto = Once'." << endl;
	//cout << "Initial Gain = " << camera.Gain.GetValue() << endl;

	// Set the gain ranges for luminance control.
	camera.AutoGainLowerLimit.SetValue(camera.Gain.GetMin());
	camera.AutoGainUpperLimit.SetValue(camera.Gain.GetMax());

	camera.GainAuto.SetValue(GainAuto_Once);

	// When the "once" mode of operation is selected, 
	// the parameter values are automatically adjusted until the related image property
	// reaches the target value. After the automatic parameter value adjustment is complete, the auto
	// function will automatically be set to "off" and the new parameter value will be applied to the
	// subsequently grabbed images.

	int n = 0;
	while (camera.GainAuto.GetValue() != GainAuto_Off)
	{
		GrabResultPtr_t ptrGrabResult;
		camera.GrabOne(5000, ptrGrabResult);
	//	Pylon::DisplayImage(1, ptrGrabResult);
		++n;
		//For demonstration purposes only. Wait until the image is shown.
	//	::Sleep(100);
	}

	//cout << "GainAuto went back to 'Off' after " << n << " frames." << endl;
	//cout << "Final Gain = " << camera.Gain.GetValue() << endl << endl;
}

void AutoExposureOnce(Camera_t& camera)
{
	// Check whether auto exposure is available
	if (!IsWritable(camera.ExposureAuto))
	{
		cout << "The camera does not support Exposure Auto." << endl << endl;
		return;
	}

	// Maximize the grabbed area of interest (Image AOI).
	if (IsWritable(camera.OffsetX))
	{
		camera.OffsetX.SetValue(camera.OffsetX.GetMin());
	}
	if (IsWritable(camera.OffsetY))
	{
		camera.OffsetY.SetValue(camera.OffsetY.GetMin());
	}
	camera.Width.SetValue(camera.Width.GetMax());
	camera.Height.SetValue(camera.Height.GetMax());

	// Set the Auto Function AOI for luminance statistics.
	// Currently, AutoFunctionAOISelector_AOI1 is predefined to gather
	// luminance statistics.
	camera.AutoFunctionAOISelector.SetValue(AutoFunctionAOISelector_AOI1);
	camera.AutoFunctionAOIOffsetX.SetValue(0);
	camera.AutoFunctionAOIOffsetY.SetValue(0);
	camera.AutoFunctionAOIWidth.SetValue(camera.Width.GetMax());
	camera.AutoFunctionAOIHeight.SetValue(camera.Height.GetMax());

	// Set the target value for luminance control.
	// A value of 0.3 means that the target brightness is 30 % of the maximum brightness of the raw pixel value read out from the sensor.
	// A value of 0.4 means 40 % and so forth.
	camera.AutoTargetBrightness.SetValue(0.3);

	// Try ExposureAuto = Once.
	//cout << "Trying 'ExposureAuto = Once'." << endl;
	//cout << "Initial exposure time = ";
	//cout << camera.ExposureTime.GetValue() << " us" << endl;

	// Set the exposure time ranges for luminance control.
	camera.AutoExposureTimeLowerLimit.SetValue(camera.AutoExposureTimeLowerLimit.GetMin());
	camera.AutoExposureTimeUpperLimit.SetValue(camera.AutoExposureTimeLowerLimit.GetMax());

	camera.ExposureAuto.SetValue(ExposureAuto_Once);

	// When the "once" mode of operation is selected, 
	// the parameter values are automatically adjusted until the related image property
	// reaches the target value. After the automatic parameter value adjustment is complete, the auto
	// function will automatically be set to "off", and the new parameter value will be applied to the
	// subsequently grabbed images.
	int n = 0;
	while (camera.ExposureAuto.GetValue() != ExposureAuto_Off)
	{
		GrabResultPtr_t ptrGrabResult;
		camera.GrabOne(5000, ptrGrabResult);
	//	Pylon::DisplayImage(1, ptrGrabResult);
		++n;

		//For demonstration purposes only. Wait until the image is shown.
	//	::Sleep(100);
	}
//	cout << "ExposureAuto went back to 'Off' after " << n << " frames." << endl;
	//cout << "Final exposure time = ";
	//cout << camera.ExposureTime.GetValue() << " us" << endl << endl;
}

void AutoWhiteBalance(Camera_t& camera)
{
	// Check whether the Balance White Auto feature is available.
	if (!IsWritable(camera.BalanceWhiteAuto))
	{
		cout << "The camera does not support Balance White Auto." << endl << endl;
		return;
	}

	// Maximize the grabbed area of interest (Image AOI).
	if (IsWritable(camera.OffsetX))
	{
		camera.OffsetX.SetValue(camera.OffsetX.GetMin());
	}
	if (IsWritable(camera.OffsetY))
	{
		camera.OffsetY.SetValue(camera.OffsetY.GetMin());
	}
	camera.Width.SetValue(camera.Width.GetMax());
	camera.Height.SetValue(camera.Height.GetMax());

	// Set the Auto Function AOI for white balance statistics.
	// Currently, AutoFunctionAOISelector_AOI2 is predefined to gather
	// white balance statistics.
	camera.AutoFunctionAOISelector.SetValue(AutoFunctionAOISelector_AOI2);
	camera.AutoFunctionAOIOffsetX.SetValue(0);
	camera.AutoFunctionAOIOffsetY.SetValue(0);
	camera.AutoFunctionAOIWidth.SetValue(camera.Width.GetMax());
	camera.AutoFunctionAOIHeight.SetValue(camera.Height.GetMax());

	//cout << "Trying 'BalanceWhiteAuto = Once'." << endl;
	//cout << "Initial balance ratio: ";
	camera.BalanceRatioSelector.SetValue(BalanceRatioSelector_Red);
	//cout << "R = " << camera.BalanceRatio.GetValue() << "   ";
	camera.BalanceRatioSelector.SetValue(BalanceRatioSelector_Green);
//	cout << "G = " << camera.BalanceRatio.GetValue() << "   ";
	camera.BalanceRatioSelector.SetValue(BalanceRatioSelector_Blue);
	//cout << "B = " << camera.BalanceRatio.GetValue() << endl;

	camera.BalanceWhiteAuto.SetValue(BalanceWhiteAuto_Once);

	// When the "once" mode of operation is selected, 
	// the parameter values are automatically adjusted until the related image property
	// reaches the target value. After the automatic parameter value adjustment is complete, the auto
	// function will automatically be set to "off" and the new parameter value will be applied to the
	// subsequently grabbed images.
	int n = 0;
	while (camera.BalanceWhiteAuto.GetValue() != BalanceWhiteAuto_Off)
	{
		GrabResultPtr_t ptrGrabResult;
		camera.GrabOne(5000, ptrGrabResult);
	//	Pylon::DisplayImage(1, ptrGrabResult);
		++n;

		//For demonstration purposes only. Wait until the image is shown.
	//	::Sleep(100);
	}
	//cout << "BalanceWhiteAuto went back to 'Off' after ";
	//cout << n << " frames." << endl;
	//cout << "Final balance ratio: ";
	camera.BalanceRatioSelector.SetValue(BalanceRatioSelector_Red);
	//cout << "R = " << camera.BalanceRatio.GetValue() << "   ";
	camera.BalanceRatioSelector.SetValue(BalanceRatioSelector_Green);
	//cout << "G = " << camera.BalanceRatio.GetValue() << "   ";
	camera.BalanceRatioSelector.SetValue(BalanceRatioSelector_Blue);
	//cout << "B = " << camera.BalanceRatio.GetValue() << endl;
}

void AutoGainContinuous(Camera_t& camera)
{
	// Check whether the Gain Auto feature is available.
	if (!IsWritable(camera.GainAuto))
	{
		cout << "The camera does not support Gain Auto." << endl << endl;
		return;
	}

	// Maximize the grabbed image area of interest (Image AOI).
	if (IsWritable(camera.OffsetX))
	{
		camera.OffsetX.SetValue(camera.OffsetX.GetMin());
	}
	if (IsWritable(camera.OffsetY))
	{
		camera.OffsetY.SetValue(camera.OffsetY.GetMin());
	}
	camera.Width.SetValue(camera.Width.GetMax());
	camera.Height.SetValue(camera.Height.GetMax());

	// Set the Auto Function AOI for luminance statistics.
	// Currently, AutoFunctionAOISelector_AOI1 is predefined to gather
	// luminance statistics.
	camera.AutoFunctionAOISelector.SetValue(AutoFunctionAOISelector_AOI1);
	camera.AutoFunctionAOIOffsetX.SetValue(0);
	camera.AutoFunctionAOIOffsetY.SetValue(0);
	camera.AutoFunctionAOIWidth.SetValue(camera.Width.GetMax());
	camera.AutoFunctionAOIHeight.SetValue(camera.Height.GetMax());

	// Set the target value for luminance control.
	// A value of 0.3 means that the target brightness is 30 % of the maximum brightness of the raw pixel value read out from the sensor.
	// A value of 0.4 means 40 % and so forth.
	camera.AutoTargetBrightness.SetValue(0.3);

	// We are trying GainAuto = Continuous.
	//cout << "Trying 'GainAuto = Continuous'." << endl;
	//cout << "Initial Gain = " << camera.Gain.GetValue() << endl;

	camera.GainAuto.SetValue(GainAuto_Continuous);

	// When "continuous" mode is selected, the parameter value is adjusted repeatedly while images are acquired.
	// Depending on the current frame rate, the automatic adjustments will usually be carried out for
	// every or every other image unless the camera’s micro controller is kept busy by other tasks.
	// The repeated automatic adjustment will proceed until the "once" mode of operation is used or
	// until the auto function is set to "off", in which case the parameter value resulting from the latest
	// automatic adjustment will operate unless the value is manually adjusted.
	for (int n = 0; n < 20; n++)            // For demonstration purposes, we will grab "only" 20 images.
	{
		GrabResultPtr_t ptrGrabResult;
		camera.GrabOne(5000, ptrGrabResult);
	//	Pylon::DisplayImage(1, ptrGrabResult);

		//For demonstration purposes only. Wait until the image is shown.
	//	::Sleep(100);
	}
	//camera.GainAuto.SetValue(GainAuto_Off); // Switch off GainAuto.

	//cout << "Final Gain = " << camera.Gain.GetValue() << endl << endl;
}

void AutoExposureContinuous(Camera_t& camera)
{
	// Check whether the Exposure Auto feature is available.
	if (!IsWritable(camera.ExposureAuto))
	{
		cout << "The camera does not support Exposure Auto." << endl << endl;
		return;
	}

	// Maximize the grabbed area of interest (Image AOI).
	if (IsWritable(camera.OffsetX))
	{
		camera.OffsetX.SetValue(camera.OffsetX.GetMin());
	}
	if (IsWritable(camera.OffsetY))
	{
		camera.OffsetY.SetValue(camera.OffsetY.GetMin());
	}
	camera.Width.SetValue(camera.Width.GetMax());
	camera.Height.SetValue(camera.Height.GetMax());

	// Set the Auto Function AOI for luminance statistics.
	// Currently, AutoFunctionAOISelector_AOI1 is predefined to gather
	// luminance statistics.
	camera.AutoFunctionAOISelector.SetValue(AutoFunctionAOISelector_AOI1);
	camera.AutoFunctionAOIOffsetX.SetValue(0);
	camera.AutoFunctionAOIOffsetY.SetValue(0);
	camera.AutoFunctionAOIWidth.SetValue(camera.Width.GetMax());
	camera.AutoFunctionAOIHeight.SetValue(camera.Height.GetMax());

	// Set the target value for luminance control.
	// A value of 0.3 means that the target brightness is 30 % of the maximum brightness of the raw pixel value read out from the sensor.
	// A value of 0.4 means 40 % and so forth.
	camera.AutoTargetBrightness.SetValue(0.3);

//	cout << "ExposureAuto 'GainAuto = Continuous'." << endl;
//	cout << "Initial exposure time = ";
//	cout << camera.ExposureTime.GetValue() << " us" << endl;

	camera.ExposureAuto.SetValue(ExposureAuto_Continuous);

	// When "continuous" mode is selected, the parameter value is adjusted repeatedly while images are acquired.
	// Depending on the current frame rate, the automatic adjustments will usually be carried out for
	// every or every other image, unless the camera’s microcontroller is kept busy by other tasks.
	// The repeated automatic adjustment will proceed until the "once" mode of operation is used or
	// until the auto function is set to "off", in which case the parameter value resulting from the latest
	// automatic adjustment will operate unless the value is manually adjusted.
	for (int n = 0; n < 20; n++)    // For demonstration purposes, we will use only 20 images.
	{
		GrabResultPtr_t ptrGrabResult;
		camera.GrabOne(5000, ptrGrabResult);
	//	Pylon::DisplayImage(1, ptrGrabResult);

		//For demonstration purposes only. Wait until the image is shown.
	//	::Sleep(100);
	}
	//camera.ExposureAuto.SetValue(ExposureAuto_Off); // Switch off Exposure Auto.

	//cout << "Final exposure time = ";
	//cout << camera.ExposureTime.GetValue() << " us" << endl << endl;
}