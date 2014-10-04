#pragma once
#include <QQuickItem>
#include <memory>

class ShapefileModel: public QObject
{
	Q_OBJECT

public:

	ShapefileModel(const QString& source);

	size_t itemsCount() const;

	const std::vector<QPointF>& getItem(size_t index) const;	
private:
	std::vector<std::vector<QPointF>> polygons;
};



class ShapefileView : public QQuickItem
{
    Q_OBJECT

    Q_PROPERTY(QString source READ source WRITE setSource NOTIFY sourceChanged)

public:
    ShapefileView(QQuickItem *parent = 0);
    ~ShapefileView();

    QSGNode *updatePaintNode(QSGNode *, UpdatePaintNodeData *);

    QString source() const { return m_source; }
    void setSource(const QString &s);

signals:
    void sourceChanged(const QString &s);

private:
    QString m_source;
    std::unique_ptr<ShapefileModel> model;
};