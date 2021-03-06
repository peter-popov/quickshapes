#include "ShapefileView.hpp"
#include <QSGSimpleRectNode>
#include <GL/glut.h>
#include <QTransform>
#include "WktShapesModel.hpp"
#include <vector>

#ifndef CALLBACK
#define CALLBACK
#endif


struct TessResult
{
    TessResult():errorCode(0) {}
    
    GLenum errorCode;
    std::vector<std::pair<GLenum,std::vector<QPointF>>> points;      
};

struct GlPointRef
{
    GLdouble coords[3];
    TessResult* result;
};

void CALLBACK tessBeginCB(GLenum which, GLvoid *data)
{
    //glBegin(which);
    TessResult* res = (TessResult*)(data);
    res->points.push_back(std::make_pair(which, std::vector<QPointF>()));
}

void CALLBACK tessVertexCB(const GLvoid *data)
{
    GlPointRef *ptr = (GlPointRef*)data;
    ptr->result->points.back().second.push_back(QPointF(ptr->coords[0], ptr->coords[1]));
}

void CALLBACK tessErrorCB(GLenum errorCode, GLvoid *data)
{
    TessResult* res = (TessResult*)(data);
    res->errorCode = errorCode;    
    //const GLubyte *errorStr = gluErrorString(errorCode);
    //std::cerr << errorStr << std::endl;
}


bool triangulate(const std::vector<QPointF>& poly, TessResult& result)
{
    GLUtesselator *tess = gluNewTess(); // create a tessellator
    if(!tess) return false;  // failed to create tessellation object, return 0

    std::vector<GlPointRef> buf;
    for (auto& qtp: poly)
        buf.push_back({{qtp.x(), qtp.y(), 0}, &result});
    
    // register callback functions
    gluTessCallback(tess, GLU_TESS_BEGIN_DATA, (void (CALLBACK *)())tessBeginCB);
    //gluTessCallback(tess, GLU_TESS_END, (void (CALLBACK *)())tessEndCB);
    gluTessCallback(tess, GLU_TESS_ERROR_DATA, (void (CALLBACK *)())tessErrorCB);
    gluTessCallback(tess, GLU_TESS_VERTEX, (void (CALLBACK *)())tessVertexCB);

    gluTessBeginPolygon(tess, &result);
        gluTessBeginContour(tess);
            for(auto& p: buf)
                gluTessVertex(tess, &p.coords[0], &p);
        gluTessEndContour(tess);
    gluTessEndPolygon(tess);

    gluDeleteTess(tess);

    return true; 
}

//=============================

ShapefileView::ShapefileView(QQuickItem *parent)
: QQuickItem(parent)
, m_zoom(1.0)
{
 	setFlag(ItemHasContents, true);
    setFlag(ItemClipsChildrenToShape, true);
}

ShapefileView::~ShapefileView()
{

}


QSGNode* create_geometry_node(GLenum type, const std::vector<QPointF>& points)
{
    auto node = new QSGGeometryNode;
    auto geometry = new QSGGeometry(QSGGeometry::defaultAttributes_Point2D(), points.size());
    geometry->setLineWidth(2);
    geometry->setDrawingMode(type);
    node->setGeometry(geometry);
    node->setFlag(QSGNode::OwnsGeometry);
    QSGFlatColorMaterial *material = new QSGFlatColorMaterial;
    material->setColor(QColor(200,89,39,100));
    node->setMaterial(material);
    node->setFlag(QSGNode::OwnsMaterial);
    
    QSGGeometry::Point2D *vertices = geometry->vertexDataAsPoint2D();
    for (size_t i = 0; i < points.size(); ++i)
    {
        vertices[i].set(points[i].x(), points[i].y());
    }

    node->markDirty(QSGNode::DirtyGeometry);
    return node;
}

QMatrix4x4 fit_matrix(QRectF scene, QRectF canvas, double& zoom, QVector2D& translate)
{
    QMatrix4x4 m;
    if (canvas.width() == 0 || canvas.height() == 0 ||
        scene.width() == 0 || scene.height() == 0)
    {
        m.setToIdentity();
        zoom = 1.0;
        return m;
    }

    auto scale = std::min(std::abs(scene.width() / canvas.width()),
                          std::abs(scene.height() / canvas.height()));

    auto sign_x = (scene.width() > 0) ^ (canvas.width() > 0) ? -1 : 1;
    auto sign_y = (scene.height() > 0) ^ (canvas.height() > 0) ? -1 : 1;
    
    QTransform transform;
    transform.scale(scale, scale);

    translate.setX(scene.x() - canvas.x());
    translate.setY(scene.y() - canvas.bottom());

    transform.translate(translate.x(), translate.y());

    zoom = scale;

    return QMatrix4x4(transform);
}


QSGNode* ShapefileView::updatePaintNode(QSGNode* oldNode, UpdatePaintNodeData*)
{
    if (!model || model->itemsCount() == 0)
        return nullptr;
    
    QSGTransformNode* node = dynamic_cast<QSGTransformNode*>(oldNode);    
	if (!oldNode)
	{
        node = new QSGTransformNode();                
        for (size_t i = 0; i < model->itemsCount(); ++i)
        {
            TessResult result;
            triangulate(model->getItem(i), result);
            for (auto& p: result.points)
            {
                node->appendChildNode(create_geometry_node(p.first, p.second));    
            }            
        }        
    }

    node->markDirty(QSGNode::DirtyGeometry);
    if (model.get())
        node->setMatrix(fit_matrix(boundingRect(), model->boundingRect(), m_zoom, m_translate));
    
	return node;
}

void ShapefileView::setSource(const QString &s)
{
    if (m_source == s)
        return;

    model.reset(ModelFactory::create(s));
    //qDebug() << "Added " << model->itemsCount() << " items\n";
    m_source = s;
    emit sourceChanged(m_source);
    update();
}
