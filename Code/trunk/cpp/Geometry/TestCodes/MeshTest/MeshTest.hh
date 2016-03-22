// Date Last Altered: $Date: 2008-12-01 13:34:05 -0700 (Mon, 01 Dec 2008) $
// Revision Number: $Revision: 423 $
//----------------------------------*-C++-*------------------------------------//
/*! \file MeshTest.hh
 *  \author Greg Davidson
 *  \date June 9, 2008 
 *
 *  \brief Runs a series of tests on the Mesh Module.
 *
 *  This file contains a series of tests on the Mesh Module. */
 
#ifndef MESHTEST_HH
#define MESHTEST_HH

/// Runs a series of tests on the left ghost zones.
void check_left_ghost_zones(const Zone<OneD>& left_ghost_zone1, const Zone<OneD>& left_ghost_zone2,
                            const Zone<OneD>& first_zone, const Zone<OneD>& second_zone);
/// Runs a series of tests on the right ghost zones.
void check_right_ghost_zones(const Zone<OneD>& right_ghost_zone1, const Zone<OneD>& right_ghost_zone2,
                             const Zone<OneD>& lastm1_zone, const Zone<OneD>& last_zone);
/// Runs a series of tests on the left ghost nodes.
void check_left_ghost_nodes(const Node<OneD>& left_ghost_node1, const Node<OneD>& left_ghost_node2);
/// Runs a series of tests on the right ghost nodes.
void check_right_ghost_nodes(const Node<OneD>& right_ghost_node1, const Node<OneD>& right_ghost_node2,
                             double mesh_length);
/// Runs a series of tests on the left ghost corners.
void check_left_ghost_corners(const Corner<OneD>& left_ghost_corner1,
                              const Corner<OneD>& left_ghost_corner2,
                              const Corner<OneD>& left_ghost_corner3,
                              const Corner<OneD>& left_ghost_corner4);
/// Runs a series of tests on the right ghost corners.
void check_right_ghost_corners(const Corner<OneD>& right_ghost_corner1,
                               const Corner<OneD>& right_ghost_corner2,
                               const Corner<OneD>& right_ghost_corner3,
                               const Corner<OneD>& right_ghost_corner4);



BOOST_AUTO_TEST_SUITE( meshtest )

/// Runs a series of simple tests on a simple uniform mesh.
BOOST_AUTO_TEST_CASE( simple_uniform_mesh )
{
   // Build a uniform cartesian mesh without ghost zones
   // Length: 2.0
   // Area: 3.0
   // Num zones: 10
   CartMesh1D mesh(2.0, 3.0, 10);
  
   // Check the mesh length.
   BOOST_CHECK( fabs(mesh.length() - 2.0) 
                   < 10.0*numeric_limits<CartMesh1D::LengthType>::epsilon() );
   // Check the mesh area.
   BOOST_CHECK( fabs(mesh.area() - 3.0)
                   < 10.0*numeric_limits<CartMesh1D::LengthType>::epsilon() );
   // Check the mesh volume.
   BOOST_CHECK( fabs(mesh.volume() - 3.0*2.0)
                   < 10.0*numeric_limits<CartMesh1D::LengthType>::epsilon() );
   // Check the number of zones.
   BOOST_CHECK_EQUAL( mesh.numZones(), 10 );
   // Check the number of nodes.
   BOOST_CHECK_EQUAL( mesh.numNodes(), 11 );
   // Check the number of corners.
   BOOST_CHECK_EQUAL( mesh.numCorners(), 20 );
  
   // Check the number of left and right ghost zones.
   BOOST_CHECK_EQUAL( mesh.numLeftGhostZones(), 0 );
   BOOST_CHECK_EQUAL( mesh.numRightGhostZones(), 0 );
   // Check the number of left and right ghost nodes.
   BOOST_CHECK_EQUAL( mesh.numLeftGhostNodes(), 0 );   
   BOOST_CHECK_EQUAL( mesh.numRightGhostNodes(), 0 );
   // Check the number of left and right ghost corners.
   BOOST_CHECK_EQUAL( mesh.numLeftGhostCorners(), 0 );
   BOOST_CHECK_EQUAL( mesh.numRightGhostCorners(), 0 );
  
   // Loop over the mesh and check each zone.
   unsigned int zone_counter = 0;
   for(CartMesh1D::const_ZoneIterator zone_iter = mesh.zoneBegin();
       zone_iter != mesh.zoneEnd(); ++zone_iter)
   {
      // Check each zone properties.
      const Zone<OneD>& zone = *zone_iter;
      BOOST_CHECK_EQUAL( zone.id().idNumber(), zone_counter );
      BOOST_CHECK( fabs(zone.length()-0.2) < 10.0*numeric_limits< Zone<OneD>::LengthType >::epsilon() );
      BOOST_CHECK( fabs(zone.area()-3.0) < 10.0*numeric_limits< Zone<OneD>::LengthType >::epsilon() );
      BOOST_CHECK( fabs(zone.volume()-0.6) < 10.0*numeric_limits< Zone<OneD>::LengthType >::epsilon() );
      // Check the zone center position.
      BOOST_CHECK( fabs(zone.center().x() - (zone_counter*0.2+0.1) )
               < 10.0*numeric_limits< Zone<OneD>::LengthType >::epsilon() );
      // Check the zone connectivity.
      BOOST_CHECK_EQUAL( zone.getLeftNode().id().idNumber(), zone_counter );
      BOOST_CHECK_EQUAL( zone.getRightNode().id().idNumber(), zone_counter+1 );
      BOOST_CHECK_EQUAL( zone.getLeftCorner().id().idNumber(), 2*zone_counter );
      BOOST_CHECK_EQUAL( zone.getRightCorner().id().idNumber(), 2*zone_counter+1 );
      // Check the zone id.
      BOOST_CHECK_EQUAL( &zone, &mesh.getZone(zone.id()) );
      BOOST_CHECK_EQUAL( &zone, &mesh[zone.id()] );
      
      ++zone_counter;
   }
   
   // Loop over the mesh and check each node.
   unsigned int node_counter = 0;
   for(CartMesh1D::const_NodeIterator node_iter = mesh.nodeBegin();
       node_iter != mesh.nodeEnd(); ++node_iter)
   {
      // Check each node property
      const Node<OneD>& node = *node_iter;
      BOOST_CHECK_EQUAL( node.id().idNumber(), node_counter );
      BOOST_CHECK( fabs(node.x()-node_counter*0.2)
            < 10.0*numeric_limits< Node<OneD>::PointType::PointType >::epsilon() );
      
      // Check the left-most real node.
      if(node_counter == 0)
      {
         BOOST_CHECK_EQUAL( node.onLeftBoundary(), true );
         BOOST_CHECK_EQUAL( node.getRightZone().id().idNumber(), node_counter );
         BOOST_CHECK_EQUAL( node.getRightCorner().id().idNumber(), node_counter );
      }
      // Check the right-most real node.
      else if(node_counter == 10)
      {
         BOOST_CHECK_EQUAL( node.onRightBoundary(), true );
         BOOST_CHECK_EQUAL( node.getLeftZone().id().idNumber(), node_counter-1 );
         BOOST_CHECK_EQUAL( node.getLeftCorner().id().idNumber(), 2*(node_counter-1)+1 );
      }
      // Check the interior nodes.
      else
      {
         BOOST_CHECK_EQUAL( node.onBoundary(), false );
         BOOST_CHECK_EQUAL( node.getLeftZone().id().idNumber(), node_counter-1 );
         BOOST_CHECK_EQUAL( node.getRightZone().id().idNumber(), node_counter );
         BOOST_CHECK_EQUAL( node.getLeftCorner().id().idNumber(), 2*(node_counter-1)+1 );
         BOOST_CHECK_EQUAL( node.getRightCorner().id().idNumber(), 2*node_counter );
      }

      // Check the node id.
      BOOST_CHECK_EQUAL( &node, &mesh.getNode(node.id()) );
      BOOST_CHECK_EQUAL( &node, &mesh[node.id()] );
      
      ++node_counter;
   }
   
   // Loop over the mesh and check each corner.
   unsigned int corner_counter = 0;
   for(CartMesh1D::const_CornerIterator corner_iter = mesh.cornerBegin();
       corner_iter != mesh.cornerEnd(); ++corner_iter)
   {
      // Check each corner's property
      const Corner<OneD>& corner = *corner_iter;
      BOOST_CHECK_EQUAL( corner.id().idNumber(), corner_counter );
      BOOST_CHECK_EQUAL( corner.getZone().id().idNumber(), corner_counter/2 );
      BOOST_CHECK_EQUAL( corner.getNode().id().idNumber(), corner_counter/2 + corner_counter%2);
      
      // Check the corner id.
      BOOST_CHECK_EQUAL( &corner, &mesh.getCorner(corner.id()) );
      BOOST_CHECK_EQUAL( &corner, &mesh[corner.id()] );

      ++corner_counter;
   }
}

// Run a series of tests on the iterators
BOOST_AUTO_TEST_CASE( iterator_test )
{
   CartMesh1D mesh(1.0, 1.0, 10);
   
   unsigned int zone_counter = 0;
   for(CartMesh1D::const_ZoneIterator zone_iter = mesh.zoneBegin();
       zone_iter != mesh.zoneEnd(); ++zone_iter)
   {
      BOOST_CHECK_EQUAL( zone_iter->id().idNumber(), zone_counter );
      ++zone_counter;
   }
   unsigned int node_counter = 0;
   for(CartMesh1D::const_NodeIterator node_iter = mesh.nodeBegin();
       node_iter != mesh.nodeEnd(); ++node_iter)
   {
      BOOST_CHECK_EQUAL( node_iter->id().idNumber(), node_counter );
      ++node_counter;
   }
   unsigned int corner_counter = 0;
   for(CartMesh1D::const_CornerIterator corner_iter = mesh.cornerBegin();
       corner_iter != mesh.cornerEnd(); ++corner_iter)
   {
      BOOST_CHECK_EQUAL( corner_iter->id().idNumber(), corner_counter );
      ++corner_counter;
   }
   
   zone_counter = mesh.numZones()-1;
   for(CartMesh1D::const_reverse_ZoneIterator rev_zone_iter = mesh.zoneRBegin();
       rev_zone_iter != mesh.zoneREnd(); ++rev_zone_iter)
   {
      BOOST_CHECK_EQUAL( rev_zone_iter->id().idNumber(), zone_counter );
      --zone_counter;
   }
   node_counter = mesh.numNodes()-1;
   for(CartMesh1D::const_reverse_NodeIterator rev_node_iter = mesh.nodeRBegin();
       rev_node_iter != mesh.nodeREnd(); ++rev_node_iter)
   {
      BOOST_CHECK_EQUAL( rev_node_iter->id().idNumber(), node_counter );
      --node_counter;
   }
   corner_counter = mesh.numCorners()-1;
   for(CartMesh1D::const_reverse_CornerIterator rev_corner_iter = mesh.cornerRBegin();
       rev_corner_iter != mesh.cornerREnd(); ++rev_corner_iter)
   {
      BOOST_CHECK_EQUAL( rev_corner_iter->id().idNumber(), corner_counter );
      --corner_counter;
   }
   
   
   zone_counter = 0;
   Angle<OneD> angle1( GeomVector<OneD>(1.0) );
   for(CartMesh1D::SweepIterator sweep_iter = mesh.sweepBegin(angle1);
       sweep_iter != mesh.sweepEnd(angle1); ++sweep_iter)
   {
      BOOST_CHECK_EQUAL( sweep_iter->id().idNumber(), zone_counter );
      ++zone_counter;
   }
   zone_counter = mesh.numZones()-1;
   Angle<OneD> angle2( GeomVector<OneD>(-1.0) );
   for(CartMesh1D::SweepIterator sweep_iter = mesh.sweepBegin( angle2 );
       sweep_iter != mesh.sweepEnd( angle2 ); ++sweep_iter)
   {
      BOOST_CHECK_EQUAL( sweep_iter->id().idNumber(), zone_counter );
      --zone_counter;
   }
}
   
// Run a series of tests on a simple nonuniform mesh with no ghost zones.
BOOST_AUTO_TEST_CASE( simple_nonuniform_mesh )
{
   // Build a non-uniform cartesian mesh without ghost zones
   
   vector<double> zone_lengths(25);
   unsigned int length_counter = 1;
   for(vector<double>::iterator iter = zone_lengths.begin();
       iter != zone_lengths.end(); ++iter)
   {
      *iter = length_counter;
      length_counter += length_counter;
   }
   
   CartMesh1D mesh(zone_lengths);
   double length = accumulate(zone_lengths.begin(), zone_lengths.end(), 0.0);
  
   BOOST_CHECK( fabs(mesh.length() - length) 
                   < 10.0*numeric_limits<CartMesh1D::LengthType>::epsilon() );
   BOOST_CHECK( fabs(mesh.area() - 1.0)
                   < 10.0*numeric_limits<CartMesh1D::LengthType>::epsilon() );
   BOOST_CHECK( fabs(mesh.volume() - length)
                   < 10.0*numeric_limits<CartMesh1D::LengthType>::epsilon() );
   BOOST_CHECK_EQUAL( mesh.numZones(), 25 );
   BOOST_CHECK_EQUAL( mesh.numNodes(), 26 );
   BOOST_CHECK_EQUAL( mesh.numCorners(), 50 );

   BOOST_CHECK_EQUAL( mesh.numZones(), mesh.numElements< Zone<OneD> >() );
   BOOST_CHECK_EQUAL( mesh.numNodes(), mesh.numElements< Node<OneD> >() );
   BOOST_CHECK_EQUAL( mesh.numCorners(), mesh.numElements< Corner<OneD> >() );
  
   BOOST_CHECK_EQUAL( mesh.numLeftGhostZones(), 0 );
   BOOST_CHECK_EQUAL( mesh.numLeftGhostNodes(), 0 );
   BOOST_CHECK_EQUAL( mesh.numLeftGhostCorners(), 0 );
   BOOST_CHECK_EQUAL( mesh.numRightGhostZones(), 0 );
   BOOST_CHECK_EQUAL( mesh.numRightGhostNodes(), 0 );
   BOOST_CHECK_EQUAL( mesh.numRightGhostCorners(), 0 );
  
   // Check each zone
   unsigned int zone_counter = 0;
   for(CartMesh1D::const_ZoneIterator zone_iter = mesh.zoneBegin();
       zone_iter != mesh.zoneEnd(); ++zone_iter)
   {
      // Check each zone properties
      const Zone<OneD>& zone = *zone_iter;
      BOOST_CHECK_EQUAL( zone.id().idNumber(), zone_counter );
        
      BOOST_CHECK_EQUAL( zone.getLeftNode().id().idNumber(), zone_counter );
      BOOST_CHECK_EQUAL( zone.getRightNode().id().idNumber(), zone_counter+1 );
      BOOST_CHECK_EQUAL( zone.getLeftCorner().id().idNumber(), 2*zone_counter );
      BOOST_CHECK_EQUAL( zone.getRightCorner().id().idNumber(), 2*zone_counter+1 );
      
      ++zone_counter;
   }
   
   unsigned int node_counter = 0;
   for(CartMesh1D::const_NodeIterator node_iter = mesh.nodeBegin();
       node_iter != mesh.nodeEnd(); ++node_iter)
   {
      // Check each node property
      const Node<OneD>& node = *node_iter;
      BOOST_CHECK_EQUAL( node.id().idNumber(), node_counter );
      
      if(node_counter == 0)
      {
         BOOST_CHECK_EQUAL( node.onLeftBoundary(), true );
         BOOST_CHECK_EQUAL( node.getRightZone().id().idNumber(), node_counter );
         BOOST_CHECK_EQUAL( node.getRightCorner().id().idNumber(), node_counter );
      }
      else if(node_counter == 25)
      {
         BOOST_CHECK_EQUAL( node.onRightBoundary(), true );
         BOOST_CHECK_EQUAL( node.getLeftZone().id().idNumber(), node_counter-1 );
         BOOST_CHECK_EQUAL( node.getLeftCorner().id().idNumber(), 2*(node_counter-1)+1 );
      }
      else
      {
         BOOST_CHECK_EQUAL( node.onBoundary(), false );
         BOOST_CHECK_EQUAL( node.getLeftZone().id().idNumber(), node_counter-1 );
         BOOST_CHECK_EQUAL( node.getRightZone().id().idNumber(), node_counter );
         BOOST_CHECK_EQUAL( node.getLeftCorner().id().idNumber(), 2*(node_counter-1)+1 );
         BOOST_CHECK_EQUAL( node.getRightCorner().id().idNumber(), 2*node_counter );
      }
      
      ++node_counter;
   }
   
   unsigned int corner_counter = 0;
   for(CartMesh1D::const_CornerIterator corner_iter = mesh.cornerBegin();
       corner_iter != mesh.cornerEnd(); ++corner_iter)
   {
      // Check each corner property
      const Corner<OneD>& corner = *corner_iter;
      BOOST_CHECK_EQUAL( corner.id().idNumber(), corner_counter );
      BOOST_CHECK_EQUAL( corner.getZone().id().idNumber(), corner_counter/2 );
      BOOST_CHECK_EQUAL( corner.getNode().id().idNumber(), corner_counter/2 + corner_counter%2);
      
      ++corner_counter;
   }
}

// Test a mesh with ghost zones
BOOST_AUTO_TEST_CASE( ghostzonetest )
{
   CartMesh1D mesh(2.0, 3.0, 10, 2, true, 2, true);
   
   // Get the first and second zone
   const Zone<OneD>& first_zone = mesh.getZone( Zone<OneD>::Id(0) );
   const Zone<OneD>& second_zone = first_zone.getRightNode().getRightZone();
   // Get the last and second-to-last zone
   const Zone<OneD>& last_zone = mesh.getZone( Zone<OneD>::Id(9) );
   const Zone<OneD>& lastm1_zone = last_zone.getLeftNode().getLeftZone();   
   // Get the four ghost zones
   const Zone<OneD>& left_ghost_zone1 = first_zone.getLeftNode().getLeftZone();
   const Zone<OneD>& left_ghost_zone2 = left_ghost_zone1.getLeftNode().getLeftZone();
   const Zone<OneD>& right_ghost_zone1 = last_zone.getRightNode().getRightZone();
   const Zone<OneD>& right_ghost_zone2 = right_ghost_zone1.getRightNode().getRightZone();
   // Check the ghost zones
   check_left_ghost_zones(left_ghost_zone1, left_ghost_zone2,
                          first_zone, second_zone);
   check_right_ghost_zones(right_ghost_zone1, right_ghost_zone2,
                           lastm1_zone, last_zone);
   
   // Get the four ghost nodes
   const Node<OneD>& left_ghost_node1 = left_ghost_zone1.getLeftNode();
   const Node<OneD>& left_ghost_node2 = left_ghost_zone2.getLeftNode();
   const Node<OneD>& right_ghost_node1 = right_ghost_zone1.getRightNode();
   const Node<OneD>& right_ghost_node2 = right_ghost_zone2.getRightNode();
   check_left_ghost_nodes(left_ghost_node1, left_ghost_node2);
   check_right_ghost_nodes(right_ghost_node1, right_ghost_node2, mesh.length());
   
   // Get the four left ghost corners
   const Corner<OneD>& left_ghost_corner1 = left_ghost_zone1.getLeftCorner();
   const Corner<OneD>& left_ghost_corner2 = left_ghost_zone1.getRightCorner();
   const Corner<OneD>& left_ghost_corner3 = left_ghost_zone2.getLeftCorner();
   const Corner<OneD>& left_ghost_corner4 = left_ghost_zone2.getRightCorner();
   check_left_ghost_corners(left_ghost_corner1, left_ghost_corner2,
                            left_ghost_corner3, left_ghost_corner4);
   
   // Get the four right ghost corners
   const Corner<OneD>& right_ghost_corner1 = right_ghost_zone1.getLeftCorner();
   const Corner<OneD>& right_ghost_corner2 = right_ghost_zone1.getRightCorner();
   const Corner<OneD>& right_ghost_corner3 = right_ghost_zone2.getLeftCorner();
   const Corner<OneD>& right_ghost_corner4 = right_ghost_zone2.getRightCorner();
   check_right_ghost_corners(right_ghost_corner1, right_ghost_corner2,
                             right_ghost_corner3, right_ghost_corner4);

   // Check the first "real" node
   const Node<OneD>& first_node = mesh.getNode( Node<OneD>::Id(0) );
   BOOST_CHECK_EQUAL( first_node.onLeftBoundary(), false );
   BOOST_CHECK_EQUAL( first_node.getLeftZone().id().idNumber(), 10 );
   BOOST_CHECK_EQUAL( first_node.getLeftCorner().id().idNumber(), 21 );
   // Check the last "real" node
   const Node<OneD>& last_node = mesh.getNode( Node<OneD>::Id(10) );
   BOOST_CHECK_EQUAL( last_node.onRightBoundary(), false );
   BOOST_CHECK_EQUAL( last_node.getRightZone().id().idNumber(), 12 );
   BOOST_CHECK_EQUAL( last_node.getRightCorner().id().idNumber(), 24 );
}


BOOST_AUTO_TEST_SUITE_END()

// Run a series of tests on the left ghost zones
inline void check_left_ghost_zones(const Zone<OneD>& left_ghost_zone1, const Zone<OneD>& left_ghost_zone2,
                                   const Zone<OneD>& first_zone, const Zone<OneD>& second_zone)
{  
   // Check left ghost zone 1
   BOOST_CHECK_EQUAL( left_ghost_zone1.id().idNumber(), 10 );
   BOOST_CHECK( fabs(left_ghost_zone1.length()-first_zone.length())
                        < 10.0*numeric_limits<Zone<OneD>::LengthType>::epsilon() );
   BOOST_CHECK( fabs(left_ghost_zone1.area()-first_zone.area())
                        < 10.0*numeric_limits<Zone<OneD>::LengthType>::epsilon() );
   BOOST_CHECK( fabs(left_ghost_zone1.volume()-first_zone.volume())
                        < 10.0*numeric_limits<Zone<OneD>::LengthType>::epsilon() );
   BOOST_CHECK_EQUAL( left_ghost_zone1.getLeftNode().id().idNumber(), 11 );
   BOOST_CHECK_EQUAL( left_ghost_zone1.getRightNode().id().idNumber(), 0 );
   BOOST_CHECK_EQUAL( left_ghost_zone1.getLeftCorner().id().idNumber(), 20 );
   BOOST_CHECK_EQUAL( left_ghost_zone1.getRightCorner().id().idNumber(), 21 );
   
   // Check left ghost zone 2
   BOOST_CHECK_EQUAL( left_ghost_zone2.id().idNumber(), 11 );
   BOOST_CHECK( fabs(left_ghost_zone2.length()-second_zone.length())
                        < 10.0*numeric_limits<Zone<OneD>::LengthType>::epsilon() );
   BOOST_CHECK( fabs(left_ghost_zone2.area()-second_zone.area())
                        < 10.0*numeric_limits<Zone<OneD>::LengthType>::epsilon() );
   BOOST_CHECK( fabs(left_ghost_zone2.volume()-second_zone.volume())
                        < 10.0*numeric_limits<Zone<OneD>::LengthType>::epsilon() );
   BOOST_CHECK_EQUAL( left_ghost_zone2.getLeftNode().id().idNumber(), 12 );
   BOOST_CHECK_EQUAL( left_ghost_zone2.getRightNode().id().idNumber(), 11 );
   BOOST_CHECK_EQUAL( left_ghost_zone2.getLeftCorner().id().idNumber(), 22 );
   BOOST_CHECK_EQUAL( left_ghost_zone2.getRightCorner().id().idNumber(), 23 );  
}

// Run a series of tests on the right ghost zones
inline void check_right_ghost_zones(const Zone<OneD>& right_ghost_zone1, const Zone<OneD>& right_ghost_zone2,
                                    const Zone<OneD>& lastm1_zone, const Zone<OneD>& last_zone)
{  
   // Check right ghost zone 1
   BOOST_CHECK_EQUAL( right_ghost_zone1.id().idNumber(), 12 );
   BOOST_CHECK( fabs(right_ghost_zone1.length()-last_zone.length())
                        < 10.0*numeric_limits<Zone<OneD>::LengthType>::epsilon() );
   BOOST_CHECK( fabs(right_ghost_zone1.area()-last_zone.area())
                        < 10.0*numeric_limits<Zone<OneD>::LengthType>::epsilon() );
   BOOST_CHECK( fabs(right_ghost_zone1.volume()-last_zone.volume())
                        < 10.0*numeric_limits<Zone<OneD>::LengthType>::epsilon() );
   BOOST_CHECK_EQUAL( right_ghost_zone1.getLeftNode().id().idNumber(), 10 );
   BOOST_CHECK_EQUAL( right_ghost_zone1.getRightNode().id().idNumber(), 13 );
   BOOST_CHECK_EQUAL( right_ghost_zone1.getLeftCorner().id().idNumber(), 24 );
   BOOST_CHECK_EQUAL( right_ghost_zone1.getRightCorner().id().idNumber(), 25 );
   
   // Check right ghost zone 2
   BOOST_CHECK_EQUAL( right_ghost_zone2.id().idNumber(), 13 );
   BOOST_CHECK( fabs(right_ghost_zone2.length()-lastm1_zone.length())
                        < 10.0*numeric_limits<Zone<OneD>::LengthType>::epsilon() );
   BOOST_CHECK( fabs(right_ghost_zone2.area()-lastm1_zone.area())
                        < 10.0*numeric_limits<Zone<OneD>::LengthType>::epsilon() );
   BOOST_CHECK( fabs(right_ghost_zone2.volume()-lastm1_zone.volume())
                        < 10.0*numeric_limits<Zone<OneD>::LengthType>::epsilon() );
   BOOST_CHECK_EQUAL( right_ghost_zone2.getLeftNode().id().idNumber(), 13 );
   BOOST_CHECK_EQUAL( right_ghost_zone2.getRightNode().id().idNumber(), 14 );
   BOOST_CHECK_EQUAL( right_ghost_zone2.getLeftCorner().id().idNumber(), 26 );
   BOOST_CHECK_EQUAL( right_ghost_zone2.getRightCorner().id().idNumber(), 27 );
}

// Run a series of tests on the left ghost nodes
inline void check_left_ghost_nodes(const Node<OneD>& left_ghost_node1, const Node<OneD>& left_ghost_node2)
{
   // Check left ghost node 1
   const Zone<OneD>& first_ghost_zone = left_ghost_node1.getRightZone();
   BOOST_CHECK_EQUAL( left_ghost_node1.id().idNumber(), 11 );
   BOOST_CHECK( fabs(left_ghost_node1.x() + first_ghost_zone.length())
                        < 10.0*numeric_limits<Zone<OneD>::LengthType>::epsilon() );
   BOOST_CHECK_EQUAL( left_ghost_node1.onBoundary(), false );
   BOOST_CHECK_EQUAL( left_ghost_node1.getLeftZone().id().idNumber(), 11 );
   BOOST_CHECK_EQUAL( left_ghost_node1.getRightZone().id().idNumber(), 10 );
   BOOST_CHECK_EQUAL( left_ghost_node1.getLeftCorner().id().idNumber(), 23 );
   BOOST_CHECK_EQUAL( left_ghost_node1.getRightCorner().id().idNumber(), 20 );
   
   // Check left ghost node 2
   const Zone<OneD>& second_ghost_zone = left_ghost_node2.getRightZone();
   BOOST_CHECK_EQUAL( left_ghost_node2.id().idNumber(), 12 );
   BOOST_CHECK( fabs(left_ghost_node2.x() + second_ghost_zone.length() + first_ghost_zone.length())
                        < 10.0*numeric_limits<Zone<OneD>::LengthType>::epsilon() );
   BOOST_CHECK_EQUAL( left_ghost_node2.onLeftBoundary(), true );
   BOOST_CHECK_EQUAL( left_ghost_node2.onRightBoundary(), false );
   BOOST_CHECK_EQUAL( left_ghost_node2.getRightZone().id().idNumber(), 11 );
   BOOST_CHECK_EQUAL( left_ghost_node2.getRightCorner().id().idNumber(), 22 );
}
   
// Run a series of tests on the right ghost nodes
inline void check_right_ghost_nodes(const Node<OneD>& right_ghost_node1, const Node<OneD>& right_ghost_node2,
                                    double mesh_length)   
{
   // Check right ghost node 1
   const Zone<OneD>& lastm1_ghost_zone = right_ghost_node1.getLeftZone();
   BOOST_CHECK_EQUAL( right_ghost_node1.id().idNumber(), 13 );
   BOOST_CHECK( fabs(right_ghost_node1.x()-(mesh_length + lastm1_ghost_zone.length()))
                        < 10.0*numeric_limits<Zone<OneD>::LengthType>::epsilon() );
   BOOST_CHECK_EQUAL( right_ghost_node1.onBoundary(), false );
   BOOST_CHECK_EQUAL( right_ghost_node1.getLeftZone().id().idNumber(), 12 );
   BOOST_CHECK_EQUAL( right_ghost_node1.getRightZone().id().idNumber(), 13 );
   BOOST_CHECK_EQUAL( right_ghost_node1.getLeftCorner().id().idNumber(), 25 );
   BOOST_CHECK_EQUAL( right_ghost_node1.getRightCorner().id().idNumber(), 26 );
   
   // Check right ghost node 2
   const Zone<OneD>& last_ghost_zone = right_ghost_node2.getLeftZone();
   BOOST_CHECK_EQUAL( right_ghost_node2.id().idNumber(), 14 );
   BOOST_CHECK( fabs(right_ghost_node2.x() - 
                           (mesh_length + lastm1_ghost_zone.length() + last_ghost_zone.length()))
                        < 10.0*numeric_limits<Zone<OneD>::LengthType>::epsilon() );
   BOOST_CHECK_EQUAL( right_ghost_node2.onLeftBoundary(), false );
   BOOST_CHECK_EQUAL( right_ghost_node2.onRightBoundary(), true );
   BOOST_CHECK_EQUAL( right_ghost_node2.getLeftZone().id().idNumber(), 13 );
   BOOST_CHECK_EQUAL( right_ghost_node2.getLeftCorner().id().idNumber(), 27 );
}



// Run a series of tests on the left ghost corners
inline void check_left_ghost_corners(const Corner<OneD>& left_ghost_corner1,
                                     const Corner<OneD>& left_ghost_corner2,
                                     const Corner<OneD>& left_ghost_corner3,
                                     const Corner<OneD>& left_ghost_corner4)
{
   // Check the left-most ghost corner
   BOOST_CHECK_EQUAL( left_ghost_corner1.id().idNumber(), 20 );
   BOOST_CHECK_EQUAL( left_ghost_corner1.getZone().id().idNumber(), 10 );
   BOOST_CHECK_EQUAL( left_ghost_corner1.getNode().id().idNumber(), 11 );
   // Check the second ghost corner
   BOOST_CHECK_EQUAL( left_ghost_corner2.id().idNumber(), 21 );
   BOOST_CHECK_EQUAL( left_ghost_corner2.getZone().id().idNumber(), 10 );
   BOOST_CHECK_EQUAL( left_ghost_corner2.getNode().id().idNumber(), 0 );
   // Check the third ghost corner
   BOOST_CHECK_EQUAL( left_ghost_corner3.id().idNumber(), 22 );
   BOOST_CHECK_EQUAL( left_ghost_corner3.getZone().id().idNumber(), 11 );
   BOOST_CHECK_EQUAL( left_ghost_corner3.getNode().id().idNumber(), 12 );
   // Check the fourth ghost corner
   BOOST_CHECK_EQUAL( left_ghost_corner4.id().idNumber(), 23 );
   BOOST_CHECK_EQUAL( left_ghost_corner4.getZone().id().idNumber(), 11 );
   BOOST_CHECK_EQUAL( left_ghost_corner4.getNode().id().idNumber(), 11 );
}

// Run a series of tests on the right ghost corners.
inline void check_right_ghost_corners(const Corner<OneD>& right_ghost_corner1,
                                      const Corner<OneD>& right_ghost_corner2,
                                      const Corner<OneD>& right_ghost_corner3,
                                      const Corner<OneD>& right_ghost_corner4)
{
   // Check right ghost corner 1
   BOOST_CHECK_EQUAL( right_ghost_corner1.id().idNumber(), 24 );
   BOOST_CHECK_EQUAL( right_ghost_corner1.getZone().id().idNumber(), 12 );
   BOOST_CHECK_EQUAL( right_ghost_corner1.getNode().id().idNumber(), 10 );
   // Check right ghost corner 2
   BOOST_CHECK_EQUAL( right_ghost_corner2.id().idNumber(), 25 );
   BOOST_CHECK_EQUAL( right_ghost_corner2.getZone().id().idNumber(), 12 );
   BOOST_CHECK_EQUAL( right_ghost_corner2.getNode().id().idNumber(), 13 );
   // Check right ghost corner 3
   BOOST_CHECK_EQUAL( right_ghost_corner3.id().idNumber(), 26 );
   BOOST_CHECK_EQUAL( right_ghost_corner3.getZone().id().idNumber(), 13 );
   BOOST_CHECK_EQUAL( right_ghost_corner3.getNode().id().idNumber(), 13 );
   // Check right ghost corner 4
   BOOST_CHECK_EQUAL( right_ghost_corner4.id().idNumber(), 27 );
   BOOST_CHECK_EQUAL( right_ghost_corner4.getZone().id().idNumber(), 13 );
   BOOST_CHECK_EQUAL( right_ghost_corner4.getNode().id().idNumber(), 14 );
}

 


#endif

