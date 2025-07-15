#pragma once

#ifdef SKETCH
#define PROCESS_API _declspec(dllexport)
#else
#define PROCESS_API _declspec(dllimport)
#endif

#ifndef M_ZERO
#define M_ZERO 1e-6
#endif


// 二维点，可表示平面点或者向量
class PROCESS_API Point2D
{
public:
public:
	Point2D();
	Point2D(const double& px, const double& py);
	Point2D(Point2D start, Point2D end);  // 创建从start到end的向量
	~Point2D();

	// 向量长度
	double Magnitude() const;

	// this 与 other 之间的距离
	double Distance(Point2D other) const;

	// 长度单位化
	void Normalize_();
	Point2D Normalize() const;

	double X() const;
	double Y() const;

	void SetX(const double& px);
	void SetY(const double& py);
	void SetXY(const double& px, const double& py);

	// 计算 self 和 other 之间的夹角，范围[0, PI]
	double Angle(Point2D other) const;

	// 计算 self 和 other 之间的锐角，范围[0, PI / 2]
	double AcuteAngle(Point2D other) const;

	// 计算 self 与 +X 方向夹角，逆时针方向转动，从+X开始，范围[0, 2 * PI]
	double RotAngle() const;

	// 判断是否与目标方向平行
	bool IsParallelTo(Point2D other, const double& angular_tol = M_ZERO) const;

	// 计算 self(x1, y1, 0) 与 other(x2, y2, 0) 之间叉积的 Z 分量
	double Cross(Point2D other) const;

	// 向量数量积
	double operator * (Point2D other) const;

	// 向量相加
	Point2D operator + (Point2D other) const;

	// 向量相减
	Point2D operator - (Point2D other) const;


private:
	double m_x;
	double m_y;


};

// 实数与二维向量相乘
PROCESS_API Point2D operator *(double mag, Point2D me);

PROCESS_API Point2D operator -(Point2D me);
