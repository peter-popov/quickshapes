#pragma once
#include <qshapeview/ShapesModel.hpp>

class WktShapesModel: public ShapesModel
{
public:
	WktShapesModel(const QString &source);

	virtual size_t itemsCount() const;

	virtual const std::vector<QPointF>& getItem(size_t index) const;

private:
	std::vector<std::vector<QPointF>> polygons;
	//QRectF bounding_rect;
};



class ModelFactory
{
public:
	static ShapesModel* create(const QString& s)
	{
		//return std::make_unique<WktShapesModel>(s);
		return new WktShapesModel(s);
	}
};