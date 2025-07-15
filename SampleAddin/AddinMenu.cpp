#include "stdafx.h"
#include "AddinMenu.h"
#include "../../SampleAddin.h"
#include "../../SEAddIn.h"
//#include "System/Nex3DHelper/Nex3DHelper.h"
//#include "System/NDSFileHelper.h"
#include <tlhelp32.h>
#include "CmdUser.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


void IDispathPtr2Variant(std::vector<IDispatchPtr>& inPutArray, CComVariant& OuputVariant)
{
	if (inPutArray.size() <= 0)
		return;

	SAFEARRAY* parray = SafeArrayCreateVector(VT_VARIANT, 0, (ULONG)inPutArray.size());
	for (long i = 0; i < inPutArray.size(); i++)
	{
		SafeArrayPutElement(parray, &i, &CComVariant((IDispatch*)inPutArray[i]));
	}
	OuputVariant.vt = VT_ARRAY | VT_VARIANT;
	SafeArrayCopy(parray, &(OuputVariant.parray));
}


AddinMenu::AddinMenu()
{

}
AddinMenu::~AddinMenu()
{

}

bool AddinMenu::CallCommand(MenuFunc id)
{
	switch (id)
	{
	case idCustomPage:
	{
		return CallCustomPage();
		break;
	}
	case idCustomCommand:
	{
		return CallCustomCommand();
		break;
	}
	case idCustomDrawBox:
	{
		return CallCreatBoxCommand();
		break;
	}
	case idDraw:
	{
		return CallCreateEllipse();
		break;
	}
	case idTest1:
	{
		return Test1();
		break;
	}
	case idTest2:
	{
		return Test2();
		break;
	}
	case idTest3:
	{
		return Test3();
		break;
	}
	case idTest4:
	{
		return Test4();
		break;
	}
	case idTest5:
	{
		return Test5();
		break;
	}
	case idTest6:
	{
		return Test6();
		break;
	}
	case idTest7:
	{
		return Test7();
		break;
	}
	default:
		return false;
	}

	return false;
}

bool AddinMenu::Uninit()
{
	//	CTaskMgr::Instance()->Stop();
	//	NDSFileHelper::ClearTempFolder();
	return true;
}

bool AddinMenu::Test1()
{
	ApplicationPtr application = CSampleAddinApp::GetSEApp()->GetApplication();

	application->Documents->Open("../Documents/gear2.par");


	//MessageBox(NULL, TEXT("按钮1 !"), TEXT("错误！"), MB_OK | MB_YESNO);
	return true;
}

bool AddinMenu::Test2()
{
	SolidEdgePart::PartDocumentPtr partDocument = CSampleAddinApp::GetSEApp()->GetApplication()->GetActiveDocument();

	SolidEdgePart::ProfileSetsPtr profileSets = NULL;
	SolidEdgePart::ProfileSetPtr profileSet = NULL;
	SolidEdgePart::ProfilesPtr profiles = NULL;
	SolidEdgePart::ProfilePtr profile = NULL;
	SolidEdgePart::RefPlanesPtr refplanes = NULL;
	SolidEdgeFrameworkSupport::Relations2dPtr relations2d = NULL;
	SolidEdgeFrameworkSupport::Relation2dPtr relation2d = NULL;
	SolidEdgeFrameworkSupport::Lines2dPtr lines2d = NULL;
	SolidEdgeFrameworkSupport::Line2dPtr line2d = NULL;
	SolidEdgePart::ModelsPtr models = NULL;
	SolidEdgePart::ModelPtr model = NULL;
	//	System.Array aProfiles = null;

	// Get a reference to the profile sets collection.
	profileSets = partDocument->ProfileSets;

	// Add a new profile set.
	profileSet = profileSets->Add();

	// Get a reference to the profiles collection.
	profiles = profileSet->Profiles;

	// Get a reference to the ref planes collection.
	refplanes = partDocument->RefPlanes;

	// Add a new profile.
	profile = profiles->Add(refplanes->Item(1));

	// Get a reference to the lines2d collection.
	lines2d = profile->Lines2d;

	// UOM = meters.
	double lineMatrix[4][4] =
	{
		//{x1, y1, x2, y2}
		{ 0, 0, 0.03, 0 },
		{ 0.03, 0, 0.03, 0.03 },
		{ 0.03, 0.03, 0, 0.03 },
		{0, 0.03, 0, 0}
	};

	// Draw the Base Profile.
	for (int i = 0; i < 4; i++)
	{
		line2d = lines2d->AddBy2Points(
			lineMatrix[i][0],
			lineMatrix[i][1],
			lineMatrix[i][2],
			lineMatrix[i][3]);
	}

	// Define Relations among the Line objects to make the Profile closed.
	relations2d = (SolidEdgeFrameworkSupport::Relations2dPtr)profile->Relations2d;

	// Connect all of the lines.
	for (int i = 1; i <= lines2d->Count; i++)
	{
		int j = i + 1;

		// When we reach the last line, wrap around and connect it to the first line.
		if (j > lines2d->Count)
		{
			j = 1;
		}

		relation2d = relations2d->AddKeypoint(
			lines2d->Item(i),
			(int)SolidEdgeConstants::KeypointIndexConstants::igLineEnd,
			lines2d->Item(j),
			(int)SolidEdgeConstants::KeypointIndexConstants::igLineStart,
			true);
	}

	// Close the profile.
	profile->End(SolidEdgePart::ProfileValidationType::igProfileClosed);

	// Hide the profile.
	profile->Visible = false;

	// Create a new array of profile objects.
	//	aProfiles = Array.CreateInstance(typeof(SolidEdgePart.Profile), 1);
	//	aProfiles.SetValue(profile, 0);

	CComSafeArray<IDispatch*> aProfiles(1);
	aProfiles[0] = profile;

	// Get a reference to the models collection.
	models = partDocument->Models;

	// Create the extended protrusion.
	model = models->AddFiniteExtrudedProtrusion(
		1,
		aProfiles.GetSafeArrayPtr(),
		SolidEdgePart::FeaturePropertyConstants::igRight,
		0.02);




	//MessageBox(NULL, TEXT("按钮2 !"), TEXT("错误！"), MB_OK | MB_YESNO);
	return true;
}



bool AddinMenu::StartSketching()
{
	MessageBox(NULL, TEXT("激活鼠标 !"), TEXT("错误！"), MB_OK | MB_YESNO);
	return false;

	//// 调整视角
	//double EyeX, EyeY, EyeZ, TargetX, TargetY, TargetZ, UpX, UpY, UpZ, ScaleOrAngle;
	//VARIANT_BOOL Perspective;// 是否透视
	//static_cast<WindowPtr>(CSampleAddinApp::GetApplication()->GetWindows()->Item(1))->View->GetCamera(&EyeX, &EyeY, &EyeZ, &TargetX, &TargetY, &TargetZ, &UpX, &UpY, &UpZ, &Perspective, &ScaleOrAngle);
	//static_cast<WindowPtr>(CSampleAddinApp::GetApplication()->GetWindows()->Item(1))->View->BeginCameraDynamics();
	////-0.0459294,-0.94749,0.425348,0.261098,-0.0536459,0.0985821,0.125473,0.302334,0.944908,0,2944.59
	//static_cast<WindowPtr>(CSampleAddinApp::GetApplication()->GetWindows()->Item(1))->View->SetCamera(0, 0, 1, 0, 0, 0, 0, 1, 0, Perspective, ScaleOrAngle);
	//static_cast<WindowPtr>(CSampleAddinApp::GetApplication()->GetWindows()->Item(1))->View->PanCamera(-772, 323);
	//static_cast<WindowPtr>(CSampleAddinApp::GetApplication()->GetWindows()->Item(1))->View->EndCameraDynamics();

	//// 创建草图
	//ApplicationPtr application = CSampleAddinApp::GetSEApp()->GetApplication();
	//SolidEdgePart::PartDocumentPtr partDocument = application->GetActiveDocument();
	//if (partDocument == NULL)
	//	return false;

	//// 参考面隐藏
	//partDocument->RefPlanes->Item(1)->PutVisible(false);
	//partDocument->RefPlanes->Item(2)->PutVisible(false);
	//partDocument->RefPlanes->Item(3)->PutVisible(false);

	//[zheng.wentong][2024.7.16]将dataShare放入datashareController进行管理
	//gDataShare = TGDataShareController::GetInstance()->FindDataShare(partDocument);
	//if (!gDataShare)
	//{
	//	gDataShare = new DataShare(partDocument);
	//	TGDataShareController::GetInstance()->Insert(partDocument, gDataShare);
	//	gDataShare->CreateProxyDlg();

	//}
	//// 重复点击则尝试重启代理窗口
	//else
	//{
	//	gDataShare->RestartProxyDlg();
	//}

	//if (!gProxyDlg)
	//{
	//	ApplicationPtr application = CSampleAddinApp::GetSEApp()->GetApplication();
	//	HWND seWindow;
	//	seWindow = reinterpret_cast<HWND>(static_cast<WindowPtr>(application->GetWindows()->Item(1))->GethWnd());
	//	gProxyDlg = new TraspProxyDlg;
	//	gProxyDlg->Create(NULL, L"Your MFC Drawing View", WS_CHILD | WS_BORDER, CRect(0, 0, 1884, 731), CWnd::FromHandle(seWindow), 0);

	//	// 显示代理窗口
	//	gProxyDlg->ShowWindow(SW_SHOW);
	//	gProxyDlg->BringWindowToTop();
	//	gProxyDlg->UpdateWindow();

	//	MSG msg;
	//	while (GetMessage(&msg, nullptr, 0, 0))
	//	{
	//		//std::stringstream ss;
	//		//ss << "Message: " << msg.message << std::endl;
	//		//WriteLog(ss.str().c_str(), true);
	//		TranslateMessage(&msg);
	//		DispatchMessage(&msg);
	//		AfxPumpMessage();  // or use CWinThread::PumpMessage()
	//	}
	//}

	//// 刷新显示
	//static_cast<WindowPtr>(CSampleAddinApp::GetApplication()->GetWindows()->Item(1))->View->Update();
	//// 激活工具
	//SEAddin* pAddin = CSampleAddinApp::GetSEApp()->GetSEAddin();
	//if (!pAddin)
	//	return false;
	//CDemoCommandObj* pTheCommand = NULL;
	//CDemoCommandObj::CreateInstance(&pTheCommand);
	//if (!pTheCommand)
	//	return false;
	//pTheCommand->AddRef();
	//pTheCommand->SetCommandsObject(pAddin->GetCommands());
	//HRESULT hr = pTheCommand->CreateCommand(SolidEdgeConstants::seNoDeactivate);
	//if (SUCCEEDED(hr))
	//{
	//	// Very very important! Start the command so events will start flowing this way.
	//	pTheCommand->GetCommand()->Start();
	//}
	//gDataShare->SetMouse(pTheCommand->GetMouse());
	////gDataShare->Set3DSketchFlag(false);
	//return true;

}

// 第一个按钮响应函数，用于初始化
bool AddinMenu::CallCustomPage()
{
	// 调整视角
	double EyeX, EyeY, EyeZ, TargetX, TargetY, TargetZ, UpX, UpY, UpZ, ScaleOrAngle;
	VARIANT_BOOL Perspective;// 是否透视

	static_cast<WindowPtr>(CSampleAddinApp::GetApplication()->GetWindows()->Item(1))->View->GetCamera(&EyeX, &EyeY, &EyeZ, &TargetX, &TargetY, &TargetZ, &UpX, &UpY, &UpZ, &Perspective, &ScaleOrAngle);

	static_cast<WindowPtr>(CSampleAddinApp::GetApplication()->GetWindows()->Item(1))->View->BeginCameraDynamics();

	//-0.0459294,-0.94749,0.425348,0.261098,-0.0536459,0.0985821,0.125473,0.302334,0.944908,0,2944.59

	static_cast<WindowPtr>(CSampleAddinApp::GetApplication()->GetWindows()->Item(1))->View->SetCamera(0, 0, 1, 0, 0, 0, 0, 1, 0, Perspective, ScaleOrAngle);

	static_cast<WindowPtr>(CSampleAddinApp::GetApplication()->GetWindows()->Item(1))->View->PanCamera(-772, 323);

	static_cast<WindowPtr>(CSampleAddinApp::GetApplication()->GetWindows()->Item(1))->View->EndCameraDynamics();

	// 创建草图
	ApplicationPtr application = CSampleAddinApp::GetSEApp()->GetApplication();
	SolidEdgePart::PartDocumentPtr partDocument = application->GetActiveDocument();
	if (partDocument == NULL)
		return false;

	// 参考面隐藏
	partDocument->RefPlanes->Item(1)->PutVisible(false);
	partDocument->RefPlanes->Item(2)->PutVisible(false);
	partDocument->RefPlanes->Item(3)->PutVisible(false);

	//[zheng.wentong][2024.7.16]将dataShare放入datashareController进行管理
	//gDataShare = TGDataShareController::GetInstance()->FindDataShare(partDocument);
	//if (!gDataShare)
	//{
	//	gDataShare = new DataShare(partDocument);
	//	TGDataShareController::GetInstance()->Insert(partDocument, gDataShare);
	//}

	static_cast<WindowPtr>(CSampleAddinApp::GetApplication()->GetWindows()->Item(1))->View->Update();

	return true;

}

// 第二个按钮响应函数，画草图
bool AddinMenu::CallCustomCommand()
{


	// 激活工具
	//CEdgeBar::ActiveCurDocPage();

	SEAddin* pAddin = CSampleAddinApp::GetSEApp()->GetSEAddin();
	if (!pAddin)
		return false;

	CDemoCommandObj* pTheCommand = NULL;

	CDemoCommandObj::CreateInstance(&pTheCommand);
	if (!pTheCommand)
		return false;

	//pTheCommand->m_pPage = dynamic_cast<IntelligentSketchingDialog*>(CEdgeBar::GetCurDocPageDialog());
	//if (!pTheCommand->m_pPage)
	//	return false;

	//pTheCommand->m_pPage->CloseWindow();

	pTheCommand->AddRef();

	pTheCommand->SetCommandsObject(pAddin->GetCommands());
	HRESULT hr = pTheCommand->CreateCommand(SolidEdgeConstants::seNoDeactivate);
	if (SUCCEEDED(hr))
	{
		// Very very important! Start the command so events will start flowing this way.
		pTheCommand->GetCommand()->Start();
	}

	//pTheCommand->GetPage()->CloseWindow();

	//gDataShare->SetMouse(pTheCommand->GetMouse());
	//gDataShare->InitRefPlaneProfileConstrain();
	//gDataShare->Set3DSketchFlag(false);

	return true;

}

// 第三个按钮响应函数，清空笔划
bool AddinMenu::CallCreatBoxCommand()
{
	//if (gDataShare == NULL)
	//{
	//	MessageBox(NULL, TEXT("未初始化!"), TEXT("错误！"), MB_OK | MB_YESNO);
	//	return false;
	//}

	//gDataShare->Clear();

	return true;
}

// 第四个按钮响应函数，测试1
bool AddinMenu::CallCreateEllipse()
{
	//if (gDataShare == NULL)
	//{
	//	MessageBox(NULL, TEXT("未初始化!"), TEXT("错误！"), MB_OK | MB_YESNO);
	//	return false;
	//}

	//CreateBox(gDataShare->GetDocument());

	//gDataShare->Test1();

	//gDataShare->GetConstrainsRecy()->AddVertical(gDataShare->GetLinesOrig()->Item(2));
	// 
	//ApplicationPtr application = CSampleAddinApp::GetSEApp()->GetApplication();

	//SolidEdgePart::PartDocumentPtr partDocument = application->GetActiveDocument();
	//if (partDocument == NULL)
	//	return false;

	//SolidEdgePart::ProfilesPtr profiles = NULL;
	//SolidEdgePart::ProfilePtr profile = NULL;
	//SolidEdgePart::RefPlanesPtr refplanes = NULL;
	////	System.Array aProfiles = null;

	//// Get a reference to the profile sets collection.
	//SolidEdgePart::SketchsPtr sketchs = partDocument->Sketches;
	//SolidEdgePart::SketchPtr skectch = sketchs->Add();
	//profiles = skectch->Profiles;

	//// Get a reference to the ref planes collection.
	//refplanes = partDocument->RefPlanes;

	//// Add a new profile.
	//profile = profiles->Add(refplanes->Item(1));

	//SolidEdgeFrameworkSupport::Ellipses2dPtr ellipses2d = profile->Ellipses2d;
	//SolidEdgeFrameworkSupport::Ellipse2dPtr ellipse2dPtr = NULL;

	//ellipse2dPtr = ellipses2d->AddByCenter(0, 0, 0.01, 0.01, 0.5, SolidEdgeFrameworkSupport::Geom2dOrientationConstants::igGeom2dOrientCounterClockwise);

	//profile->End(SolidEdgePart::ProfileValidationType::igProfileClosed);

	//profile->Visible = false;
	return true;
}

// 第五个按钮相应函数，测试2
bool AddinMenu::CreateSubtracts()
{
	//if (gDataShare == NULL)
	//{
	//	MessageBox(NULL, TEXT("未初始化!"), TEXT("错误！"), MB_OK | MB_YESNO);
	//	return false;
	//}

	//gDataShare->Test2();
	//gDataShare->GetConstrains()->AddHorizontal(gDataShare->GetLinesOrig()->Item(2));



	//ApplicationPtr application = CSampleAddinApp::GetSEApp()->GetApplication();
	//SolidEdgePart::PartDocumentPtr partDocument = application->GetActiveDocument();

	//application->DoIdle();

	//SolidEdgePart::ModelsPtr models = partDocument->Models;
	//if (models->GetCount() < 2)
	//	return false;

	//SolidEdgePart::UnionsPtr unions = NULL;
	//SolidEdgePart::UnionPtr unionPtr = NULL;

	//CComSafeArray<IDispatch*> targetArray(1);
	//targetArray[0] = models->Item(1)->Body;

	//CComSafeArray<IDispatch*> toolArray(1);
	//toolArray[0] = models->Item(2)->Body;

	//CComSafeArray<long> toolDirectionArray(1);
	//toolDirectionArray[0] = SolidEdgePart::SESubtractDirection::igSubtractDirectionNone;

	//SolidEdgePart::SubtractsPtr subtracts = NULL;
	//SolidEdgePart::SubtractPtr subtract = NULL;

	//subtracts = models->Item(1)->Subtracts;
	//subtract = subtracts->Add(1, targetArray.GetSafeArrayPtr(), 1, toolArray.GetSafeArrayPtr(), toolDirectionArray.GetSafeArrayPtr(),
	//	SolidEdgePart::SETargetDesignBodyOption::igCreateMultipleDesignBodiesOnNonManifoldOption,
	//	SolidEdgePart::SETargetConstructionBodyOption::igCreateMultipleConstructionBodiesOnNonManifoldOption);

	return true;
}

bool AddinMenu::Test3()
{
	ActiveMouse();
	//MessageBox(NULL, TEXT("按钮3 !"), TEXT("错误！"), MB_OK | MB_YESNO);
	return true;
}

bool AddinMenu::Test4()
{
	MessageBox(NULL, TEXT("按钮4 !"), TEXT("错误！"), MB_OK | MB_YESNO);
	return true;

}

bool AddinMenu::Test5()
{
	//if (gDataShare == NULL)
	//{
	//	MessageBox(NULL, TEXT("未初始化!"), TEXT("错误！"), MB_OK | MB_YESNO);
	//	return false;
	//}

	//gDataShare->Test5();

	return true;
}

bool AddinMenu::Test6()
{
	// 调整视角
	double EyeX, EyeY, EyeZ, TargetX, TargetY, TargetZ, UpX, UpY, UpZ, ScaleOrAngle;
	VARIANT_BOOL Perspective;// 是否透视
	static_cast<WindowPtr>(CSampleAddinApp::GetApplication()->GetWindows()->Item(1))->View->GetCamera(&EyeX, &EyeY, &EyeZ, &TargetX, &TargetY, &TargetZ, &UpX, &UpY, &UpZ, &Perspective, &ScaleOrAngle);
	static_cast<WindowPtr>(CSampleAddinApp::GetApplication()->GetWindows()->Item(1))->View->BeginCameraDynamics();
	//-0.0459294,-0.94749,0.425348,0.261098,-0.0536459,0.0985821,0.125473,0.302334,0.944908,0,2944.59
	static_cast<WindowPtr>(CSampleAddinApp::GetApplication()->GetWindows()->Item(1))->View->SetCamera(0, 0, 1, 0, 0, 0, 0, 1, 0, Perspective, ScaleOrAngle);
	static_cast<WindowPtr>(CSampleAddinApp::GetApplication()->GetWindows()->Item(1))->View->PanCamera(-772, 323);
	static_cast<WindowPtr>(CSampleAddinApp::GetApplication()->GetWindows()->Item(1))->View->EndCameraDynamics();

	// 创建草图
	ApplicationPtr application = CSampleAddinApp::GetSEApp()->GetApplication();
	SolidEdgePart::PartDocumentPtr partDocument = application->GetActiveDocument();
	if (partDocument == NULL)
		return false;

	// 参考面隐藏
	partDocument->RefPlanes->Item(1)->PutVisible(false);
	partDocument->RefPlanes->Item(2)->PutVisible(false);
	partDocument->RefPlanes->Item(3)->PutVisible(false);

	//[zheng.wentong][2024.7.16]将dataShare放入datashareController进行管理
	//gDataShare = TGDataShareController::GetInstance()->FindDataShare(partDocument);
	//if (!gDataShare)
	//{
	//	gDataShare = new DataShare(partDocument);
	//	TGDataShareController::GetInstance()->Insert(partDocument, gDataShare);
	//	gDataShare->CreateProxyDlg();

	//}
	//// 重复点击则尝试重启代理窗口
	//else
	//{
	//	gDataShare->RestartProxyDlg();
	//}







	//if (gDataShare == NULL)
	//{
	//	MessageBox(NULL, TEXT("未初始化!"), TEXT("错误！"), MB_OK | MB_YESNO);
	//	return false;
	//}

	//gDataShare->Test6();

	//return true;
}

bool AddinMenu::Test7()
{
	//if (gDataShare == NULL)
	//{
	//	MessageBox(NULL, TEXT("未初始化!"), TEXT("错误！"), MB_OK | MB_YESNO);
	//	return false;
	//}

	//gDataShare->Test7();

	return true;
}

// 未完善
bool AddinMenu::HandSketch3D()
{
	// 创建草图
	ApplicationPtr application = CSampleAddinApp::GetSEApp()->GetApplication();
	SolidEdgePart::PartDocumentPtr partDocument = application->GetActiveDocument();
	if (partDocument == NULL)
		return false;

	// 参考面隐藏
	partDocument->RefPlanes->Item(1)->PutVisible(false);
	partDocument->RefPlanes->Item(2)->PutVisible(false);
	partDocument->RefPlanes->Item(3)->PutVisible(false);

	//if(gDataShare == NULL)	gDataShare = new DataShare(partDocument);

	//static_cast<WindowPtr>(CSampleAddinApp::GetApplication()->GetWindows()->Item(1))->View->Update();

	//// 激活鼠标工具
	//if (gDataShare == NULL)
	//{
	//	MessageBox(NULL, TEXT("未初始化!"), TEXT("错误！"), MB_OK | MB_YESNO);
	//	return false;
	//}

	//// 激活工具
	//CEdgeBar::ActiveCurDocPage();

	SEAddin* pAddin = CSampleAddinApp::GetSEApp()->GetSEAddin();
	if (!pAddin)
		return false;

	CDemoCommandObj* pTheCommand = NULL;

	CDemoCommandObj::CreateInstance(&pTheCommand);
	if (!pTheCommand)
		return false;

	//pTheCommand->m_pPage = dynamic_cast<IntelligentSketchingDialog*>(CEdgeBar::GetCurDocPageDialog());
	//if (!pTheCommand->m_pPage)
	//	return false;

	//pTheCommand->m_pPage->CloseWindow();

	pTheCommand->AddRef();

	pTheCommand->SetCommandsObject(pAddin->GetCommands());
	HRESULT hr = pTheCommand->CreateCommand(SolidEdgeConstants::seNoDeactivate);
	if (SUCCEEDED(hr))
	{
		// Very very important! Start the command so events will start flowing this way.
		pTheCommand->GetCommand()->Start();
	}

	//pTheCommand->GetPage()->CloseWindow();

	//gDataShare->SetMouse(pTheCommand->GetMouse());
	//gDataShare->InitRefPlaneProfileConstrain();

	//gDataShare->Set3DSketchFlag();

	return true;
}

bool AddinMenu::CallCreateLine()
{
	ApplicationPtr application = CSampleAddinApp::GetSEApp()->GetApplication();
	SolidEdgePart::PartDocumentPtr partDocument = application->GetActiveDocument();
	if (partDocument == NULL)
		return false;

	SolidEdgePart::ProfilesPtr profiles = NULL;
	SolidEdgePart::ProfilePtr profile = NULL;
	SolidEdgePart::RefPlanesPtr refplanes = NULL;
	SolidEdgeFrameworkSupport::Relations2dPtr relations2d = NULL;
	SolidEdgeFrameworkSupport::Relation2dPtr relation2d = NULL;
	SolidEdgeFrameworkSupport::Lines2dPtr lines2d = NULL;
	SolidEdgeFrameworkSupport::Line2dPtr line2d = NULL;

	SolidEdgePart::SketchsPtr sketchs = partDocument->Sketches;
	SolidEdgePart::SketchPtr skectch = sketchs->Add();

	// Get a reference to the profile sets collection.
	profiles = skectch->Profiles;

	// Get a reference to the ref planes collection.
	refplanes = partDocument->RefPlanes;

	long count = refplanes->GetCount();

	// Add a new profile.
	profile = profiles->Add(refplanes->Item(1));

	// Get a reference to the lines2d collection.
	lines2d = profile->Lines2d;

	//x1,y1,x2, y2
	line2d = lines2d->AddBy2Points(0.01, 0.01, 0.08, 0.01);

	relation2d = relations2d->AddKeypoint(
		lines2d->Item(0),
		(int)SolidEdgeConstants::KeypointIndexConstants::igLineEnd,
		lines2d->Item(1),
		(int)SolidEdgeConstants::KeypointIndexConstants::igLineStart,
		true);

	profile->End(SolidEdgePart::ProfileValidationType::igProfileClosed);

	// Hide the profile.
	profile->Visible = false;

	return true;
}

bool AddinMenu::CallCreateCircle()
{
	ApplicationPtr application = CSampleAddinApp::GetSEApp()->GetApplication();

	SolidEdgePart::PartDocumentPtr partDocument = application->GetActiveDocument();
	if (partDocument == NULL)
		return false;

	SolidEdgePart::ProfilesPtr profiles = NULL;
	SolidEdgePart::ProfilePtr profile = NULL;
	SolidEdgePart::RefPlanesPtr refplanes = NULL;

	SolidEdgePart::SketchsPtr sketchs = partDocument->Sketches;
	SolidEdgePart::SketchPtr skectch = sketchs->Add();

	// Get a reference to the profiles collection.
	profiles = skectch->Profiles;

	// Get a reference to the ref planes collection.
	refplanes = partDocument->RefPlanes;

	// Add a new profile.
	profile = profiles->Add(refplanes->Item(1));

	SolidEdgeFrameworkSupport::Circles2dPtr circles2d = profile->Circles2d;
	SolidEdgeFrameworkSupport::Circle2dPtr circle2d = NULL;

	circle2d = circles2d->AddByCenterRadius(0, 0, 0.05);

	profile->End(SolidEdgePart::ProfileValidationType::igProfileClosed);

	profile->Visible = false;
	return true;
}

bool AddinMenu::CallCreateRectangle()
{
	ApplicationPtr application = CSampleAddinApp::GetSEApp()->GetApplication();

	SolidEdgePart::PartDocumentPtr partDocument = application->GetActiveDocument();
	if (partDocument == NULL)
		return false;

	SolidEdgePart::ProfilesPtr profiles = NULL;
	SolidEdgePart::ProfilePtr profile = NULL;
	SolidEdgePart::RefPlanesPtr refplanes = NULL;
	SolidEdgeFrameworkSupport::RectangularPatterns2dPtr rectangularPatterns = NULL;
	SolidEdgeFrameworkSupport::RectangularPattern2dPtr rectangularPattern = NULL;

	SolidEdgePart::SketchsPtr sketchs = partDocument->Sketches;
	SolidEdgePart::SketchPtr skectch = sketchs->Add();

	// Get a reference to the profiles collection.
	profiles = skectch->Profiles;

	// Get a reference to the ref planes collection.
	refplanes = partDocument->RefPlanes;

	// Add a new profile.
	profile = profiles->Add(refplanes->Item(1));

	rectangularPatterns = profile->RectangularPatterns2d;

	SolidEdgeFrameworkSupport::PatternOffsetTypeConstants OffsetType = SolidEdgeFrameworkSupport::PatternOffsetTypeConstants::sePatternFillOffset;
	rectangularPattern = rectangularPatterns->Add(0, 0, 0.1, 0.05, 0, OffsetType, 0, 0, 0, 0);

	profile->End(SolidEdgePart::ProfileValidationType::igProfileClosed);

	profile->Visible = false;
	return true;
}

void AddinMenu::CreateFiniteExtrudedProtrusion(SolidEdgePart::PartDocumentPtr partDocument)
{
	SolidEdgePart::ProfileSetsPtr profileSets = NULL;
	SolidEdgePart::ProfileSetPtr profileSet = NULL;
	SolidEdgePart::ProfilesPtr profiles = NULL;
	SolidEdgePart::ProfilePtr profile = NULL;
	SolidEdgePart::RefPlanesPtr refplanes = NULL;
	SolidEdgeFrameworkSupport::Relations2dPtr relations2d = NULL;
	SolidEdgeFrameworkSupport::Relation2dPtr relation2d = NULL;
	SolidEdgeFrameworkSupport::Lines2dPtr lines2d = NULL;
	SolidEdgeFrameworkSupport::Line2dPtr line2d = NULL;
	SolidEdgePart::ModelsPtr models = NULL;
	SolidEdgePart::ModelPtr model = NULL;
	//	System.Array aProfiles = null;

		// Get a reference to the profile sets collection.
	profileSets = partDocument->ProfileSets;

	// Add a new profile set.
	profileSet = profileSets->Add();

	// Get a reference to the profiles collection.
	profiles = profileSet->Profiles;

	// Get a reference to the ref planes collection.
	refplanes = partDocument->RefPlanes;

	// Add a new profile.
	profile = profiles->Add(refplanes->Item(1));

	// Get a reference to the lines2d collection.
	lines2d = profile->Lines2d;

	// UOM = meters.
	double lineMatrix[12][4] =
	{
		//{x1, y1, x2, y2}
		{ 0, 0, 0.08, 0 },
		{ 0.08, 0, 0.08, 0.06 },
		{ 0.08, 0.06, 0.064, 0.06 },
		{ 0.064, 0.06, 0.064, 0.02 },
		{ 0.064, 0.02, 0.048, 0.02 },
		{ 0.048, 0.02, 0.048, 0.06 },
		{ 0.048, 0.06, 0.032, 0.06 },
		{ 0.032, 0.06, 0.032, 0.02 },
		{ 0.032, 0.02, 0.016, 0.02 },
		{ 0.016, 0.02, 0.016, 0.06 },
		{ 0.016, 0.06, 0, 0.06 },
		{ 0, 0.06, 0, 0 }
	};

	// Draw the Base Profile.
	for (int i = 0; i < 12; i++)
	{
		line2d = lines2d->AddBy2Points(
			lineMatrix[i][0],
			lineMatrix[i][1],
			lineMatrix[i][2],
			lineMatrix[i][3]);
	}

	// Define Relations among the Line objects to make the Profile closed.
	relations2d = (SolidEdgeFrameworkSupport::Relations2dPtr)profile->Relations2d;

	// Connect all of the lines.
	for (int i = 1; i <= lines2d->Count; i++)
	{
		int j = i + 1;

		// When we reach the last line, wrap around and connect it to the first line.
		if (j > lines2d->Count)
		{
			j = 1;
		}

		relation2d = relations2d->AddKeypoint(
			lines2d->Item(i),
			(int)SolidEdgeConstants::KeypointIndexConstants::igLineEnd,
			lines2d->Item(j),
			(int)SolidEdgeConstants::KeypointIndexConstants::igLineStart,
			true);
	}

	// Close the profile.
	profile->End(SolidEdgePart::ProfileValidationType::igProfileClosed);

	// Hide the profile.
	profile->Visible = false;

	// Create a new array of profile objects.
//	aProfiles = Array.CreateInstance(typeof(SolidEdgePart.Profile), 1);
//	aProfiles.SetValue(profile, 0);

	CComSafeArray<IDispatch*> aProfiles(1);
	aProfiles[0] = profile;

	// Get a reference to the models collection.
	models = partDocument->Models;
	\

		// Create the extended protrusion.
		model = models->AddFiniteExtrudedProtrusion(
			1,
			aProfiles.GetSafeArrayPtr(),
			SolidEdgePart::FeaturePropertyConstants::igRight,
			0.008);
}

void AddinMenu::CreateBox(SolidEdgePart::PartDocumentPtr partDocument)
{
	SolidEdgePart::ProfileSetsPtr profileSets = NULL;
	SolidEdgePart::ProfileSetPtr profileSet = NULL;
	SolidEdgePart::ProfilesPtr profiles = NULL;
	SolidEdgePart::ProfilePtr profile = NULL;
	SolidEdgePart::RefPlanesPtr refplanes = NULL;
	SolidEdgeFrameworkSupport::Relations2dPtr relations2d = NULL;
	SolidEdgeFrameworkSupport::Relation2dPtr relation2d = NULL;
	SolidEdgeFrameworkSupport::Lines2dPtr lines2d = NULL;
	SolidEdgeFrameworkSupport::Line2dPtr line2d = NULL;
	SolidEdgePart::ModelsPtr models = NULL;
	SolidEdgePart::ModelPtr model = NULL;
	//	System.Array aProfiles = null;

	// Get a reference to the profile sets collection.
	profileSets = partDocument->ProfileSets;

	// Add a new profile set.
	profileSet = profileSets->Add();

	// Get a reference to the profiles collection.
	profiles = profileSet->Profiles;

	// Get a reference to the ref planes collection.
	refplanes = partDocument->RefPlanes;

	// Add a new profile.
	profile = profiles->Add(refplanes->Item(1));

	// Get a reference to the lines2d collection.
	lines2d = profile->Lines2d;

	// UOM = meters.
	double lineMatrix[4][4] =
	{
		//{x1, y1, x2, y2}
		{ 0, 0, 0.08, 0 },
		{ 0.08, 0, 0.08, 0.08 },
		{ 0.08, 0.08, 0, 0.08 },
		{0, 0.08, 0, 0}
	};

	// Draw the Base Profile.
	for (int i = 0; i < 4; i++)
	{
		line2d = lines2d->AddBy2Points(
			lineMatrix[i][0],
			lineMatrix[i][1],
			lineMatrix[i][2],
			lineMatrix[i][3]);
	}

	// Define Relations among the Line objects to make the Profile closed.
	relations2d = (SolidEdgeFrameworkSupport::Relations2dPtr)profile->Relations2d;

	// Connect all of the lines.
	for (int i = 1; i <= lines2d->Count; i++)
	{
		int j = i + 1;

		// When we reach the last line, wrap around and connect it to the first line.
		if (j > lines2d->Count)
		{
			j = 1;
		}

		relation2d = relations2d->AddKeypoint(
			lines2d->Item(i),
			(int)SolidEdgeConstants::KeypointIndexConstants::igLineEnd,
			lines2d->Item(j),
			(int)SolidEdgeConstants::KeypointIndexConstants::igLineStart,
			true);
	}

	// Close the profile.
	profile->End(SolidEdgePart::ProfileValidationType::igProfileClosed);

	// Hide the profile.
	profile->Visible = false;

	// Create a new array of profile objects.
	//	aProfiles = Array.CreateInstance(typeof(SolidEdgePart.Profile), 1);
	//	aProfiles.SetValue(profile, 0);

	CComSafeArray<IDispatch*> aProfiles(1);
	aProfiles[0] = profile;

	// Get a reference to the models collection.
	models = partDocument->Models;

	// Create the extended protrusion.
	model = models->AddFiniteExtrudedProtrusion(
		1,
		aProfiles.GetSafeArrayPtr(),
		SolidEdgePart::FeaturePropertyConstants::igRight,
		0.04);
}

bool AddinMenu::CreateExtrudedProtrusion()
{
	ApplicationPtr application = CSampleAddinApp::GetSEApp()->GetApplication();
	SolidEdgePart::PartDocumentPtr partDocument = application->Documents->Add("SolidEdge.PartDocument");
	application->DoIdle();

	SolidEdgePart::ProfileSetsPtr profileSets = NULL;
	SolidEdgePart::ProfileSetPtr profileSet = NULL;
	SolidEdgePart::ProfilesPtr profiles = NULL;
	SolidEdgePart::ProfilePtr profile = NULL;
	SolidEdgePart::RefPlanesPtr refplanes = NULL;
	SolidEdgeFrameworkSupport::Relations2dPtr relations2d = NULL;
	SolidEdgeFrameworkSupport::Relation2dPtr relation2d = NULL;
	SolidEdgeFrameworkSupport::Lines2dPtr lines2d = NULL;
	SolidEdgeFrameworkSupport::Line2dPtr line2d = NULL;
	SolidEdgePart::ModelsPtr models = NULL;
	SolidEdgePart::ModelPtr model = NULL;
	//	System.Array aProfiles = null;

	// Get a reference to the profile sets collection.
	profileSets = partDocument->ProfileSets;

	// Add a new profile set.
	profileSet = profileSets->Add();

	// Get a reference to the profiles collection.
	profiles = profileSet->Profiles;

	// Get a reference to the ref planes collection.
	refplanes = partDocument->RefPlanes;

	// Add a new profile.
	profile = profiles->Add(refplanes->Item(1));

	// Get a reference to the lines2d collection.
	lines2d = profile->Lines2d;

	// UOM = meters.
	double lineMatrix[4][4] =
	{
		//{x1, y1, x2, y2}
		{ 0, 0, 0.08, 0 },
		{ 0.08, 0, 0.08, 0.08 },
		{ 0.08, 0.08, 0, 0.08 },
		{ 0, 0.08, 0, 0 }
	};

	// Draw the Base Profile.
	for (int i = 0; i < 4; i++)
	{
		line2d = lines2d->AddBy2Points(
			lineMatrix[i][0],
			lineMatrix[i][1],
			lineMatrix[i][2],
			lineMatrix[i][3]);
	}

	// Define Relations among the Line objects to make the Profile closed.
	relations2d = (SolidEdgeFrameworkSupport::Relations2dPtr)profile->Relations2d;

	// Connect all of the lines.
	for (int i = 1; i <= lines2d->Count; i++)
	{
		int j = i + 1;

		// When we reach the last line, wrap around and connect it to the first line.
		if (j > lines2d->Count)
		{
			j = 1;
		}

		relation2d = relations2d->AddKeypoint(
			lines2d->Item(i),
			(int)SolidEdgeConstants::KeypointIndexConstants::igLineEnd,
			lines2d->Item(j),
			(int)SolidEdgeConstants::KeypointIndexConstants::igLineStart,
			true);
	}

	// Close the profile.
	profile->End(SolidEdgePart::ProfileValidationType::igProfileClosed);

	// Hide the profile.
	profile->Visible = false;

	// Create a new array of profile objects.
	//	aProfiles = Array.CreateInstance(typeof(SolidEdgePart.Profile), 1);
	//	aProfiles.SetValue(profile, 0);

	CComSafeArray<IDispatch*> aProfiles;
	aProfiles.Add(profile);

	// Get a reference to the models collection.
	models = partDocument->Models;

	// Create the extended protrusion.
	model = models->AddFiniteExtrudedProtrusion(
		aProfiles.GetCount(),
		aProfiles.GetSafeArrayPtr(),
		SolidEdgePart::FeaturePropertyConstants::igRight,
		0.04);
	return true;
}

bool AddinMenu::CreateRound()
{
	ApplicationPtr application = CSampleAddinApp::GetSEApp()->GetApplication();
	SolidEdgePart::PartDocumentPtr partDocument = application->GetActiveDocument();

	application->DoIdle();

	if (partDocument == NULL)
		return false;

	SolidEdgePart::ModelsPtr models = NULL;
	SolidEdgePart::ModelPtr model = NULL;

	SolidEdgePart::ExtrudedProtrusionsPtr extrudedProtrusions = NULL;
	SolidEdgePart::ExtrudedProtrusionPtr extrudedProtrusion = NULL;
	SolidEdgeGeometry::EdgesPtr edges = NULL;
	SolidEdgePart::RoundsPtr rounds = NULL;
	SolidEdgePart::RoundPtr round = NULL;

	models = partDocument->Models;
	model = models->Item(1);

	extrudedProtrusions = model->ExtrudedProtrusions;
	extrudedProtrusion = extrudedProtrusions->Item(1);

	edges = extrudedProtrusion->Edges[SolidEdgeGeometry::FeatureTopologyQueryTypeConstants::igQueryRoundable];

	std::vector<IDispatchPtr> edgeVec;
	edgeVec.push_back(edges->Item(1));

	CComVariant OuputVariant1;

	SAFEARRAY* parray = SafeArrayCreateVector(VT_VARIANT, 0, (ULONG)edgeVec.size());
	for (long i = 0; i < edgeVec.size(); i++)
	{
		SafeArrayPutElement(parray, &i, &CComVariant((IDispatch*)edgeVec[i]));
	}
	OuputVariant1.vt = VT_ARRAY | VT_VARIANT;
	SafeArrayCopy(parray, &(OuputVariant1.parray));


	std::vector<double> inPutArray = { 0.01 };
	SAFEARRAY* radiusArray = SafeArrayCreateVector(VT_VARIANT, 0, (ULONG)inPutArray.size());
	for (long i = 0; i < inPutArray.size(); i++)
	{
		SafeArrayPutElement(radiusArray, &i, &CComVariant((double)inPutArray[i]));
	}

	CComVariant OuputVariant2;
	OuputVariant2.vt = VT_VARIANT | VT_ARRAY;
	SafeArrayCopy(radiusArray, &(OuputVariant2.parray));

	rounds = model->Rounds;
	round = rounds->Add(edgeVec.size(), OuputVariant1, OuputVariant2);

	return true;
}

bool AddinMenu::CreateChamfer()
{
	ApplicationPtr application = CSampleAddinApp::GetSEApp()->GetApplication();
	SolidEdgePart::PartDocumentPtr partDocument = application->GetActiveDocument();

	application->DoIdle();

	if (partDocument == NULL)
		return false;

	SolidEdgePart::ModelsPtr models = NULL;
	SolidEdgePart::ModelPtr model = NULL;

	SolidEdgePart::ExtrudedProtrusionsPtr extrudedProtrusions = NULL;
	SolidEdgePart::ExtrudedProtrusionPtr extrudedProtrusion = NULL;
	SolidEdgeGeometry::EdgesPtr edges = NULL;
	SolidEdgePart::ChamfersPtr chamfers = NULL;
	SolidEdgePart::ChamferPtr chamfer = NULL;

	models = partDocument->Models;
	model = models->Item(1);

	extrudedProtrusions = model->ExtrudedProtrusions;
	extrudedProtrusion = extrudedProtrusions->Item(1);

	edges = extrudedProtrusion->Edges[SolidEdgeGeometry::FeatureTopologyQueryTypeConstants::igQueryRoundable];

	std::vector<IDispatchPtr> edgeVec;
	edgeVec.push_back(edges->Item(1));

	SAFEARRAY* parray = SafeArrayCreateVector(VT_VARIANT, 0, (ULONG)edgeVec.size());
	for (long i = 0; i < edgeVec.size(); i++)
	{
		SafeArrayPutElement(parray, &i, &CComVariant((IDispatch*)edgeVec[i]));
	}

	CComVariant OuputVariant1;
	OuputVariant1.vt = VT_ARRAY | VT_VARIANT;
	SafeArrayCopy(parray, &(OuputVariant1.parray));

	chamfers = model->Chamfers;
	chamfer = chamfers->AddEqualSetback(edgeVec.size(), OuputVariant1, 0.01);

	return true;
}

bool AddinMenu::CreateRevolvedProtrusions()
{
	ApplicationPtr application = CSampleAddinApp::GetSEApp()->GetApplication();
	SolidEdgePart::PartDocumentPtr partDocument = application->Documents->Add("SolidEdge.PartDocument");

	application->DoIdle();

	if (partDocument == NULL)
		return false;

	SolidEdgePart::ProfileSetsPtr profileSets = NULL;
	SolidEdgePart::ProfileSetPtr profileSet = NULL;

	SolidEdgePart::ProfilesPtr profiles = NULL;
	SolidEdgePart::ProfilePtr profile = NULL;
	SolidEdgePart::RefPlanesPtr refplanes = NULL;
	SolidEdgePart::RefPlanePtr refplane = NULL;
	SolidEdgePart::RefAxisPtr axis = NULL;

	SolidEdgeFrameworkSupport::Line2dPtr line2d = NULL;
	SolidEdgePart::ModelsPtr models = NULL;
	SolidEdgePart::ModelPtr model = NULL;
	SolidEdgePart::RevolvedProtrusionPtr revolvedProtrusion = NULL;

	profileSets = partDocument->ProfileSets;

	// Add a new profile set.
	profileSet = profileSets->Add();

	// Get a reference to the profiles collection.
	profiles = profileSet->Profiles;

	// Get a reference to the ref planes collection.
	refplanes = partDocument->RefPlanes;

	// Add a new profile.
	profile = profiles->Add(refplanes->Item(1));
	profile->Circles2d->AddByCenterRadius(0, 0, 0.025);
	profile->End(SolidEdgePart::ProfileValidationType::igProfileClosed);

	CComSafeArray<IDispatch*> aProfiles(1);
	aProfiles[0] = profile;

	// Get a reference to the models collection.
	models = partDocument->Models;

	// Create the extended protrusion.
	model = models->AddFiniteExtrudedProtrusion(
		1,
		aProfiles.GetSafeArrayPtr(),
		SolidEdgePart::FeaturePropertyConstants::igSymmetric,
		0.1);

	profile->Visible = false;

	SolidEdgePart::ProfileSetPtr RPProprofileSet = NULL;

	SolidEdgePart::ProfilesPtr RPProprofiles = NULL;
	SolidEdgePart::ProfilePtr RPProfile = NULL;

	RPProprofileSet = profileSets->Add();
	RPProprofiles = RPProprofileSet->Profiles;

	RPProfile = RPProprofiles->Add(refplanes->Item(3));
	RPProfile->Circles2d->AddByCenterRadius(0.05, 0, 0.015);
	line2d = RPProfile->Lines2d->AddBy2Points(0, -0.05, 0, 0.05);
	axis = RPProfile->SetAxisOfRevolution(line2d);
	RPProfile->End(SolidEdgePart::ProfileValidationType::igProfileClosed);

	revolvedProtrusion = model->RevolvedProtrusions->AddFinite(RPProfile, axis, SolidEdgePart::FeaturePropertyConstants::igLeft, SolidEdgePart::FeaturePropertyConstants::igLeft, 3.14159265358979);
	//RPProfile->Visible = false;

	return true;
}

bool AddinMenu::CreateRevolvedCutouts()
{
	ApplicationPtr application = CSampleAddinApp::GetSEApp()->GetApplication();
	SolidEdgePart::PartDocumentPtr partDocument = application->Documents->Add("SolidEdge.PartDocument");

	application->DoIdle();

	if (partDocument == NULL)
		return false;

	SolidEdgePart::ProfileSetsPtr profileSets = NULL;
	SolidEdgePart::ProfileSetPtr profileSet = NULL;
	SolidEdgePart::ProfilesPtr profiles = NULL;
	SolidEdgePart::ProfilePtr profile = NULL;
	SolidEdgePart::RefPlanesPtr refplanes = NULL;
	SolidEdgePart::RefPlanePtr refplane = NULL;
	SolidEdgePart::RefAxisPtr axis = NULL;
	SolidEdgePart::RefAxisPtr cutOutAxis = NULL;

	SolidEdgeFrameworkSupport::Line2dPtr line2d = NULL;
	SolidEdgeFrameworkSupport::Line2dPtr cutOutLine2d = NULL;

	SolidEdgePart::ModelsPtr models = NULL;
	SolidEdgePart::ModelPtr model = NULL;
	SolidEdgePart::RevolvedCutoutPtr revolvedCutout = NULL;

	profileSets = partDocument->ProfileSets;

	// Add a new profile set.
	profileSet = profileSets->Add();

	// Get a reference to the profiles collection.
	profiles = profileSet->Profiles;

	// Get a reference to the ref planes collection.
	refplanes = partDocument->RefPlanes;

	// Add a new profile.
	profile = profiles->Add(refplanes->Item(2));
	profile->Circles2d->AddByCenterRadius(0.05, 0, 0.025);
	profile->End(SolidEdgePart::ProfileValidationType::igProfileClosed);

	line2d = profile->Lines2d->AddBy2Points(0, -0.05, 0, 0.05);
	axis = profile->SetAxisOfRevolution(line2d);

	CComSafeArray<IDispatch*> aProfiles(1);
	aProfiles[0] = profile;

	// Get a reference to the models collection.
	models = partDocument->Models;

	// Create the extended protrusion.
	long PI = 3.14159265358979;
	model = models->AddFiniteRevolvedProtrusion(1, aProfiles.GetSafeArrayPtr(), axis, SolidEdgePart::FeaturePropertyConstants::igSymmetric, 3 * PI / 2);

	//profile->Visible = false;

	SolidEdgePart::ProfileSetPtr cutOutprofileSet = NULL;
	SolidEdgePart::ProfilesPtr cutOutprofiles = NULL;
	SolidEdgePart::ProfilePtr cutOutProfile = NULL;

	cutOutprofileSet = profileSets->Add();
	cutOutprofiles = cutOutprofileSet->Profiles;

	cutOutProfile = cutOutprofiles->Add(refplanes->Item(2));
	cutOutProfile->Circles2d->AddByCenterRadius(0.05, 0, 0.01);
	cutOutLine2d = cutOutProfile->Lines2d->AddBy2Points(0, -0.05, 0, 0.05);

	cutOutAxis = cutOutProfile->SetAxisOfRevolution(cutOutLine2d);
	cutOutProfile->End(SolidEdgePart::ProfileValidationType::igProfileClosed);

	revolvedCutout = model->RevolvedCutouts->AddFinite(cutOutProfile, cutOutAxis, SolidEdgePart::FeaturePropertyConstants::igLeft, SolidEdgePart::FeaturePropertyConstants::igRight, 3 * PI / 4);
	//cutOutProfile->Visible = false;

	return true;
}

bool AddinMenu::CreateUnions()
{
	ApplicationPtr application = CSampleAddinApp::GetSEApp()->GetApplication();
	SolidEdgePart::PartDocumentPtr partDocument = application->GetActiveDocument();

	application->DoIdle();

	SolidEdgePart::ModelsPtr models = partDocument->Models;
	if (models->GetCount() < 2)
		return false;

	SolidEdgePart::UnionsPtr unions = NULL;
	SolidEdgePart::UnionPtr unionPtr = NULL;

	CComSafeArray<IDispatch*> targetArray(1);
	targetArray[0] = models->Item(1)->Body;

	CComSafeArray<IDispatch*> toolArray(1);
	toolArray[0] = models->Item(2)->Body;

	unions = models->Item(1)->Unions;

	unionPtr = unions->Add(1, targetArray.GetSafeArrayPtr(), 1, toolArray.GetSafeArrayPtr(),
		SolidEdgePart::SETargetDesignBodyOption::igCreateMultipleDesignBodiesOnNonManifoldOption,
		SolidEdgePart::SETargetConstructionBodyOption::igCreateMultipleConstructionBodiesOnNonManifoldOption);

	return true;
}

bool AddinMenu::CreateIntersects()
{
	ApplicationPtr application = CSampleAddinApp::GetSEApp()->GetApplication();
	SolidEdgePart::PartDocumentPtr partDocument = application->GetActiveDocument();

	application->DoIdle();

	SolidEdgePart::ModelsPtr models = partDocument->Models;
	if (models->GetCount() < 2)
		return false;

	SolidEdgePart::UnionsPtr unions = NULL;
	SolidEdgePart::UnionPtr unionPtr = NULL;

	CComSafeArray<IDispatch*> targetArray(1);
	targetArray[0] = models->Item(1)->Body;

	CComSafeArray<IDispatch*> toolArray(1);
	toolArray[0] = models->Item(2)->Body;

	SolidEdgePart::IntersectsPtr intersects = NULL;
	SolidEdgePart::IntersectPtr intersect = NULL;

	intersects = models->Item(1)->Intersects;
	intersect = intersects->Add(1, targetArray.GetSafeArrayPtr(), 1, toolArray.GetSafeArrayPtr(),
		SolidEdgePart::SETargetDesignBodyOption::igCreateMultipleDesignBodiesOnNonManifoldOption,
		SolidEdgePart::SETargetConstructionBodyOption::igCreateMultipleConstructionBodiesOnNonManifoldOption);

	return true;
}

bool AddinMenu::GetSolidEdgeType(IDispatchPtr disp, SolidEdgeConstants::ObjectType& getType)
{
	if (!disp)
		return false;

	DISPID PropertyID[1] = { 0 };
	BSTR PropName[1];
	PropName[0] = SysAllocString(L"Type");

	HRESULT hr = disp->GetIDsOfNames(IID_NULL, PropName, 1, LOCALE_SYSTEM_DEFAULT, PropertyID);

	SysFreeString(PropName[0]);

	DISPPARAMS params2 = { NULL, NULL, 0, 0 };

	CComVariant Result;
	hr = disp->Invoke(PropertyID[0], IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_PROPERTYGET, &params2, &Result, NULL, NULL);
	if (hr == S_OK && Result.vt == VT_I4)
	{
		getType = (SolidEdgeConstants::ObjectType)Result.lVal;
		return true;
	}
	return true;
}

int AddinMenu::SetPartColor(SolidEdgePart::PartDocumentPtr pPartDoc, const std::string& strStyle)
{
	if (pPartDoc == nullptr)
	{
		return -1;
	}

	try
	{
		std::string strStyleName;
		FaceStylePtr pCurrentStyle = nullptr;
		FaceStylesPtr pStyles = pPartDoc->FaceStyles;
		if (pStyles != nullptr)
		{
			long lCount = pStyles->GetCount();
			for (long i = 1; i <= lCount; ++i)
			{
				FaceStylePtr pStyle = pStyles->Item(i);
				if (pStyle != nullptr)
				{
					strStyleName = (LPCSTR)pStyle->StyleName;
					if (strStyleName == strStyle)
					{
						pCurrentStyle = pStyle;
					}
				}
			}
		}

		SolidEdgePart::ModelsPtr models = pPartDoc->Models;
		if (models != nullptr)
		{
			long lCount = models->Count;
			for (long i = 1; i <= lCount; ++i)
			{
				try
				{
					SolidEdgePart::ModelPtr model = models->Item(i);
					if (model != nullptr)
					{
						SolidEdgeGeometry::BodyPtr body = model->Body;
						if (body != nullptr)
						{
							SetBodyColor(body, pCurrentStyle);
						}
					}
				}
				catch (_com_error& error)
				{
					continue;
				}
			}
		}

		SolidEdgePart::ConstructionsPtr constructions = pPartDoc->Constructions;
		if (constructions != nullptr)
		{
			long lCount = constructions->Count;
			for (long i = 1; i <= lCount; ++i)
			{
				try
				{
					SolidEdgePart::ConstructionModelPtr model = constructions->Item(i);
					if (model != nullptr)
					{
						SolidEdgeGeometry::BodyPtr body = model->Body;
						if (body != nullptr)
						{
							SetBodyColor(body, pCurrentStyle);
						}
					}
				}
				catch (_com_error& error)
				{
					continue;
				}
			}
		}
	}
	catch (_com_error& error)
	{
		return -3;
	}
}

int AddinMenu::SetBodyColor(SolidEdgeGeometry::BodyPtr pBody, FaceStylePtr pStyle)
{
	if (pBody == nullptr || pStyle == nullptr)
	{
		return -1;
	}

	// body
	pBody->PutStyle(pStyle);

	//faces
	//SolidEdgeGeometry::FacesPtr faceList = pBody->Faces[SolidEdgeGeometry::FeatureTopologyQueryTypeConstants::igQueryAll];
	//long lCount = faceList->Count;
	//for (long i = 1; i <= lCount; ++i)
	//{
	//	SolidEdgeGeometry::FacePtr face = faceList->Item(i);
	//	if (face != nullptr)
	//	{
	//		face->PutStyle(pStyle);
	//	}
	//}

	return 0;
}

void AddinMenu::CreateHoles(SolidEdgePart::PartDocumentPtr partDocument)
{
	SolidEdgePart::RefPlanesPtr refplanes = NULL;
	SolidEdgePart::RefPlanePtr refplane = NULL;
	SolidEdgePart::ModelsPtr models = NULL;
	SolidEdgePart::ModelPtr model = NULL;
	SolidEdgePart::HoleDataCollectionPtr holeDataCollection = NULL;
	SolidEdgePart::ProfileSetsPtr profileSets = NULL;
	SolidEdgePart::ProfileSetPtr profileSet = NULL;
	SolidEdgePart::ProfilesPtr profiles = NULL;
	SolidEdgePart::ProfilePtr profile = NULL;
	SolidEdgePart::Holes2dPtr holes2d = NULL;
	SolidEdgePart::Hole2dPtr hole2d = NULL;
	SolidEdgePart::HolesPtr holes = NULL;
	SolidEdgePart::HolePtr hole = NULL;
	long profileStatus = 0;

	// Get a reference to the RefPlanes collection.
	refplanes = partDocument->RefPlanes;

	// Get a reference to Front (xz) plane.
	refplane = refplanes->Item(1);

	// Get a reference to the Models collection.
	models = partDocument->Models;

	// Get a reference to Model.
	model = models->Item(1);

	// Get a reference to the ProfileSets collection.
	profileSets = partDocument->ProfileSets;

	// Add new ProfileSet.
	profileSet = profileSets->Add();

	// Get a reference to the Profiles collection.
	profiles = profileSet->Profiles;

	// Add new Profile.
	profile = profiles->Add(refplane);

	// Get a reference to the Holes2d collection.
	holes2d = profile->Holes2d;

	// Add new Hole2d.
	hole2d = holes2d->Add(
		0.01,
		0.01);

	// Close profile.
	profileStatus = profile->End(SolidEdgePart::ProfileValidationType::igProfileClosed);

	profile->Visible = false;

	// Get a reference to the HoleDataCollection collection.
	holeDataCollection = partDocument->HoleDataCollection;

	// Add new HoleData.
	SolidEdgePart::HoleDataPtr holeData = holeDataCollection->Add(
		SolidEdgePart::FeaturePropertyConstants::igRegularHole,
		0.005,
		90);

	// Get a reference to the Holes collection.
	holes = model->Holes;

	// Add hole.
	hole = holes->AddFinite(
		profile,
		SolidEdgePart::FeaturePropertyConstants::igRight,
		0.005,
		holeData);
}

bool AddinMenu::ActiveMouse()
{
	MessageBox(NULL, TEXT("激活鼠标 !"), TEXT("错误！"), MB_OK | MB_YESNO);

	// 激活工具
	SEAddin* pAddin = CSampleAddinApp::GetSEApp()->GetSEAddin();
	if (!pAddin)
		return false;

	CDemoCommandObj* pTheCommand = NULL;
	CDemoCommandObj::CreateInstance(&pTheCommand);
	if (!pTheCommand)
		return false;

	pTheCommand->AddRef();
	pTheCommand->SetCommandsObject(pAddin->GetCommands());
	HRESULT hr = pTheCommand->CreateCommand(SolidEdgeConstants::seNoDeactivate);
	if (SUCCEEDED(hr))
	{
		// Very very important! Start the command so events will start flowing this way.
		pTheCommand->GetCommand()->Start();
	}

	return false;

}