#include "surf.h"
#include "extra.h"
using namespace std;

namespace
{
    
    // We're only implenting swept surfaces where the profile curve is
    // flat on the xy-plane.  This is a check function.
    static bool checkFlat(const Curve &profile)
    {
        for (unsigned i=0; i<profile.size(); i++)
            if (profile[i].V[2] != 0.0 ||
                profile[i].T[2] != 0.0 ||
                profile[i].N[2] != 0.0)
                return false;
    
        return true;
    }
}

Surface makeSurfRev(const Curve &profile, unsigned steps)
{
    Surface surface;
    
    if (!checkFlat(profile))
    {
        cerr << "surfRev profile curve must be flat on xy plane." << endl;
        exit(0);
    }
    // TODO: Here you should build the surface.  See surf.h for details.
	unsigned profile_num = profile.size();
	for (unsigned i = 0; i < profile_num; ++i) {
		surface.VV.push_back(profile[i].V);
		surface.VN.push_back(-profile[i].N);
	}
	float step_radians = M_PI * 2 / steps;

	for (unsigned i = 1; i < steps; ++i) {
		Matrix4f rotate_m = Matrix4f::rotateY(step_radians * i);
		for (unsigned j = 0; j < profile_num; ++j) {
			Vector4f new_v = rotate_m * Vector4f(profile[j].V, 1.0f);
			surface.VV.push_back(new_v.xyz());
			Vector4f new_n = rotate_m * Vector4f(profile[j].N, 1.0f);
			surface.VN.push_back(-new_n.xyz());
		}
	}
	for (unsigned i = 0; i < profile_num; ++i) {
		surface.VV.push_back(profile[i].V);
		surface.VN.push_back(-profile[i].N);
	}
	for (unsigned i = 0; i < steps; ++i)
	{	
		unsigned start = profile_num * i;
		for (unsigned j = 0; j < profile_num - 1; ++j) {
			surface.VF.push_back(Tup3u(start + j, start + j + 1, start + j + profile_num));
			surface.VF.push_back(Tup3u(start + j + 1, start + j + profile_num + 1, start + j + profile_num));
		}
	}
    cerr << "\t>>> makeSurfRev called (but not implemented).\n\t>>> Returning empty surface." << endl;
 
	return surface;
}

Surface makeGenCyl(const Curve &profile, const Curve &sweep )
{
    Surface surface;

    if (!checkFlat(profile))
    {
        cerr << "genCyl profile curve must be flat on xy plane." << endl;
        exit(0);
    }
	unsigned sweep_num = sweep.size();
	unsigned profile_num = profile.size();
	for (unsigned i = 0; i < sweep_num; ++i) {
		const CurvePoint& p(sweep[i]);
		Matrix4f trans(Vector4f(p.N, 0.0f), Vector4f(p.B, 0.0f), Vector4f(p.T, 0.0f), Vector4f(p.V, 1.0f));
		for (unsigned j = 0; j < profile_num; ++j) {
			Vector4f new_v = trans * Vector4f(profile[j].V, 1.0f);
			surface.VV.push_back(new_v.xyz());
			Vector4f new_n = trans * Vector4f(profile[j].N, 0.0f);
			surface.VN.push_back(-new_n.xyz());
		}
	}
    // TODO: Here you should build the surface.  See surf.h for details.
	for (unsigned i = 0; i < sweep_num - 1; ++i) {
		unsigned start = profile_num * i;
		for (unsigned j = 0; j < profile_num - 1; ++j) {
			surface.VF.push_back(Tup3u(start + j, start + j + 1, start + j + profile_num));
			surface.VF.push_back(Tup3u(start + j + 1, start + j + profile_num + 1, start + j + profile_num));
		}
	}
    cerr << "\t>>> makeGenCyl called (but not implemented).\n\t>>> Returning empty surface." <<endl;

    return surface;
}

void drawSurface(const Surface &surface, bool shaded)
{
    // Save current state of OpenGL
    glPushAttrib(GL_ALL_ATTRIB_BITS);

    if (shaded)
    {
        // This will use the current material color and light
        // positions.  Just set these in drawScene();
        glEnable(GL_LIGHTING);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        // This tells openGL to *not* draw backwards-facing triangles.
        // This is more efficient, and in addition it will help you
        // make sure that your triangles are drawn in the right order.
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
    }
    else
    {        
        glDisable(GL_LIGHTING);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        
        glColor4f(0.4f,0.4f,0.4f,1.f);
        glLineWidth(1);
    }

    glBegin(GL_TRIANGLES);
    for (unsigned i=0; i<surface.VF.size(); i++)
    {
		const Vector3f &color1 = surface.VN[surface.VF[i][0]];
		glColor4f(color1.x(), color1.y(), color1.z(), 1.0f);
        glNormal(surface.VN[surface.VF[i][0]]);
        glVertex(surface.VV[surface.VF[i][0]]);
		const Vector3f &color2 = surface.VN[surface.VF[i][1]];
		glColor4f(color2.x(), color2.y(), color2.z(), 1.0f);
        glNormal(surface.VN[surface.VF[i][1]]);
        glVertex(surface.VV[surface.VF[i][1]]);
		const Vector3f &color3 = surface.VN[surface.VF[i][2]];
		glColor4f(color3.x(), color3.y(), color3.z(), 1.0f);
        glNormal(surface.VN[surface.VF[i][2]]);
        glVertex(surface.VV[surface.VF[i][2]]);
    }
    glEnd();

    glPopAttrib();
}

void drawNormals(const Surface &surface, float len)
{
    // Save current state of OpenGL
    glPushAttrib(GL_ALL_ATTRIB_BITS);

    glDisable(GL_LIGHTING);
    glColor4f(0,1,1,1);
    glLineWidth(1);

    glBegin(GL_LINES);
    for (unsigned i=0; i<surface.VV.size(); i++)
    {
        glVertex(surface.VV[i]);
        glVertex(surface.VV[i] + surface.VN[i] * len);
    }
    glEnd();

    glPopAttrib();
}

void outputObjFile(ostream &out, const Surface &surface)
{
    
    for (unsigned i=0; i<surface.VV.size(); i++)
        out << "v  "
            << surface.VV[i][0] << " "
            << surface.VV[i][1] << " "
            << surface.VV[i][2] << endl;

    for (unsigned i=0; i<surface.VN.size(); i++)
        out << "vn "
            << surface.VN[i][0] << " "
            << surface.VN[i][1] << " "
            << surface.VN[i][2] << endl;

    out << "vt  0 0 0" << endl;
    
    for (unsigned i=0; i<surface.VF.size(); i++)
    {
        out << "f  ";
        for (unsigned j=0; j<3; j++)
        {
            unsigned a = surface.VF[i][j]+1;
            out << a << "/" << "1" << "/" << a << " ";
        }
        out << endl;
    }
}
