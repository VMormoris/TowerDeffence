#include "Loader.h"
#include "utils.h"
#include <iostream>
#include <fstream>
#include <sstream>


inline void read_vertex(const char* buff, std::vector<glm::vec3>& shared_vertices)
{
	glm::vec3 v;
	char* pEnd;
	v.x = strtof(buff, &pEnd);
	v.y = strtof(pEnd, &pEnd);
	v.z = strtof(pEnd, &pEnd);
	//sscanf(buff, "%f %f %f", &v.x, &v.y, &v.z);
	shared_vertices.push_back(v);
}

// read texture coordinates u,v
inline void read_texcoord(const char* buff, std::vector<glm::vec2>& shared_textcoord)
{
	glm::vec2 vt;
	char* pEnd;
	vt.x = strtof(buff, &pEnd);
	vt.y = strtof(pEnd, &pEnd);
	//sscanf(buff, "%f %f", &vt.x, &vt.y);
	shared_textcoord.push_back(vt);
}

// read normals x,y,z
inline void read_normal(const char* buff, std::vector<glm::vec3>& shared_normals)
{
	glm::vec3 n;
	char* pEnd;
	n.x = strtof(buff, &pEnd);
	n.y = strtof(pEnd, &pEnd);
	n.z = strtof(pEnd, &pEnd);
	//sscanf(buff, "%f %f %f", &n.x, &n.y, &n.z);
	shared_normals.push_back(n);
}

glm::ivec3 read_face_component(const char* buff, int& offset, std::vector<glm::vec3>& sv, std::vector<glm::vec2>& st, std::vector<glm::vec3>& sn, std::vector<unsigned int>& se)
{
	int index = 0;
	int v, vt, vn;
	// v/vt/vn format
	if (sscanf(buff, "%d/%d/%d%n", &v, &vt, &vn, &index) >= 3)
	{
		v += v < 0 ? sv.size() : -1;
		vn += vn < 0 ? sn.size() : -1;
		vt += vt < 0 ? st.size() : -1;
		se.push_back(v);
		offset += index;
		return glm::ivec3(v, vn, vt);
	}
	// v//vn format
	if (sscanf(buff, "%d//%d%n", &v, &vn, &index) >= 2)
	{
		v += v < 0 ? sv.size() : -1;
		vn += vn < 0 ? sn.size() : -1;
		se.push_back(v);
		offset += index;
		return glm::ivec3(v, vn, -1);
	}
	// v/vt format
	if (sscanf(buff, "%d/%d%n", &v, &vt, &index) >= 2)
	{
		v += v < 0 ? sv.size() : -1;
		vt += vt < 0 ? st.size() : -1;
		se.push_back(v);
		offset += index;
		return glm::ivec3(v, -1, vt);
	}
	// v format
	if (sscanf(buff, "%d%n", &v, &index) >= 1)
	{
		v += v < 0 ? sv.size() : -1;
		se.push_back(v);
		offset += index;
		return glm::ivec3(v, -1, -1);
	}
	return glm::ivec3(-1);
}

// format v/vt/vn v/vt/vn v/vt/vn, only read a triangle or a quad (Or more not tested)
void read_face(const char* buff, std::vector<glm::vec3>& sv, std::vector<glm::vec2>& st, std::vector<glm::vec3>& sn, std::vector<unsigned int>& se, std::vector<Engine::Face>& sf)
{
	int step = 0;
	std::vector<glm::ivec3> vertices_pointers;
	vertices_pointers.reserve(6);
	glm::ivec3 vertex_pointer(0);

	vertex_pointer = read_face_component(buff + step, step, sv, st, sn, se);
	//while (glm::all(glm::greaterThanEqual(vertex_pointer, glm::ivec3(0))))
	while (vertex_pointer.x >= 0)// && (vertex_pointer.x >= 0 || vertex_pointer.y >= 0 || vertex_pointer.z >= 0)) // vertex index should always exist
	{
		vertices_pointers.push_back(vertex_pointer);
		vertex_pointer = read_face_component(buff + step, step, sv, st, sn, se);
	}

	/*
	*  I read 5 vertices                              |1|2|3|4|5|
	*  I add the extra necessery padding ( pad = 2 )
	*  So i add 2*pad+size                            |1|2|3|4|5|_|_|_|_|
	*  Then i put from the last index to the 3rd
	*  into their correct positions                   |1|2|3|4|5|_|_|_|5|  and  |1|2|3|4|5|4|_|_|5|
	*  Then put at x and x+1 position
	*  ( e.g. from 4th -> last with i+=3 ) the
	*  correct vertices
	*  |1|2|3|x|5|4|_|_|5| -> |1|2|3|1|5|_|_|_|5| and |1|2|3|1|x|4|_|_|5| -> |1|2|3|1|3|4|_|_|5|
	*
	*/
	
	int number_of_faces = (vertices_pointers.size() - 3) + 1;

	for (int face = 0; face < number_of_faces; face++)
	{
		//break them using triangle fan :( it is easy for quads

		Engine::Face f;
		f.vertices = glm::ivec3(vertices_pointers[face + 0].x, vertices_pointers[face + 1].x, vertices_pointers[face + 2].x);
		f.normals = glm::ivec3(vertices_pointers[face + 0].y, vertices_pointers[face + 1].y, vertices_pointers[face + 2].y);
		f.textcoords = glm::ivec3(vertices_pointers[face + 0].z, vertices_pointers[face + 1].z, vertices_pointers[face + 2].z);

		sf.push_back(f);
	}
}

// format v/vt/vn v/vt/vn v/vt/vn, only read a triangle or a quad
void read_faceLimited(const char* buff, std::vector<glm::vec3>& sv, std::vector<glm::vec2>& st, std::vector<glm::vec3>& sn, std::vector<unsigned int>& se, std::vector<Engine::Face>& sf)
{
	int step = 0;
	glm::ivec3 vertices_pointers[4];
	vertices_pointers[0] = read_face_component(buff + step, step, sv, st, sn, se);
	vertices_pointers[1] = read_face_component(buff + step, step, sv, st, sn, se);
	vertices_pointers[2] = read_face_component(buff + step, step, sv, st, sn, se);
	vertices_pointers[3] = read_face_component(buff + step, step, sv, st, sn, se);

	// if it is a triangle
	if (vertices_pointers[0].x >= 0 && vertices_pointers[1].x >= 0 && vertices_pointers[2].x >= 0)
	{
		Engine::Face f;
		f.vertices = glm::ivec3(vertices_pointers[0].x, vertices_pointers[1].x, vertices_pointers[2].x);
		f.normals = glm::ivec3(vertices_pointers[0].y, vertices_pointers[1].y, vertices_pointers[2].y);
		f.textcoords = glm::ivec3(vertices_pointers[0].z, vertices_pointers[1].z, vertices_pointers[2].z);
		sf.push_back(f);

		// if it was a quad, add another triangle so we can form a quad
		if (vertices_pointers[3].x >= 0)
		{
			Engine::Face f;
			f.vertices = glm::ivec3(vertices_pointers[2].x, vertices_pointers[3].x, vertices_pointers[0].x);
			f.normals = glm::ivec3(vertices_pointers[2].y, vertices_pointers[3].y, vertices_pointers[0].y);
			f.textcoords = glm::ivec3(vertices_pointers[2].z, vertices_pointers[3].z, vertices_pointers[0].z);
			sf.push_back(f);
		}
	}
}



void read_usemtl(const char* buff, int& currentMaterialID, size_t faces_size, Engine::Mesh& mesh)
{
	char temp[1024];
	sscanf(buff, "%s", temp);
	// read the material
	std::string str(temp);

	//check if we have already defined a material
	if (mesh.objects.back().material_id > 0)
	{
		// set where the current object ends
		mesh.objects.back().end = 3 * faces_size;
		//create a new MeshObject
		Engine::Object mo;
		mo.name = mesh.objects.back().name;
		mo.material_id = mesh.findMaterialID(str);
		mo.start = 3 * faces_size;
		mesh.objects.push_back(mo);
	}
	else
	{
		mesh.objects.back().material_id = mesh.findMaterialID(str);
	}
	currentMaterialID = mesh.objects.back().material_id;
}


void parseMTL(const char* filename, Engine::Mesh& mesh)
{
	std::ifstream in(filename, std::ios::in);
	if (!in)
	{
		std::cerr << "Cannot open material " << filename << std::endl;
		exit(1);
	}
	printf("Opened %s\n", filename);

	std::string folder(filename);
	folder = Engine::utils::GetFolderPath(folder.c_str());

	//the materials to return
	Engine::Material temp;
	temp.name = "default";
	mesh.materials.push_back(temp);
	int currentMaterialID = mesh.materials.size() - 1;
	const int firsMaterialID = currentMaterialID;

	float r, g, b;

	std::string line;
	while (getline(in, line))
	{
		if (line.size() == 0) continue;

		{
			int pos = 0;
			while ((line[pos] == ' ' || line[pos] == '\n' || line[pos] == '\r' || line[pos] == '\t') && pos < line.size())
				pos++;
			line = line.substr(pos);
		}

		if (line.substr(0, 7) == "newmtl ") { // read vertices x,y,z
			std::istringstream s(line.substr(7));
			std::string str;
			s >> str;
			// if it is the default mtl
			str = (str.empty()) ? "default" : str;

			// find if the material with that name exists
			currentMaterialID = -1;
			for (unsigned int i = 0; i < mesh.materials.size(); i++)
			{
				if (mesh.materials[i].name == str)
				{
					currentMaterialID = i;
					break;
				}
			}
			if (currentMaterialID == -1) // didn't find  the material
			{
				Engine::Material mat;
				mat.name = str;
				mesh.materials.push_back(mat);
				currentMaterialID = mesh.materials.size() - 1;
			}
			//printf("material name = %s\n",currentMat->name.c_str());
		}
		else if (line.substr(0, 3) == "Ka ") { // read ambient color
			std::istringstream s(line.substr(3));
			s >> r; s >> g; s >> b;
			//printf("ambient =%f %f %f\n",r,g,b);
			mesh.materials[currentMaterialID].ambient[0] = r;
			mesh.materials[currentMaterialID].ambient[1] = g;
			mesh.materials[currentMaterialID].ambient[2] = b;
			mesh.materials[currentMaterialID].ambient[3] = 1;
		}
		else if (line.substr(0, 3) == "Kd ") { // read diffuse color
			std::istringstream s(line.substr(3));
			s >> r; s >> g; s >> b;
			//printf("diffuse =%f %f %f\n",r,g,b);
			mesh.materials[currentMaterialID].diffuse[0] = r;
			mesh.materials[currentMaterialID].diffuse[1] = g;
			mesh.materials[currentMaterialID].diffuse[2] = b;
			mesh.materials[currentMaterialID].diffuse[3] = 1;
		}
		else if (line.substr(0, 3) == "Ks ") { // read specular color
			std::istringstream s(line.substr(3));
			s >> r; s >> g; s >> b;
			mesh.materials[currentMaterialID].specular[0] = r;
			mesh.materials[currentMaterialID].specular[1] = g;
			mesh.materials[currentMaterialID].specular[2] = b;
			mesh.materials[currentMaterialID].specular[3] = 1.0f;
		}
		else if (line.substr(0, 3) == "Ns ") { // read shineness
			std::istringstream s(line.substr(3));
			s >> r;
			mesh.materials[currentMaterialID].shininess = r;
		}
		else if (line.substr(0, 2) == "d ") { // read alpha
			std::istringstream s(line.substr(2));
			s >> r;
			mesh.materials[currentMaterialID].alpha = r;
		}
		else if (line.substr(0, 6) == "illum ") { // read illumination model
			std::istringstream s(line.substr(6));
			int i;
			s >> i;
			mesh.materials[currentMaterialID].illumination_model = i;
		}
		else if (Engine::utils::compareStringIgnoreCase(line.substr(0, 7), "map_kd ")) { // read texture
			std::istringstream s(line.substr(7));
			s >> mesh.materials[currentMaterialID].texture;
			mesh.materials[currentMaterialID].texture = folder + mesh.materials[currentMaterialID].texture;
		}
		else if (Engine::utils::compareStringIgnoreCase(line.substr(0, 7), "map_ka ")) { // read ambient texture
			std::istringstream s(line.substr(7));
			s >> mesh.materials[currentMaterialID].textureAmbient;
			mesh.materials[currentMaterialID].textureAmbient = folder + mesh.materials[currentMaterialID].textureAmbient;
		}
		else if (Engine::utils::compareStringIgnoreCase(line.substr(0, 7), "map_ks ")) { // read specular texture
			std::istringstream s(line.substr(7));
			s >> mesh.materials[currentMaterialID].textureSpecular;
			mesh.materials[currentMaterialID].textureSpecular = folder + mesh.materials[currentMaterialID].textureSpecular;
		}
		else if (Engine::utils::compareStringIgnoreCase(line.substr(0, 6), "map_d ")) { // read opacity texture
			std::istringstream s(line.substr(6));
			s >> mesh.materials[currentMaterialID].textureOpacity;
			mesh.materials[currentMaterialID].textureOpacity = folder + mesh.materials[currentMaterialID].textureOpacity;
		}
		else if (Engine::utils::compareStringIgnoreCase(line.substr(0, 9), "map_bump ")) { // read bump texture
			std::istringstream s(line.substr(9));
			s >> mesh.materials[currentMaterialID].textureBump;
			mesh.materials[currentMaterialID].textureBump = folder + mesh.materials[currentMaterialID].textureBump;
		}
		else if (line.substr(0, 7) == "map_ns ") { // read specularity
			std::istringstream s(line.substr(7));
			s >> mesh.materials[currentMaterialID].textureSpecularity;
			mesh.materials[currentMaterialID].textureSpecularity = folder + mesh.materials[currentMaterialID].textureSpecularity;
		}
		else if (line[0] == '#') { /* ignoring this line */ }
		else { /* ignoring this line */ }
	}
	in.close();
}

void calculate_avg_normals(std::vector<glm::vec3>& shared_vertices, std::vector<glm::vec3>& normals, std::vector<unsigned int>& elements)
{
	std::vector<int> nb_seen;
	std::vector<glm::vec3> temp_normals;
	temp_normals.resize(shared_vertices.size(), glm::vec3(0.0, 0.0, 0.0));
	nb_seen.resize(shared_vertices.size(), 0);
	for (unsigned int i = 0; i < elements.size(); i += 3) {
		unsigned int ia = elements[i];
		unsigned int ib = elements[i + 1];
		unsigned int ic = elements[i + 2];
		glm::vec3 normal = glm::normalize(glm::cross(
			glm::vec3(shared_vertices[ib]) - glm::vec3(shared_vertices[ia]),
			glm::vec3(shared_vertices[ic]) - glm::vec3(shared_vertices[ia])));

		int v[3];  v[0] = ia;  v[1] = ib;  v[2] = ic;
		for (int j = 0; j < 3; j++) {
			unsigned int cur_v = v[j];
			nb_seen[cur_v]++;
			if (nb_seen[cur_v] == 1) {
				temp_normals[cur_v] = normal;
			}
			else {
				// average
				temp_normals[cur_v].x = temp_normals[cur_v].x * (1.0 - 1.0 / nb_seen[cur_v]) + normal.x * 1.0 / nb_seen[cur_v];
				temp_normals[cur_v].y = temp_normals[cur_v].y * (1.0 - 1.0 / nb_seen[cur_v]) + normal.y * 1.0 / nb_seen[cur_v];
				temp_normals[cur_v].z = temp_normals[cur_v].z * (1.0 - 1.0 / nb_seen[cur_v]) + normal.z * 1.0 / nb_seen[cur_v];
				temp_normals[cur_v] = glm::normalize(temp_normals[cur_v]);
			}
		}
	}
	// add normals
	for (unsigned int i = 0; i < elements.size(); i += 3)
	{
		unsigned int ia = elements[i];
		unsigned int ib = elements[i + 1];
		unsigned int ic = elements[i + 2];
		normals.push_back(temp_normals[ia]);
		normals.push_back(temp_normals[ib]);
		normals.push_back(temp_normals[ic]);
	}

}



void read_mtllib(const char* buff, const std::string& folderPath, Engine::Mesh& mesh)
{
	char temp[1024];
	sscanf(buff, "%s", temp);

	std::string str(temp);
	str = folderPath + str;
	parseMTL(str.c_str(), mesh);
}

void add_new_group(const char* buff, int& currentMaterialID, size_t faces_size, Engine::Mesh& mesh)
{
	char name[1024];
	sscanf(buff, "%s", name);

	// end the previous MeshObject
	mesh.objects.back().end = 3 * faces_size;

	// create a new object
	Engine::Object mo;
	mo.start = 3 * faces_size;
	mo.material_id = currentMaterialID;
	mo.name = name;
	mesh.objects.push_back(mo);
}


void generateDataFromFaces(Engine::Mesh& mesh, std::vector<glm::vec3>& sv, std::vector<glm::vec2>& st, std::vector<glm::vec3>sn, std::vector<Engine::Face> sf)
{
	bool hasTextures = !st.empty();

	mesh.setmem(sf.size(), hasTextures);

	const glm::ivec3 zeroVec(0);

	for (int face = 0; face < sf.size(); face++)
	{
		//mesh->vertices.push_back(shared_vertices[shared_faces[face].vertices.x]);
		mesh.setNextPosition(sv[sf[face].vertices.x].x); mesh.setNextPosition(sv[sf[face].vertices.x].y); mesh.setNextPosition(sv[sf[face].vertices.x].z);
		//mesh->vertices.push_back(shared_vertices[shared_faces[face].vertices.y]);
		mesh.setNextPosition(sv[sf[face].vertices.y].x); mesh.setNextPosition(sv[sf[face].vertices.y].y); mesh.setNextPosition(sv[sf[face].vertices.y].z);
		//mesh->vertices.push_back(shared_vertices[shared_faces[face].vertices.z])
		mesh.setNextPosition(sv[sf[face].vertices.z].x); mesh.setNextPosition(sv[sf[face].vertices.z].y); mesh.setNextPosition(sv[sf[face].vertices.z].z);


		if (glm::any(glm::greaterThanEqual(sf[face].normals, glm::ivec3(0))))
		{
			//mesh->normals.push_back(shared_normals[shared_faces[face].normals.x]);
			mesh.setNextNormal(sn[sf[face].normals.x].x); mesh.setNextNormal(sn[sf[face].normals.x].y); mesh.setNextNormal(sn[sf[face].normals.x].z);
			//mesh->normals.push_back(shared_normals[shared_faces[face].normals.y]);
			mesh.setNextNormal(sn[sf[face].normals.y].x); mesh.setNextNormal(sn[sf[face].normals.y].y); mesh.setNextNormal(sn[sf[face].normals.y].z);
			//mesh->normals.push_back(shared_normals[shared_faces[face].normals.z]);
			mesh.setNextNormal(sn[sf[face].normals.z].x); mesh.setNextNormal(sn[sf[face].normals.z].y); mesh.setNextNormal(sn[sf[face].normals.z].z);


			if (glm::any(glm::greaterThanEqual(sf[face].textcoords, glm::ivec3(0))))
			{
				//std::cout << "In the correct road" << std::endl;
				//mesh->textureCoord.push_back(shared_textcoord[shared_faces[face].texcoords.x]);
				mesh.setNextTexture(st[sf[face].textcoords.x].x); mesh.setNextTexture(st[sf[face].textcoords.x].y);
				//mesh->textureCoord.push_back(shared_textcoord[shared_faces[face].texcoords.y]);
				mesh.setNextTexture(st[sf[face].textcoords.y].x); mesh.setNextTexture(st[sf[face].textcoords.y].y);
				//mesh->textureCoord.push_back(shared_textcoord[shared_faces[face].texcoords.z]);
				mesh.setNextTexture(st[sf[face].textcoords.z].x); mesh.setNextTexture(st[sf[face].textcoords.z].y);
			}
			else if (hasTextures)
			{
				for (int i = 0; i < 2 * 3; i++) mesh.setNextTexture(0.f);
				//mesh->textureCoord.emplace_back(0);
				//mesh->textureCoord.emplace_back(0);
				//mesh->textureCoord.emplace_back(0);
			}
		}
	}
}



namespace Engine {
	Mesh* loadObj(const char* filename) {
		printf("Start ObjReading MeshNext reading\n");

		std::vector<glm::vec3> shared_vertices;
		std::vector<glm::vec3> shared_normals;
		std::vector<glm::vec2> shared_textures;
		std::vector<unsigned int> elements;
		std::vector<Face> shared_faces;
		bool hasTextures = false; bool hasNormals = false;

		char buff[1024];
		char str[1024];

		FILE* pFile;
		std::string folderPath = utils::GetFolderPath(filename);
		pFile = fopen(filename, "r");

		if (pFile == NULL)
		{
			printf("ObjLoaderMeshNext: Error opening file %s \n", filename);
			return nullptr;
		}

		Mesh* mesh = new Mesh();

		int str_pos;

		// add a default material
		mesh->materials.push_back(Material());

		//add a default meshObject
		Object defaultOb;
		defaultOb.start = 0;
		defaultOb.material_id = 0;
		mesh->objects.push_back(defaultOb);
		int currentMaterialID = 0;

		//printf("Initialization done\n");

		//read the file
		while (fgets(buff, 1024, pFile) != NULL)
		{
			//printf("%s\n",buff);

			if (sscanf(buff, "%s %n", str, &str_pos) >= 1)
			{
				if (strcmp(str, "v") == 0) read_vertex(buff + str_pos, shared_vertices);
				else if (strcmp(str, "vt") == 0) read_texcoord(buff + str_pos, shared_textures);
				else if (strcmp(str, "vn") == 0) read_normal(buff + str_pos, shared_normals);
				else if (strcmp(str, "f") == 0) read_faceLimited(buff + str_pos, shared_vertices, shared_textures, shared_normals, elements, shared_faces);
				else if (strcmp(str, "usemtl") == 0) read_usemtl(buff + str_pos, currentMaterialID, shared_faces.size(), *mesh);
				else if (strcmp(str, "mtllib") == 0) read_mtllib(buff + str_pos, folderPath, *mesh);
				else if (strcmp(str, "g") == 0 || strcmp(str, "o") == 0) add_new_group(buff + str_pos, currentMaterialID, shared_faces.size(), *mesh);
				else if (strcmp(str, "#") == 0) { /* ignoring this line */ }
				else { /* ignoring this line */ }
			}
		}
		fclose(pFile);

		//printf("File closed!");
		// Generate vertices and other data from faces
		generateDataFromFaces(*mesh,shared_vertices, shared_textures, shared_normals, shared_faces);

		//printf("Faces generated");


		// close the last object
		mesh->objects.back().end = mesh->Vertices();

		printf("Done reading OBJ file \n");

		// remove empty objects
		mesh->objects.erase(std::remove_if(mesh->objects.begin(), mesh->objects.end(), [](Object ob) { return ob.start == ob.end; }), mesh->objects.end());

		// if normals doesn't exist create them
		if (mesh->Normals()==0)
		{
			printf("normals not found\n");

			bool flatShading = false;
			//if (flatShading) calculate_flat_normals();
			//else calculate_avg_normals(shared_vertices, mesh->normals, elements);
		}

		// check if we loaded an normal map
		if (std::find_if(mesh->materials.begin(), mesh->materials.end(), [](Material mat) { return mat.textureBump.length() > 0; }) != mesh->materials.end())
		{
			mesh->calculate_tangents();
			hasNormals = true;
		}

		return mesh;
	}
}