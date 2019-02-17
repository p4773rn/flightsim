#include "terrain.h"
#include <iostream>
#include <fstream>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "../misc.h"
#include <queue>


Terrain::Terrain(const glm::vec3& origin, float size) :
    shader({
        std::pair<std::string, GLuint>("src/openGL/shaders/terrain.vrtx", GL_VERTEX_SHADER),
        std::pair<std::string, GLuint>("src/openGL/shaders/terrain.tec", GL_TESS_CONTROL_SHADER),
        std::pair<std::string, GLuint>("src/openGL/shaders/terrain.tse", GL_TESS_EVALUATION_SHADER),
        std::pair<std::string, GLuint>("src/openGL/shaders/terrain.frgmnt", GL_FRAGMENT_SHADER)
    })
{
    this->size = size;
    this->origin = origin;

    GLfloat patch[] = {
        -1.0f, -1.0f,
         1.0f, -1.0f,
         1.0f, 1.0f,
        -1.0f, 1.0f
    };
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(patch), patch, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2*sizeof(GLfloat), (void*) 0);
    glEnableVertexAttribArray(0);
    
    
    load_textures();
}


void Terrain::render(const glm::vec3& camera_pos) {
  	shader.set("heightmap", 0);
  	heightmap.activate(GL_TEXTURE0);
    
    shader.set("splatmap", 1);
  	splatmap.activate(GL_TEXTURE1);
    
    shader.set("lightmap", 2);
  	lightmap.activate(GL_TEXTURE2);
    
    int t_units[MAX_TEXTURES] = {3, 4, 5, 6};
    shader.setv("textures", t_units, MAX_TEXTURES);
  	for(size_t i = 0; i < MAX_TEXTURES; ++i) {
  		textures[i].activate(GL_TEXTURE3 + i);
  	}

  	shader.set("terrain_size", size);
  	shader.set("terrain_origin", origin);
  	shader.set("tex_coord_offset", tex_coord_offset);
  
    shader.set("camera_pos", camera_pos);

  	Node* root = construct_tree(camera_pos);
  	//std::cerr<<node_count<<std::endl;
    calc_tess(root);
    render_tree(root, camera_pos);
}

Terrain::Node* Terrain::construct_tree(const glm::vec3& camera_pos) {
    node_count = 0;
    Terrain::Node* root = create_node(nullptr, glm::vec2(origin.x, origin.z), size);
    divide(root, camera_pos);
    return root;
}


Terrain::Node* Terrain::create_node(Terrain::Node* parent, const glm::vec2& origin, float size) {
    if (node_count >= MAX_NODES) {
        return nullptr;
    }

    Terrain::Node* node = &nodes[node_count];
    
    if (parent)
        node->depth = parent->depth + 1;
    else
        node->depth = 0;

    node->parent = parent;
    node->origin = origin;
    node->size = size;
    node->tl = 1.0f;
    node->tr = 1.0f;
    node->tt = 1.0f;
    node->tb = 1.0f;
    node->lt = nullptr;
    node->lb = nullptr;
    node->rt = nullptr;
    node->rb = nullptr;
    node->nl = nullptr;
    node->nr = nullptr;
    node->nt = nullptr;
    node->nb = nullptr;
    
    node_count++;

    return node;
}


bool Terrain::is_divisible(Node* node, const glm::vec3& camera_pos) {
    // TODO: get node height from heightmap for them tall mountains
    glm::vec3 node_pos(node->origin.x, origin.y, -node->origin.y);
    //glm::vec3 node_pos(node->origin.x, camera_pos.y + 10.0, -node->origin.y);
    float dist = glm::distance(node_pos, camera_pos);
    float diag = glm::length(glm::vec3(node->size));
    return dist < diag * LOD && node->size > MIN_NODE_SIZE;// && node->depth < 3;
}


void Terrain::divide(Node* node, const glm::vec3& camera_pos) {
    if (!node) // might happend when create_node returns nullptr
        return;
    if (is_divisible(node, camera_pos)) {
        node->lt = create_node(node, glm::vec2(node->origin.x - node->size/4, node->origin.y + node->size/4), node->size/2);
        node->lb = create_node(node, glm::vec2(node->origin.x - node->size/4, node->origin.y - node->size/4), node->size/2);
        node->rt = create_node(node, glm::vec2(node->origin.x + node->size/4, node->origin.y + node->size/4), node->size/2);
        node->rb = create_node(node, glm::vec2(node->origin.x + node->size/4, node->origin.y - node->size/4), node->size/2);

        node->lt->name = node->name + "0";
        node->lb->name = node->name + "2";
        node->rt->name = node->name + "1";
        node->rb->name = node->name + "3";

        divide(node->lt, camera_pos);
        divide(node->lb, camera_pos);
        divide(node->rt, camera_pos);
        divide(node->rb, camera_pos);
    }
}


void Terrain::calc_tess(Node* node) {
    std::queue<Terrain::Node*> q({node});

    while(!q.empty()) {
        Node* node = q.front();
        q.pop();

        //std::cerr << node->name << std::endl;

        // if node doesn't have children
        if (!node->lt) 
            continue;

        node->lt->nr = node->rt;
        node->lt->nb = node->lb;
        node->lb->nt = node->lt;
        node->lb->nr = node->rb;
        node->rt->nl = node->lt;
        node->rt->nb = node->rb;
        node->rb->nt = node->rt;
        node->rb->nl = node->lb;

        if (node->nt) {
            calc_tess_help(node->lt, node->rt, node->lt->tt, node->rt->tt, &node->lt->nt, &node->rt->nt, node->nt, node->nt->lb, node->nt->rb);
        }
        if (node->nb) {
            calc_tess_help(node->lb, node->rb, node->lb->tb, node->rb->tb, &node->lb->nb, &node->rb->nb, node->nb, node->nb->lt, node->nb->rt);
        }
        if (node->nl) {
            calc_tess_help(node->lt, node->lb, node->lt->tl, node->lb->tl, &node->lt->nl, &node->lb->nl, node->nl, node->nl->rt, node->nl->rb);
        }
        if (node->nr) {
            calc_tess_help(node->rt, node->rb, node->rt->tr, node->rb->tr, &node->rt->nr, &node->rb->nr, node->nr, node->nr->lt, node->nr->lb);
        }

        q.push(node->lt);
        q.push(node->lb);
        q.push(node->rt);
        q.push(node->rb);
    }
}

// child1->tesselation, child2->tesselation, child1->neighbor, child2->neighbor, neighbor, neighbor->child1, neighbor->child2
void Terrain::calc_tess_help(Node* c1, Node* c2, float& c1t, float& c2t, Node** c1n, Node** c2n, Node* n, Node* nc1, Node* nc2) {
    if (nc1) {
        *c1n = nc1;
        *c2n = nc2;
    } else {
        *c1n = n;
        *c2n = n;
        c1t = 0.5;
        c2t = 0.5;
    }
}

void Terrain::render_tree(Node* node, const glm::vec3& camera_pos) {
    if(!node)
        return;
    if (!node->lt && !node->lb && !node->rb && !node->rt){
        render_node(node, camera_pos);
    } else {
        render_tree(node->lt, camera_pos);
        render_tree(node->rt, camera_pos);
        render_tree(node->lb, camera_pos);
        render_tree(node->rb, camera_pos);
    }
}

void Terrain::render_node(Node* node, const glm::vec3& camera_pos) {
    glm::vec3 node_pos(node->origin.x, origin.y, -node->origin.y);
    
    float render_dist = std::max(16000.0f, camera_pos.y * 15);
    // closest possible point of the node
    float node_dist = glm::distance(glm::vec2(camera_pos.x, camera_pos.z), glm::vec2(node_pos.x, node_pos.z)) -
                      sqrt(node->size*node->size*2)*0.5;
    
    if (node_dist > render_dist) {
        //std::cerr << "culled" << std::endl;
        return;
    }

    glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(node->size/2.0f, 1.0f, node->size/2.0f));
    glm::mat4 trans = glm::translate(glm::mat4(1.0f), node_pos);
    glm::mat4 model = trans * scale;
    
    shader.set("model", model);
    shader.set("tesselation_right", node->tr);
    shader.set("tesselation_left", node->tl);
    shader.set("tesselation_top", node->tt);
    shader.set("tesselation_bottom", node->tb);
    shader.set("depth", node->depth);

    glBindVertexArray(vao);
    glPatchParameteri(GL_PATCH_VERTICES, 4);
    glDrawArrays(GL_PATCHES, 0, 4);
    //glBindVertexArray(0);
}



void Terrain::load_textures() {
    heightmap = Texture("assets/terrain/hm.png");
    splatmap = Texture("assets/terrain/alpha.png");
    lightmap = Texture("assets/terrain/normal.png");
	
	std::string line;
	std::ifstream texture_files("assets/terrain/alphamap.txt");

	for(unsigned int i = 0; getline(texture_files, line); ++i) {
		if (line.length() > 0) {
			textures[i] = Texture(line);
		}
	}
}
