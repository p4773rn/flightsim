#include "quadtree.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cmath>
#include <vector>
#include <utility>
#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>

void print_tree(TreeNode* n, int i) {
	if(n) {
		std::string s(i * 2, '_');
		if (n->parent){
			std::cout << s;
			if (n->parent->left_top == n)
				std::cout << "left top" << std::endl;
			if (n->parent->left_bottom == n)
				std::cout << "left bottom" << std::endl;
			if (n->parent->right_top == n)
				std::cout << "right top" << std::endl;
			if (n->parent->right_bottom == n)
				std::cout << "right_bottom" << std::endl;
		}
		std::cout << s;
		std::cout << "Origin: " << n->origin.x << " " << n->origin.y << std::endl;
		std::cout << s;
		std::cout << "W/H: " << n->width << " " << n->height << std::endl;
		i = i+1;
		print_tree(n->left_top, i);
		print_tree(n->left_bottom, i);
		print_tree(n->right_top, i);
		print_tree(n->right_bottom, i);
	}
}

Quadtree::Quadtree(const glm::vec3& _camera_position, const std::string& path, const float width, const float height) :
	camera_position{_camera_position}
{
	std::vector<std::pair<std::string, GLuint>> paths = {
      std::pair<std::string, GLuint>("shaders/terrain.vrtx", GL_VERTEX_SHADER),
      std::pair<std::string, GLuint>("shaders/terrain.tec", GL_TESS_CONTROL_SHADER),
      std::pair<std::string, GLuint>("shaders/terrain.tse", GL_TESS_EVALUATION_SHADER),
      std::pair<std::string, GLuint>("shaders/terrain.frgmnt", GL_FRAGMENT_SHADER)
    };
    shader = new Shader(paths);

	nodes = new TreeNode[MAX_NODES];
	root = nodes;
	root->origin.x = 0.0f;
	root->origin.y = 0.0f;
	root->width = width;
	root->height = height;
	root->parent = nullptr;

	// create patch
	GLfloat patch[] = {
		-1.0f, 1.0f,
		 1.0f, 1.0f,
		 1.0f, -1.0f,
		-1.0f, -1.0f,
	};
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(patch), patch, GL_STATIC_DRAW);
	glVertexAttribPointer(0,2, GL_FLOAT, GL_FALSE, 2*sizeof(GLfloat), (void*) 0);
	glEnableVertexAttribArray(0);


	heightmap = Texture(path);
	load_textures();
	construct_tree();
}

Quadtree::~Quadtree() {
	if (nodes)
		delete[] nodes;
}

TreeNode* Quadtree::create_node(TreeNode* p, const glm::vec2& origin) {
	nodes_count++;
	if (nodes_count >= MAX_NODES)
		return nullptr;
	current++; //move to next node
	current->parent = p;
	current->origin.x = origin.x;
	current->origin.y = origin.y;
	current->tesselation_left = 1.0f;
	current->tesselation_right = 1.0f;
	current->tesselation_top = 1.0f;
	current->tesselation_bottom = 1.0f;
	current->left_top = nullptr;
	current->left_bottom = nullptr;
	current->right_top = nullptr;
	current->right_bottom = nullptr;

	current->width = p->width/2.0f;
	current->height = p->height/2.0f;
	//std::cout << nodes_count << std::endl;
	//std::cout << p->width << std::endl;
	return current;
}

bool Quadtree::isDivisible(TreeNode* node) {
	float dist = sqrt(pow(node->origin.x - camera_position.x, 2) + pow(node->origin.y - camera_position.z, 2));
	float diagonal = sqrt(pow(node->width,2) + pow(node->height,2));
	return dist < diagonal && node->width > MIN_NODE_WIDTH && node->height > MIN_NODE_HEIGHT;
}

//find closest Node to target vec2
TreeNode* Quadtree::find(TreeNode* node, float x, float y) {
	if (node->origin.x == x && node->origin.y == y)
		return node;
	if (!node->left_top && !node->left_bottom && !node->right_top && !node->right_bottom)
		return node;
	if (x < node->origin.x && y > node->origin.y)
		return find(node->left_top, x, y);
	else if(x < node->origin.x && y < node->origin.y)
		return find(node->left_bottom, x, y);
	else if(x > node->origin.x && y > node->origin.y)
		return find(node->right_top, x, y);
	else if(x > node->origin.x && y < node->origin.y)
		return find(node->right_bottom, x, y);
	return node;
}

void Quadtree::calcTess(TreeNode* node) {
	if (!node->parent)
		return;
	TreeNode* top = find(root, node->origin.x, node->origin.y + 1 + node->height / 2.0f);
	TreeNode* right = find(root, node->origin.x + 1 + node->width / 2.0f, node->origin.y);
	TreeNode* bottom = find(root, node->origin.x, node->origin.y - 1 - node->height / 2.0f);
	TreeNode* left = find(root, node->origin.x - 1 - node->width / 2.0f, node->origin.y);
	if(left->width > node->width)
		node->tesselation_left = 2.0f;
	if(right->width > node->width)
		node->tesselation_right = 2.0f;
	if(top->width > node->width)
		node->tesselation_top = 2.0f;
	if(bottom->width > node->width)
		node->tesselation_bottom = 2.0f;
}

void Quadtree::divide(TreeNode* node) {
	node->left_top = create_node(node, glm::vec2(node->origin.x - node->width/4.0f, node->origin.y + node->height/4.0f));
	node->left_bottom = create_node(node, glm::vec2(node->origin.x - node->width/4.0f, node->origin.y - node->height/4.0f));
	node->right_top = create_node(node, glm::vec2(node->origin.x + node->width/4.0f, node->origin.y + node->height/4.0f));
	node->right_bottom = create_node(node, glm::vec2(node->origin.x + node->width/4.0f, node->origin.y - node->height/4.0f));

	if (isDivisible(node->left_top))
		divide(node->left_top);
	if (isDivisible(node->left_bottom))
		divide(node->left_bottom);
	if (isDivisible(node->right_top))
		divide(node->right_top);
	if (isDivisible(node->right_bottom))
		divide(node->right_bottom);
}

void Quadtree::renderNode(TreeNode* node) {
	calcTess(node);
	glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(node->width/2.0f, 1.0f, node->height/2.0f));
	glm::mat4 trans = glm::translate(glm::mat4(1.0f), glm::vec3(node->origin.x, 0.0f, node->origin.y));

	glm::mat4 model = trans * scale;
	glUniformMatrix4fv(glGetUniformLocation(shader->getID(), "model"), 1, GL_FALSE, glm::value_ptr(model));
	glUniform1f(glGetUniformLocation(shader->getID(), "tesselation_right"), node->tesselation_right);
	glUniform1f(glGetUniformLocation(shader->getID(), "tesselation_left"), node->tesselation_left);
	glUniform1f(glGetUniformLocation(shader->getID(), "tesselation_top"), node->tesselation_top);
	glUniform1f(glGetUniformLocation(shader->getID(), "tesselation_bottom"), node->tesselation_bottom);
	glBindVertexArray(VAO);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glPatchParameteri(GL_PATCH_VERTICES, 4);
	glDrawArrays(GL_PATCHES, 0, 4);
}

void Quadtree::render_rec(TreeNode* node) {
	if (!node->left_top && !node->left_bottom && !node->right_bottom && !node->right_top){
		renderNode(node);
	} else {
		render_rec(node->left_top);
		render_rec(node->right_top);
		render_rec(node->left_bottom);
		render_rec(node->right_bottom);
	}
}

void Quadtree::construct_tree() {
	current = root;
	nodes_count = 1;
	divide(root);
}

void Quadtree::render(const glm::vec3 _camera_position, const glm::mat4& view, const glm::mat4& proj) {
	shader->use();
	camera_position = _camera_position;
	int t_units[MAX_TEXTURES] = {2, 3, 4, 5};
	glUniformMatrix4fv(glGetUniformLocation(shader->getID(), "view"),
                     1, GL_FALSE, glm::value_ptr(view));
  	glUniformMatrix4fv(glGetUniformLocation(shader->getID(), "projection"),
                     1, GL_FALSE,
                     glm::value_ptr(proj));
  	glUniform1i(glGetUniformLocation(shader->getID(), "heightmap"), 0);
  	heightmap.activate(GL_TEXTURE0);
  	glUniform1i(glGetUniformLocation(shader->getID(), "splatmap"), 1);
  	splatmap.activate(GL_TEXTURE1);
  	glUniform1iv(glGetUniformLocation(shader->getID(), "textures"), MAX_TEXTURES, t_units);
  	for(size_t i = 0; i < MAX_TEXTURES; ++i) {
  		textures[i].activate(GL_TEXTURE2 + i);
  	}

	construct_tree();
	glEnable(GL_BLEND);
	render_rec(root);
	glDisable(GL_BLEND);
}

void Quadtree::set_wh(const float w, const float h) {
	root->width = w;
	root->height = h;
}

void Quadtree::load_textures() {
	splatmap = Texture("assets/terrain/alpha.png");

	std::string path = "assets/terrain/alphamap.txt";
	std::string line;
	std::ifstream texture_files(path);

	for(unsigned int i = 0; getline(texture_files, line); ++i) {
		if (line.length() > 0) {
			textures[i] = Texture(line);
		}
	}
}
