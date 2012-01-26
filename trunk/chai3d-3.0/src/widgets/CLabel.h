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
    \version   $MAJOR.$MINOR.$RELEASE $Rev: 714 $
*/
//===========================================================================

//---------------------------------------------------------------------------
#ifndef CLabelH
#define CLabelH
//---------------------------------------------------------------------------
#include "world/CGenericObject.h"
#include "graphics/CFont.h"
#include "graphics/CColor.h"
#include "extras/CString.h"
//---------------------------------------------------------------------------

//===========================================================================
/*!
    \file       CLabel.h

    \brief 
    <b> Widgets </b> \n 
    String Label.
*/
//===========================================================================

//===========================================================================
/*!
    \class      cLabel
    \ingroup    widgets  

    \brief      
    This class provides functionalities to display one line of text.
*/
//===========================================================================
class cLabel : public cGenericObject
{
    //-----------------------------------------------------------------------
    // CONSTRUCTOR & DESTRUCTOR:
    //-----------------------------------------------------------------------

    public:

    //! Constructor of cLabel.
    cLabel(cFont* a_font);

    //! Destructor of cLabel.
    virtual ~cLabel();


	//-----------------------------------------------------------------------
    // MEMBERS:
    //-----------------------------------------------------------------------

    public:

    //! Font type.
    cFont* m_font;

    //! Font color.
    cColorf m_fontColor;

    //! String to be displayed.
    string m_string;

    //! Set font scale factor.
    void setFontScale(double a_scale) { m_fontScale = cAbs(a_scale); }

    //! Get font scale factor.
    double getFontScale() { return (m_fontScale); }

    //-----------------------------------------------------------------------
    // METHODS:
    //-----------------------------------------------------------------------

    public:

    //! Create a copy of current object.
    cLabel* copy(bool a_duplicateMaterialData = false,
                 bool a_duplicateTextureData = false, 
                 bool a_duplicateMeshData = false,
                 bool a_buildCollisionDetector = false);

    //! Get length of string
    double getLength();

    //! Render object in OpenGL.
    virtual void render(cRenderOptions& a_options);


	//-----------------------------------------------------------------------
    // MEMBERS:
    //-----------------------------------------------------------------------

    protected:

    //! Scale factor of fonts.
    double m_fontScale;
};

//---------------------------------------------------------------------------
#endif
//---------------------------------------------------------------------------