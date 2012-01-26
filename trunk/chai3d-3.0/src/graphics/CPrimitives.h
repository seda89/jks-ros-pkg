//===========================================================================
/*
    Software License Agreement (BSD License)
    Copyright (c) 2003-2012, CHAI3D.
    (www.chai3d.org)

    All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions
    are met:

    * Redistributions of source code must retain the above copyright
    notice, this list of conditions and the following disclaimer.

    * Redistributions in binary form must reproduce the above
    copyright notice, this list of conditions and the following
    disclaimer in the documentation and/or other materials provided
    with the distribution.

    * Neither the name of CHAI3D nor the names of its contributors may
    be used to endorse or promote products derived from this software
    without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
    "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
    LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
    FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
    COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
    INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
    BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
    LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
    CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
    LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
    ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
    POSSIBILITY OF SUCH DAMAGE. 

    \author    <http://www.chai3d.org>
    \author    Francois Conti
    \version   $MAJOR.$MINOR.$RELEASE $Rev: 550 $
*/
//===========================================================================

//---------------------------------------------------------------------------
#ifndef CPrimitivesH
#define CPrimitivesH
//---------------------------------------------------------------------------

//===========================================================================
/*!
    \file       CPrimitives.h
    \ingroup    graphics

    \brief
    <b> Graphics </b> \n 
    Building Mesh Primitives.
*/
//===========================================================================

//---------------------------------------------------------------------------
#include "world/CMesh.h"
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// GENERAL PURPOSE FUNCTIONS
//---------------------------------------------------------------------------

//! Create a plane.
void cCreatePlane(cMesh* a_mesh, 
                const double a_lengthX, 
                const double a_lengthY, 
                const cVector3d a_pos = cVector3d(0,0,0),
                const cMatrix3d a_rot = cIdentity3d());

//! Create a map.
void cCreateMap(cMesh* a_mesh, 
                const double a_lengthX, 
                const double a_lengthY, 
                const unsigned int a_numSidesX = 10,
                const unsigned int a_numSidesY = 10,
                const cVector3d a_pos = cVector3d(0,0,0),
                const cMatrix3d a_rot = cIdentity3d());

//! Create a box.
void cCreateBox(cMesh* a_mesh, 
                const double a_lengthX, 
                const double a_lengthY, 
                const double a_lengthZ,
                const cVector3d a_pos = cVector3d(0,0,0),
                const cMatrix3d a_rot = cIdentity3d());

//! Create a tea pot.
void cCreateTeaPot(cMesh* a_mesh, 
                const double a_size,
                const cVector3d a_pos = cVector3d(0,0,0),
                const cMatrix3d a_rot = cIdentity3d());

//! Create a cylinder.
void cCreateCylinder(cMesh* a_mesh, 
                const double a_height,  
                const double a_radius,
                const unsigned int a_numSides = 32,
                const unsigned int a_numHeightSegments = 1,
                const bool a_includeTop = true,
                const bool a_includeBottom = true,
                const cVector3d a_pos = cVector3d(0,0,0),
                const cMatrix3d a_rot = cIdentity3d());

//! Create a pipe.
void cCreatePipe(cMesh* a_mesh, 
                const double a_height,  
                const double a_innerRadius,
                const double a_outerRadius,
                const unsigned int a_numSides = 32,
                const unsigned int a_numHeightSegments = 1,
                const cVector3d a_pos = cVector3d(0,0,0),
                const cMatrix3d a_rot = cIdentity3d());

//! Create a sphere.
void cCreateSphere(cMesh* a_mesh, 
                const double a_radius,  
                const unsigned int a_numSlices = 32,		
                const unsigned int a_numStacks = 32,  
                const cVector3d a_pos = cVector3d(0,0,0),
                const cMatrix3d a_rot = cIdentity3d());

//! Create an ellipsoid.
void cCreateEllipsoid(cMesh* a_mesh, 
                const double a_radiusX,
                const double a_radiusY,
                const double a_radiusZ,
                const unsigned int a_numSlices = 32,  					
                const unsigned int a_numStacks = 32,  
                const cVector3d a_pos = cVector3d(0,0,0),
                const cMatrix3d a_rot = cIdentity3d());

//! Create a ring.
void cCreateRing(cMesh* a_mesh, 
                const double a_innerRadius,      
                const double a_outerRadius,      
                const unsigned int a_numSides = 32,   
                const unsigned int a_numRings = 32,   
                const cVector3d a_pos = cVector3d(0,0,0),
                const cMatrix3d a_rot = cIdentity3d());

//! Create a ring section.
void cCreateRingSection(cMesh* a_mesh, 
                        const double a_innerRadius0,
                        const double a_innerRadius1,
                        const double a_outerRadius,    
                        const double a_coverageAngleDEG = 360,
                        const bool a_includeExtremityFaces = true,
                        const unsigned int a_numSides = 32,   
                        const unsigned int a_numRings = 32,   
                        const cVector3d a_pos = cVector3d(0,0,0),
                        const cMatrix3d a_rot = cIdentity3d());

//! Create a cone.
void cCreateCone(cMesh* a_mesh, 
                const double a_height,  
                const double a_radiusBottom,
                const double a_radiusTop = 0.0,
                const unsigned int a_numSides = 32,
                const unsigned int a_numHeightSegments = 1,
                const bool a_includeBottom = true,
                const bool a_includeTop = true,
                const cVector3d a_pos = cVector3d(0,0,0),
                const cMatrix3d a_rot = cIdentity3d());

//! Create a pyramid
void cCreateSquarePyramid(cMesh* a_mesh, 
                const double a_height,  
                const double a_baseSize,
                const bool a_includeBottom = true,
                const cVector3d a_pos = cVector3d(0,0,0),
                const cMatrix3d a_rot = cIdentity3d());

//! Create an arrow
void cCreateArrow(cMesh* a_mesh,
                  const double a_length = 0.4,
                  const double a_radiusShaft = 0.01,
                  const double a_lengthTip = 0.1,
                  const double a_radiusTip = 0.03,
                  const bool a_includeTipsAtBothExtremities = false,
                  const unsigned int a_numSides = 32,
                  const cVector3d a_direction = cVector3d(0,0,1),
                  const cVector3d a_pos = cVector3d(0,0,0));

//! Create an circular arrow
void cCreateCircularArrow(cMesh* a_mesh, 
                          const double a_innerRadius0 = 0.05,
                          const double a_innerRadius1 = 0.05,
                          const double a_outerRadius = 0.3,
                          const double a_lengthTip = 0.2,
                          const double a_radiusTip = 0.1,
                          const double a_coverageAngleDEG = 270,
                          const bool a_includeTipsAtBothExtremities = false,
                          const unsigned int a_numSides = 32,
                          const unsigned int a_numRings = 32,  
                          const cVector3d a_direction = cVector3d(0,0,1),
                          const cVector3d a_pos = cVector3d(0,0,0));

//---------------------------------------------------------------------------
#endif
//---------------------------------------------------------------------------


