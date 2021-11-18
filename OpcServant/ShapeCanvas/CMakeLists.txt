cmake_minimum_required(VERSION 3.10)
project(ShapeCanvas)

set(CMAKE_CXX_STANDARD 17)

set(LIB_SOURCES
ArrowBase.cpp
AutoLayout.cpp
BitmapShape.cpp
BitmapShapeXml.cpp
CanvasHistory.cpp
CanvasState.cpp
CircleArrow.cpp
CircleShape.cpp
CommonFcn.cpp
ControlShape.cpp
CurveShape.cpp
DiagramManager.cpp
DiamondArrow.cpp
DiamondShape.cpp
EditTextShape.cpp
EllipseShape.cpp
FixedRectShape.cpp
FlexGridShape.cpp
GridShape.cpp
LineShape.cpp
MultiSelRect.cpp
OpenArrow.cpp
OrthoShape.cpp
PolygonShape.cpp
PolygonShapeXml.cpp
Printout.cpp
RectShape.cpp
RoundOrthoShape.cpp
RoundRectShape.cpp
ScaledDC.cpp
SFEvents.cpp
ShapeBase.cpp
ShapeBaseXml.cpp
ShapeCanvas.cpp
ShapeDataObject.cpp
ShapeDockpoint.cpp
ShapeHandle.cpp
SolidArrow.cpp
TextShape.cpp
TextShapeXml.cpp
Thumbnail.cpp
wx_pch.cpp
wxxmlserializer/PropertyIO.cpp
wxxmlserializer/XmlSerializer.cpp
        )

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_library(ShapeCanvas SHARED ${LIB_SOURCES})
target_link_libraries(ShapeCanvas ${wxWidgets_LIBRARIES} ${WTLIBS} ${BOOST_FLAGS} ${EXTRA_LIB_FLAGS} ${PROJECT_LIB_FLAGS})

file(GLOB SHAPECANVAS_HEADERS "*.h")
file(GLOB SHAPECANVAS_HEADERS_CPP "*.hpp")
install(FILES ${SHAPECANVAS_HEADERS} ${SHAPE_CANVAS_HEADERS_CPP} DESTINATION include/ShapeCanvas)

install(TARGETS ShapeCanvas DESTINATION ${EXE_DIR})


