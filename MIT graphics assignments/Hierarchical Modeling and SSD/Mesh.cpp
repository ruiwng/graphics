#include "Mesh.h"

using namespace std;

void Mesh::load( const char* filename )
{
	// 2.1.1. load() should populate bindVertices, currentVertices, and faces

	// Add your code here.
	ifstream in(filename);
	if (!in) {
		cerr << "file " << filename << "not exists!!!!" << endl;
		exit(0);
	}
	string type;
	while (in >> type) {
		if (type == "v") {
			float x, y, z;
			in >> x >> y >> z;
			bindVertices.push_back(Vector3f(x, y, z));
		}
		else if (type == "f") {
			int a, b, c;
			in >> a >> b >> c;
			faces.push_back(Tuple3u(a - 1, b - 1, c - 1));
		}
		
	}
	in.close();
	// make a copy of the bind vertices as the current vertices
	currentVertices = bindVertices;
}

void Mesh::draw()
{
	// Since these meshes don't have normals
	// be sure to generate a normal per triangle.
	// Notice that since we have per-triangle normals
	// rather than the analytical normals from
	// assignment 1, the appearance is "faceted".
	glBegin(GL_TRIANGLES);
	for (unsigned i = 0; i < faces.size(); ++i) {
		const Vector3f& a = currentVertices[faces[i][0]];
		const Vector3f& b = currentVertices[faces[i][1]];
		const Vector3f& c = currentVertices[faces[i][2]];
		Vector3f normal = Vector3f::cross(b - a, c - b);
		normal.normalize();
		glNormal3fv(normal);
		glVertex3fv(a);
		glVertex3fv(b);
		glVertex3fv(c);
	}
	glEnd();
}

void Mesh::loadAttachments( const char* filename, int numJoints )
{
	// 2.2. Implement this method to load the per-vertex attachment weights
	// this method should update m_mesh.attachments
	ifstream in(filename);
	if (!in) {
		cerr << "file " << filename << "not exists" << endl;
		exit(0);
	}
	float weight;
	while (in >> weight)
	{
		attachments.push_back(vector<float>(1, weight));
		for (unsigned i = 0; i < numJoints - 2; ++i)
		{
			in >> weight;
			attachments.back().push_back(weight);
		}
	}
	in.close();
}
