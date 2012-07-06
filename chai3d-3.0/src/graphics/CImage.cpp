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
#include "graphics/CImage.h"
#include "files/CFileImageBMP.h"
#include "files/CFileImageGIF.h"
#include "files/CFileImageJPG.h"
#include "files/CFileImagePNG.h"
#include "files/CFileImagePPM.h"
#include "files/CFileImageRAW.h"
//---------------------------------------------------------------------------

//===========================================================================
/*!
    Default constructor of cImage.

    \fn     cImage::cImage()
*/
//===========================================================================
cImage::cImage()
{
    // init internal variables
    defaults();
}


//===========================================================================
/*!
    Constructor of cImage. Allocate image by passing width, height and 
    pixel format.

    \fn     cImage::cImage(const unsigned int a_width, 
                           const unsigned int a_height,
                           const GLenum a_format,
                           const GLenum a_type)
    \param  a_width     Width of image
    \param  a_height    Height of image
    \param  a_format    Pixel format. Accepted values are for instance (GL_LUMINANCE, GL_RGB, GL_RGBA)
    \param  a_type      Pixel type. Accepted values are for instance (GL_UNSIGNED_BYTE, GL_UNSIGNED_INT)
*/
//===========================================================================
cImage::cImage(const unsigned int a_width, 
               const unsigned int a_height,
               const GLenum a_format,
               const GLenum a_type)
{
    // init internal variables
    defaults();

    // allocate image
    allocate(a_width, a_height, a_format, a_type);
}


//===========================================================================
/*!
    Destructor of cImage.

    \fn     cImage::~cImage()
*/
//===========================================================================
cImage::~cImage()
{
    // clean up memory
    cleanup();
}


//===========================================================================
/*!
    Initializes internal variables.

    \fn     void cImage::defaults()
*/
//===========================================================================
void cImage::defaults()
{
    // no filename defined
    m_filename          = "";

    // size of image is zero since not yet defined and allocated
    m_width             = 0;
    m_height            = 0;

    // default format is RGB
    m_format            = GL_RGB;
    m_type              = GL_UNSIGNED_BYTE;
    m_bytesPerPixel     = queryBytesPerPixel(m_format, m_type);

    // image has not yet been allocated
    m_allocated         = false;
    m_data              = NULL;
    m_memorySize        = 0;

    // this value is set to 'true' when the user calls the allocate() function.
    // however, if the user uses setData() instead to define the memory where the image
    // data is located, then its value is set to 'false', at the current object
    // will not take care of freeing the image data.
    m_responsibleForMemoryAllocation = false;

    // default border color is black
    m_borderColor.set(0x00, 0x00, 0x00, 0xFF);
}


//===========================================================================
/*!
    Free memory that was used for image data, and re-initialize
    internal variables.

    \fn     void cImage::cleanup()
*/
//===========================================================================
void cImage::cleanup()
{
    // delete image data
    if ((m_data != NULL) && (m_responsibleForMemoryAllocation))
    {
        delete [] m_data;
    }

    // reset to default values
    defaults();
}


//===========================================================================
/*!
    Allocate a new image by defining its width, height and pixel format.

    \fn     bool cImage::allocate(const unsigned int a_width, 
                                  const unsigned int a_height,
                                  const GLenum a_format,
                                  const GLenum a_type)
    \param  a_width     Width of image
    \param  a_height    Height of image
    \param  a_format    Pixel format. Accepted values are for instance (GL_LUMINANCE, GL_RGB, GL_RGBA)
    \param  a_type      Pixel type. Accepted values are for instance (GL_UNSIGNED_BYTE, GL_UNSIGNED_INT)
*/
//===========================================================================
bool cImage::allocate(const unsigned int a_width, 
                      const unsigned int a_height,
                      const GLenum a_format,
                      const GLenum a_type)
{
    // verify the memory requirements for given format
    int bytesPerPixel = queryBytesPerPixel(a_format, a_type);
    if (bytesPerPixel < 1)
    {
        // format not valid
        return (false);
    }

    // allocate memory
    m_width             = a_width;
    m_height            = a_height;
    m_bytesPerPixel     = bytesPerPixel;
    m_format            = a_format;
    m_type              = a_type;
    m_memorySize        = m_width * m_height * m_bytesPerPixel;
    
    // delete current image data
    if (m_data)
    {
        delete [] m_data;
    }

    // allocated new image data
    m_data = new unsigned char[m_memorySize];

    // check if memory has been allocated, otherwise cleanup
    if (m_data == NULL)
    {
        // allocation failed
        cleanup();
        return (false);
    }
    else
    {
        // image data has been allocated
        m_allocated = true;
        m_responsibleForMemoryAllocation = true;
    }

    // clear image
    clear();

    // success
    return (true);
}


//===========================================================================
/*!
    Creates a copy of itself.

    \fn     cImage* cImage::copy()
    \return Return pointer to new object.
*/
//===========================================================================
cImage* cImage::copy()
{
    // allocate new image
    cImage* image = new cImage();
    image->allocate(m_width, m_height, m_format);

    // copy image data
    copyTo(image);

    // return new image
    return (image);
}


//===========================================================================
/*!
    Query the number of bytes per pixel for a given format.

    \fn       int cImage::queryBytesPerPixel(const GLenum a_format, const GLenum a_type)
    \param    a_format  Pixel format of image.
    \param    a_type  Pixel data type of image.
    \return   Returns the number of bytes used for storing a single pixel.
*/
//===========================================================================
int cImage::queryBytesPerPixel(const GLenum a_format, const GLenum a_type)
{
    int bytesPerPixel = -1;
    int bytesPerPixelComponent = -1;
    
    // verify if requested type is supported
    switch (a_type)
    {
        // type: BYTE (1 bytes)
        case GL_BYTE:
        case GL_UNSIGNED_BYTE:
        {
            bytesPerPixelComponent = 1;
        }
        break;

        // type: BYTE (1 bytes)
        case GL_SHORT:
        case GL_UNSIGNED_SHORT:
        {
            bytesPerPixelComponent = 2;
        }
        break;

        // type: BYTE (4 bytes)
        case GL_UNSIGNED_INT:
        {
            bytesPerPixelComponent = 4;
        }
        break;
    }

    // if the number of bytes per pixel is unknown, then exit.
    if (bytesPerPixelComponent == -1) { return (-1); }


    // verify if requested format is supported
    switch (a_format)
    {    
        // 1 component per pixel:
        case GL_LUMINANCE:
        case GL_DEPTH_COMPONENT:
            {
                bytesPerPixel = 1 * bytesPerPixelComponent;    
            }
            break;

        // 2 components per pixel:
        case GL_LUMINANCE_ALPHA:
            {
                bytesPerPixel = 2 * bytesPerPixelComponent;
            }
            break;

        // 3 components per pixel:
        case GL_RGB:
            {
                bytesPerPixel = 3 * bytesPerPixelComponent;    
            }
            break;

        // 4 components per pixel:
        case GL_RGBA:
            {
                bytesPerPixel = 4 * bytesPerPixelComponent;    
            }
            break;
    }

    // return result
    return (bytesPerPixel);
}


//===========================================================================
/*!
    Convert current image into a new pixel format.

    \fn     bool cImage::convert(unsigned int a_newFormat)
    \param  a_format  New desired pixel format. (GL_LUMINANCE, GL_RGB, GL_RGBA)
    \return Returns \e true if success, \e false otherwise.
*/
//===========================================================================
bool cImage::convert(unsigned int a_newFormat)
{
    // verify if new format is dfferent
    if (a_newFormat == m_format) { return (true); }

    // allocate memory for converted image
    cImage* image = new cImage();
    image->allocate(m_width, m_height, a_newFormat);

    // convert image by using the copy function
    copyTo(0, 0, m_width, m_height, image, 0, 0);

    // free current image
    cleanup();

    // retrieve converted image
    unsigned char* data = image->getData();
    unsigned int size   = image->getSizeInBytes();

    // assign new values to current image
    setData(data, size);
	setProperties(image->getWidth(), image->getHeight(), image->getFormat(), GL_UNSIGNED_BYTE);

    // current object will be responsible for freeing memory
    m_responsibleForMemoryAllocation = true;
    
    // set data property of temp image to NULL so that we do not delete 
    // allocated data memory when deleting object image
    image->setData(NULL, 0);
    delete image;

    // success
    return (true);
}


//===========================================================================
/*!
    Assign new properties including width, height and pixel format to the image. 
    If the requested image properties do not match the exact amount of allocated data 
    image, then the operation fails and the desired properties are ignored. 

    \fn     bool cImage::setProperties(unsigned int a_width, 
                           unsigned int a_height, 
                           unsigned int a_format)
    \param  a_width     Width of new image.
    \param  a_height    Height of new image.
    \param  a_format    Pixel format. Accepted values are (GL_LUMINANCE, GL_RGB, GL_RGBA).
    \return Returns \e true if success, \e false otherwise.
*/
//===========================================================================
bool cImage::setProperties(unsigned int a_width, 
                           unsigned int a_height, 
                           GLenum a_format,
                           GLenum a_type)
{
    // sanity check on image format
    int bytesPerPixel = queryBytesPerPixel(a_format, a_type);
    if (bytesPerPixel < 1) { return (false); }

    // compute image size
    unsigned int size = a_width * a_height * bytesPerPixel;
    
    // verify that image properties matches allocated memory
    if (size != m_memorySize)
    {
        return (false);
    }

    // all looks good, we can update image properties
    m_width  = a_width;
    m_height = a_height;
    m_format = a_format;

    // success
    return (true);
}


//===========================================================================
/*!
    Copy a section of this current image to a different location or onto 
    a different destination image. 

    \fn     void cImage::copyTo(unsigned int a_sourcePosX,
                    unsigned int a_sourcePosY,
                    unsigned int a_sourceSizeX,
                    unsigned int a_sourceSizeY,
                    cImage* a_destImage, 
                    unsigned int a_destPosX,
                    unsigned int a_destPosY)
    \param  a_sourcePosX  X coordinate of top left pixel to copy.
    \param  a_sourcePosY  Y coordinate of top left pixel to copy.
    \param  a_sourceSizeX  Width of area to copy.
    \param  a_sourceSizeY  Height of area to copy.
    \param  a_destImage  Destination image when area will be pasted.
    \param  a_destPosX  X coordinate of top left pixel on destination image where area is pasted.
    \param  a_destPosY  Y coordinate of top left pixel on destination image where area is pasted.
*/
//===========================================================================
void cImage::copyTo(unsigned int a_sourcePosX,
                    unsigned int a_sourcePosY,
                    unsigned int a_sourceSizeX,
                    unsigned int a_sourceSizeY,
                    cImage* a_destImage, 
                    unsigned int a_destPosX,
                    unsigned int a_destPosY)
{
    // temp variables
    unsigned int i,j;

    // sanity check
    if ((!m_allocated) || (a_destImage == NULL))
        { return; }

    if (!a_destImage->isInitialized())
        { return; }

    // if source and destination images are the same object, then we need 
    // to create a copied image of the source. (overwritting problem)
    cImage* temp_image = NULL;
    if (a_destImage == this)
    {
        temp_image = copy();
    }

    // store values
    unsigned int src_x = a_sourcePosX;
    unsigned int src_y = a_sourcePosY;
    unsigned int src_dx = a_sourceSizeX;
    unsigned int src_dy = a_sourceSizeY;
    unsigned int dst_x = a_destPosX;
    unsigned int dst_y = a_destPosY;
    unsigned char* src_img_data = m_data;
    if (temp_image)
    {
        src_img_data = temp_image->m_data;
    }
    unsigned char* dst_img_data = a_destImage->m_data;
    unsigned int src_w = m_width;
    unsigned int src_h = m_height;
    unsigned int dst_w = a_destImage->m_width;
    unsigned int dst_h = a_destImage->m_height;

    // verifications and area clamping related to source image
    if ((src_x >= src_w) || (src_y >= src_h))
        { return; }

    unsigned int origin = 0;
    unsigned int max_src_x = src_w - src_x;
    unsigned int max_src_y = src_h - src_y; 
    src_dx = cClamp(src_dx, origin, max_src_x);
    src_dy = cClamp(src_dy, origin, max_src_y);

    // verifications and area clamping related to destination image
    if ((dst_x >= dst_w) || (dst_y >= dst_h))
        { return; }

    unsigned int max_dst_x = dst_w - dst_x;
    unsigned int max_dst_y = dst_h - dst_y;
    src_dx = cClamp(src_dx, origin, max_dst_x);
    src_dy = cClamp(src_dy, origin, max_dst_y);

    // the area has now been clamped and we can safely proceed with the data copying
    unsigned int src_format = m_format;
    unsigned int dst_format = a_destImage->getFormat();

    // if source and destimation carry the same format and size, and if the 
    // entire image is being copied, then perform a fast copy
    if ((src_format == dst_format) &&
        (src_x == 0) && (src_y == 0) &&
        (dst_x == 0) && (dst_y == 0) &&
        (src_dx == src_w) && (src_dy == src_h))
    {
        memcpy(dst_img_data, src_img_data, m_memorySize);
    }

    // src format: RGB
    else if (src_format == GL_RGB)
    {
        // information about src image
        int src_bpp = 3;
        int src_segment_size = src_dx * src_bpp;
        int src_offset = (src_w - src_dx) * src_bpp;
        unsigned int src_index = src_bpp * (src_x + src_y * src_w);
        unsigned char* src_data = &(src_img_data[src_index]);

        // dst format: RGB
        if (dst_format == GL_RGB)
        {
            int dst_bpp = 3;
            int dst_offset = dst_w * dst_bpp;
			src_offset = src_w * src_bpp;
            unsigned int dst_index = dst_bpp * (dst_x + dst_y * dst_w);
            unsigned char* dst_data = &(dst_img_data[dst_index]);
            for (i=0; i<src_dy; i++)
            {
				memcpy(dst_data, src_data, src_segment_size);
                src_data += src_offset;
                dst_data += dst_offset;
            }
        }

        // dst format: RGBA
        if (dst_format == GL_RGBA)
        {
            int dst_bpp = 4;
            int dst_offset = (dst_w - src_dx) * dst_bpp;
            unsigned int dst_index = dst_bpp * (dst_x + dst_y * dst_w);
            unsigned char* dst_data = &(dst_img_data[dst_index]);
            for (i=0; i<src_dy; i++)
            {
                for (j=0; j<src_dx; j++)
                {
                    *dst_data = *src_data; dst_data++; src_data++;
                    *dst_data = *src_data; dst_data++; src_data++;
                    *dst_data = *src_data; dst_data++; src_data++;
                    *dst_data = 0xff; dst_data++;
                }
                src_data += src_offset;
                dst_data += dst_offset;
            }
        }

        // dst format: LUMINANCE
        else if (dst_format == GL_LUMINANCE)
        {
            int dst_bpp = 1;
            int dst_offset = (dst_w - src_dx) * dst_bpp;
            unsigned int dst_index = dst_bpp * (dst_x + dst_y * dst_w);
            unsigned char* dst_data = &(dst_img_data[dst_index]);
            for (i=0; i<src_dy; i++)
            {
                for (j=0; j<src_dx; j++)
                {
                    *dst_data = (unsigned char)(0.33333 * ( (float)(src_data[0]) +
                                                            (float)(src_data[1]) +
                                                            (float)(src_data[2]) )); 
                    dst_data++; src_data+=3;
                }
                src_data += src_offset;
                dst_data += dst_offset;
            }
        }
    }

    // src format: RGBA
    else if (src_format == GL_RGBA)
    {
        // information about src image
        int src_bpp = 4;
        int src_segment_size = src_dx * src_bpp;
        int src_offset = (src_w - src_dx) * src_bpp;
        unsigned int src_index = src_bpp * (src_x + src_y * src_w);
        unsigned char* src_data = &(src_img_data[src_index]);

        // dst format: RGB
        if (dst_format == GL_RGB)
        {
            int dst_bpp = 3;
            int dst_offset = (dst_w - src_dx) * dst_bpp;
            unsigned int dst_index = dst_bpp * (dst_x + dst_y * dst_w);
            unsigned char* dst_data = &(dst_img_data[dst_index]);
            for (i=0; i<src_dy; i++)
            {
                for (j=0; j<src_dx; j++)
                {
                    *dst_data = *src_data; dst_data++; src_data++;
                    *dst_data = *src_data; dst_data++; src_data++;
                    *dst_data = *src_data; dst_data++; src_data+=2;
                }
                src_data += src_offset;
                dst_data += dst_offset;
            }
        }

        // dst format: RGBA
        if (dst_format == GL_RGBA)
        {
            int dst_bpp = 4;
			int dst_offset = dst_w * dst_bpp;
			src_offset = src_w * src_bpp;
            unsigned int dst_index = dst_bpp * (dst_x + dst_y * dst_w);
            unsigned char* dst_data = &(dst_img_data[dst_index]);
            for (i=0; i<src_dy; i++)
            {
				memcpy(dst_data, src_data, src_segment_size);
                src_data += src_offset;
                dst_data += dst_offset;
            }
        }

        // dst format: LUMINANCE
        if (dst_format == GL_LUMINANCE)
        {
            int dst_bpp = 1;
            int dst_offset = (dst_w - src_dx) * dst_bpp;
            unsigned int dst_index = dst_bpp * (dst_x + dst_y * dst_w);
            unsigned char* dst_data = &(dst_img_data[dst_index]);
            for (i=0; i<src_dy; i++)
            {
                for (j=0; j<src_dx; j++)
                {
                    *dst_data = (unsigned char)(0.33333 * ( (float)(src_data[0]) +
                                                            (float)(src_data[1]) +
                                                            (float)(src_data[2]) )); 
                    dst_data++; src_data+=4;
                }
                src_data += src_offset;
                dst_data += dst_offset;
            }
        }
    }

    // src format: LUMINANCE
    else if (src_format == GL_LUMINANCE)
    {
        // information about src image
        int src_bpp = 1;
        int src_segment_size = src_dx * src_bpp;
        int src_offset = (src_w - src_dx) * src_bpp;
        unsigned int src_index = src_bpp * (src_x + src_y * src_w);
        unsigned char* src_data = &(src_img_data[src_index]);

        // dst format: RGB
        if (dst_format == GL_RGB)
        {
            int dst_bpp = 3;
            int dst_offset = (dst_w - src_dx) * dst_bpp;
            unsigned int dst_index = dst_bpp * (dst_x + dst_y * dst_w);
            unsigned char* dst_data = &(dst_img_data[dst_index]);
            for (i=0; i<src_dy; i++)
            {
                for (j=0; j<src_dx; j++)
                {
                    *dst_data = *src_data; dst_data++; 
                    *dst_data = *src_data; dst_data++;
                    *dst_data = *src_data; dst_data++;
                    src_data++;
                }
                src_data += src_offset;
                dst_data += dst_offset;
            }
        }

        // dst format: RGBA
        if (dst_format == GL_RGBA)
        {
            int dst_bpp = 4;
            int dst_offset = (dst_w - src_dx) * dst_bpp;
            unsigned int dst_index = dst_bpp * (dst_x + dst_y * dst_w);
            unsigned char* dst_data = &(dst_img_data[dst_index]);
            for (i=0; i<src_dy; i++)
            {
                for (j=0; j<src_dx; j++)
                {
                    *dst_data = *src_data; dst_data++; 
                    *dst_data = *src_data; dst_data++;
                    *dst_data = *src_data; dst_data++;
                    *dst_data = 0xff; dst_data++;
                    src_data++;
                }
                src_data += src_offset;
                dst_data += dst_offset;
            }
        }

        // dst format: LUMINANCE
        if (dst_format == GL_LUMINANCE)
        {
            int dst_bpp = 1;
            int dst_offset = dst_w * dst_bpp;
			src_offset = src_w * src_bpp;
            unsigned int dst_index = dst_bpp * (dst_x + dst_y * dst_w);
            unsigned char* dst_data = &(dst_img_data[dst_index]);
            for (i=0; i<src_dy; i++)
            {
				memcpy(dst_data, src_data, src_segment_size);
                src_data += src_offset;
                dst_data += dst_offset;
            }
        }
    }

    // cleanup
    if (temp_image != NULL)
    {
        delete temp_image;
    }
}

//! 
//===========================================================================
/*!
    Copy this entire image to another destination image. If the destination 
	images is smaller than the source, the copied area is cropped before being 
	copied.

    \fn     void cImage::copyTo(cImage* a_destImage, 
                    unsigned int a_destPosX,
                    unsigned int a_destPosY)
    \param  a_destImage  Destination image when area will be pasted.
    \param  a_destPosX  X coordinate of top left pixel on destination image where area is pasted.
    \param  a_destPosY  Y coordinate of top left pixel on destination image where area is pasted.
*/
//===========================================================================
void cImage::copyTo(cImage* a_destImage, 
                    unsigned int a_destPosX,
                    unsigned int a_destPosY)
{
    copyTo(0, 0, m_width, m_height, a_destImage, 0, 0);
}


//===========================================================================
/*!
    Assign a different memory location for the image data. Use with care!
	Make sure to call function setProperties() afterwards in order to correctly
	set the dimension and pixel format of the image described by the new data.

    \fn     void cImage::setData(unsigned char* a_data, 
	                             unsigned int a_dataSizeInBytes)
	\param	Pointer to new image data.
	\param  Size in byte of the image data.
*/
//===========================================================================
void cImage::setData(unsigned char* a_data, unsigned int a_dataSizeInBytes, bool a_dealloc)
{
    // update data information
    m_data            = a_data;
    m_memorySize      = a_dataSizeInBytes;
    m_allocated       = true;
    m_responsibleForMemoryAllocation = a_dealloc;

    // verify if image dimension match, otherwise set dimensions to zero
    unsigned int size = m_width * m_height * m_bytesPerPixel;
    if (size != m_memorySize)
    {
        m_width  = 0;
        m_height = 0;
    }
}


//===========================================================================
/*!
    Clear an image with defaults data.

    \fn     void cImage::clear()
*/
//===========================================================================
void cImage::clear()
{
    clear(m_borderColor);
}


//===========================================================================
/*!
    Clear an image with a defined color

    \fn     void cImage::clear(const cColorb& a_color)
    \param  a_color  new color of the image
*/
//===========================================================================
void cImage::clear(const cColorb& a_color)
{
    // check if image exists
    if (!m_allocated) { return; }

    // image size
    unsigned int size = m_width * m_height;

    // format: RGB
    if (m_format == GL_RGB)
    {
        unsigned char r = a_color.getR();
        unsigned char g = a_color.getG();
        unsigned char b = a_color.getB();
        unsigned char* data = (unsigned char*)m_data;
        unsigned int i;
        for (i=0; i<size; i++)
        {
            *data = r; data++;
            *data = g; data++;
            *data = b; data++;
        }
    }

    // format: RGBA
    else if (m_format == GL_RGBA)
    {
        int* data = (int*)m_data;
        int* color = (int*)a_color.pColor();
        unsigned int i;
        for (i=0; i<size; i++)
        {
            *data = *color; data++;
        }
    }

    // format: LUMINANCE
    else if (m_format == GL_LUMINANCE)
    {
        unsigned char l = a_color.getLuminance();
        unsigned char* data = (unsigned char*)m_data;
        unsigned int i;
        for (i=0; i<size; i++)
        {
            *data = l; data++;
        }
    }
}


//===========================================================================
/*!
    Get the color of a pixel by passing its x and y coordinate

    \fn     bool cImage::getPixelColor(const unsigned int a_x, const
            unsigned int a_y)

    \param  a_x X coordinate of the pixel
    \param  a_y Y coordinate of the pixel
    \param  a_color  Return the color of the pixel
    \return return \b true if operation succeeds, \b false otherwise.
*/
//===========================================================================
bool cImage::getPixelColor(const unsigned int a_x, 
                           const unsigned int a_y, 
                           cColorb& a_color)
{

    //  TODO:
    //  Support various FORMAT and pixel data TYPES (currently bytes only)
    //

    if ((m_allocated) && (a_x < ((unsigned int)(m_width))) && (a_y < ((unsigned int)(m_height))))
    {
        // format: RGB
        if (m_format == GL_RGB)
        {
            unsigned int index = 3 * (a_x + a_y * m_width);
            a_color.set(m_data[index],
                        m_data[index+1],
                        m_data[index+2]);
            return (true);
        }

        // format: RGBA
        else if (m_format == GL_RGBA)
        {
            unsigned int index = 4 * (a_x + a_y * m_width);
            int* color = (int*)a_color.pColor();
            int* data = (int*)&(m_data[index]);
            *color = *data;
            return (true);
        }

        // format: LUMINANCE
        else if (m_format == GL_LUMINANCE)
        {
            unsigned int index = a_x + a_y * m_width;
            unsigned char l = m_data[index];
            a_color.set(l, l, l);
            return (true);
        }

        // format not defined
        else
        {
            a_color = m_borderColor;
            return (false);
        }
    }
    else
    {
        a_color = m_borderColor;
        return (false);
    }
}


//===========================================================================
/*!
    Set the color of a pixel

    \fn     void cImage::setPixelColor(const unsigned int a_x,
            const unsigned int a_y, const cColorb& a_color)

    \param  a_x X coordinate of the pixel
    \param  a_y Y coordinate of the pixel
    \param  a_color new color of the pixel
*/
//===========================================================================
void cImage::setPixelColor(const unsigned int a_x, const unsigned int a_y, const cColorb& a_color)
{
    // check if image exists
    if (!m_allocated) { return; }

    if ((a_x < ((unsigned int)(m_width))) && (a_y < ((unsigned int)(m_height))))
    {
        // format: RGB
        if (m_format == GL_RGB)
        {
            unsigned int index = 3 * (a_x + a_y * m_width);
            unsigned char* data = (unsigned char*)m_data;
            data[index]   = a_color.getR();
            data[index+1] = a_color.getG();
            data[index+2] = a_color.getB();
        }

        // format: RGBA
        else if (m_format == GL_RGBA)
        {
            unsigned int index = (a_x + a_y * m_width);
            int* data   = (int*)m_data;
            int* color  = (int*)a_color.pColor();
            data[index] = *color;
        }

        // format: LUMINANCE
        else if (m_format == GL_LUMINANCE)
        {
            unsigned int index  = (a_x + a_y * m_width);
            unsigned char* data = (unsigned char*)m_data;
            data[index] = a_color.getLuminance();
        }
    }
}


//===========================================================================
/*!
    Define a pixel color to be transparent. If the image is not in GL_RGBA 
	format, it is first converted in order to enable pixel transparency
	capabilities.

    \fn     void cImage::setTransparentColor(const cColorb &a_color, 
								 const unsigned char a_transparencyLevel)

    \param  a_color Selected pixel color.
	\param  a_transparencyLevel  Transparency level.
*/
//===========================================================================
void cImage::setTransparentColor(const cColorb &a_color, 
								 const unsigned char a_transparencyLevel)
{
    // check if image exists
    if (!m_allocated) { return; }

    // image size
    unsigned int size = m_width * m_height;

	// verify format, convert otherwise
	if (m_format != GL_RGBA)
	{
		convert(GL_RGBA);
	}

	// format: RGBA
	if (m_format == GL_RGBA)
	{
		unsigned char r = a_color.getR();
        unsigned char g = a_color.getG();
        unsigned char b = a_color.getB();
        unsigned char* data = (unsigned char*)m_data;
        unsigned int i;
        for (i=0; i<size; i++)
        {
			if ((data[0] == r) && (data[1] == g) && (data[2] == b))
			{
				data[3] = a_transparencyLevel;
			}
            
            data+=4;
        }
	}
}


//===========================================================================
/*!
    Define a transparent component to all image pixels.

    \fn     void cImage::setTransparency(const unsigned char a_transparencyLevel)
	\param  a_transparencyLevel  Transparency level.
*/
//===========================================================================
void cImage::setTransparency(const unsigned char a_transparencyLevel)
{
    // check if image exists
    if (!m_allocated) { return; }

    // image size
    unsigned int size = m_width * m_height;

	// verify format, convert otherwise
	if (m_format != GL_RGBA)
	{
		convert(GL_RGBA);
	}

	// format: RGBA
	if (m_format == GL_RGBA)
	{
		unsigned char* data = &m_data[3];
        unsigned int i;
        for (i=0; i<size; i++)
        {
			*data = a_transparencyLevel;
            data+=4;
        }
	}
}


//===========================================================================
/*!
    Define a pixel color to be transparent. If the image is not in GL_RGBA 
	format, it is first converted in order to enable pixel transparency
	capabilities.

    \fn     void cImage::setTransparentColor(const cColorb &a_color, 
								 const unsigned char a_transparencyLevel)

    \param  a_r Red component of selected pixel color.
	\param  a_r Green component of selected pixel color.
	\param  a_r Blue component of selected pixel color.
	\param  a_transparencyLevel  Transparency level.
*/
//===========================================================================
void cImage::setTransparentColor(const unsigned char a_r,
								 const unsigned char a_g,
								 const unsigned char a_b, 
								 const unsigned char a_transparencyLevel)
{
	cColorb color(a_r, a_g, a_b);
	setTransparentColor(color, a_transparencyLevel);
}


//===========================================================================
/*!
    Loads this image from the specified file.  Returns true if all
    goes well.  Note that regardless of whether it succeeds, 
    this over-writes any image that had previously been loaded by this object.    
    Always converts the resulting image to RGB.    

    \fn     bool cImage::loadFromFile(const char* a_filename)
    \param  a_filename  Image filename
*/
//===========================================================================
bool cImage::loadFromFile(string a_filename)
{
    // cleanup previous image
    cleanup();

    // find extension
    string extension = cFindFileExtension(a_filename);

    // we need a file extension to figure out file type
    if (extension.length() == 0)
    {
        return (false);
    }

    // convert string to lower extension
    string fileType = cStringToLower(extension);

    // result for loading file
    bool result = false;

    //--------------------------------------------------------------------
    // .BMP FORMAT
    //--------------------------------------------------------------------
    if (fileType == "bmp")
    {
        result = cLoadFileBMP(this, a_filename);
    }

    //--------------------------------------------------------------------
    // .GIF FORMAT
    //--------------------------------------------------------------------
    else if (fileType == "gif")
    {
        result = cLoadFileGIF(this, a_filename);
    }

    //--------------------------------------------------------------------
    // .JPG FORMAT
    //--------------------------------------------------------------------
    else if (fileType == "jpg")
    {
        result = cLoadFileJPG(this, a_filename);
    }

    //--------------------------------------------------------------------
    // .PNG FORMAT
    //--------------------------------------------------------------------
    else if (fileType == "png")
    {
        result = cLoadFilePNG(this, a_filename);
    }

    //--------------------------------------------------------------------
    // .PPM FORMAT
    //--------------------------------------------------------------------
    else if (fileType == "ppm")
    {
        result = cLoadFilePPM(this, a_filename);
    }

    //--------------------------------------------------------------------
    // .RAW FORMAT
    //--------------------------------------------------------------------
    else if (fileType == "raw")
    {
        result = cLoadFileRAW(this, a_filename);
    }

    return (result);
}


//===========================================================================
/*!
    Saves this image from the specified file.  Returns 0 if all
    goes well. All images are saved in RGB format. 

    \fn     bool cImage::saveToFile(const char* a_filename)
    \param  a_filename  Image filename
*/
//===========================================================================
bool cImage::saveToFile(string a_filename)
{
    // find extension
    string extension = cFindFileExtension(a_filename);

    // we need a file extension to figure out file type
    if (extension.length() == 0)
    {
        return (false);
    }

    // convert string to lower extension
    string fileType = cStringToLower(extension);

    // image
    cImage* image = this;

    // result for saving file
    bool result = false;

    //--------------------------------------------------------------------
    // .BMP FORMAT
    //--------------------------------------------------------------------
    if (fileType == "bmp")
    {
        result = cSaveFileBMP(image, a_filename);
    }

    //--------------------------------------------------------------------
    // .GIF FORMAT
    //--------------------------------------------------------------------
    else if (fileType == "gif")
    {
        result = cSaveFileGIF(image, a_filename);
    }

    //--------------------------------------------------------------------
    // .JPG FORMAT
    //--------------------------------------------------------------------
    else if (fileType == "jpg")
    {
        result = cSaveFileJPG(image, a_filename);
    }

    //--------------------------------------------------------------------
    // .PNG FORMAT
    //--------------------------------------------------------------------
    else if (fileType == "png")
    {
        result = cSaveFilePNG(image, a_filename);
    }

    //--------------------------------------------------------------------
    // .PPM FORMAT
    //--------------------------------------------------------------------
    else if (fileType == "ppm")
    {
        result = cSaveFilePPM(image, a_filename);
    }

    //--------------------------------------------------------------------
    // .RAW FORMAT
    //--------------------------------------------------------------------
    else if (fileType == "raw")
    {
        result = cSaveFileRAW(image, a_filename);
    }

    return (result);
}

