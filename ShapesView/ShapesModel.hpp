#pragma once
#include <QQuickItem>
#include <memory>

class ShapesModel: public QObject
{
public:
	virtual size_t itemsCount() const = 0;

	virtual const std::vector<QPointF>& getItem(size_t index) const = 0;

	virtual QRectF boundingRect() const = 0;
};
