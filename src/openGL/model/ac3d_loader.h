#ifndef CLASS_AC3D_LOADER
#define CLASS_AC3D_LOADER
#include "loader.h"
#include "model.h"
#include <istream>
/*
Assume that each object in model uses on material.
Assume that each surface has at least 3 vertices
*/

class AC3D_loader : public Loader {
private:
	void load(const std::string& fname) override;
	Object load_object(std::istream& inp, const std::string& fname);
public:
	AC3D_loader(const std::string& fname);
	std::vector<Texture> textures;
	std::vector<Material> materials;
	std::vector<Object> objects;
};
#endif
