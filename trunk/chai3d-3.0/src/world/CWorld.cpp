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
    \version   $MAJOR.$MINOR.$RELEASE $Rev: 709 $
*/
//===========================================================================

//---------------------------------------------------------------------------
#include "world/CWorld.h"
//---------------------------------------------------------------------------
#include "lighting/CGenericLight.h"
//---------------------------------------------------------------------------
#ifndef _MSVC
#include <float.h>
#endif
//---------------------------------------------------------------------------

//==========================================================================
/*!
    Constructor of cWorld.

    \fn       cWorld::cWorld()
*/
//===========================================================================
cWorld::cWorld()
{
    // create fog
    m_fog = new cFog();

    // setup background color
    m_backgroundColor.set(0.0f, 0.0f, 0.0f, 1.0f);

    // light sources shell be rendered
    m_renderLightSources = true;  

    // set intensity of shadows
    m_shadowIntensity = 0.7;

    // initialize matrix
    memset(m_worldModelView, 0, sizeof(m_worldModelView));
}


//===========================================================================
/*!
    Destructor of cWorld.  Deletes the world, all his children, and all
    his textures.

    \fn       cWorld::~cWorld()
*/
//===========================================================================
cWorld::~cWorld()
{
    // delete all children
    deleteAllChildren();

    // clear textures list
    deleteAllTextures();
}


//===========================================================================
/*!
    Create new texture and add it to textures list.

    \fn         cTexture2d* cWorld::newTexture()
    \return     Return pointer to new texture entity.
*/
//===========================================================================
cTexture2d* cWorld::newTexture()
{
    // create new texture entity
    cTexture2d* newTexture = new cTexture2d();

    // add texture to list
    m_textures.push_back(newTexture);

    // return pointer to new texture
    return (newTexture);
}


//===========================================================================
/*!
    Add texture to texture list.

    \fn         void cWorld::addTexture(cTexture2d* a_texture)
    \param      a_texture  Texture to be added to the textures list.
*/
//===========================================================================
void cWorld::addTexture(cTexture2d* a_texture)
{
    // add texture to list
    m_textures.push_back(a_texture);
}


//===========================================================================
/*!
    Remove texture from textures list. Texture is not deleted from memory.

    \fn         bool cWorld::removeTexture(cTexture2d* a_texture)
    \param      a_texture  Texture to be removed from textures list.
    \return     Return \b true if operation succeeded
*/
//===========================================================================
bool cWorld::removeTexture(cTexture2d* a_texture)
{
    // set iterator
    std::vector<cTexture2d*>::iterator nextTexture;
    nextTexture = m_textures.begin();

    // search texture in  list and remove it
    for (unsigned int i=0; i<m_textures.size(); i++)
    {
        if ((*nextTexture) == a_texture)
        {
            // remove object from list
            m_textures.erase(nextTexture);

            // return success
            return (true);
        }
    }

    // operation failed
    return (false);
}


//===========================================================================
/*!
    Delete texture from textures list and erase it from memory.

    \fn         bool cWorld::deleteTexture(cTexture2d* a_texture)
    \param      a_texture  Texture to be deleted.
    \return     Return \b true if operation succeeded
*/
//===========================================================================
bool cWorld::deleteTexture(cTexture2d* a_texture)
{
    // remove texture from list
    bool result = removeTexture(a_texture);

    // if operation succeeds, delete object
    if (result)
    {
        delete a_texture;
    }

    // return result
    return (result);
}


//===========================================================================
/*!
    Delete all texture from memory.

    \fn         void cWorld::deleteAllTextures()
*/
//===========================================================================
void cWorld::deleteAllTextures()
{
    // delete all textures
    for (unsigned int i=0; i<m_textures.size(); i++)
    {
        cTexture2d* nextTexture = m_textures[i];
        delete nextTexture;
    }

    // clear textures list
    m_textures.clear();
}


//===========================================================================
/*!
    Set the background color used when rendering.  This really belongs in
    cCamera or cViewport; it's a historical artifact that it lives here.

    \fn         void cWorld::setBackgroundColor(const GLfloat a_red,
                const GLfloat a_green, const GLfloat a_blue)
    \param      a_red  Red component.
    \param      a_green  Green component.
    \param      a_blue  Blue component.
*/
//===========================================================================
void cWorld::setBackgroundColor(const GLfloat a_red, const GLfloat a_green,
                               const GLfloat a_blue)
{
    m_backgroundColor.set(a_red, a_green, a_blue);
}


//===========================================================================
/*!
    Set the background color used when rendering.  This really belongs in
    cCamera or cViewport; it's a historical artifact that it lives here.

    \fn         void cWorld::setBackgroundColor(const cColorf& a_color)
    \param      a_color  new background color.
*/
//===========================================================================
void cWorld::setBackgroundColor(const cColorf& a_color)
{
    m_backgroundColor = a_color;
}


//===========================================================================
/*!
    Add an OpenGL light source to the world. A maximum of eight light
    sources can be registered. For each registered light source, an
    OpenGL lightID number is defined

    \fn         bool cWorld::addLightSource(CGenericLight* a_light)
    \param      a_light light source to register.
    \return     return \b true if light source was registered, otherwise
                return \b false.
*/
//===========================================================================
bool cWorld::addLightSource(cGenericLight* a_light)
{
    // check if number of lights already equal to 8.
    if (m_lights.size() >= CHAI_MAXIMUM_OPENGL_LIGHT_COUNT)
    {
        return (false);
    }

    // search for a free ID number
    int light_id = GL_LIGHT0;
    bool found = false;

    while (light_id < GL_LIGHT0+CHAI_MAXIMUM_OPENGL_LIGHT_COUNT)
    {
        
        // check if ID is not already used
        unsigned int i;
        bool free = true;
        for (i=0; i<m_lights.size(); i++)
        {
            cGenericLight* nextLight = m_lights[i];

            if (nextLight->getGLLightNumber() == light_id)
            {
                free = false;
            }
        }

        // check if a free ID was found
        if (free)
        {
            a_light->setGLLightNumber(light_id);
            found = true;
            break;
        }

        light_id++;
    }

    // finalize
    if (found)
    {
        m_lights.push_back(a_light);
        return (true);
    }
    else
    {
        return (false);
    }
}


//===========================================================================
/*!
    Remove a light source from world.

    \fn         bool cWorld::removeLightSource(cGenericLight* a_light)
    \param      a_light light source to be removed.
    \return     return \b true if light source was removed, otherwise
                return \b false.
*/
//===========================================================================
bool cWorld::removeLightSource(cGenericLight* a_light)
{
    // set iterator
    std::vector<cGenericLight*>::iterator nextLight;

    for(nextLight = m_lights.begin();
        nextLight != m_lights.end();
        nextLight++ ) {


        if ((*nextLight) == a_light)
        {
            // remove object from list
            m_lights.erase(nextLight);

            // return success
            return (true);
        }

    }

    // operation failed
    return (false);
}


//===========================================================================
/*!
    Get access to a particular light source (between 0 and MAXIMUM_OPENGL_LIGHT_COUNT-1).
    Returns a pointer to the requested light, or zero if it's not available.

    \fn         cGenericLight cWorld::getLightSource(int index)
    \param      index  Specifies the light (0 -> 7) that should be accessed
    \return     return \b A pointer to a valid light or 0 if that light doesn't exist                
*/
//===========================================================================
cGenericLight* cWorld::getLightSource(int index) 
{

  // Make sure this is a valid index
  if (index < 0 || (unsigned int)(index) >= m_lights.size()) return 0;

  // Return the light that we were supplied with by the creator of the world
  return m_lights[index];
}


//===========================================================================
/*!
    Render the world in OpenGL.

    \fn         void cWorld::render(cRenderOptions& a_options)
    \param      a_options  Rendering options.
*/
//===========================================================================
void cWorld::render(cRenderOptions& a_options)
{
    // make sure these values always remain at origin.
    // translating the world can create a bug with shadowcasting.
    m_localPos.zero();
    m_globalPos.zero();
    m_localRot.identity();
    m_globalRot.identity();

    // set up the CHAI openGL defaults (see cGenericObject::render())
    glEnable(GL_LIGHTING);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);

    // back up the "global" modelview matrix for future reference
    glGetDoublev(GL_MODELVIEW_MATRIX, m_worldModelView);

    // render light sources
    if (m_renderLightSources && a_options.m_enable_lighting) 
    {
		// enable lighting
		glEnable(GL_LIGHTING);

		// render light sources
		unsigned int i;
		for (i=0; i<m_lights.size(); i++)
		{
			m_lights[i]->renderLightSource(a_options);
		}    
    }

    // render fog
    m_fog->render(a_options);
}


//===========================================================================
/*!
    Determine whether the given segment intersects a triangle in this world.
    The segment is described by a start point /e a_segmentPointA and end point
    /e a_segmentPointB. Collision detection functions of all children of the
    world are called, which recursively call the collision detection functions
    for all objects in this world.  If there is more than one collision,
    the one closest to a_segmentPointA is the one returned.

	\fn	bool cWorld::computeCollisionDetection(cVector3d& a_segmentPointA,
                                       cVector3d& a_segmentPointB,
                                       cCollisionRecorder& a_recorder,
                                       cCollisionSettings& a_settings)
    \param  a_segmentPointA  Start point of segment.  Value may be changed if
                             returned collision is with a moving object.
    \param  a_segmentPointB  End point of segment.
    \param  a_recorder  Stores all collision events
    \param  a_settings  Contains collision settings information.
*/
//===========================================================================
bool cWorld::computeCollisionDetection(cVector3d& a_segmentPointA,
                                       cVector3d& a_segmentPointB,
                                       cCollisionRecorder& a_recorder,
                                       cCollisionSettings& a_settings)
{
    // temp variable
    bool hit = false;
    cVector3d segmentPointA = a_segmentPointA;
    cVector3d segmentPointB = a_segmentPointB;

    // check for collisions with all children of this world
    unsigned int nChildren = (int)(m_children.size());
    for (unsigned int i=0; i<nChildren; i++)
    {
        hit = hit | m_children[i]->computeCollisionDetection(a_segmentPointA,
                                                       a_segmentPointB,
                                                       a_recorder,
                                                       a_settings);
    }

    // restore values.
    a_segmentPointA = segmentPointA;
    a_segmentPointB = segmentPointB;

    // return whether there was a collision between the segment and this world
    return (hit);
}

