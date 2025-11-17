#include "SKPnt_2d.h"
#include <cmath>

#ifndef M_PI
#define M_PI 3.141592653589793
#endif

SKPnt_2d::SKPnt_2d()
{
	m_x = 0;
	m_y = 0;
}

SKPnt_2d::SKPnt_2d(const double& px, const double& py)
{
	m_x = px;
	m_y = py;
}

SKPnt_2d::SKPnt_2d(const int& px, const int& py)
{
	m_x = px;
	m_y = py;
}

SKPnt_2d::SKPnt_2d(SKPnt_2d start, SKPnt_2d end)
{
	m_x = end.X() - start.X();
	m_y = end.Y() - start.Y();
}

SKPnt_2d::~SKPnt_2d()
{
}

double SKPnt_2d::Magnitude() const
{
	return sqrt(m_x * m_x + m_y * m_y);
}

double SKPnt_2d::Distance(SKPnt_2d other) const
{
	double dx = m_x - other.X();
	double dy = m_y - other.Y();

	return sqrt(dx * dx + dy * dy);
}

void SKPnt_2d::Normalize_()
{
	double mag = this->Magnitude();
	m_x /= mag;
	m_y /= mag;

}

SKPnt_2d SKPnt_2d::Normalize() const
{
	double mag = this->Magnitude();
	return SKPnt_2d(m_x / mag, m_y / mag);
}

double SKPnt_2d::X() const
{
	return m_x;
}

double SKPnt_2d::Y() const
{
	return m_y;
}

void SKPnt_2d::SetX(const double& px)
{
	m_x = px;
}

void SKPnt_2d::SetY(const double& py)
{
	m_y = py;
}

void SKPnt_2d::SetXY(const double& px, const double& py)
{
	m_x = px;
	m_y = py;

}

double SKPnt_2d::Angle(SKPnt_2d other) const
{
	double product_num = (*this) * other;
	product_num = product_num / (this->Magnitude() * other.Magnitude());

	return acos(product_num);

}

double SKPnt_2d::AcuteAngle(SKPnt_2d other) const
{
	double ang = this->Angle(other);

	if (ang > (M_PI / 2.0))
	{
		ang = M_PI - ang;
	}

	return ang;

}

double SKPnt_2d::RotAngle() const
{
	if (this->Magnitude() < 1e-6)
	{
		//cout << "计算圆心角时，输入了零向量" << endl;
		return 0.0;
	}

	// 相当于计算(1, 0)到 this 之间的夹角
	double theta = this->Angle(SKPnt_2d(1, 0));

	if (m_y >= 0)
	{
		return theta;
	}
	else
	{
		return 2 * M_PI - theta;
	}

}

bool SKPnt_2d::IsParallelTo(SKPnt_2d other, const double& angular_tol) const
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

double SKPnt_2d::Cross(SKPnt_2d other) const
{
	double crossed_z = m_x * other.Y() - m_y * other.X();
	return crossed_z;

}

double SKPnt_2d::operator*(SKPnt_2d other) const
{
	return m_x * other.X() + m_y * other.Y();
}

SKPnt_2d SKPnt_2d::operator+(SKPnt_2d other) const
{
	return SKPnt_2d(m_x + other.X(), m_y + other.Y());
}

SKPnt_2d SKPnt_2d::operator-(SKPnt_2d other) const
{
	return SKPnt_2d(m_x - other.X(), m_y - other.Y());
}

SKPnt_2d operator*(double mag, SKPnt_2d me)
{
	return SKPnt_2d(mag * me.X(), mag * me.Y());
}

PROCESS_API SKPnt_2d operator-(SKPnt_2d me)
{
	return -1.0 * me;
}
