#pragma once

#include <vector>

#include "SKPnt.h"
#include "SKPnt_2d.h"

using std::pair;
using std::vector;

namespace SEUtils
{
	////////////////////////////////////////////// 坐标转化 ////
	// 将当前模型空间下的xyz转化为屏幕像素坐标
	SKPnt_2d ModelToScreen(const double& model_x, const double& model_y, const double& model_z, WindowPtr wnd);
	SKPnt_2d ModelToScreen(SKPnt model_pnt, WindowPtr wnd);

	// 计算模型空间中的两个点之间的距离转化为屏幕距离
	double ModelDistToScreen(SKPnt model_pnt1, SKPnt model_pnt2, WindowPtr wnd);
	double ModelDistToScreen(const double& SapceDist, WindowPtr wnd);

	// 将草图参考面上的点转化为模型空间中的点
	SKPnt RefToModel(SKPnt_2d ref_pnt);

	// 将草图参考面上的点转化为屏幕中的点
	SKPnt_2d RefToScreen(SKPnt_2d ref_pnt, WindowPtr wnd);
	vector<SKPnt_2d> RefToScreen(vector<SKPnt_2d> ref_pnt, WindowPtr wnd);

	// 将草图参考面上的距离转化为屏幕像素距离
	double RefDistToScreen(SKPnt_2d ref_pnt1, SKPnt_2d ref_pnt2, WindowPtr wnd);
	double RefDistToScreen(const double& ref_dist, WindowPtr wnd);

	// 将屏幕上的像素坐标转化为模型三维坐标系中的点坐标
	SKPnt ScreenToModel(SKPnt_2d screen_pnt, WindowPtr wnd);
	SKPnt ScreenToModel(const double& sc_x, const double& sc_y, WindowPtr wnd);

	// 将鼠标坐标转化为当前激活的草图参考面坐标
	SKPnt_2d ScreenToRef(double fpCursorX, double fpCursorY, WindowPtr wnd);
	SKPnt_2d ScreenToRef(SKPnt_2d screen_pnt, WindowPtr wnd);

	// 将屏幕上的两点之间距离转化为参考面上的距离
	double ScreenDistToRef(SKPnt_2d screen_pnt1, SKPnt_2d screen_pnt2, WindowPtr wnd);
	double ScreenDistToRef(const double& scren_dist, WindowPtr wnd);

	// 将当前视图下的xyz（put_WindowTypes(1)时，从鼠标事件中获取的坐标）转化为当前激活的草图参考面坐标
	SKPnt_2d ViewToRef(const double& view_x, const double& view_y, const double& view_z, WindowPtr wnd);

	// 将当前视图下的xyz（put_WindowTypes(1)时，即从鼠标事件中获取的坐标）转化为屏幕像素坐标
	SKPnt_2d ViewToScreen(const double& view_x, const double& view_y, const double& view_z, WindowPtr wnd);




	////////////////////////////////////////////// 其他功能 ////

	// 将视角平滑转化到目标视角
	void ViewTo(double vx, double vy, double vz, double tx, double ty, double tz, double ux, double uy, double uz, double scale, WindowPtr wnd);

	// 刷新窗口显示
	void UpdateScreen(WindowPtr wnd);

	// 显示消息
	void MesgBox(const char* _mesg);
	void MesgBox(CString _mesg);

	// 在状态栏显示消息
	void ShowStatusBarInfo(_bstr_t const& status);



};

