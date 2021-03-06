#ifndef lint
#ifdef sccs
static char sccsid[] = "@(#)cms_impl.h 1.10 89/08/18";
#endif
#endif

/*
 *	(c) Copyright 1989 Sun Microsystems, Inc. Sun design patents 
 *	pending in the U.S. and foreign countries. See LEGAL NOTICE 
 *	file for terms of the license.
 */

#ifndef xview_cms_impl_h_DEFINED
#define	xview_cms_impl_h_DEFINED

#include <X11/Xlib.h>
#include <xview/screen.h>
#include <xview/cms.h>
#include <xview/pkg.h>
#include <xview/attr.h>

/*
 ***********************************************************************
 *			Definitions and Macros
 ***********************************************************************
 */
#define XV_DEFAULT_CMS          "xv_default_cms"

#define CMS_PRIVATE(cms_public)	XV_PRIVATE(Cms_info, Xv_cms_struct, cms_public)
#define CMS_PUBLIC(cms)		XV_PUBLIC(cms)

#define XV_CMS_BACKGROUND(cms) (cms)->index_table[0]
#define XV_CMS_FOREGROUND(cms) (cms)->index_table[(cms)->size - 1]

#define XV_TO_X_PIXEL(index, cms) \
        (cms)->index_table[((index) >= (cms)->size) ? (cms)->size - 1:(index)]

#define XV_CMS_IS_STATIC(cms)	(cms->type == XV_STATIC_CMS) ? TRUE : FALSE
#define XV_CMS_IS_DEFAULT(cms)	strcmp(cms->name, XV_DEFAULT_CMS) ? FALSE : TRUE

#define XV_INVALID_PIXEL	-1

#define STATUS(cms, field)           ((cms)->status_bits.field)
#define STATUS_SET(cms, field)       STATUS(cms, field) = TRUE
#define STATUS_RESET(cms, field)     STATUS(cms, field) = FALSE
#define BIT_FIELD(field)             int field : 1

/*
 ***********************************************************************
 *		Typedefs, enumerations, and structs
 ***********************************************************************
 */

typedef struct xv_colormap {
    Colormap	    	id;
    Cmap_type		type;
    int			visual_class;  /* BUG: We should be using the visual ids */
    struct cms_info 	*cms_list;
    struct xv_colormap  *next
} Xv_Colormap;

typedef struct cms_info {
    Cms			public_self;
    char		*name;
    Cms_type		type;
    unsigned long	size;
    unsigned long	*index_table;
    Xv_Colormap		*cmap;
    Xv_Screen           screen;
    int			visual_class;
    struct cms_info  	*next;
    struct {
	BIT_FIELD(default_cms);
	BIT_FIELD(frame_cms);
	BIT_FIELD(control_cms);
    } status_bits;
} Cms_info;

/*
 *************************************************************************
 *		Private functions
 *************************************************************************
 */
Pkg_private int   	cms_init();
Pkg_private Xv_opaque	cms_set_avlist();
Pkg_private Xv_opaque   cms_get_attr();
Pkg_private int   	cms_destroy();
Pkg_private Xv_opaque   cms_find_cms();
Pkg_private void	cms_init_default_cms();
Pkg_private void	cms_set_size();
Pkg_private void	cms_free_colors();
Pkg_private void	cms_set_name();
Pkg_private void	cms_set_unique_name();
Pkg_private int		cms_set_colors();
Pkg_private int		cms_set_static_colors();
Pkg_private int		cms_set_dynamic_colors();


#endif	~xview_cms_impl_h_DEFINED
