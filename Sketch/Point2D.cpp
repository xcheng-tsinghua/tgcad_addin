#include "Point2D.h"
#include <cmath>

#ifndef M_PI
#define M_PI 3.141592653589793
#endif

Point2D::Point2D()
{
	m_x = 0;
	m_y = 0;
}

Point2D::Point2D(const double& px, const double& py)
{
	m_x = px;
	m_y = py;
}

Point2D::Point2D(Point2D start, Point2D end)
{
	m_x = end.X() - start.X();
	m_y = end.Y() - start.Y();
}

Point2D::~Point2D()
{
}

double Point2D::Magnitude() const
{
	return sqrt(m_x * m_x + m_y * m_y);
}

double Point2D::Distance(Point2D other) const
{
	double dx = m_x - other.X();
	double dy = m_y - other.Y();

	return sqrt(dx * dx + dy * dy);
}

void Point2D::Normalize_()
{
	double mag = this->Magnitude();
	m_x /= mag;
	m_y /= mag;

}

Point2D Point2D::Normalize() const
{
	double mag = this->Magnitude();
	return Point2D(m_x / mag, m_y / mag);
}

double Point2D::X() const
{
	return m_x;
}

double Point2D::Y() const
{
	return m_y;
}

void Point2D::SetX(const double& px)
{
	m_x = px;
}

void Point2D::SetY(const double& py)
{
	m_y = py;
}

void Point2D::SetXY(const double& px, const double& py)
{
	m_x = px;
	m_y = py;

}

double Point2D::Angle(Point2D other) const
{
	double product_num = (*this) * other;
	product_num = product_num / (this->Magnitude() * other.Magnitude());

	return acos(product_num);

}

double Point2D::AcuteAngle(Point2D other) const
{
	double ang = this->Angle(other);

	if (ang > (M_PI / 2.0))
	{
		ang = M_PI - ang;
	}

	return ang;

}

double Point2D::RotAngle() const
{
	if (this->Magnitude() < 1e-6)
	{
		//cout << "计算圆心角时，输入了零向量" << endl;
		return 0.0;
	}

	// 相当于计算(1, 0)到 this 之间的夹角
	double theta = this->Angle(Point2D(1, 0));

	if (m_y >= 0)
	{
		return theta;
	}
	else
	{
		return 2 * M_PI - theta;
	}

}

bool Point2D::IsParallelTo(Point2D other, const double& angular_tol) const
{
	double ang = this->AcuteAngle(other);

	if (ang < angular_tol)
	{
		return true;
	}
	else
	{
		return false;
	}

}

double Point2D::Cross(Point2D other) const
{
	double crossed_z = m_x * other.Y() - m_y * other.X();
	return crossed_z;

}

double Point2D::operator*(Point2D other) const
{
	return m_x * other.X() + m_y * other.Y();
}

Point2D Point2D::operator+(Point2D other) const
{
	return Point2D(m_x + other.X(), m_y + other.Y());
}

Point2D Point2D::operator-(Point2D other) const
{
	return Point2D(m_x - other.X(), m_y - other.Y());
}

Point2D operator*(double mag, Point2D me)
{
	return Point2D(mag * me.X(), mag * me.Y());
}

PROCESS_API Point2D operator-(Point2D me)
{
	return -1.0 * me;
}
