#pragma once

#ifdef SKETCH
#define PROCESS_API _declspec(dllexport)
#else
#define PROCESS_API _declspec(dllimport)
#endif

#include "SKUtils.h"


// 二维点，可表示平面点或者向量
class PROCESS_API SKPnt_2d
{
public:
	SKPnt_2d();
	SKPnt_2d(const double& px, const double& py);
	SKPnt_2d(SKPnt_2d start, SKPnt_2d end);  // 创建从start到end的向量
	~SKPnt_2d();

	// 向量长度
	double Magnitude() const;

	// this 与 other 之间的距离
	double Distance(SKPnt_2d other) const;

	// 长度单位化
	void Normalize_();
	SKPnt_2d Normalize() const;

	double X() const;
	double Y() const;

	void SetX(const double& px);
	void SetY(const double& py);
	void SetXY(const double& px, const double& py);

	// 计算 self 和 other 之间的夹角，范围[0, PI]
	double Angle(SKPnt_2d other) const;

	// 计算 self 和 other 之间的锐角，范围[0, PI / 2]
	double AcuteAngle(SKPnt_2d other) const;

	// 计算 self 与 +X 方向夹角，逆时针方向转动，从+X开始，范围[0, 2 * PI]
	double RotAngle() const;

	// 判断是否与目标方向平行
	bool IsParallelTo(SKPnt_2d other, const double& angular_tol = M_ZERO) const;

	// 计算 self(x1, y1, 0) 与 other(x2, y2, 0) 之间叉积的 Z 分量
	double Cross(SKPnt_2d other) const;

	// 向量数量积
	double operator * (SKPnt_2d other) const;

	// 向量相加
	SKPnt_2d operator + (SKPnt_2d other) const;

	// 向量相减
	SKPnt_2d operator - (SKPnt_2d other) const;


private:
	double m_x;
	double m_y;


};

// 实数与二维向量相乘
PROCESS_API SKPnt_2d operator *(double mag, SKPnt_2d me);

PROCESS_API SKPnt_2d operator -(SKPnt_2d me);
