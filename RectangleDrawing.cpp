#include "RectangleDrawing.h"




void RectangleDrawing::InstallNewInstance(int toolId, int toolPrompt)
{
	RectangleDrawing* tool = new RectangleDrawing(toolId, toolPrompt);
	tool->InstallTool();
}


bool RectangleDrawing::_OnDataButton(DgnButtonEventCR ev)
{
	if (0 == m_points.size())
		_BeginDynamics();
	
	m_points.push_back(*ev.GetPoint());
	//�ڶ��ε��ʱ������rectangle
	if (m_points.size() == 2)
	{
		 m_points = calcPoint(m_points);
	}
	
	if (m_points.size() < 4)
		return false;

	EditElementHandle eeh;

	if (CreateRectangle(eeh, m_points))
	{
		eeh.AddToModel();
		m_points.clear();
	}
	
	return false;
}

void RectangleDrawing::_OnPostInstall()
{
	AccuSnap::GetInstance().EnableSnap(true); // Enable snapping for create tools.
	__super::_OnPostInstall();
}

void RectangleDrawing::_OnDynamicFrame(DgnButtonEventCR ev)
{
	m_dynamicPoints = m_points;
	ElementAgenda agenda;
	//���Ӿ��εĵڶ����㣨��̬�㣩
	m_dynamicPoints.push_back(*ev.GetPoint());

	m_dynamicPoints = calcPoint(m_dynamicPoints);

	EditElementHandle eeh;
	if (!CreateRectangle(eeh, m_dynamicPoints))
		return;

	RedrawElems redrawElems;

	redrawElems.SetDynamicsViews(IViewManager::GetActiveViewSet(), ev.GetViewport()); // Display in all views, draws to cursor view first...
	redrawElems.SetDrawMode(DRAW_MODE_TempDraw);
	redrawElems.SetDrawPurpose(DrawPurpose::Dynamics);
	redrawElems.DoRedraw(eeh);
	
	
}

bool RectangleDrawing::CreateDynamicRectangle(ElementAgenda agenda, vector<DPoint3d> points)
{
	m_dynamicPoints = calcPoint(points);
	bvector<DPoint3d> bPoints;
	EditElementHandle eeh;

	for (auto& p : m_dynamicPoints)
	{
		bPoints.push_back(p);
	}
	bvector<DPoint3d> const &dynamicPoints = bPoints;
	if (SUCCESS == DraftingElementSchema::ToElement(eeh, *ICurvePrimitive::CreateLineString(dynamicPoints), NULL, ACTIVEMODEL->Is3d(), *ACTIVEMODEL))
	{
		ElementPropertyUtils::ApplyActiveSettings(eeh);
		agenda.Insert(eeh);
	}
	return true;
}

bool RectangleDrawing::CreateRectangle(EditElementHandleR eeh, vector<DPoint3d> points)
{
	if (4 != points.size())
		return false;
	bvector<DPoint3d> bPoints;
	for (auto& p : points)
	{
		bPoints.push_back(p);
	}
	PolyfaceHeaderPtr polyface = PolyfaceHeader::CreateVariableSizeIndexed();
	polyface->AddPolygon(bPoints);
	if (SUCCESS != MeshHeaderHandler::CreateMeshElement(eeh, NULL, *polyface, true, *ACTIVEMODEL))
		return false;
	ElementPropertyUtils::ApplyActiveSettings(eeh);

	return true;
}


vector<DPoint3d> RectangleDrawing::calcPoint(vector<DPoint3d> points)
{
	//p1��һ�ε���ĵ㣬p2����ƶ���׽��
	vector<DPoint3d> rectanglePoint;
	double width = 100000;
	DPoint3d p1 = points[0];
	DPoint3d p2 = points[1];
	rectanglePoint = points;
	DPoint3d p3;
	DPoint3d p4;
	p1.Distance(p2);
	//��ȡ��̬�ߣ����αߣ���x���γɵ�������
	double xLenP12 = abs(p1.x - p2.x);
	double yLenp12 = abs(p1.y - p2.y);
	double cLenP12 = sqrt(pow(xLenP12, 2) + pow(yLenp12, 2));
	//��̬�߳���x��ļн�
	double coos = xLenP12 / cLenP12;
	double siin = yLenp12 / cLenP12;
	//point3���������һ���x��y����ֵ
	double xLenP23 = width * siin;
	double yLenP23 = width * coos;

 	if (p1.x<p2.x && p1.y<p2.y || p1.x>p2.x && p1.y>p2.y)
	{
		p3.y = p2.y + yLenP23;
		p3.x = p2.x - xLenP23;
		p4.y = p1.y + yLenP23;
		p4.x = p1.x - xLenP23;
		rectanglePoint.push_back(p3);
		rectanglePoint.push_back(p4);
	}
	else if (p1.x<p2.x && p1.y>p2.y || p1.x>p2.x && p1.y<p2.y)
	{
		p3.y = p2.y + yLenP23;
		p3.x = p2.x + xLenP23;
		p4.y = p1.y + yLenP23;
		p4.x = p1.x + xLenP23;
		rectanglePoint.push_back(p3);
		rectanglePoint.push_back(p4);
	}
	else if (p1.y == p2.y)
	{
		p3.x = p2.x;
		p3.y = p2.y + width;
		p4.x = p1.x;
		p4.y = p1.y + width;
		rectanglePoint.push_back(p3);
		rectanglePoint.push_back(p4);
	}
	else if (p1.x == p2.x)
	{
		p3.x = p1.x + width;
		p3.y = p2.y;
		p4.x = p1.x + width;
		p4.y = p1.y;
		rectanglePoint.push_back(p3);
		rectanglePoint.push_back(p4);
	}
	return rectanglePoint;
}