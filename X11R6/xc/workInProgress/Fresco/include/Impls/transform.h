/*
 * Copyright (c) 1987-91 Stanford University
 * Copyright (c) 1991-93 Silicon Graphics, Inc.
 *
 * Permission to use, copy, modify, distribute, and sell this software and 
 * its documentation for any purpose is hereby granted without fee, provided
 * that (i) the above copyright notices and this permission notice appear in
 * all copies of the software and related documentation, and (ii) the names of
 * Stanford and Silicon Graphics may not be used in any advertising or
 * publicity relating to the software without the specific, prior written
 * permission of Stanford and Silicon Graphics.
 * 
 * THE SOFTWARE IS PROVIDED "AS-IS" AND WITHOUT WARRANTY OF ANY KIND, 
 * EXPRESS, IMPLIED OR OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY 
 * WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.  
 *
 * IN NO EVENT SHALL STANFORD OR SILICON GRAPHICS BE LIABLE FOR
 * ANY SPECIAL, INCIDENTAL, INDIRECT OR CONSEQUENTIAL DAMAGES OF ANY KIND,
 * OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
 * WHETHER OR NOT ADVISED OF THE POSSIBILITY OF DAMAGE, AND ON ANY THEORY OF 
 * LIABILITY, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE 
 * OF THIS SOFTWARE.
 */

#ifndef Fresco_lib_xform_h
#define Fresco_lib_xform_h

#include <X11/Fresco/Impls/fobjects.h>

//- TransformImpl*
class TransformImpl : public Transform {
    //. TransformImpl is a default implementation of
    //. the Transform interface.  The current implementation
    //. only supports 3x2 matrices, but this will be replaced
    //. with 4x4 matrix code in the near future.
public:
    TransformImpl();
    TransformImpl(Transform::Matrix m);
    ~TransformImpl();

    //+ Transform::*
    /* FrescoObject */
    Long ref__(Long references);
    Tag attach(FrescoObject_in observer);
    void detach(Tag attach_tag);
    void disconnect();
    void notify_observers();
    void update();
    /* Transform */
    void load(Transform_in t);
    void load_identity();
    void load_matrix(Matrix m);
    void store_matrix(Matrix m);
    Boolean equal(Transform_in t);
    Boolean identity();
    Boolean det_is_zero();
    void scale(const Vertex& v);
    void rotate(Float angle, Axis a);
    void translate(const Vertex& v);
    void premultiply(Transform_in t);
    void postmultiply(Transform_in t);
    void invert();
    void transform_vertex(Vertex& v);
    void inverse_transform_vertex(Vertex& v);
    //+

    Transform::Matrix& matrix();
protected:
    SharedFrescoObjectImpl object_;
    Transform::Matrix mat_;
    Boolean identity_ : 1;
    Boolean translate_only_ : 1;
    Boolean xy_ : 1;
    Boolean modified_ : 1;

    void init();
    void modified();
    void recompute();
    Coord det();
};

inline Transform::Matrix& TransformImpl::matrix() { return mat_; }

#endif
