#include "physics_scene_render.h"

using namespace physx;

#define MAX_NUM_MESH_VEC3S  1024

static PxVec3 kVertexBuffer[MAX_NUM_MESH_VEC3S];
static float kCylinderData[] = {
	1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
	0.866025f, 0.500000f, 1.0f, 0.866025f, 0.500000f, 1.0f, 0.866025f, 0.500000f, 0.0f, 0.866025f, 0.500000f, 0.0f,
	0.500000f, 0.866025f, 1.0f, 0.500000f, 0.866025f, 1.0f, 0.500000f, 0.866025f, 0.0f, 0.500000f, 0.866025f, 0.0f,
	-0.0f, 1.0f, 1.0f, -0.0f, 1.0f, 1.0f, -0.0f, 1.0f, 0.0f, -0.0f, 1.0f, 0.0f,
	-0.500000f, 0.866025f, 1.0f, -0.500000f, 0.866025f, 1.0f, -0.500000f, 0.866025f, 0.0f, -0.500000f, 0.866025f, 0.0f,
	-0.866025f, 0.500000f, 1.0f, -0.866025f, 0.500000f, 1.0f, -0.866025f, 0.500000f, 0.0f, -0.866025f, 0.500000f, 0.0f,
	-1.0f, -0.0f, 1.0f, -1.0f, -0.0f, 1.0f, -1.0f, -0.0f, 0.0f, -1.0f, -0.0f, 0.0f,
	-0.866025f, -0.500000f, 1.0f, -0.866025f, -0.500000f, 1.0f, -0.866025f, -0.500000f, 0.0f, -0.866025f, -0.500000f, 0.0f,
	-0.500000f, -0.866025f, 1.0f, -0.500000f, -0.866025f, 1.0f, -0.500000f, -0.866025f, 0.0f, -0.500000f, -0.866025f, 0.0f,
	0.0f, -1.0f, 1.0f, 0.0f, -1.0f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f,
	0.500000f, -0.866025f, 1.0f, 0.500000f, -0.866025f, 1.0f, 0.500000f, -0.866025f, 0.0f, 0.500000f, -0.866025f, 0.0f,
	0.866026f, -0.500000f, 1.0f, 0.866026f, -0.500000f, 1.0f, 0.866026f, -0.500000f, 0.0f, 0.866026f, -0.500000f, 0.0f,
	1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f
};

void RenderGeometry(const PxGeometryHolder &h)
{
	switch(h.getType())
	{
		case PxGeometryType::eBOX:			
			{
				glScalef(h.box().halfExtents.x, h.box().halfExtents.y, h.box().halfExtents.z);
				glutSolidCube(2.0f);		
			}
			break;
		case PxGeometryType::eSPHERE:		
			{
				glutSolidSphere(h.sphere().radius, 10, 10);		
			}
			break;
		case PxGeometryType::eCAPSULE:
			{

				const PxF32 radius = h.capsule().radius;
				const PxF32 halfHeight = h.capsule().halfHeight;

				//Sphere
				glPushMatrix();
				glTranslatef(halfHeight, 0.0f, 0.0f);
				glScalef(radius,radius,radius);
				glutSolidSphere(1, 10, 10);		
				glPopMatrix();

				//Sphere
				glPushMatrix();
				glTranslatef(-halfHeight, 0.0f, 0.0f);
				glScalef(radius,radius,radius);
				glutSolidSphere(1, 10, 10);		
				glPopMatrix();

				//Cylinder
				glPushMatrix();
				glTranslatef(-halfHeight, 0.0f, 0.0f);
				glScalef(2.0f*halfHeight, radius,radius);
				glRotatef(90.0f,0.0f,1.0f,0.0f);
				glEnableClientState(GL_VERTEX_ARRAY);
				glEnableClientState(GL_NORMAL_ARRAY);
				glVertexPointer(3, GL_FLOAT, 2*3*sizeof(float), kCylinderData);
				glNormalPointer(GL_FLOAT, 2*3*sizeof(float), kCylinderData+3);
				glDrawArrays(GL_TRIANGLE_STRIP, 0, 13*2);
				glDisableClientState(GL_VERTEX_ARRAY);
				glDisableClientState(GL_NORMAL_ARRAY);
				glPopMatrix();
			}
			break;
		case PxGeometryType::eCONVEXMESH:
			{

				//Compute triangles for each polygon.
				const PxVec3 scale = h.convexMesh().scale.scale;
				PxConvexMesh* mesh = h.convexMesh().convexMesh;
				const PxU32 nbPolys = mesh->getNbPolygons();
				const PxU8* polygons = mesh->getIndexBuffer();
				const PxVec3* verts = mesh->getVertices();
				PxU32 nbVerts = mesh->getNbVertices();
				PX_UNUSED(nbVerts);

				PxU32 numTotalTriangles = 0;
				for(PxU32 i = 0; i < nbPolys; i++)
				{
					PxHullPolygon data;
					mesh->getPolygonData(i, data);

					const PxU32 nbTris = data.mNbVerts - 2;
					const PxU8 vref0 = polygons[data.mIndexBase + 0];
					PX_ASSERT(vref0 < nbVerts);
					for(PxU32 j=0;j<nbTris;j++)
					{
						const PxU32 vref1 = polygons[data.mIndexBase + 0 + j + 1];
						const PxU32 vref2 = polygons[data.mIndexBase + 0 + j + 2];

						//generate face normal:
						PxVec3 e0 = verts[vref1] - verts[vref0];
						PxVec3 e1 = verts[vref2] - verts[vref0];

						PX_ASSERT(vref1 < nbVerts);
						PX_ASSERT(vref2 < nbVerts);

						PxVec3 fnormal = e0.cross(e1);
						fnormal.normalize();
			
						if(numTotalTriangles*6 < MAX_NUM_MESH_VEC3S)
						{
							kVertexBuffer[numTotalTriangles*6 + 0] = fnormal;
							kVertexBuffer[numTotalTriangles*6 + 1] = verts[vref0];
							kVertexBuffer[numTotalTriangles*6 + 2] = fnormal;
							kVertexBuffer[numTotalTriangles*6 + 3] = verts[vref1];
							kVertexBuffer[numTotalTriangles*6 + 4] = fnormal;
							kVertexBuffer[numTotalTriangles*6 + 5] = verts[vref2];
							numTotalTriangles++;
						}
					}
				}
				glPushMatrix();
				glScalef(scale.x, scale.y, scale.z);
				glEnableClientState(GL_NORMAL_ARRAY);
				glEnableClientState(GL_VERTEX_ARRAY);
				glNormalPointer(GL_FLOAT, 2*3*sizeof(float), kVertexBuffer);
				glVertexPointer(3, GL_FLOAT, 2*3*sizeof(float), kVertexBuffer+1);
				glDrawArrays(GL_TRIANGLES, 0, numTotalTriangles * 3);
				glPopMatrix();
			}
			break;
		case PxGeometryType::eTRIANGLEMESH:
			{
				const PxTriangleMeshGeometry& triGeom = h.triangleMesh();
				const PxTriangleMesh& mesh = *triGeom.triangleMesh;
				const PxVec3 scale = triGeom.scale.scale;

				const PxU32 triangleCount = mesh.getNbTriangles();
				const PxU32 has16BitIndices = mesh.getTriangleMeshFlags() & PxTriangleMeshFlag::e16_BIT_INDICES;
				const void* indexBuffer = mesh.getTriangles();

				const PxVec3* vertexBuffer = mesh.getVertices();

				const PxU32* intIndices = reinterpret_cast<const PxU32*>(indexBuffer);
				const PxU16* shortIndices = reinterpret_cast<const PxU16*>(indexBuffer);
				PxU32 numTotalTriangles = 0;
				for(PxU32 i=0; i < triangleCount; ++i)
				{
					PxVec3 triVert[3];

					if(has16BitIndices)
					{
						triVert[0] = vertexBuffer[*shortIndices++];
						triVert[1] = vertexBuffer[*shortIndices++];
						triVert[2] = vertexBuffer[*shortIndices++];
					}
					else
					{
						triVert[0] = vertexBuffer[*intIndices++];
						triVert[1] = vertexBuffer[*intIndices++];
						triVert[2] = vertexBuffer[*intIndices++];
					}

					PxVec3 fnormal = (triVert[1] - triVert[0]).cross(triVert[2] - triVert[0]);
					fnormal.normalize();

					if(numTotalTriangles*6 < MAX_NUM_MESH_VEC3S)
					{
						kVertexBuffer[numTotalTriangles*6 + 0] = fnormal;
						kVertexBuffer[numTotalTriangles*6 + 1] = triVert[0];
						kVertexBuffer[numTotalTriangles*6 + 2] = fnormal;
						kVertexBuffer[numTotalTriangles*6 + 3] = triVert[1];
						kVertexBuffer[numTotalTriangles*6 + 4] = fnormal;
						kVertexBuffer[numTotalTriangles*6 + 5] = triVert[2];
						numTotalTriangles++;
					}
				}
				glPushMatrix();
				glScalef(scale.x, scale.y, scale.z);
				glEnableClientState(GL_NORMAL_ARRAY);
				glEnableClientState(GL_VERTEX_ARRAY);
				glNormalPointer(GL_FLOAT, 2*3*sizeof(float), kVertexBuffer);
				glVertexPointer(3, GL_FLOAT, 2*3*sizeof(float), kVertexBuffer+1);
				glDrawArrays(GL_TRIANGLES, 0, numTotalTriangles * 3);
				glDisableClientState(GL_VERTEX_ARRAY);
				glDisableClientState(GL_NORMAL_ARRAY);
				glPopMatrix();
			}
			break;
		default:
			break;
		}
}

namespace PhysicsSceneRender
{
	namespace
	{
		void ReshapeCallback(int width, int height)
		{
			glViewport(0, 0, width, height);
		}
	}

	void SetupDefaultWindow(const char *name)
	{
		char* namestr = new char[strlen(name) + 1];
		strcpy(namestr, name);
		int argc = 1;
		char* argv[1] = { namestr };

		glutInit(&argc, argv);

		glutInitWindowSize(512, 512);
		glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
		int mainHandle = glutCreateWindow(name);
		glutSetWindow(mainHandle);
		glutReshapeFunc(ReshapeCallback);

		delete[] namestr;
	}

	void SetupDefaultRenderState()
	{
		// Setup default render states
		glClearColor(0.3f, 0.4f, 0.5f, 1.0);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_COLOR_MATERIAL);

		// Setup lighting
		glEnable(GL_LIGHTING);
		PxReal ambientColor[] = { 0.0f, 0.1f, 0.2f, 0.0f };
		PxReal diffuseColor[] = { 1.0f, 1.0f, 1.0f, 0.0f };
		PxReal specularColor[] = { 0.0f, 0.0f, 0.0f, 0.0f };
		PxReal position[] = { 100.0f, 100.0f, 400.0f, 1.0f };
		glLightfv(GL_LIGHT0, GL_AMBIENT, ambientColor);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseColor);
		glLightfv(GL_LIGHT0, GL_SPECULAR, specularColor);
		glLightfv(GL_LIGHT0, GL_POSITION, position);
		glEnable(GL_LIGHT0);
	}

	void StartRender(const PxVec3& cameraEye, const PxVec3& cameraDir)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Setup camera
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(60.0f, (float)glutGet(GLUT_WINDOW_WIDTH) / (float)glutGet(GLUT_WINDOW_HEIGHT), 1.0f, 10000.0f);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt(cameraEye.x, cameraEye.y, cameraEye.z, cameraEye.x + cameraDir.x, cameraEye.y + cameraDir.y, cameraEye.z + cameraDir.z, 0.0f, 1.0f, 0.0f);

		glColor4f(0.4f, 0.4f, 0.4f, 1.0f);
	}

	void RenderActors(PxRigidActor** actors, const PxU32 numActors, bool shadows, const PxVec3 & color)
	{
		PxShape* shapes[MAX_NUM_ACTOR_SHAPES];
		//render every shape on every actor
		for (PxU32 i = 0; i < numActors; i++)
		{
			//every actor has an attached shape
			const PxU32 nbShapes = actors[i]->getNbShapes();  //Nb = number
			PX_ASSERT(nbShapes <= MAX_NUM_ACTOR_SHAPES);
			actors[i]->getShapes(shapes, nbShapes);
			bool sleeping = actors[i]->isRigidDynamic() ? actors[i]->isRigidDynamic()->isSleeping() : false;

			//loop over every shape
			for (PxU32 j = 0; j < nbShapes; j++)
			{
				//get the world position of every shape
				const PxMat44 shapePose(PxShapeExt::getGlobalPose(*shapes[j], *actors[i]));
				//get the geometry in every shape
				PxGeometryHolder h = shapes[j]->getGeometry();

				if (shapes[j]->getFlags() & PxShapeFlag::eTRIGGER_SHAPE)
					glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

				// render object
				glPushMatrix();
				glMultMatrixf((float*)&shapePose);
				if (sleeping)
				{
					PxVec3 darkColor = color * 0.25f;
					glColor4f(darkColor.x, darkColor.y, darkColor.z, 1.0f);
				}
				else
					glColor4f(color.x, color.y, color.z, 1.0f);

				RenderGeometry(h);
				glPopMatrix();

				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

				if (shadows)
				{
					const PxVec3 shadowDir(0.0f, -0.7071067f, -0.7071067f);
					const PxReal shadowMat[] = { 1, 0, 0, 0, -shadowDir.x / shadowDir.y, 0, -shadowDir.z / shadowDir.y, 0, 0, 0, 1, 0, 0, 0, 0, 1 };
					glPushMatrix();
					glMultMatrixf(shadowMat);
					glMultMatrixf((float*)&shapePose);
					glDisable(GL_LIGHTING);
					glColor4f(0.1f, 0.2f, 0.3f, 1.0f);
					RenderGeometry(h);
					glEnable(GL_LIGHTING);
					glPopMatrix();
				}
			}
		}
	}

	void FinishRender()
	{
		glutSwapBuffers();
	}
}
