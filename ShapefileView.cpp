#include "ShapefileView.hpp"
#include <QSGSimpleRectNode>
#include <boost/geometry.hpp>
#include <fstream>
#include <GL/glut.h>

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
    std::array<GLdouble,3> coords;
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
    ptr->result->points.back().second.push_back({ptr->coords[0], ptr->coords[1]});
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

ShapefileModel::ShapefileModel(const QString& source)
{
    // std::vector<QPointF> dt =
    //     {{0, 0},
    //     {100, 0},
    //     {100, 100},
    //     {80, 100},
    //     {80, 20},
    //     {20, 20},
    //     {20, 100},
    //     {0, 100},
    //     {0, 0}};
    // polygons.push_back(dt);

    typedef double coord_t;
    typedef boost::geometry::model::point<coord_t, 2, boost::geometry::cs::cartesian> Point;
    typedef boost::geometry::model::polygon<Point> Polygon;

    std::ifstream file(source.toStdString());   
    if (!file)
        return; 
    int skip = 1;
    for( std::string line; getline( file, line ); )
    {
        if (skip) {skip--;continue;}
        auto pos_a = line.find_first_of('\"') + 1;
        auto pos_b = line.find_last_of('\"');
        if ( pos_b <= pos_a )
            continue;

        Polygon poly;
        auto wtk_feature = line.substr(pos_a, pos_b - pos_a);
        boost::geometry::read_wkt(wtk_feature, poly);
        std::vector<QPointF> v;
        auto ring = boost::geometry::exterior_ring(poly);
        for(auto& p: ring)
        {
            v.push_back({2*boost::geometry::get<0>(p), -2*boost::geometry::get<1>(p)});
        }
        polygons.push_back(std::move(v));
    }        
}

size_t ShapefileModel::itemsCount() const
{
    return polygons.size();
}

const std::vector<QPointF>& ShapefileModel::getItem(size_t index) const  
{
    return polygons[index];
}



//=============================

ShapefileView::ShapefileView(QQuickItem *parent)
: QQuickItem(parent)
{
 	setFlag(ItemHasContents, true);
    setFlag(ItemClipsChildrenToShape, true);
}

ShapefileView::~ShapefileView()
{

}


auto create_geometry_node(GLenum type, const std::vector<QPointF>& points)
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

QSGNode* ShapefileView::updatePaintNode(QSGNode* oldNode, UpdatePaintNodeData*)
{
    if (!model || model->itemsCount() == 0)
        return nullptr;

	if (!oldNode)
	{
        oldNode = new QSGNode();
        for (size_t i = 0; i < model->itemsCount(); ++i)
        {
            TessResult result;
            triangulate(model->getItem(i), result);
            for (auto& p: result.points)
            {
                oldNode->appendChildNode(create_geometry_node(p.first, p.second));    
            }            
        }
        oldNode->markDirty(QSGNode::DirtyGeometry);
    }
	
	return oldNode;
}

void ShapefileView::setSource(const QString &s)
{
    if (m_source == s)
        return;

    model.reset(new ShapefileModel(s));
    std::cout << "About to render " << model->itemsCount() << " items" << std::endl;
    m_source = s;
    emit sourceChanged(m_source);
    update();
}
