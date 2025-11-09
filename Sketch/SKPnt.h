#pragma once

#ifdef SKETCH
#define PROCESS_API _declspec(dllexport)
#else
#define PROCESS_API _declspec(dllimport)
#endif

class PROCESS_API SKPnt
{
public:
	SKPnt();
	SKPnt(double fpX, double fpY, double fpZ);
	~SKPnt();

	double X();
	double Y();
	double Z();

	void SetX(double fpX);
	void SetY(double fpY);
	void SetZ(double fpZ);

	void SetXYZ(double fpX, double fpY, double fpZ);

private:
	double m_X;
	double m_Y;
	double m_Z;

};

