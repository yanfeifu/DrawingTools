#pragma once

#include <Mstn/MdlApi/MdlApi.h>
#include <Mstn/ElementPropertyUtils.h>
#include <DgnView/AccuDraw.h>
#include <DgnView/DgnElementSetTool.h>
#include <DgnView/DgnRegionElementTool.h>
#include <DgnPlatform/DropGraphics.h>
#include <DgnPlatform/NotificationManager.h>
#include <Geom/CurveVector.h>
#include <Geom/CurveDetails.h>
#include <Geom/CurvePrimitive.h>
#include <Geom/CurvePrimitiveId.h>
#include <Geom/CurveTopologyId.h>
#include <DgnView/LocateSubEntityTool.h>

#include "DrawingToolsIds.h"
#include "DrawingToolsCmd.h"

void drawingLine(WCharCP);
void drawingMline(WCharCP);