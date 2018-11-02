#ifndef C_TERRAIN_QUADTREE
#define C_TERRAIN_QUADTREE
#include <glm/glm.hpp>
#include "../shaders/shader.h"

const unsigned int MAX_NODES = 1024;
const unsigned int MIN_NODE_WIDTH = 64;
const unsigned int MIN_NODE_HEIGHT = 64;

struct TreeNode {
	float width;
	float height;

	float tesselation_left;
	float tesselation_right;
	float tesselation_top;
	float tesselation_bottom;

	TreeNode* parent;
	TreeNode* left_top;
	TreeNode* left_bottom;
	TreeNode* right_top;
	TreeNode* right_bottom;

	glm::vec2 origin;
};

class Quadtree {
public:
	Quadtree(const glm::vec3& _camera_position, const float width, const float height);
	~Quadtree();
	void render(const glm::vec3 _camera_position, const glm::mat4& view, const glm::mat4& proj);
private:
	bool isDivisible(TreeNode* node);
	void divide(TreeNode* node);
	void calcTess(TreeNode* node);
	void renderNode(TreeNode* node);
	void render_rec(TreeNode* node);
	void construct_tree();
	TreeNode* create_node(TreeNode* p, const glm::vec2& origin);
	TreeNode* find(TreeNode* node, const glm::vec2& target);
	glm::vec3 camera_position;
	unsigned int nodes_count;
	TreeNode* nodes;
	TreeNode* root;
	TreeNode* current;
	Shader* shader;
	GLuint VAO, VBO;
};
#endif

