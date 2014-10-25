#pragma once
#include <QQuickItem>
#include <memory>
#include "ShapesModel.hpp"


class ShapefileView : public QQuickItem
{
    Q_OBJECT

    Q_PROPERTY(QString source READ source WRITE setSource NOTIFY sourceChanged)
    Q_PROPERTY(double zoom READ zoom)
    Q_PROPERTY(QVector2D translate READ translate)

public:
    ShapefileView(QQuickItem *parent = 0);
    ~ShapefileView();

    QSGNode *updatePaintNode(QSGNode *, UpdatePaintNodeData *);

    QString source() const { return m_source; }
    void setSource(const QString &s);
    double zoom() const {return m_zoom;}
    QVector2D translate() const {return m_translate;};

signals:
    void sourceChanged(const QString&);

private:
    QString m_source;
    std::unique_ptr<ShapesModel> model;
    double m_zoom;    
    QVector2D m_translate;
};