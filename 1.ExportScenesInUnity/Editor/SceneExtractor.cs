using System;
using System.IO;
using UnityEditor;
using UnityEngine;
using killer.proto;
using ProtoBuf;

namespace Scene
{
	public class SceneExtractor
	{
	    private static int    m_box_id;
	    private static int    m_sphere_id;
	    private static int    m_capsule_id;
	    private static int    m_mesh_id;

	    public SceneExtractor()
	    {
	        m_box_id     = 0;
	        m_sphere_id  = 0;
	        m_capsule_id = 0;
	        m_mesh_id    = 0;
	    }

		[MenuItem("Tools/SceneExtractor/ExtractColliders")]
		public static void ExtractorColliders() 
		{
            //var path = EditorUtility.SaveFilePanel("Save Scene Collider to...", "E:\\KillerProject_Cooperation_Mode\\KillerProject\\Assets\\Scenes\\Editor\\SceneCollider", "scene");
		    var path = EditorUtility.SaveFilePanel("Save Scene Collider to..", "", "", "scene");
		    if (path.Length == 0)
		        return;

            Debug.Log(path); // + "/" + Application.loadedLevelName
            var f_scene_out = File.Create(path);

		    Collider[] allColliders = Resources.FindObjectsOfTypeAll<Collider>();
            Debug.Log(allColliders.Length);

            U3DPhysxScene scene = new U3DPhysxScene();
		    scene.scene_name = Application.loadedLevelName;

		    for (int i = 0; i < allColliders.Length; ++i)
		    {
		        Collider col = allColliders[i];
		        
		        BoxCollider box_col         = col as BoxCollider;
		        SphereCollider sphere_col   = col as SphereCollider;
                CapsuleCollider capsule_col = col as CapsuleCollider;
                MeshCollider mesh_col       = col as MeshCollider;

		        if (box_col != null)
		        {
		            m_box_id++;
                    scene.box_collider.Add(GeneratePhysxBoxCollider(m_box_id, box_col));
		        }else if (sphere_col != null)
		        {
		            m_sphere_id++;
                    scene.sphere_collider.Add(GeneratePhysxSphereCollider(m_sphere_id, sphere_col));
		        }else if (capsule_col != null)
		        {
		            m_capsule_id++;
                    scene.capsule_collider.Add(GeneratePhysxCapsuleCollider(m_capsule_id, capsule_col));
		        }else if(mesh_col != null)
		        {
		            m_mesh_id++;
                    scene.mesh_collider.Add(GeneratePhysxConvexMeshCollider(m_mesh_id, mesh_col));
		        }
		    }
            
            Serializer.Serialize(f_scene_out, scene);
            f_scene_out.Close();
        }

        [MenuItem("Tools/SceneExtractor/LoadColliders")]
        public static void LoadColliders()
        {
            var path = EditorUtility.OpenFilePanel("Load Colliders From...", "", "scene");
            if(path.Length == 0)
                return;

            U3DPhysxScene s;
            using (var file = File.OpenRead(path))
            {
                s = Serializer.Deserialize<U3DPhysxScene>(file);
                Debug.Log("load scene box: " + s.box_collider.Count);
                Debug.Log("load scene sphere: " + s.sphere_collider.Count);
                Debug.Log("load scene cap: " + s.capsule_collider.Count);
            }
        }
        
        private static U3DPhysxBox GeneratePhysxBoxCollider(int id, BoxCollider box_col)
	    {
	        U3DPhysxBox box = new U3DPhysxBox();

	        box.id = id;
            box.type = ColliderType.BOX;

            //world pos
            box.pos = new killer.proto.Vector3();
            box.pos.x = box_col.transform.position.x;
            box.pos.y = box_col.transform.position.y;
            box.pos.z = box_col.transform.position.z;

            //scale, BUG:考虑父节点对子节点的缩放
	        box.x_extents = box_col.size.x * box_col.transform.localScale.x;
            box.y_extents = box_col.size.y * box_col.transform.localScale.y;
            box.z_extents = box_col.size.z * box_col.transform.localScale.z;

            //rotation considered
            box.rotation = new killer.proto.Vector4();
            box.rotation.x = box_col.transform.rotation.x;
            box.rotation.y = box_col.transform.rotation.y;
            box.rotation.z = box_col.transform.rotation.z;
            box.rotation.w = box_col.transform.rotation.w;

            Debug.Log("BOX ID: " + box.id);
            Debug.Log("BOX position x: " + box.pos.x);
            Debug.Log("BOX position y: " + box.pos.y);
            Debug.Log("BOX position z: " + box.pos.z);

            return box;
	    }

        private static U3DPhysxSphere GeneratePhysxSphereCollider(int id, SphereCollider sphere_col)
	    {
	        U3DPhysxSphere sphere = new U3DPhysxSphere();
	        sphere.id = id;
            sphere.type = ColliderType.SPHERE;

            sphere.pos = new killer.proto.Vector3();
            //world pos
            sphere.pos.x = sphere_col.transform.position.x;
            sphere.pos.y = sphere_col.transform.position.y;
            sphere.pos.z = sphere_col.transform.position.z;

            //scale
            double max_scale = Math.Max(Math.Max(sphere_col.transform.localScale.x, 
                                                 sphere_col.transform.localScale.y),
                                        sphere_col.transform.localScale.z);
            sphere.radius = sphere_col.radius * max_scale;

            //rotation considered
            sphere.rotation = new killer.proto.Vector4();
            sphere.rotation.x = sphere_col.transform.rotation.x;
            sphere.rotation.y = sphere_col.transform.rotation.y;
            sphere.rotation.z = sphere_col.transform.rotation.z;
            sphere.rotation.w = sphere_col.transform.rotation.w;


            Debug.Log("Sphere ID: " + sphere.id);
            Debug.Log("Sphere position x: " + sphere.pos.x);
            Debug.Log("Sphere position y: " + sphere.pos.y);
            Debug.Log("Sphere position z: " + sphere.pos.z);
            
            return sphere;
	    }

        private static U3DPhysxCapsule GeneratePhysxCapsuleCollider(int id, CapsuleCollider capsule_col)
	    {
	        U3DPhysxCapsule cap = new U3DPhysxCapsule();
	        cap.id = id;
            cap.type = ColliderType.CAPSULE;

            cap.pos = new killer.proto.Vector3();
            //world pos
	        cap.pos.x = capsule_col.transform.position.x;
            cap.pos.y = capsule_col.transform.position.y;
            cap.pos.z = capsule_col.transform.position.z;

            //scale, In unity, scale of capsule is a little bit complicated
            //if you scale x/z, the height of the capsule will remain the same, until the threshold
            //if you scale y, the height of the capsule will stretch
            //so currently, we won't consider the scale of capsule
            cap.height = capsule_col.height; // * capsule_col.transform.localScale.y;
            //double max_scale = Math.Max(capsule_col.transform.localScale.x, capsule_col.transform.localScale.z);
            cap.raduis = capsule_col.radius; // * max_scale;

            //rotation considered
            cap.rotation = new killer.proto.Vector4();
            cap.rotation.x = capsule_col.transform.rotation.x;
            cap.rotation.y = capsule_col.transform.rotation.y;
            cap.rotation.z = capsule_col.transform.rotation.z;
            cap.rotation.w = capsule_col.transform.rotation.w;

            Debug.Log("CAPSULE ID: " + cap.id);
            Debug.Log("Capsule position x: " + cap.pos.x);
            Debug.Log("Capsule position y: " + cap.pos.y);
            Debug.Log("Capsule position z: " + cap.pos.z);

	        return cap;
	    }

	    private static U3DPhysxMesh GeneratePhysxConvexMeshCollider(int id, MeshCollider mesh_col)
	    {
	        U3DPhysxMesh mesh = new U3DPhysxMesh();
	        mesh.id = id;
	        mesh.type = ColliderType.MESH;

	        mesh.vertex_count = mesh_col.sharedMesh.vertexCount;
	        for (int i = 0; i < mesh_col.sharedMesh.vertexCount; ++i)
	        {
                killer.proto.Vector3 new_vertice = new killer.proto.Vector3();
                //consider scale
	            new_vertice.x = mesh_col.sharedMesh.vertices[i].x * mesh_col.transform.localScale.x;
                new_vertice.y = mesh_col.sharedMesh.vertices[i].y * mesh_col.transform.localScale.y;
                new_vertice.z = mesh_col.sharedMesh.vertices[i].z * mesh_col.transform.localScale.z;

                mesh.vertices.Add(new_vertice);
	        }

            //BUG:这里还差一个rotate

	        Debug.Log("Mesh ID: " + mesh.id);
            Debug.Log("Mesh Vertex num: " + mesh.vertex_count);
            
	        return mesh;
	    }
	}
}