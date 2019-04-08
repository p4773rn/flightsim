#include "ac3d_loader.h"
#include <fstream>
#include <sstream>
#include <cassert>
#include <algorithm>
#include <iterator>
#include <cstdio>
#include <utility>
#include <stack>

static void push_vertice(Object& o, const Vertex& verts);
static void process_string(std::string& str);
static void buffer_data(Object& obj);
static void reset_obj(Object& obj);
//const int MAX_SURFACES = 500;

AC3D_loader::AC3D_loader(const std::string& fname) {
	load(fname);
	int vertices = 0, indices = 0;
	for (auto& o : objects) {
		vertices += o.vertices.size();
		indices += o.indices.size();
	}
	std::cout << "Vertices: " << vertices << "\nIndices: " << indices << std::endl;
}

void AC3D_loader::load(const std::string& fname) {
	enum state { VERTEX, OBJECT, SURFACE, GLOBAL};
	//state data
	std::stack<std::pair<int, int>> parents; //object id, kids remaining
	Object current_object;
	reset_obj(current_object);

	int surfaces_left = 0;
	int n_refs = 0;
	int vertices_left = 0;
	int i = 0;
	std::vector<int> surface_intervals;
	surface_intervals.push_back(0);
	std::vector<glm::vec3> temp_positions; //temp positions
	std::vector<Vertex> surface_vertices; //vector of surface_vertices
	//glm::vec3 surface_normal; //surface normal
	std::string parent_dir = fname.substr(0, fname.find_last_of("/\\"));
	state current_state = GLOBAL;
	
	std::ifstream file_input(fname);
	if (!file_input.is_open())
		std::cerr << "Can't open " << fname << std::endl;
	for(std::string line; getline(file_input, line);) {
		std::istringstream sin(line);
		std::string key;
		if (current_state != SURFACE && current_state != VERTEX) sin >> key;
		
		if (current_state == GLOBAL && key == "MATERIAL") {
			Material m;
			std::string ignored;
			sin >> m.name >> ignored;
			sin >> m.rgb.x >> m.rgb.y >> m.rgb.z >> ignored;
			sin >> m.ambient.x >> m.ambient.y >> m.ambient.z >> ignored;
			sin >> m.emission.x >> m.emission.y >> m.emission.z >> ignored;
			sin >> m.specular.x >> m.specular.y >> m.specular.z >> ignored;
			sin >> m.shininess >> ignored >> m.transparency;
			//erase quotes
			process_string(m.name);
			materials.push_back(m);
		} else if(current_state == GLOBAL && key == "OBJECT") {
			sin >> current_object.type;
			current_state = OBJECT;
		} else if(current_state == OBJECT && key == "name") {
			sin >> current_object.name; 
			process_string(current_object.name);
		} else if(current_state == OBJECT && key == "texture") {
			std::string texture_name;
			sin >> texture_name;
			process_string(texture_name);
			auto texture_it = std::find_if(textures.begin(), textures.end(), [&texture_name](const Texture& t){return t.name == texture_name;});
			if (texture_it == textures.end()){
				textures.push_back(Texture(parent_dir + "/textures/" + texture_name, true, texture_name));
				current_object.texture_id = textures.size() - 1;
			} else {
				current_object.texture_id = std::distance(textures.begin(), texture_it);
			}
		} else if(current_state == OBJECT && key == "loc") {
			sin >> current_object.position.x >> current_object.position.y >> current_object.position.z;
		} else if(current_state == OBJECT && key == "numvert") {
			sin >> vertices_left;
			current_state = VERTEX;
		} else if(current_state == VERTEX && vertices_left > 0) {
			glm::vec3 pos;
			sin >> pos.x >> pos.y >> pos.z;
			temp_positions.push_back(pos);
			if (--vertices_left == 0) current_state = OBJECT;
		} else if(current_state == OBJECT && key == "numsurf") {
			sin >> surfaces_left;
			current_state = SURFACE;
		} else if(current_state == SURFACE) {
			if (i < n_refs) {
				Vertex v;
				int idx;	
				sin >> idx;
				v.position = temp_positions[idx];
				//v.normal = surface_normal;
				sin >> v.uv.x >> v.uv.y;
				surface_vertices.push_back(v);
				////calculate normal for surface
				//if(i == 2) {
				//	glm::vec3 a = surface_vertices[1].position - surface_vertices[0].position;
				//	glm::vec3 b = surface_vertices[2].position - surface_vertices[0].position;
				//	surface_normal = glm::normalize(glm::cross(a,b));
				//	surface_vertices[0].normal = surface_vertices[1].normal = surface_vertices[2].normal = surface_normal;
				//}
				if (++i == n_refs && --surfaces_left == 0) current_state = OBJECT;
			} else {
				sin >> key;
				if (key == "mat") {
					sin >> current_object.material_id;
				} else if (key == "refs") {
					sin >> n_refs;
					i = 0;
					int s = surface_intervals.size();
					surface_intervals.push_back(surface_intervals[s-1] + n_refs);
				}
			}
		} else if(current_state == OBJECT && key == "kids") {
			int kids;
			int v_len = surface_vertices.size();
			int ci = objects.size();
			sin >> kids;
			for (int j = 1; j < surface_intervals.size(); j++) {
				for (int k = surface_intervals[j-1] + 2; k < surface_intervals[j]; k++) {
					push_vertice(current_object, surface_vertices[surface_intervals[j-1]]);
					push_vertice(current_object, surface_vertices[k-1]);
					push_vertice(current_object, surface_vertices[k]);
				}
			}
			for (auto& vertex : current_object.vertices) vertex.normal = glm::normalize(vertex.normal);
	
			if (!current_object.vertices.empty()) buffer_data(current_object);
		
			objects.push_back(current_object);
			if (!parents.empty()) {
				auto& parent = parents.top();
				objects[parent.first].children.push_back(ci);
				if (--parent.second == 0) parents.pop();
			}
			
			if (kids) {
				parents.push({ci, kids});
			}
			reset_obj(current_object);
			temp_positions.clear();
			surface_vertices.clear();
			surface_intervals.clear();
			surface_intervals.push_back(0);
			current_state = GLOBAL;
		}
	}
}

static void reset_obj(Object& obj) {
	obj.name = "";
	obj.type = "";
	obj.vertices.clear();
	obj.indices.clear();
	obj.children.clear();
	obj.position.x = 0; obj.position.y = 0; obj.position.z = 0;
	//obj.yaw_roll_pitch.x = 0; obj.yaw_roll_pitch.y = 0; obj.yaw_roll_pitch.z = 0;
	obj.orientation = glm::dquat();
	obj.texture_id = -1;
	obj.material_id = -1;
	obj.VAO = -1;
	obj.VBO = -1;
	obj.EBO = -1;
}

static void buffer_data(Object& obj) {
    for (int i = 0; i < obj.indices.size(); ++i) {
        if (i % 3 == 2) {
            Vertex& v1 = obj.vertices[obj.indices[i-2]];
            Vertex& v2 = obj.vertices[obj.indices[i-1]];
            Vertex& v3 = obj.vertices[obj.indices[i-0]];
            glm::vec3 a = v2.position - v1.position;
            glm::vec3 b = v3.position - v1.position;
            glm::vec3 n = glm::normalize(glm::cross(a,b));
            v1.normal = n;
            v2.normal = n;
            v3.normal = n;
        }
    }


	glGenVertexArrays(1, &obj.VAO);
	glGenBuffers(1, &obj.EBO);
	glGenBuffers(1, &obj.VBO);
	glBindVertexArray(obj.VAO);
	glBindBuffer(GL_ARRAY_BUFFER, obj.VBO);
	glBufferData(GL_ARRAY_BUFFER, obj.vertices.size() * sizeof(Vertex), obj.vertices.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, obj.EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, obj.indices.size() * sizeof(unsigned int), obj.indices.data(), GL_STATIC_DRAW);
	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	glEnableVertexAttribArray(2);
	
	glBindVertexArray(0);
}

static void process_string(std::string& str) {
	if (str[0] == '"') {
		str.erase(0, 1);
		str.erase(str.size() - 1);
	}
}

static void push_vertice(Object& o, const Vertex& v) {
	auto v_iterator = std::find_if(o.vertices.begin(), o.vertices.end(), [&v](const Vertex& v2) {
			bool eq1 = v.position.x == v2.position.x &&
						v.position.y == v2.position.y &&
						v.position.z == v2.position.z;
			bool eq2 = v.uv.x == v2.uv.x && v.uv.y == v2.uv.y;
			return eq1 && eq2;
	});
	int indice = 0;
	if (v_iterator == o.vertices.end()) {
		indice = o.vertices.size();
		o.vertices.push_back(v);
	} else {
		//std::cout << "MERGING NORMALS " << v_iterator->normal.x << " " << v_iterator->normal.y << " " << v_iterator->normal.z << std::endl;
		v_iterator->normal = v_iterator->normal + v.normal;
		indice = std::distance(o.vertices.begin(), v_iterator);
	}
	o.indices.push_back(indice);
}

