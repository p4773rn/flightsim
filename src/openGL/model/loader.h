#ifndef CLASS_LOADER_INTERFACE
#define CLASS_LOADER_INTERFACE
#include <string>

class Loader {
public:
	virtual void load(const std::string& filename) = 0;
	//virtual std::vector<Mesh> get_meshes();
	//virtual std::vector<Material> get_materials();
};
#endif
