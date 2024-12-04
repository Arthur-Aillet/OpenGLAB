#include "Material.hpp"

Material::Material(glm::vec3 ka, glm::vec3 kd, glm::vec3 ks, float shiness): Kd(kd), Ka(ka), Ks(ks), Shiness(shiness) {
}
Material::Material() {
}