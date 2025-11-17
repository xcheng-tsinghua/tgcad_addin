#include "stdafx.h"
#include "SEUtils.h"
#include "SampleAddin.h"
#include "SKUtils.h"


namespace SEUtils
{

	SKPnt ScreenToModel(SKPnt_2d screen_pnt, WindowPtr wnd)
	{
		double WorldX = 0, WorldY = 0, WorldZ = 0;

		wnd->View->TransformDCToModel(screen_pnt.X(), screen_pnt.Y(), &WorldX, &WorldY, &WorldZ);

		return SKPnt(WorldX, WorldY, WorldZ);
	}

	SKPnt ScreenToModel(const double& sc_x, const double& sc_y, WindowPtr wnd)
	{
		return ScreenToModel(SKPnt_2d(sc_x, sc_y), wnd);
	}

	void UpdateScreen(WindowPtr wnd)
	{
		// 当指针为空时出现提示
		if (!wnd)
		{
			MesgBox("更新显示时输入了空窗口指针！");
		}
		else
		{
			wnd->View->Update();
		}

	}

	double ModelDistToScreen(SKPnt model_pnt1, SKPnt model_pnt2, WindowPtr wnd)
	{
		SKPnt_2d ScreenPnt1 = ModelToScreen(model_pnt1, wnd);
		SKPnt_2d ScreenPnt2 = ModelToScreen(model_pnt2, wnd);

		return ScreenPnt1.Distance(ScreenPnt2);

	}

	double ModelDistToScreen(const double& SapceDist, WindowPtr wnd)
	{
		return ModelDistToScreen(SKPnt(0, 0, 0), SKPnt(SapceDist, 0, 0), wnd);
	}

	double RefDistToScreen(SKPnt_2d ref_pnt1, SKPnt_2d ref_pnt2, WindowPtr wnd)
	{
		SKPnt_2d screen_pnt1 = RefToScreen(ref_pnt1, wnd);
		SKPnt_2d screen_pnt2 = RefToScreen(ref_pnt2, wnd);

		return screen_pnt1.Distance(screen_pnt2);

	}

	double RefDistToScreen(const double& ref_dist, WindowPtr wnd)
	{
		return RefDistToScreen(SKPnt_2d(0.0, 0.0), SKPnt_2d(ref_dist, 0.0), wnd);

	}

	SKPnt RefToModel(SKPnt_2d ref_pnt)
	{
		double model_x = 0.0, model_y = 0.0, model_z = 0.0;
		ProfilePtr(PartDocumentPtr(CSampleAddinApp::GetApplication()->GetActiveDocument())->ActiveSketch)->Convert2DCoordinate(ref_pnt.X(), ref_pnt.Y(), &model_x, &model_y, &model_z);

		return SKPnt(model_x, model_y, model_z);

	}

	SKPnt_2d RefToScreen(SKPnt_2d ref_pnt, WindowPtr wnd)
	{
		// 先将参考面上的点转化为模型空间中的点
		SKPnt model_pnt = RefToModel(ref_pnt);

		// 再将模型空间中的点转化为屏幕上的点
		SKPnt_2d screen_pnt = ModelToScreen(model_pnt, wnd);

		return screen_pnt;

	}

	vector<SKPnt_2d> RefToScreen(vector<SKPnt_2d> ref_pnt, WindowPtr wnd)
	{
		vector<SKPnt_2d> screen_pnts;
		for (const auto& c_ref_pnt : ref_pnt)
		{
			screen_pnts.push_back(RefToScreen(c_ref_pnt, wnd));
		}

		return screen_pnts;

	}


	void ViewTo(double vx, double vy, double vz, double tx, double ty, double tz, double ux, double uy, double uz, double scale, WindowPtr wnd)
	{
		double EyeX, EyeY, EyeZ, TargetX, TargetY, TargetZ, UpX, UpY, UpZ, ScaleOrAngle;
		VARIANT_BOOL Perspective;

		wnd->View->GetCamera(&EyeX, &EyeY, &EyeZ, &TargetX, &TargetY, &TargetZ, &UpX, &UpY, &UpZ, &Perspective, &ScaleOrAngle);

		double EyeXSt = EyeX, EyeYSt = EyeY, EyeZSt = EyeZ, TargetXSt = TargetX, TargetYSt = TargetY, TargetZSt = TargetZ, UpXSt = UpX, UpYSt = UpY, UpZSt = UpZ, ScaleOrAngleSt = ScaleOrAngle;
		double EyeXEd = vx, EyeYEd = vy, EyeZEd = vz, TargetXEd = tx, TargetYEd = ty, TargetZEd = tz, UpXEd = ux, UpYEd = uy, UpZEd = uz, ScaleOrAngleEd = scale;

		int ItaNum = 20;
		for (int i = 0; i < ItaNum; i++)
		{
			double currEyeX = EyeXSt + ((EyeXEd - EyeXSt) / (ItaNum - 1)) * i;
			double currEyeY = EyeYSt + ((EyeYEd - EyeYSt) / (ItaNum - 1)) * i;
			double currEyeZ = EyeZSt + ((EyeZEd - EyeZSt) / (ItaNum - 1)) * i;
			double currTarX = TargetXSt + ((TargetXEd - TargetXSt) / (ItaNum - 1)) * i;
			double currTarY = TargetYSt + ((TargetYEd - TargetYSt) / (ItaNum - 1)) * i;
			double currTarZ = TargetZSt + ((TargetZEd - TargetZSt) / (ItaNum - 1)) * i;
			double currUpX = UpXSt + ((UpXEd - UpXSt) / (ItaNum - 1)) * i;
			double currUpY = UpYSt + ((UpYEd - UpYSt) / (ItaNum - 1)) * i;
			double currUpZ = UpZSt + ((UpZEd - UpZSt) / (ItaNum - 1)) * i;
			double currScale = ScaleOrAngleSt + ((ScaleOrAngleEd - ScaleOrAngleSt) / (ItaNum - 1)) * i;

			static_cast<WindowPtr>(CSampleAddinApp::GetApplication()->GetWindows()->Item(1))->View->SetCamera(currEyeX, currEyeY, currEyeZ, currTarX, currTarY, currTarZ, currUpX, currUpY, currUpZ, 0, currScale);

			static_cast<WindowPtr>(CSampleAddinApp::GetApplication()->GetWindows()->Item(1))->View->Update();

			//Sleep(2000.0 / ItaNum);

		}


	}

	void MesgBox(const char* _mesg)
	{
		CString ErrorMessage = _mesg;
		MessageBox(NULL, ErrorMessage, TEXT("information"), MB_OK | MB_YESNO);

	}

	void MesgBox(CString _mesg)
	{
		MessageBox(NULL, _mesg, TEXT("information"), MB_OK | MB_YESNO);
	}

	SKPnt_2d ScreenToRef(double fpCursorX, double fpCursorY, WindowPtr wnd)
	{
		// 先将鼠标坐标转化为模型三维坐标系坐标
		double WorldX = 0, WorldY = 0, WorldZ = 0;
		SKPnt model_pnt = ScreenToModel(fpCursorX, fpCursorY, wnd);

		// 再将模型三维坐标系坐标转化为参考面二维坐标
		double refX = 0, refY = 0;
		ProfilePtr(PartDocumentPtr(CSampleAddinApp::GetApplication()->GetActiveDocument())->ActiveSketch)->Convert3DCoordinate(model_pnt.X(), model_pnt.Y(), model_pnt.Z(), &refX, &refY);

		return SKPnt_2d(refX, refY);

	}

	SKPnt_2d ScreenToRef(SKPnt_2d screen_pnt, WindowPtr wnd)
	{
		return ScreenToRef(screen_pnt.X(), screen_pnt.Y(), wnd);

	}

	double ScreenDistToRef(SKPnt_2d screen_pnt1, SKPnt_2d screen_pnt2, WindowPtr wnd)
	{
		// 先将屏幕坐标转化为参考面坐标
		SKPnt_2d ref_pnt1 = ScreenToRef(screen_pnt1, wnd);
		SKPnt_2d ref_pnt2 = ScreenToRef(screen_pnt2, wnd);

		return ref_pnt1.Distance(ref_pnt2);

	}

	double ScreenDistToRef(const double& scren_dist, WindowPtr wnd)
	{
		return ScreenDistToRef(SKPnt_2d(0.0, 0.0), SKPnt_2d(0.0, scren_dist), wnd);
	}

	SKPnt_2d ViewToRef(const double& view_x, const double& view_y, const double& view_z, WindowPtr wnd)
	{
		// 先将当前视图xyz转化到模型坐标系
		double model_x = 0, model_y = 0, model_z = 0;
		wnd->View->TransformViewToModel(view_x, view_y, view_z, &model_x, &model_y, &model_z);

		// 再将模型坐标系xyz转化为草图参考面局部坐标
		double refX = 0, refY = 0;
		ProfilePtr(PartDocumentPtr(CSampleAddinApp::GetApplication()->GetActiveDocument())->ActiveSketch)->Convert3DCoordinate(model_x, model_y, model_z, &refX, &refY);

		return SKPnt_2d(refX, refY);

	}

	SKPnt_2d ViewToScreen(const double& view_x, const double& view_y, const double& view_z, WindowPtr wnd)
	{
		// 先将当前视图xyz转化到模型坐标系
		double model_x = 0, model_y = 0, model_z = 0;
		wnd->View->TransformViewToModel(view_x, view_y, view_z, &model_x, &model_y, &model_z);

		// 再将模型坐标系xyz转化为屏幕像素坐标
		long dc_x = 0, dc_y = 0;
		wnd->View->TransformModelToDC(model_x, model_y, model_z, &dc_x, &dc_y);

		return SKPnt_2d(dc_x, dc_y);

	}

	SKPnt_2d ModelToScreen(const double& model_x, const double& model_y, const double& model_z, WindowPtr wnd)
	{
		long dc_x = 0, dc_y = 0;
		wnd->View->TransformModelToDC(model_x, model_y, model_z, &dc_x, &dc_y);

		return SKPnt_2d(dc_x, dc_y);

	}

	SKPnt_2d ModelToScreen(SKPnt model_pnt, WindowPtr wnd)
	{
		return ModelToScreen(model_pnt.X(), model_pnt.Y(), model_pnt.Z(), wnd);

	}

}

