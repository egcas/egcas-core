#include <QPainter>
#include <QGraphicsItem>
#include <QCursor>
#include <QGraphicsSceneMouseEvent>
#include <QtCore/qmath.h>
#include "resizehandle.h"

ResizeHandle::ResizeHandle(QGraphicsItem *content, const QSizeF& size) :
        m_resizableContent(content), m_handleSize(size), m_contentStartScale(0.0), m_contentStartDiag(0.0)
{
        setFlags(ItemIsMovable | ItemClipsToShape | ItemIsSelectable);
}

QRectF ResizeHandle::boundingRect() const
{
        qreal marginX = m_handleSize.width() / 2.0;
        qreal marginY = m_handleSize.height() / 2.0;
        QRectF bounds(0.0, 0.0, m_handleSize.width(), m_handleSize.height());
        bounds.adjust(-marginX, -marginY, marginX, marginY);
        return bounds;
}

void ResizeHandle::mousePressEvent(QGraphicsSceneMouseEvent * event)
{
        m_scaleStartPos = mapToScene(event->pos());
        m_resizableContent->setFlag(ItemIsMovable, false);
        QGraphicsItem::mousePressEvent(event);
        m_contentStartScale = m_resizableContent->scale();

        qreal content_width = m_resizableContent->boundingRect().width();
        qreal content_height = m_resizableContent->boundingRect().height();


        m_contentStartDiag = sqrt(qPow(content_width, 2) + qPow(content_height, 2));
}

void ResizeHandle::mouseMoveEvent(QGraphicsSceneMouseEvent * event)
{

        QGraphicsItem::mouseMoveEvent(event);

        QPointF event_pos = mapToScene(event->pos());
        qreal width = event_pos.x() - m_scaleStartPos.x();
        qreal height = event_pos.y() - m_scaleStartPos.y();
        qreal diag = sqrt(qPow(width, 2) + qPow(height, 2));
        if (width < 0.0)
                diag = -1.0 * diag;

        qreal m_scale = (diag/m_contentStartDiag) + m_contentStartScale;

        m_resizableContent->setScale(m_scale);
}

void ResizeHandle::mouseReleaseEvent(QGraphicsSceneMouseEvent * event)
{        
        QGraphicsItem::mouseReleaseEvent(event);
        m_resizableContent->setFlag(ItemIsMovable, true);
        setPos(mapToScene(mapFromItem(m_resizableContent, m_resizableContent->boundingRect().bottomRight())));
}


void ResizeHandle::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
        (void) option;
        (void) widget;

        painter->save();

        painter->setRenderHint(QPainter::Antialiasing, true);
        painter->setPen(QPen(Qt::blue, m_handleSize.width() * 0.1, Qt::SolidLine));
        painter->setBrush(QBrush(Qt::blue));
        QRectF boundRect(0.0, 0.0, m_handleSize.width(), m_handleSize.height());
        QVector<QPointF> triangle;
        triangle.append(boundRect.topRight());
        triangle.append(boundRect.bottomRight());
        triangle.append(boundRect.bottomLeft());
        painter->drawPolygon(QPolygonF(triangle));

        painter->restore();
}

QVariant ResizeHandle::itemChange(GraphicsItemChange change, const QVariant &value)
 {
     if (change == ItemSelectedChange) {
         // value is the new position.
         bool selected = value.toBool();

         if (selected) {
                 setCursor(QCursor(Qt::SizeFDiagCursor));
         } else {
                 setCursor(QCursor(Qt::ArrowCursor));
         }
     }

     return QGraphicsItem::itemChange(change, value);
 }
