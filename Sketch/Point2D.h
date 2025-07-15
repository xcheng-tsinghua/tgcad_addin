#pragma once

#ifdef SKETCH
#define PROCESS_API _declspec(dllexport)
#else
#define PROCESS_API _declspec(dllimport)
#endif

#ifndef M_ZERO
#define M_ZERO 1e-6
#endif


// ��ά�㣬�ɱ�ʾƽ����������
class PROCESS_API Point2D
{
public:
public:
	Point2D();
	Point2D(const double& px, const double& py);
	Point2D(Point2D start, Point2D end);  // ������start��end������
	~Point2D();

	// ��������
	double Magnitude() const;

	// this �� other ֮��ľ���
	double Distance(Point2D other) const;

	// ���ȵ�λ��
	void Normalize_();
	Point2D Normalize() const;

	double X() const;
	double Y() const;

	void SetX(const double& px);
	void SetY(const double& py);
	void SetXY(const double& px, const double& py);

	// ���� self �� other ֮��ļнǣ���Χ[0, PI]
	double Angle(Point2D other) const;

	// ���� self �� other ֮�����ǣ���Χ[0, PI / 2]
	double AcuteAngle(Point2D other) const;

	// ���� self �� +X ����нǣ���ʱ�뷽��ת������+X��ʼ����Χ[0, 2 * PI]
	double RotAngle() const;

	// �ж��Ƿ���Ŀ�귽��ƽ��
	bool IsParallelTo(Point2D other, const double& angular_tol = M_ZERO) const;

	// ���� self(x1, y1, 0) �� other(x2, y2, 0) ֮������ Z ����
	double Cross(Point2D other) const;

	// ����������
	double operator * (Point2D other) const;

	// �������
	Point2D operator + (Point2D other) const;

	// �������
	Point2D operator - (Point2D other) const;


private:
	double m_x;
	double m_y;


};

// ʵ�����ά�������
PROCESS_API Point2D operator *(double mag, Point2D me);

PROCESS_API Point2D operator -(Point2D me);
