#include "WktShapesModel.hpp"
#include <boost/geometry.hpp>
#include <fstream>


WktShapesModel::WktShapesModel(const QString &wkt_file)
{
    polygons.clear();

    typedef double coord_t;
    typedef boost::geometry::model::point<coord_t, 2, boost::geometry::cs::cartesian> Point;
    typedef boost::geometry::model::polygon<Point> Polygon;
    typedef boost::geometry::model::box<Point> Box;

    std::ifstream file(wkt_file.toStdString());   
    if (!file)
        return; 
	Box bbox;
    int skip = 1;    
    for( std::string line; getline( file, line ); )
    {
        if (skip) {skip--; continue;}
        auto pos_a = line.find_first_of('\"') + 1;
        auto pos_b = line.find_last_of('\"');
        if ( pos_b <= pos_a )
            continue;

        Polygon poly;
        auto wtk_feature = line.substr(pos_a, pos_b - pos_a);        

        boost::geometry::read_wkt(wtk_feature, poly);

        Box tmp;
        boost::geometry::envelope(poly, tmp);
        boost::geometry::expand(bbox, tmp);

        std::vector<QPointF> v;
        auto ring = boost::geometry::exterior_ring(poly);
        for(auto& p: ring)
            v.push_back({boost::geometry::get<0>(p), boost::geometry::get<1>(p)});            

        polygons.push_back(std::move(v));
    }        
    bounding_rect.setLeft(boost::geometry::get<0>(bbox.min_corner()));
    bounding_rect.setRight(boost::geometry::get<0>(bbox.max_corner()));
    bounding_rect.setBottom(boost::geometry::get<1>(bbox.min_corner()));
    bounding_rect.setTop(boost::geometry::get<1>(bbox.max_corner()));        
}

size_t WktShapesModel::itemsCount() const
{
    return polygons.size();
}

const std::vector<QPointF>& WktShapesModel::getItem(size_t index) const  
{
    return polygons[index];
}
