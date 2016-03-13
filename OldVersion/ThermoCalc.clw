; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CThermoCalcView
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "thermocalc.h"
LastPage=0

ClassCount=15
Class1=CButtonChem
Class2=CChildFrame
Class3=DlgEmptyDoc
Class4=CRichEditCtrlChem
Class5=CFrameOutput
Class6=CMainFrame
Class7=CPropertyPageChem
Class8=CPropertySheetChem
Class9=CRichEditCtrlEnter
Class10=CStaticChem
Class11=CThermoCalcApp
Class12=CAboutDlg
Class13=CThermoCalcDoc
Class14=CThermoCalcView
Class15=CViewOutput

ResourceCount=12
Resource1=IDD_ABOUTBOX
Resource2=IDD_ForCalcFlow_IS
Resource3=IDD_ForCalcFlow_V
Resource4=IDD_ForCalcFlow_EPS
Resource5=IDD_THERMOCALC_FORM (RES2)
Resource6=IDD_THERMOCALC_FORM
Resource7=IDD_ForCalcFlow_Pa
Resource8=IDR_OUTPUT
Resource9=IDR_INPUTDLG
Resource10=IDD_PP_TAMP1
Resource11=IDD_ForCalcFlow_F
Resource12=IDR_MAINFRAME (English (U.S.))

[CLS:CButtonChem]
Type=0
BaseClass=CButton
HeaderFile=ButtonChem.h
ImplementationFile=ButtonChem.cpp

[CLS:CChildFrame]
Type=0
BaseClass=CMDIChildWnd
HeaderFile=ChildFrm.h
ImplementationFile=ChildFrm.cpp

[CLS:DlgEmptyDoc]
Type=0
BaseClass=CDocument
HeaderFile=DlgEmptyDoc.h
ImplementationFile=DlgEmptyDoc.cpp

[CLS:CRichEditCtrlChem]
Type=0
BaseClass=CRichEditCtrl
HeaderFile=fieldinput.h
ImplementationFile=FieldInput.cpp

[CLS:CFrameOutput]
Type=0
BaseClass=CMDIChildWnd
HeaderFile=FrameOutput.h
ImplementationFile=FrameOutput.cpp

[CLS:CMainFrame]
Type=0
BaseClass=CMDIFrameWnd
HeaderFile=MainFrm.h
ImplementationFile=MainFrm.cpp

[CLS:CPropertyPageChem]
Type=0
BaseClass=CPropertyPage
HeaderFile=PropertyPageChem.h
ImplementationFile=PropertyPageChem.cpp

[CLS:CPropertySheetChem]
Type=0
BaseClass=CPropertySheet
HeaderFile=PropertySheetChem.h
ImplementationFile=PropertySheetChem.cpp

[CLS:CRichEditCtrlEnter]
Type=0
BaseClass=CRichEditCtrl
HeaderFile=RichEditCtrlEnter.h
ImplementationFile=RichEditCtrlEnter.cpp

[CLS:CStaticChem]
Type=0
BaseClass=CStatic
HeaderFile=StaticChem.h
ImplementationFile=StaticChem.cpp

[CLS:CThermoCalcApp]
Type=0
BaseClass=CWinApp
HeaderFile=ThermoCalc.h
ImplementationFile=ThermoCalc.cpp

[CLS:CAboutDlg]
Type=0
BaseClass=CDialog
HeaderFile=ThermoCalc.cpp
ImplementationFile=ThermoCalc.cpp
LastObject=ID_FILE_OPEN_IN_EXCEL

[CLS:CThermoCalcDoc]
Type=0
BaseClass=CDocument
HeaderFile=ThermoCalcDoc.h
ImplementationFile=ThermoCalcDoc.cpp
LastObject=CThermoCalcDoc

[CLS:CThermoCalcView]
Type=0
BaseClass=CFormView
HeaderFile=ThermoCalcView.h
ImplementationFile=ThermoCalcView.cpp
LastObject=ID_VIEW_PARTIALPRESSURES
Filter=D

[CLS:CViewOutput]
Type=0
BaseClass=CRichEditView
HeaderFile=ViewOutput.h
ImplementationFile=ViewOutput.cpp
LastObject=CViewOutput

[DLG:IDD_ABOUTBOX]
Type=1
Class=CPropertyPageChem
ControlCount=5
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889
Control5=IDC_STATIC,static,1342308352

[DLG:IDD_THERMOCALC_FORM]
Type=1
Class=CThermoCalcView
ControlCount=45
Control1=65535,button,1342177287
Control2=65535,button,1342177287
Control3=IDC_BUTTON1,button,1342242817
Control4=IDC_BUTTON2,button,1342242816
Control5=65535,static,1342308354
Control6=65535,static,1342308354
Control7=65535,static,1342308352
Control8=65535,static,1342308354
Control9=65535,static,1342308352
Control10=65535,static,1342308353
Control11=IDC_CHECK_CALC_UXF,button,1342188803
Control12=IDC_ButtonAppendAlfa,button,1342177280
Control13=IDC_ButtonDisAlfa,button,1342177280
Control14=IDC_ButtonAppendPressure,button,1342177280
Control15=IDC_ButtonDisPressure,button,1342177280
Control16=65535,button,1342177287
Control17=65535,static,1342308352
Control18=65535,static,1342308352
Control19=65535,static,1342308353
Control20=65535,button,1342177287
Control21=65535,button,1342177287
Control22=65535,button,1342177287
Control23=IDC_Temperature,RICHEDIT,1350566016
Control24=65535,button,1342177799
Control25=IDC_BUTTON_CALC_UXF,button,1342185472
Control26=IDC_ButtonAppendFormulO,button,1342177280
Control27=IDC_ButtonDisFormulO,button,1342177280
Control28=IDC_ButtonAppendFormulC,button,1342177280
Control29=IDC_ButtonDisFormulC,button,1342177280
Control30=IDC_CHECK_CRITIC_SECTION,button,1342188835
Control31=65535,button,1342177287
Control32=65535,static,1342308354
Control33=IDC_TemperatureMax,RICHEDIT,1350631552
Control34=65535,static,1342308354
Control35=IDC_TemperatureMin,RICHEDIT,1350631552
Control36=IDC_RADIO1,button,1342177289
Control37=IDC_RADIO2,button,1342177289
Control38=IDC_CHECK_INCLUDE_CC,button,1342188835
Control39=65535,button,1342177287
Control40=IDC_CHECK_ATMOSPHERE_SECTION,button,1342188803
Control41=65535,button,1342177287
Control42=65535,static,1073872898
Control43=65535,button,1073742343
Control44=IDC_ButtonAppendAlfa2,button,1073741824
Control45=IDC_ButtonDisAlfa2,button,1073741824

[MNU:IDR_OUTPUT]
Type=1
Class=CViewOutput
Command1=ID_FILE_OPEN
Command2=ID_FILE_CLOSE
Command3=ID_FILE_SAVE
Command4=ID_FILE_SAVE_AS
Command5=ID_FILE_SAVE_TEXT
Command6=ID_FILE_OPEN_IN_EXCEL
Command7=ID_FILE_PRINT
Command8=ID_FILE_PRINT_PREVIEW
Command9=ID_FILE_PRINT_SETUP
Command10=ID_FILE_MRU_FILE1
Command11=ID_APP_EXIT
Command12=ID_EDIT_UNDO
Command13=ID_EDIT_CUT
Command14=ID_EDIT_COPY
Command15=ID_EDIT_PASTE
Command16=ID_VIEW_STATUS_BAR
Command17=ID_OUTPUT_FULL
Command18=ID_OUTPUT_SHORT
Command19=ID_VIEW_PRECISION
Command20=ID_VIEW_ASTRAVIEW
Command21=ID_VIEW_EXPONENTVIEW
Command22=ID_VIEW_PARTIALPRESSURES
Command23=ID_VIEW_MOLESFRACTION
Command24=ID_VIEW
Command25=ID_EXCELOPTIONS
Command26=ID_WINDOW_NEW
Command27=ID_WINDOW_CASCADE
Command28=ID_WINDOW_TILE_HORZ
Command29=ID_WINDOW_ARRANGE
Command30=ID_APP_ABOUT
CommandCount=30

[DLG:IDD_PP_TAMP1]
Type=1
Class=?
ControlCount=1
Control1=IDC_STATIC,static,1208090624

[DLG:IDD_ForCalcFlow_F]
Type=1
Class=?
ControlCount=4
Control1=IDC_STATIC,static,1342177282
Control2=IDC_BUTTON1,button,1342242816
Control3=IDC_BUTTON2,button,1342242816
Control4=IDC_EDIT1,edit,1350639744

[DLG:IDD_ForCalcFlow_IS]
Type=1
Class=?
ControlCount=6
Control1=IDC_STATIC,static,1342177282
Control2=IDC_BUTTON1,button,1342242816
Control3=IDC_BUTTON2,button,1342242816
Control4=IDC_EDIT1,edit,1350639744
Control5=IDC_RADIO1,button,1342177289
Control6=IDC_RADIO2,button,1342177289

[DLG:IDD_ForCalcFlow_EPS]
Type=1
Class=?
ControlCount=4
Control1=IDC_STATIC,static,1342177282
Control2=IDC_BUTTON1,button,1342242816
Control3=IDC_BUTTON2,button,1342242816
Control4=IDC_EDIT1,edit,1350639744

[DLG:IDD_ForCalcFlow_V]
Type=1
Class=?
ControlCount=4
Control1=IDC_STATIC,static,1342177282
Control2=IDC_BUTTON1,button,1342242816
Control3=IDC_BUTTON2,button,1342242816
Control4=IDC_EDIT1,edit,1350639744

[DLG:IDD_THERMOCALC_FORM (RES2)]
Type=1
Class=?
ControlCount=39
Control1=IDC_STATIC,button,1342177287
Control2=IDC_STATIC,button,1342177287
Control3=IDC_BUTTON1,button,1342242817
Control4=IDC_BUTTON2,button,1342242816
Control5=IDC_STATIC,static,1342308354
Control6=IDC_STATIC,static,1342308354
Control7=IDC_STATIC,static,1342308352
Control8=IDC_STATIC,static,1342308354
Control9=IDC_STATIC,static,1342308352
Control10=IDC_STATIC,static,1342308353
Control11=IDC_CHECK_CALC_UXF,button,1342188803
Control12=IDC_ButtonAppendAlfa,button,1342177280
Control13=IDC_ButtonDisAlfa,button,1342177280
Control14=IDC_ButtonAppendPressure,button,1342177280
Control15=IDC_ButtonDisPressure,button,1342177280
Control16=IDC_STATIC,button,1342177287
Control17=IDC_STATIC,button,1342177287
Control18=IDC_BUTTON_CALCSECTION,button,1208033280
Control19=IDC_STATIC,static,1342308352
Control20=IDC_STATIC,static,1342308352
Control21=IDC_STATIC,static,1342308353
Control22=IDC_STATIC,button,1342177287
Control23=IDC_STATIC,button,1342177287
Control24=IDC_STATIC,button,1342177287
Control25=IDC_Temperature,RICHEDIT,1350631552
Control26=IDC_STATIC,button,1342177799
Control27=IDC_BUTTON_CALC_UXF,button,1342185472
Control28=IDC_ButtonAppendFormulO,button,1342177280
Control29=IDC_ButtonDisFormulO,button,1342177280
Control30=IDC_ButtonAppendFormulC,button,1342177280
Control31=IDC_ButtonDisFormulC,button,1342177280
Control32=IDC_CHECK_SECTION_PA2,button,1342188835
Control33=IDC_STATIC,button,1342177287
Control34=IDC_STATIC,static,1342308354
Control35=IDC_TemperatureMax,RICHEDIT,1350631552
Control36=IDC_STATIC,static,1342308354
Control37=IDC_TemperatureMin,RICHEDIT,1350631552
Control38=IDC_RADIO1,button,1342177289
Control39=IDC_RADIO2,button,1342177289

[DLG:IDD_ForCalcFlow_Pa]
Type=1
Class=?
ControlCount=4
Control1=IDC_STATIC,static,1342177282
Control2=IDC_BUTTON1,button,1342242816
Control3=IDC_BUTTON2,button,1342242816
Control4=IDC_EDIT1,edit,1350639744

[MNU:IDR_INPUTDLG]
Type=1
Class=?
Command1=ID_FILE_OPEN
Command2=ID_FILE_CLOSE
Command3=ID_FILE_SAVE
Command4=ID_FILE_SAVE_AS
Command5=ID_FILE_PRINT
Command6=ID_FILE_PRINT_PREVIEW
Command7=ID_FILE_PRINT_SETUP
Command8=ID_FILE_MRU_FILE1
Command9=ID_APP_EXIT
Command10=ID_EDIT_UNDO
Command11=ID_EDIT_CUT
Command12=ID_EDIT_COPY
Command13=ID_EDIT_PASTE
Command14=ID_VIEW_STATUS_BAR
Command15=ID_VIEW_PRECISION
Command16=ID_VIEW_SETUPTABSIZE
Command17=ID_SAVEOPTIONS_S
Command18=ID_LOADOPTIONS_LOADDATABASESONSTART
Command19=ID_OUTPUT_FULL
Command20=ID_OUTPUT_SHORT
Command21=ID_UNITMEASURE
Command22=ID_VIEW_PARTIALPRESSURES
Command23=ID_VIEW_MOLESFRACTION
Command24=ID_VIEW_ASTRAVIEW
Command25=ID_VIEW_EXPONENTVIEW
Command26=ID_VIEW_OTHERVIEW
Command27=ID_MINNAMESINOUTPUT
Command28=ID_WINDOW_NEW
Command29=ID_WINDOW_CASCADE
Command30=ID_WINDOW_TILE_HORZ
Command31=ID_WINDOW_ARRANGE
Command32=ID_APP_ABOUT
CommandCount=32

[MNU:IDR_MAINFRAME (English (U.S.))]
Type=1
Class=?
Command1=ID_FILE_OPEN
Command2=ID_FILE_PRINT_SETUP
Command3=ID_FILE_MRU_FILE1
Command4=ID_APP_EXIT
Command5=ID_VIEW_STATUS_BAR
Command6=ID_APP_ABOUT
CommandCount=6

[ACL:IDR_MAINFRAME (English (U.S.))]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_FILE_PRINT
Command5=ID_EDIT_UNDO
Command6=ID_EDIT_CUT
Command7=ID_EDIT_COPY
Command8=ID_EDIT_PASTE
Command9=ID_EDIT_UNDO
Command10=ID_EDIT_CUT
Command11=ID_EDIT_COPY
Command12=ID_EDIT_PASTE
Command13=ID_NEXT_PANE
Command14=ID_PREV_PANE
CommandCount=14

