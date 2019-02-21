#include "SkeletalModel.h"

#include <FL/Fl.H>

using namespace std;

void SkeletalModel::load(const char *skeletonFile, const char *meshFile, const char *attachmentsFile)
{
	loadSkeleton(skeletonFile);

	m_mesh.load(meshFile);
	m_mesh.loadAttachments(attachmentsFile, m_joints.size());

	computeBindWorldToJointTransforms();
	updateCurrentJointToWorldTransforms();
}

void SkeletalModel::draw(Matrix4f cameraMatrix, bool skeletonVisible)
{
	// draw() gets called whenever a redraw is required
	// (after an update() occurs, when the camera moves, the window is resized, etc)

	m_matrixStack.clear();
	m_matrixStack.push(cameraMatrix);

	if( skeletonVisible )
	{
		drawJoints();

		drawSkeleton();
	}
	else
	{
		// Clear out any weird matrix we may have been using for drawing the bones and revert to the camera matrix.
		glLoadMatrixf(m_matrixStack.top());

		// Tell the mesh to draw itself.
		m_mesh.draw();
	}
}

void SkeletalModel::loadSkeleton( const char* filename )
{
	// Load the skeleton from file here.
	ifstream in(filename);
	if (!in) {
		cerr << filename << " not found\a" << endl;
		exit(0);
	}

	cerr << endl << "*** loading skeleton " << filename << endl;

	int joint_index = 0;
	float x, y, z;
	int parent_index;
	map<int, int> parent_record;
	while (in >> x >> y >> z >> parent_index) {
		Joint *j = new Joint;
		j->transform = Matrix4f::translation(x, y, z);
		m_joints.push_back(j);
		parent_record[joint_index] = parent_index;
		joint_index += 1;
	}
	for (auto val : parent_record)
	{
		if (val.second != -1)
			m_joints[val.second]->children.push_back(m_joints[val.first]);
		else
			m_rootJoint = m_joints[val.first];
	}

	in.close();
}

void SkeletalModel::drawJoints( )
{
	// Draw a sphere at each joint. You will need to add a recursive helper function to traverse the joint hierarchy.
	//
	// We recommend using glutSolidSphere( 0.025f, 12, 12 )
	// to draw a sphere of reasonable size.
	//
	// You are *not* permitted to use the OpenGL matrix stack commands
	// (glPushMatrix, glPopMatrix, glMultMatrix).
	// You should use your MatrixStack class
	// and use glLoadMatrix() before your drawing call.
	drawJointsRecursively(m_rootJoint);
}

void SkeletalModel::drawJointsRecursively(const Joint *root) {
	m_matrixStack.push(root->transform);
	glLoadMatrixf(m_matrixStack.top());

	glutSolidSphere(0.025f, 12, 12);

	for (auto child : root->children)
		drawJointsRecursively(child);

	m_matrixStack.pop();
	glLoadMatrixf(m_matrixStack.top());
}

void SkeletalModel::drawSkeleton( )
{
	// Draw boxes between the joints. You will need to add a recursive helper function to traverse the joint hierarchy.
	drawSkeletonRecursively(m_rootJoint);
}

void SkeletalModel::drawSkeletonRecursively(const Joint *root) {
	if (root != m_rootJoint)
	{
		Vector3f oritention = root->transform.getCol(3).xyz();
		float length = oritention.abs();
		Vector3f z = Vector3f(oritention / length);
		Vector3f x(0.0f, 0.0f, 1.0f);
		Vector3f y(Vector3f::cross(z, x));
		y.normalize();
		x = Vector3f::cross(y, z);
		Matrix4f rotate(Vector4f(x, 0.0f), Vector4f(y, 0.0f), Vector4f(z, 0.0f), Vector4f(0.0f, 0.0f, 0.0f, 1.0f));
		Matrix4f translate(Matrix4f::translation(0.0f, 0.0f, 0.5f));
		Matrix4f scale(Matrix4f::scaling(0.05f, 0.05f, length));
		Matrix4f transform = rotate * scale * translate;
		glMultMatrixf(transform);
		glutSolidCube(1.0f);
	}
	
	m_matrixStack.push(root->transform);
	glLoadMatrixf(m_matrixStack.top());

	for (auto child : root->children)
	{
		drawSkeletonRecursively(child);
	}
		
	m_matrixStack.pop();
	glLoadMatrixf(m_matrixStack.top());
}

void SkeletalModel::setJointTransform(int jointIndex, float rX, float rY, float rZ)
{
	// Set the rotation part of the joint's transformation matrix based on the passed in Euler angles.
	Matrix4f r = Matrix4f::rotateX(rX) * Matrix4f::rotateY(rY) * Matrix4f::rotateZ(rZ);
	Vector3f translate = m_joints[jointIndex]->transform.getCol(3).xyz();
	m_joints[jointIndex]->transform = Matrix4f::translation(translate) * r;
}


void SkeletalModel::computeBindWorldToJointTransforms()
{
	// 2.3.1. Implement this method to compute a per-joint transform from
	// world-space to joint space in the BIND POSE.
	//
	// Note that this needs to be computed only once since there is only
	// a single bind pose.
	//
	// This method should update each joint's bindWorldToJointTransform.
	// You will need to add a recursive helper function to traverse the joint hierarchy.
	computeBindWorldToJointTransformsRecursively(m_rootJoint, Matrix4f::identity());
}

void SkeletalModel::computeBindWorldToJointTransformsRecursively(Joint* root, const Matrix4f& m) {
	Matrix4f t = m * root->transform;
	root->bindWorldToJointTransform = t.inverse();
	for (auto child : root->children)
		computeBindWorldToJointTransformsRecursively(child, t);
}

void SkeletalModel::updateCurrentJointToWorldTransforms()
{
	// 2.3.2. Implement this method to compute a per-joint transform from
	// joint space to world space in the CURRENT POSE.
	//
	// The current pose is defined by the rotations you've applied to the
	// joints and hence needs to be *updated* every time the joint angles change.
	//
	// This method should update each joint's bindWorldToJointTransform.
	// You will need to add a recursive helper function to traverse the joint hierarchy.
	updateCurrentJointToWorldTransformsRecursively(m_rootJoint, Matrix4f::identity());
}

void SkeletalModel::updateCurrentJointToWorldTransformsRecursively(Joint* root, const Matrix4f& m) {
	Matrix4f t = m * root->transform;
	root->currentJointToWorldTransform = t;
	for (auto child : root->children)
		updateCurrentJointToWorldTransformsRecursively(child, t);
}

void SkeletalModel::updateMesh()
{
	// 2.3.2. This is the core of SSD.
	// Implement this method to update the vertices of the mesh
	// given the current state of the skeleton.
	// You will need both the bind pose world --> joint transforms.
	// and the current joint --> world transforms.
	const auto &bindVertices(m_mesh.bindVertices);
	auto &currentVertices(m_mesh.currentVertices);
	const auto &attachments(m_mesh.attachments);
	auto vertex_count = bindVertices.size();

	vector<Matrix4f> joint_transform;
	for (auto& joint: m_joints)
		joint_transform.push_back(joint->currentJointToWorldTransform * joint->bindWorldToJointTransform);

	for (auto i = 0; i < vertex_count; ++i) {
		currentVertices[i] = Vector3f(0.0f, 0.0f, 0.0f);
		const auto &attachment = attachments[i];
		auto attach_count = attachment.size();
		for (auto j = 0; j < attach_count; ++j) {
			if (attachment[j] < 0.0001f)
				continue;
			auto p = joint_transform[j + 1] * Vector4f(bindVertices[i], 1.0f);
			currentVertices[i] += (attachment[j] * p).xyz();
		}
	}
}

