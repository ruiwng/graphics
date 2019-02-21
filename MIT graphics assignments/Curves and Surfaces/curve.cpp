#include "curve.h"
#include "extra.h"
#ifdef WIN32
#include <windows.h>
#endif
#include <GL/gl.h>
using namespace std;

namespace
{
    // Approximately equal to.  We don't want to use == because of
    // precision issues with floating point.
    inline bool approx( const Vector3f& lhs, const Vector3f& rhs )
    {
        const float eps = 1e-8f;
        return ( lhs - rhs ).absSquared() < eps;
    }

    
}
    

Curve evalBezier( const vector< Vector3f >& P, unsigned steps )
{
    // Check
    if( P.size() < 4 || P.size() % 3 != 1 )
    {
        cerr << "evalBezier must be called with 3n+1 control points." << endl;
        exit( 0 );
    }

    // TODO:
    // You should implement this function so that it returns a Curve
    // (e.g., a vector< CurvePoint >).  The variable "steps" tells you
    // the number of points to generate on each piece of the spline.
    // At least, that's how the sample solution is implemented and how
    // the SWP files are written.  But you are free to interpret this
    // variable however you want, so long as you can control the
    // "resolution" of the discretized spline curve with it.

    // Make sure that this function computes all the appropriate
    // Vector3fs for each CurvePoint: V,T,N,B.
    // [NBT] should be unit and orthogonal.

    // Also note that you may assume that all Bezier curves that you
    // receive have G1 continuity.  Otherwise, the TNB will not be
    // be defined at points where this does not hold.
	unsigned num = P.size() / 3;
	Curve Bez(num * steps);
	Matrix4f b0(1.0f, -3.0f, 3.0f, -1.0f, 
			   0.0f, 3.0f, -6.0f, 3.0f,
		       0.0f, 0.0f, 3.0f, -3.0f, 
			   0.0f, 0.0f, 0.0f, 1.0f);
	Matrix4f b1(-3.0f, 6.0f, -3.0f, 0.0f,
		       3.0f, -12.0f, 9.0f, 0.0f,
		       0.0f, 6.0f, -9.0f, 0.0f,
		       0.0f, 0.0f, 3.0f, 0.0f);
	float length = 1.0f / (steps - 1);
	unsigned index = 0;
	for (unsigned i = 0; i < num; i += 1) {
		unsigned start = i * 3;
		Matrix4f x(Vector4f(P[start], 0.0f), Vector4f(P[start + 1], 0.0f), Vector4f(P[start + 2], 0.0f), Vector4f(P[start + 3], 0.0f), true);
		Matrix4f y0 = x * b0;
		Matrix4f y1 = x * b1;
		float p = 0.0f;

		for (unsigned m = 0; m < steps; ++m) {
			Vector4f c0(1.0f, p, p * p, p * p * p);
			Vector4f c1(1.0f, p, p * p, 0.0f);
			Vector4f c2(1.0f, p, 0.0f, 0.0f);
			Vector4f pos = y0 * c0;
			CurvePoint& info(Bez[index]);
			info.V = (y0 * c0).xyz();
			info.T = (y1 * c1).xyz();
			info.T.normalize();
			Vector3f old_b;
			if (index != 0)
				old_b = Bez[index - 1].B;
			else
			{
				old_b = Vector3f(0.0f, 0.0f, 1.0f);
				if (abs(Vector3f::dot(old_b, info.T)) > 0.009f)
					old_b = Vector3f(0.0f, 1.0f, 0.0f);
			}
			info.N = Vector3f::cross(old_b, info.T);
			info.N.normalize();
			info.B = Vector3f::cross(info.T, info.N);
			p += length;
			index += 1;
		}
	}
    cerr << "\t>>> evalBezier has been called with the following input:" << endl;

    cerr << "\t>>> Control points (type vector< Vector3f >): "<< endl;
    for( unsigned i = 0; i < P.size(); ++i )
    {
        cerr << "\t>>> " << P[i] << endl;
    }

    cerr << "\t>>> Steps (type steps): " << steps << endl;
    cerr << "\t>>> Returning empty curve." << endl;

    // Right now this will just return this empty curve.
    return Bez;
}

Curve evalBspline( const vector< Vector3f >& P, unsigned steps )
{
    // Check
    if( P.size() < 4 )
    {
        cerr << "evalBspline must be called with 4 or more control points." << endl;
        exit( 0 );
    }

    // TODO:
    // It is suggested that you implement this function by changing
    // basis from B-spline to Bezier.  That way, you can just call
    // your evalBezier function.
	Matrix4f bez_b(1.0f, -3.0f, 3.0f, -1.0f,
			0.0f, 3.0f, -6.0f, 3.0f,
			0.0f, 0.0f, 3.0f, -3.0f,
			0.0f, 0.0f, 0.0f, 1.0f);
	Matrix4f bez_inverse = bez_b.inverse();
	Matrix4f spline_b(1.0f, -3.0f, 3.0f, -1.0f,
				4.0f, 0.0f, -6.0f, 3.0f,
				1.0f, 3.0f, 3.0f, -3.0f,
				0.0f, 0.0f, 0.0f, 1.0f);
	spline_b /= 6.0f;
	Matrix4f t = spline_b * bez_inverse;
	unsigned num = P.size();
	Curve bsp;
	vector<Vector3f> new_points(3 * (num - 3) + 1);
	for (unsigned i = 0; i < num - 3; ++i) {
		Matrix4f x(Vector4f(P[i], 1.0f), Vector4f(P[i + 1], 1.0f), Vector4f(P[i + 2], 1.0f), Vector4f(P[i + 3], 1.0f));
		Matrix4f new_base(x * t);
		vector<Vector3f> points(4);
		unsigned start = 3 * i;
		for (unsigned j = 0; j < 4; ++j)
			new_points[start + j] = new_base.getCol(j).xyz();
	}
    cerr << "\t>>> evalBSpline has been called with the following input:" << endl;

    cerr << "\t>>> Control points (type vector< Vector3f >): "<< endl;
    for( unsigned i = 0; i < P.size(); ++i )
    {
        cerr << "\t>>> " << P[i] << endl;
    }

    cerr << "\t>>> Steps (type steps): " << steps << endl;
    cerr << "\t>>> Returning empty curve." << endl;

    // Return an empty curve right now.
    return evalBezier(new_points, steps);
}

Curve evalCircle( float radius, unsigned steps )
{
    // This is a sample function on how to properly initialize a Curve
    // (which is a vector< CurvePoint >).
    
    // Preallocate a curve with steps+1 CurvePoints
    Curve R( steps+1 );

    // Fill it in counterclockwise
    for( unsigned i = 0; i <= steps; ++i )
    {
        // step from 0 to 2pi
        float t = 2.0f * M_PI * float( i ) / steps;

        // Initialize position
        // We're pivoting counterclockwise around the y-axis
        R[i].V = radius * Vector3f( cos(t), sin(t), 0 );
        
        // Tangent vector is first derivative
        R[i].T = Vector3f( -sin(t), cos(t), 0 );
        
        // Normal vector is second derivative
        R[i].N = Vector3f( -cos(t), -sin(t), 0 );

        // Finally, binormal is facing up.
        R[i].B = Vector3f( 0, 0, 1 );
    }

    return R;
}

void drawCurve( const Curve& curve, float framesize )
{
    // Save current state of OpenGL
    glPushAttrib( GL_ALL_ATTRIB_BITS );

    // Setup for line drawing
    glDisable( GL_LIGHTING ); 
    glColor4f( 1, 1, 1, 1 );
    glLineWidth( 1 );
    
    // Draw curve
    glBegin( GL_LINE_STRIP );
    for( unsigned i = 0; i < curve.size(); ++i )
    {
        glVertex( curve[ i ].V );
    }
    glEnd();

    glLineWidth( 1 );

    // Draw coordinate frames if framesize nonzero
    if( framesize != 0.0f )
    {
        Matrix4f M;

        for( unsigned i = 0; i < curve.size(); ++i )
        {
            M.setCol( 0, Vector4f( curve[i].N, 0 ) );
            M.setCol( 1, Vector4f( curve[i].B, 0 ) );
            M.setCol( 2, Vector4f( curve[i].T, 0 ) );
            M.setCol( 3, Vector4f( curve[i].V, 1 ) );

            glPushMatrix();
            glMultMatrixf( M );
            glScaled( framesize, framesize, framesize );
            glBegin( GL_LINES );
            glColor3f( 1, 0, 0 ); glVertex3d( 0, 0, 0 ); glVertex3d( 1, 0, 0 );
            glColor3f( 0, 1, 0 ); glVertex3d( 0, 0, 0 ); glVertex3d( 0, 1, 0 );
            glColor3f( 0, 0, 1 ); glVertex3d( 0, 0, 0 ); glVertex3d( 0, 0, 1 );
            glEnd();
            glPopMatrix();
        }
    }
    
    // Pop state
    glPopAttrib();
}

