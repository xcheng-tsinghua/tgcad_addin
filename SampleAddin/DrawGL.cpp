#include "stdafx.h"
#include "DrawGL.h"
#include <iostream>
#include "SampleAddin.h"
#include "SEAddin.h"
#include <fstream>


#pragma region DrawGL


HRESULT DrawGL::SetView(Window* pWindow)
{
	ViewPtr pView = NULL;

	if (pWindow != NULL)
	{
		pView = pWindow->View;
	}

	return SetView(pView);
}

HRESULT DrawGL::SetView(View* pView)
{
	HRESULT hr = S_OK;

	hr = UnadviseFromEvents();

	m_pView = pView;

	if (m_pView != NULL)
	{
		XViewEventsObj::CreateInstance(&m_pViewEventsObj);
		m_pViewEventsObj->AddRef();
		hr = m_pViewEventsObj->Connect(m_pView->ViewEvents);
		m_pViewEventsObj->m_pMyViewOverlay = this;

		XhDCDisplayEventsObj::CreateInstance(&m_phDCDisplayEventsObj);
		m_phDCDisplayEventsObj->AddRef();
		hr = m_phDCDisplayEventsObj->Connect(m_pView->DisplayEvents);
		m_phDCDisplayEventsObj->m_pMyViewOverlay = this;

		XGLDisplayEventsObj::CreateInstance(&m_pGLDisplayEventsObj);
		m_pGLDisplayEventsObj->AddRef();
		hr = m_pGLDisplayEventsObj->Connect(m_pView->GLDisplayEvents);
		m_pGLDisplayEventsObj->m_pMyViewOverlay = this;
	}

	return hr;
}

HRESULT DrawGL::ShowBoundingBox()
{
	m_bBoundingBox = true;
	m_pView->Update();
	return S_OK;
}

HRESULT DrawGL::HideBoundingBox()
{
	m_bBoundingBox = false;
	m_pView->Update();
	return S_OK;
}

HRESULT DrawGL::ShowCubes()
{
	m_bOpenGLBoxes = true;
	m_pView->Update();
	return S_OK;
}

HRESULT DrawGL::HideCubes()
{
	m_bOpenGLBoxes = false;
	m_pView->Update();
	return S_OK;
}

HRESULT DrawGL::ShowSkh()
{
	m_bShowSkh = true;
	m_pView->Update();
	return S_OK;
}

HRESULT DrawGL::HideSkh()
{
	m_bShowSkh = false;
	m_pView->Update();
	return S_OK;
}

void DrawGL::DrawOpenGlBoundingBox(LPGL pGL)
{
	double dMinX = 0.0;
	double dMinY = 0.0;
	double dMinZ = 0.0;
	double dMaxX = 0.0;
	double dMaxY = 0.0;
	double dMaxZ = 0.0;

	this->m_pView->GetModelRange(&dMinX, &dMinY, &dMinZ, &dMaxX, &dMaxY, &dMaxZ);

	if (dMinX == 0 && dMinY == 0 && dMinZ == 0 && dMaxX == 0 && dMaxY == 0 && dMaxZ == 0)
	{
		return;
	}

	pGL->glColor3i(1, 1, 0); // Yellow
	pGL->glLineWidth(2);
	pGL->glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

	{
		pGL->glBegin(GL_LINE_LOOP);

		pGL->glVertex3d(dMinX, dMinY, dMaxZ);
		pGL->glVertex3d(dMaxX, dMinY, dMaxZ);
		pGL->glVertex3d(dMaxX, dMaxY, dMaxZ);
		pGL->glVertex3d(dMinX, dMaxY, dMaxZ);

		pGL->glEnd();
	}

	{
		pGL->glBegin(GL_LINE_LOOP);

		pGL->glVertex3d(dMinX, dMinY, dMinZ);
		pGL->glVertex3d(dMaxX, dMinY, dMinZ);
		pGL->glVertex3d(dMaxX, dMaxY, dMinZ);
		pGL->glVertex3d(dMinX, dMaxY, dMinZ);

		pGL->glEnd();
	}

	{
		pGL->glBegin(GL_LINES);

		pGL->glVertex3d(dMinX, dMinY, dMinZ);
		pGL->glVertex3d(dMinX, dMinY, dMaxZ);

		pGL->glVertex3d(dMaxX, dMaxY, dMinZ);
		pGL->glVertex3d(dMaxX, dMaxY, dMaxZ);

		pGL->glVertex3d(dMinX, dMaxY, dMinZ);
		pGL->glVertex3d(dMinX, dMaxY, dMaxZ);

		pGL->glVertex3d(dMaxX, dMinY, dMinZ);
		pGL->glVertex3d(dMaxX, dMinY, dMaxZ);

		pGL->glEnd();
	}

	{
		pGL->glColor3f(1, 0, 0);
		pGL->glBegin(GL_LINES);

		// Diagonal line between min & max points.
		pGL->glVertex3d(dMinX, dMinY, dMinZ);
		pGL->glVertex3d(dMaxX, dMaxY, dMaxZ);

		pGL->glEnd();
	}
}

void DrawGL::DrawOpenGlBoxes(LPGL pGL)
{
	float fSize = 0.025f;
	double matrix0[16] = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };
	double matrix1[16] = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, fSize, 1 };
	double matrix2[16] = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, fSize, -fSize, 1 };
	double matrix3[16] = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, fSize, -fSize, 0, 1 };

	pGL->glMatrixMode(GL_MODELVIEW);


	GLint mode;
	GLint depth;
	GLenum error;

	error = pGL->glGetError();

	const GLubyte* vendor = pGL->glGetString(GL_VENDOR);
	const GLubyte* renderer = pGL->glGetString(GL_RENDERER);
	const GLubyte* version = pGL->glGetString(GL_VERSION);
	const GLubyte* extensions = pGL->glGetString(GL_EXTENSIONS);

	pGL->glGetIntegerv(GL_MATRIX_MODE, &mode);

	pGL->glGetIntegerv(GL_MODELVIEW_STACK_DEPTH, &depth);
	pGL->glPushMatrix();
	pGL->glGetIntegerv(GL_MODELVIEW_STACK_DEPTH, &depth);

	pGL->glLoadMatrixd(matrix0);
	pGL->glColor3f(1, 0, 0);
	DrawOpenGlBoxes(pGL, fSize / 2.0f);

	pGL->glPopMatrix();


	pGL->glPushMatrix();

	pGL->glMultMatrixd(matrix1);
	pGL->glColor3f(0, 1, 0);
	DrawOpenGlBoxes(pGL, fSize / 2.0f);

	pGL->glMultMatrixd(matrix2);
	pGL->glColor3f(0, 0, 1);
	DrawOpenGlBoxes(pGL, fSize / 2.0f);

	pGL->glMultMatrixd(matrix3);
	pGL->glColor4f(1, 1, 0, .25f);
	DrawOpenGlBoxes(pGL, fSize / 2.0f);

	pGL->glPopMatrix();
}

void DrawGL::DrawOpenGlBoxes(LPGL pGL, float fSize)
{
	float* p;
	float* n;

	float p0[12] = { 0.0f, 0.0f, 0.0f, 0.0f, fSize, 0.0f, fSize, 0.0f, 0.0f, fSize, fSize, 0.0f };
	float p1[12] = { 0.0f, 0.0f, fSize, 0.0f, fSize, fSize, fSize, 0.0f, fSize, fSize, fSize, fSize };

	float p2[12] = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, fSize, 0.0f, fSize, 0.0f, 0.0f, fSize, fSize };
	float p3[12] = { fSize, 0.0f, 0.0f, fSize, 0.0f, fSize, fSize, fSize, 0.0f, fSize, fSize, fSize };

	float p4[12] = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, fSize, fSize, 0.0f, 0.0f, fSize, 0.0f, fSize };
	float p5[12] = { 0.0f, fSize, 0.0f, 0.0f, fSize, fSize, fSize, fSize, 0.0f, fSize, fSize, fSize };

	float n0[21] = { 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f };

	n = n0;
	p = p0;
	pGL->glBegin(GL_TRIANGLES);
	pGL->glNormal3fv(n); n += 3;
	pGL->glEdgeFlag(TRUE);
	pGL->glVertex3fv(p); p += 3;
	pGL->glEdgeFlag(FALSE);
	pGL->glVertex3fv(p); p += 3;
	pGL->glEdgeFlag(TRUE);
	pGL->glVertex3fv(p); p += 3;

	p -= 6;
	pGL->glNormal3fv(n); n += 3;
	pGL->glEdgeFlag(FALSE);
	pGL->glVertex3fv(p); p += 3;
	pGL->glEdgeFlag(TRUE);
	pGL->glVertex3fv(p); p += 3;
	pGL->glEdgeFlag(TRUE);
	pGL->glVertex3fv(p); p += 3;
	pGL->glEnd();

	p = p1;
	pGL->glBegin(GL_TRIANGLE_STRIP);
	pGL->glNormal3fv(n); n += 3;
	pGL->glEdgeFlag(TRUE);
	pGL->glVertex3fv(p); p += 3;
	pGL->glEdgeFlag(TRUE);
	pGL->glVertex3fv(p); p += 3;
	pGL->glEdgeFlag(TRUE);
	pGL->glVertex3fv(p); p += 3;
	pGL->glEdgeFlag(TRUE);
	pGL->glVertex3fv(p); p += 3;
	pGL->glEnd();

	p = p2;
	pGL->glBegin(GL_TRIANGLE_STRIP);
	pGL->glNormal3fv(n); n += 3;
	pGL->glEdgeFlag(TRUE);
	pGL->glVertex3fv(p); p += 3;
	pGL->glEdgeFlag(TRUE);
	pGL->glVertex3fv(p); p += 3;
	pGL->glEdgeFlag(TRUE);
	pGL->glVertex3fv(p); p += 3;
	pGL->glEdgeFlag(TRUE);
	pGL->glVertex3fv(p); p += 3;
	pGL->glEnd();

	p = p3;
	pGL->glBegin(GL_TRIANGLE_STRIP);
	pGL->glNormal3fv(n); n += 3;
	pGL->glEdgeFlag(TRUE);
	pGL->glVertex3fv(p); p += 3;
	pGL->glEdgeFlag(TRUE);
	pGL->glVertex3fv(p); p += 3;
	pGL->glEdgeFlag(TRUE);
	pGL->glVertex3fv(p); p += 3;
	pGL->glEdgeFlag(TRUE);
	pGL->glVertex3fv(p); p += 3;
	pGL->glEnd();

	p = p4;
	pGL->glBegin(GL_TRIANGLE_STRIP);
	pGL->glNormal3fv(n); n += 3;
	pGL->glEdgeFlag(TRUE);
	pGL->glVertex3fv(p); p += 3;
	pGL->glEdgeFlag(TRUE);
	pGL->glVertex3fv(p); p += 3;
	pGL->glEdgeFlag(TRUE);
	pGL->glVertex3fv(p); p += 3;
	pGL->glEdgeFlag(TRUE);
	pGL->glVertex3fv(p); p += 3;
	pGL->glEnd();

	p = p5;
	pGL->glBegin(GL_TRIANGLE_STRIP);
	pGL->glNormal3fv(n); n += 3;
	pGL->glEdgeFlag(TRUE);
	pGL->glVertex3fv(p); p += 3;
	pGL->glEdgeFlag(TRUE);
	pGL->glVertex3fv(p); p += 3;
	pGL->glEdgeFlag(TRUE);
	pGL->glVertex3fv(p); p += 3;
	pGL->glEdgeFlag(TRUE);
	pGL->glVertex3fv(p); p += 3;
	pGL->glEnd();
}

void DrawGL::DrawStroke(vector<Point2D> stroke_pnt, CDC* dc)
{

	// 创建一个红色、线宽为 3 的实线画笔
	CPen pen(PS_SOLID, 5, RGB(0, 0, 0));

	// 选入到 DC 中，并保存旧画笔
	CPen* pOldPen = dc->SelectObject(&pen);


	int n_points = stroke_pnt.size();
	if (n_points > 2)
	{
		for (int i = 1; i < n_points; i++)
		{
			Point2D sec_start = stroke_pnt[i - 1];
			Point2D sec_end = stroke_pnt[i];

			dc->MoveTo(sec_start.X(), sec_start.Y());
			dc->LineTo(sec_end.X(), sec_end.Y());

		}

	}

}

HRESULT DrawGL::UnadviseFromEvents()
{
	HRESULT hr = S_OK;

	if (m_pViewEventsObj != NULL)
	{
		hr = m_pViewEventsObj->Disconnect(m_pView->ViewEvents);
		m_pViewEventsObj->Release();
		m_pViewEventsObj = NULL;
	}

	if (m_phDCDisplayEventsObj != NULL)
	{
		hr = m_phDCDisplayEventsObj->Disconnect(m_pView->DisplayEvents);
		m_phDCDisplayEventsObj->Release();
		m_phDCDisplayEventsObj = NULL;
	}

	if (m_pGLDisplayEventsObj != NULL)
	{
		hr = m_pGLDisplayEventsObj->Disconnect(m_pView->GLDisplayEvents);
		m_pGLDisplayEventsObj->Release();
		m_pGLDisplayEventsObj = NULL;
	}

	return S_OK;
}

void DrawGL::Append(double x, double y)
{
	m_line_points_current.push_back({ x, y });

}

vector<vector<Point2D>> DrawGL::GetAllPoints()
{
	return m_line_points_all;
}

vector<Point2D> DrawGL::GetCurrentPoints()
{
	return m_line_points_current;
}

void DrawGL::DrawEnd()
{
	m_line_points_all.push_back(m_line_points_current);
	m_line_points_current.clear();
}

void DrawGL::ExportSketchPoints(string save_path, int pen_up, int pen_down)
{
	ofstream of(save_path);

	for (auto c_pnt_list : m_line_points_all)
	{
		for (int i = 0; i < c_pnt_list.size(); i++)
		{
			Point2D c_pnt = c_pnt_list[i];

			if (i == c_pnt_list.size() - 1)
			{
				of << c_pnt.X() << ',' << c_pnt.Y() << ',' << pen_up << endl;
			}
			else
			{
				of << c_pnt.X() << ',' << c_pnt.Y() << ',' << pen_down << endl;
			}

		}

	}

	of.close();

}

void DrawGL::Clear()
{
	m_line_points_all.clear();
	m_line_points_current.clear();

}

#pragma endregion

#pragma region DrawGL::XViewEvents (ISEViewEvents)

HRESULT DrawGL::XViewEvents::raw_Changed()
{
	return S_OK;
}

HRESULT DrawGL::XViewEvents::raw_Destroyed()
{
	HRESULT hr = S_OK;

	ViewPtr pView = NULL;
	hr = m_pMyViewOverlay->SetView(pView);

	return S_OK;
}

HRESULT DrawGL::XViewEvents::raw_StyleChanged()
{
	return S_OK;
}

#pragma endregion

#pragma region ISEhDCDisplayEvents

HRESULT DrawGL::XhDCDisplayEvents::raw_BeginDisplay()
{
	return S_OK;
}

HRESULT DrawGL::XhDCDisplayEvents::raw_EndDisplay()
{
	return S_OK;
}

HRESULT DrawGL::XhDCDisplayEvents::raw_BeginhDCMainDisplay(long hDC, double* ModelToDC, long* Rect)
{

	return S_OK;
}

HRESULT DrawGL::XhDCDisplayEvents::raw_EndhDCMainDisplay(long hDC, double* ModelToDC, long* Rect)
{
	if (m_pMyViewOverlay->m_bShowSkh)
	{
		CDC* pDC = CDC::FromHandle((HDC)hDC);

		CMyViewOverlayObj* gl_draw = CSampleAddinApp::GetSEApp()->GetSEAddin()->GetMyViewOverlayObj();
		vector<Point2D> c_stroke = gl_draw->GetCurrentPoints();
		vector<vector<Point2D>> all_stroke = gl_draw->GetAllPoints();

		DrawGL::DrawStroke(c_stroke, pDC);

		for (auto c_hstk : all_stroke)
		{
			DrawGL::DrawStroke(c_hstk, pDC);
		}

	}
	return S_OK;
}

#pragma endregion

#pragma region ISEIGLDisplayEvents

HRESULT DrawGL::XGLDisplayEvents::raw_BeginDisplay()
{
	return S_OK;
}

HRESULT DrawGL::XGLDisplayEvents::raw_EndDisplay()
{
	return S_OK;
}

HRESULT DrawGL::XGLDisplayEvents::raw_BeginIGLMainDisplay(LPUNKNOWN pGL)
{
	IGLPtr pIGL = pGL;
	GLint iMatrixMode = 0;
	const GLubyte* pVersionString = NULL;

	// Draw OpenGL bounding box.
	if (m_pMyViewOverlay->m_bBoundingBox)
	{
		m_pMyViewOverlay->DrawOpenGlBoundingBox(pIGL);
	}

	if (m_pMyViewOverlay->m_bOpenGLBoxes)
	{
		m_pMyViewOverlay->DrawOpenGlBoxes(pIGL);
	}

#pragma region IGL
	// Demo interacting with OpenGL via IGL interface.
	if (pIGL != NULL)
	{
		// Returns "UNSUPPORTED" on my machine.
		//pVersionString = pIGL->glGetString(GL_VERSION);

		//pIGL->glGetIntegerv(GL_MATRIX_MODE, &iMatrixMode);

		//switch (iMatrixMode)
		//{
		//case GL_MODELVIEW:
		//	break;
		//case GL_PROJECTION:
		//	break;
		//case GL_TEXTURE:
		//	break;
		//}
	}
#pragma endregion

	// Reset variables.
	iMatrixMode = 0;
	pVersionString = NULL;

#pragma region No IGL
	// Demo interacting with OpenGL directly. (Not recommended...)
	// Note this required modifying linker input to include OpenGL32.lib.

	// Returns "4.4.0 NVIDIA 344.48" on my machine.
	//pVersionString = ::glGetString(GL_VERSION);

	//::glGetIntegerv(GL_MATRIX_MODE, &iMatrixMode);

	//switch (iMatrixMode)
	//{
	//case GL_MODELVIEW:
	//	break;
	//case GL_PROJECTION:
	//	break;
	//case GL_TEXTURE:
	//	break;
	//}

#pragma endregion

	return S_OK;
}

HRESULT DrawGL::XGLDisplayEvents::raw_EndIGLMainDisplay(LPUNKNOWN pGL)
{
	IGLPtr pIGL = pGL;

	return S_OK;
}

#pragma endregion