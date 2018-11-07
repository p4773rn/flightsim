#include "quadtree.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cmath>
#include <vector>
#include <utility>
#include <iostream>
#include <string>

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
		//std::cout << n->left_top->origin.x << "|" << n->left_top->origin.y << std::endl;
		//std::cout << n->left_bottom->origin.x << "|" << n->left_bottom->origin.y << std::endl;
		//std::cout << n->right_top->origin.x << "|" << n->right_top->origin.y << std::endl;
		//std::cout << n->right_bottom->origin.x << "|" << n->right_bottom->origin.y << std::endl;
		i = i+1;
		print_tree(n->left_top, i);
		print_tree(n->left_bottom, i);
		print_tree(n->right_top, i);
		print_tree(n->right_bottom, i);
	}
}

Quadtree::Quadtree(const glm::vec3& _camera_position, const float width, const float height) :
	camera_position{_camera_position}
{
	std::vector<std::pair<std::string, GLuint>> paths = {
      std::pair<std::string, GLuint>("shaders/terrain.vrtx", GL_VERTEX_SHADER),
      //std::pair<std::string, GLuint>("shaders/terrain.tec", GL_TESS_CONTROL_SHADER),
      //std::pair<std::string, GLuint>("shaders/terrain.tse", GL_TESS_EVALUATION_SHADER),
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

		-1.0f, 1.0f,
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
	//

	construct_tree();
	std::cout << nodes_count << std::endl;
	glm::mat4 model = glm::mat4(1.0f);
	//model = glm::translate(model, glm::vec3(125, 0.0f, 125));
	model = glm::scale(model, glm::vec3(200, 1.0f,200));
	model[3][0] = 125;
	model[3][2] = 125;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			std::cout << model[j][i] << " ";
		}
		std::cout << std::endl;
	}
	print_tree(root, 0);
}

Quadtree::~Quadtree() {
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
TreeNode* Quadtree::find(TreeNode* node, const glm::vec2& target) {
	//std::cout << "test" << std::endl;
	if (node->origin.x == target.x && node->origin.y == target.y)
		return node;
	if (!node->left_top && !node->left_bottom && !node->right_top && !node->right_bottom)
		return node;
	if (target.x < node->origin.x && target.y > node->origin.y)
		return find(node->left_top, target);
	else if(target.x < node->origin.x && target.y < node->origin.y)
		return find(node->left_bottom, target);
	else if(target.x > node->origin.x && target.y > node->origin.y)
		return find(node->right_top, target);
	else if(target.x > node->origin.x && target.y < node->origin.y)
		return find(node->right_bottom, target);
	return node;
}

void Quadtree::calcTess(TreeNode* node) {
	if (!node->parent)
		return;
	TreeNode* left = find(root, node->origin);
	TreeNode* right = find(root, node->origin);
	TreeNode* top = find(root, node->origin);
	TreeNode* bottom = find(root, node->origin);

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

	if (isDivisible(node->left_top)){
		//std::cout << tab << "left top" << std::endl;
		divide(node->left_top);
	}
	if (isDivisible(node->left_bottom)){
		//std::cout << tab << "left bottom" << std::endl;
		divide(node->left_bottom);
	}
	if (isDivisible(node->right_top)){
		//std::cout << tab << "right top" << std::endl;
		divide(node->right_top);
	}
	if (isDivisible(node->right_bottom)){
		//std::cout << tab << "right bottom" << std::endl;
		divide(node->right_bottom);
	}
}

void Quadtree::renderNode(TreeNode* node) {
	calcTess(node);
	glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(node->width/2.0f, 0.0f, node->height/2.0f));
	glm::mat4 trans = glm::translate(glm::mat4(1.0f), glm::vec3(node->origin.x, 0.0f, node->origin.y));

	glm::mat4 model = trans * scale;
	glUniformMatrix4fv(glGetUniformLocation(shader->getID(), "model"), 1, GL_FALSE, glm::value_ptr(model));
	glBindVertexArray(VAO);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//glPatchParameteri(GL_PATCH_VERTICES, 4);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Quadtree::render_rec(TreeNode* node) {
	if (!node->left_top && !node->left_bottom && !node->right_bottom && !node->right_top){
		renderNode(node);
	} else {
	  	glUniform3fv(glGetUniformLocation(shader->getID(), "color"),1, glm::value_ptr(glm::vec3(1.0f, 0.0f, 0.0f)));
		render_rec(node->left_top);
	  	glUniform3fv(glGetUniformLocation(shader->getID(), "color"),1, glm::value_ptr(glm::vec3(0.0f, 1.0f, 0.0f)));
		render_rec(node->right_top);
	  	glUniform3fv(glGetUniformLocation(shader->getID(), "color"),1, glm::value_ptr(glm::vec3(1.0f, 1.0f, 0.0f)));
		render_rec(node->left_bottom);
	  	glUniform3fv(glGetUniformLocation(shader->getID(), "color"),1, glm::value_ptr(glm::vec3(0.0f, 0.0f, 1.0f)));
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
	glUniformMatrix4fv(glGetUniformLocation(shader->getID(), "view"),
                     1, GL_FALSE, glm::value_ptr(view));
  	glUniformMatrix4fv(glGetUniformLocation(shader->getID(), "projection"),
                     1, GL_FALSE,
                     glm::value_ptr(proj));
	construct_tree();
	render_rec(root);
}
