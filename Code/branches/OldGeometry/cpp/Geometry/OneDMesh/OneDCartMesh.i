//--------------------------------------------*-SWIG-*------------------------------------//
/// \file Mesh.i
/// \author $Author: jlconlin $
/// \date $Date: 2008-01-02 08:12:50 -0700 (Wed, 02 Jan 2008) $
/// $Id: OneDCartMesh.i 230 2008-01-02 15:12:50Z jlconlin $
///
/// \brief This file provides the python interface for the Mesh class.
///
/// The Mesh class solves a transport Problem using the diffusion approximation.

#ifndef MESH_I
#define MESH_I

%module Mesh 

%{
#include<vector>
#include "OneDCartMesh.hh"
#include "OneDNode.hh"
#include "OneDZone.hh"
#include "MeshTraits.hh"
%}

%include "std_vector.i"
%include "Misc.i"

%feature("autodoc", "1");
// These are the elements of the header file I wish to wrap
class Mesh {
public: 

    /// Constructor to create a uniform one-dimensional Cartesian mesh.
    Mesh(const double length, const double area, const unsigned int num_zones);

    /// Constructor to create a one-dimensional Cartesian mesh with variable zone lengths.
    Mesh(const std::vector<double>& zone_lengths, const double area=1.0);

    /// Destructor
    ~Mesh();

    /// Return the length of the problem domain.
    %rename(size) length();
    double length() const { return mLength; }

    /// Return the number of zones in the problem domain.
    %rename(__len__) numZones();
    unsigned int numZones() const { return mNumZones; }

    /// Return the number of nodes in the problem domain.
    unsigned int numNodes() const { return mNumZones+1; }

    /// Return the number of corners in the problem domain.
    unsigned int numCorners() const { return mNumZones*2; }

//  %extend{
//      void framework(nmpy_array edges, nmpy_array OUTcenters){

//      }
//  }
};



#if 0
%nodefaultctor Zone; // Tell SWIG not to make a default constructor
class Zone 
{
public:
    /// Returns the length of the Zone.
    double length() const ;
    /// Returns the area of the Zone.
    double area() const ;
    /// Returns the volume of the Zone.
    double volume() const ;
    /// Returns the coordinate of the Zone center.
    double center() const;    
    /// Calculates the distance to the boundary of the Zone.
    double distanceToBndry(double x_pos, double angle) const;
    /// Returns a reference to the left Node.
    const Node& getLeftNode() const ;
    /// Returns a reference to the right Node.
    const Node& getRightNode() const ;
};

%nodefaultctor Node; // Tell SWIG not to make a default constructor
class Node 
{
public:

    /// Returns the Node Position
    double x() const;

    /// Returns true if the node is on the domain boundary and returns false otherwise.
    bool onBoundary() const;
};



%exception GeomIterator::next
{ 
    try { $action }
    catch ( IteratorOutOfRange )
    { 
        //PyErr_SetString(PyExc_StopIteration,"StopIteration called");  // either one works
        SWIG_SetErrorObj(PyExc_StopIteration, SWIG_Py_Void());
        return NULL;
    }
}

template<typename GeomElem>
class GeomIterator
{
public:
    GeomIterator();

    // Add pythonic iterator functionality
    %extend
    {
        GeomIterator&  __iter__()
        {
            return( *($self) );
        }
    }
};

%template(zoneiter) GeomIterator<Zone>;
%extend GeomIterator<Zone>
{
    Zone& next()
    {
        Zone& tmp_res = *(*($self));   // I don't understand this, but it works.
        ++(*($self));  // Call the increment operator Greg wrote
        return( tmp_res);  
    }

    Zone& value()
    {
        return(*(*($self)));   
    }
};

%template(nodeiter) GeomIterator<Node>;
%extend GeomIterator<Node>
{
    Node& next()
    {
        Node& tmp_res = *(*($self));   // I don't understand this, but it works.
        ++(*($self));  // Call the increment operator Greg wrote
        return( tmp_res);  
    }

    Node& value()
    {
        return(*(*($self)));   
    }
};
#endif

#endif
