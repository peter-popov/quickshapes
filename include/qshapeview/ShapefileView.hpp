#pragma once
#include <QQuickItem>
#include <memory>
#include "ShapesModel.hpp"


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
    void sourceChanged(const QString&);

private:
    QString m_source;
    std::unique_ptr<ShapesModel> model;    
};