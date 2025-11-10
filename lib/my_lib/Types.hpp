
#ifndef Types_hpp
#define Types_hpp

#include <boost/polygon/voronoi.hpp>
#include <boost/polygon/point_data.hpp>
#include <boost/polygon/segment_data.hpp>


typedef double cord;
typedef boost::polygon::point_data<cord> Point;
typedef boost::polygon::segment_data<cord> Segment;
typedef boost::polygon::voronoi_diagram<cord> VoronoiDiagram;


#endif /* Types_hpp */
