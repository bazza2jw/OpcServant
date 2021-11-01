#include "wscadamapwindow.h"
#include <Wt/WPen.h>
#include <Wt/WRectF.h>
#include <Wt/WPainter.h>
/*!
 * \brief MRL::WScadaMapWindow::WScadaMapWindow
 */
MRL::WScadaMapWindow::WScadaMapWindow()
{
    setPreferredMethod(Wt::RenderMethod::HtmlCanvas);
    resize(Wt::WLength(400),Wt::WLength(400));
}

/*!
 * \brief MRL::WScadaMapWindow::paintEvent
 * \param paintDevice
 */
void MRL::WScadaMapWindow::paintEvent (Wt::WPaintDevice *paintDevice)
{
    Wt::WRectF r(100,100,100,100);
    Wt::WPen pn(Wt::WColor("black"));
    Wt::WBrush br(Wt::WColor("black"));
    Wt::WPainter painter(paintDevice);
    painter.setPen(pn);
    painter.setBrush(br);
    painter.drawRect(r);
}
