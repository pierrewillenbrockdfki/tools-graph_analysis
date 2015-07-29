#ifndef GRAPH_ANALYSIS_GRAPHWIDGET_LAYERITEM_RESOURCE_HPP
#define GRAPH_ANALYSIS_GRAPHWIDGET_LAYERITEM_RESOURCE_HPP

#include <graph_analysis/gui/NodeItem.hpp>
#include <graph_analysis/gui/GraphWidget.hpp>
#include <QPen>

#define LAYER_NODE_TYPE "LayerVertex"

namespace graph_analysis {
namespace gui {
namespace layeritem {

class Label;

class Resource : public graph_analysis::gui::NodeItem
{
private:
    /// constructor; private since instances of this class only spawn via the NodeTypeManager factory class
    Resource(GraphWidget* graphWidget, graph_analysis::Vertex::Ptr vertex);
public:
    /// empty constructor
    Resource() {}
    /// destructor
    virtual ~Resource() {};

    /// retrieves the bounding rectangular box around the area occupied by the node in the scene
    virtual QRectF boundingRect() const;
    /// retrieves the path of the bounding rectangular box around the area occupied by the node in the scene
    virtual QPainterPath shape() const;
    /// qt node painting method - here the node components get placed in the scene
    virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget*);

    /// node cloning/spawning method used by the factory to produce new nodes
    virtual NodeItem* createNewItem(GraphWidget* graphWidget, graph_analysis::Vertex::Ptr vertex) const { return new Resource(graphWidget, vertex); }

protected:
    /// qt mouse double-click callback
    void mouseDoubleClickEvent(::QGraphicsSceneMouseEvent* event);
    /// qt mouse press callback
    void mousePressEvent(::QGraphicsSceneMouseEvent* event);
    /// qt mouse release callback
    void mouseReleaseEvent(::QGraphicsSceneMouseEvent* event);

    /// qt hovering ENTER callback
    virtual void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    /// qt hovering LEAVE callback
    virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);

private:
    /// main node text label
    Label* mLabel;
    /// qt drawing pen
    QPen mPen;
    /// default qt drawing pen
    QPen mPenDefault;
};

} // end namespace layeritem
} // end namespace gui
} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_GRAPHWIDGET_LAYERITEM_RESOURCE_HPP
