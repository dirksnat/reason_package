////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.kug.ac.at
//
// Implementation file
//
//    Copyright (c) 1997-1999 Mark Danks.
//    Copyright (c) G�nther Geiger.
//    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::f�r::uml�ute. IEM
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////

#include "shearYX.h"

CPPEXTERN_NEW_WITH_GIMME(shearYX)

/////////////////////////////////////////////////////////
//
// shearYX
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
shearYX :: shearYX(int argc, t_atom *argv)
{
    if (argc == 1)
    {
        shear = atom_getfloat(&argv[0]);
    }
    else if (argc == 0)
    {
        shear = 0.f;
    }
 
    // create the new inlets
    inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("shearVal"));

}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
shearYX :: ~shearYX()
{ }

/////////////////////////////////////////////////////////
// render
//
/////////////////////////////////////////////////////////
void shearYX :: render(GemState *)
{
    GLfloat matrix[16];

	matrix[0]=matrix[5]=matrix[10]=matrix[15]=1;
	matrix[1]=matrix[2]=matrix[3]=matrix[4]=0.f;
	matrix[6]=matrix[7]=matrix[8]=matrix[9]=0.f;
	matrix[11]=matrix[12]=matrix[13]=matrix[14]=0.f;

	matrix[1]=shear;

	glMultMatrixf(matrix);

}

/////////////////////////////////////////////////////////
// shear
//
/////////////////////////////////////////////////////////
void shearYX :: shearMess(float val)
{
    shear = val;
    setModified();
}


/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void shearYX :: obj_setupCallback(t_class *classPtr)
{
    class_addmethod(classPtr, (t_method)&shearYX::shearMessCallback,
    	    gensym("shearVal"), A_FLOAT, A_NULL); 
}
void shearYX :: shearMessCallback(void *data, t_floatarg val)
{
    GetMyClass(data)->shearMess((float)val);
}

