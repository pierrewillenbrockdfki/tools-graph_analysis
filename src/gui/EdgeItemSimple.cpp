#include "EdgeItemSimple.hpp"
#include <graph_analysis/gui/GraphWidget.hpp>
#include <base-logging/Logging.hpp>
#include <cmath>

namespace graph_analysis {
namespace gui {

// kiss:
EdgeItemSimple::EdgeItemSimple(GraphWidget* graphWidget,
                               graph_analysis::Edge::Ptr edge,
                               QGraphicsItem* parent)
    : EdgeItemBase(graphWidget, edge, parent)
    , mArrowSize(10)
{
    mpLabel = new QGraphicsTextItem(QString(edge->getLabel().c_str()), this);
    mpClassName =
        new QGraphicsTextItem(QString(edge->getClassName().c_str()), this);
    mpClassName->setDefaultTextColor(Qt::gray);
    mpMultiLine = new QGraphicsPolygonItem(this);
    mpMultiLine->setBrush(Qt::NoBrush);
    mpArrowHead = new QGraphicsPolygonItem(this);
    mpArrowHead->setBrush(QBrush(Qt::black));

    setFlag(ItemIsMovable, false);

    mpGraphWidget->registerEdgeItem(mpEdge, this);
}

EdgeItemSimple::~EdgeItemSimple()
{
    delete mpLabel;
    delete mpClassName;
    delete mpMultiLine;
    delete mpArrowHead;

    mpGraphWidget->deregisterEdgeItem(mpEdge, this);
}

int EdgeItemSimple::type() const
{
    return EdgeItemSimpleType;
}

void EdgeItemSimple::adjustEdgePositioning()
{
    prepareGeometryChange();

    mpMultiLine->setPolygon(mPoints);
    mpLabel->setPos(mpMultiLine->boundingRect().center() -
                    mpLabel->boundingRect().center());
    mpClassName->setPos(mpLabel->pos() +
                        QPointF(0, mpLabel->boundingRect().height()));

    // draw the arrow!
    QLineF lastSegment(mPoints.at((size_t)mPoints.size() - 2),
                       mPoints.at((size_t)mPoints.size() - 1));
    double angle = std::acos(lastSegment.dx() / lastSegment.length());
    // in case this is a very short edge we cannot infer how to actually draw
    // the arrow. in this case we'll fall back to not draw it.
    if(std::isnan(angle))
    {
        mpArrowHead->setPolygon(QPolygonF());
        return;
    }
    if(lastSegment.dy() >= 0)
    {
        angle = 2 * M_PI - angle;
    }

    QPointF destArrowP1 =
        mPoints.last() + QPointF(sin(angle - M_PI / 3) * mArrowSize,
                                          cos(angle - M_PI / 3) * mArrowSize);
    QPointF destArrowP2 = mPoints.last() +
                          QPointF(sin(angle - M_PI + M_PI / 3) * mArrowSize,
                                  cos(angle - M_PI + M_PI / 3) * mArrowSize);
    mpArrowHead->setPolygon(QPolygonF() << mPoints.last() << destArrowP1
                                        << destArrowP2);
}

void EdgeItemSimple::paint(QPainter* painter,
                           const QStyleOptionGraphicsItem* option,
                           QWidget* widget)
{
}

QRectF EdgeItemSimple::boundingRect() const
{
    return childrenBoundingRect();
}

// reimplement "shape()" because the default implementation calls
// "boundingRect()" -- we are not rect!
QPainterPath EdgeItemSimple::shape() const
{
    QPainterPath path;
    path = mpMultiLine->shape() + mpArrowHead->shape() + mpLabel->shape() +
           mpClassName->shape();
    return path;
}

EdgeItemBase* EdgeItemSimple::createNewItem(GraphWidget* graphWidget,
        const graph_analysis::Edge::Ptr& edge,
        QGraphicsItem* parent) const
{
    return new EdgeItemSimple(graphWidget, edge, parent);
}

} // end namespace gui
} // end namespace graph_analysis
