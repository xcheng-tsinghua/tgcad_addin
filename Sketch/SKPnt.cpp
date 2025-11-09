#include "SKPnt.h"


SKPnt::SKPnt()
{
	m_X = 0;
	m_Y = 0;
	m_Z = 0;
}

SKPnt::SKPnt(double fpX, double fpY, double fpZ)
{
	m_X = fpX;
	m_Y = fpY;
	m_Z = fpZ;
}

SKPnt::~SKPnt()
{

}

double SKPnt::X()
{
	return m_X;
}

double SKPnt::Y()
{
	return m_Y;
}

double SKPnt::Z()
{
	return m_Z;
}

void SKPnt::SetX(double fpX)
{
	m_X = fpX;
}

void SKPnt::SetY(double fpY)
{
	m_Y = fpY;
}

void SKPnt::SetZ(double fpZ)
{
	m_Z = fpZ;
}

void SKPnt::SetXYZ(double fpX, double fpY, double fpZ)
{
	m_X = fpX;
	m_Y = fpY;
	m_Z = fpZ;
}



