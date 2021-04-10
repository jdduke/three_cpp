#include "common.hpp"

#include <three/cameras/perspective_camera.hpp>
#include <three/core/geometry.hpp>
#include <three/lights/point_light.hpp>
#include <three/materials/mesh_basic_material.hpp>
#include <three/materials/mesh_phong_material.hpp>
#include <three/materials/mesh_lambert_material.hpp>
#include <three/objects/mesh.hpp>
#include <three/renderers/renderer_parameters.hpp>
#include <three/renderers/gl_renderer.hpp>
#include <three/scenes/fog.hpp>

#include <three/extras/geometries/sphere_geometry.hpp>

using namespace three;

Image createImage(int width = 256, int height = 256)
{
    const auto canvasSize = width * height * 3;
    std::vector<unsigned char> canvas;
    canvas.reserve(canvasSize);

    std::generate_n(std::back_inserter(canvas), canvasSize, [] {
        return static_cast<unsigned char>(Math::randomT<unsigned int>(0, 255));
    });

    return Image(std::move(canvas), width, height);
}

void test_memory(GLRenderer::Ptr renderer)
{

    auto running = true;
    sdl::addEventListener(SDL_KEYDOWN, [&](const sdl::Event&) {
        running = false;
    });
    sdl::addEventListener(SDL_QUIT, [&](const sdl::Event&) {
        running = false;
    });

    auto mouseX = 0.f, mouseY = 0.f;
    sdl::addEventListener(SDL_MOUSEMOTION, [&](const sdl::Event& event) {
        mouseX = 2.f * ((float)event.motion.x / renderer->width() - 0.5f);
        mouseY = 2.f * ((float)event.motion.y / renderer->height() - 0.5f);
    });

    //////////////////////////////////////////////////////////////////////////

    auto camera = PerspectiveCamera::create(
        60, (float)renderer->width() / renderer->height(), 1, 10000);
    camera->position.z = 200;

    auto scene = Scene::create();

    // Lights
    auto pointLight = PointLight::create(0xFFFFFF);
    pointLight->position = Vector3(10, 50, 130);
    scene->add(pointLight);

    anim::gameLoop(

        [&](float) -> bool {
            auto geometry = SphereGeometry::create(50,
                                                   Math::random() * 64,
                                                   Math::random() * 32);

            auto texture = Texture::create(TextureDesc(createImage(), THREE::RGBFormat));
            texture->needsUpdate = true;

            auto material = //MeshPhongMaterial::create(
                //MeshLambertMaterial::create(
                MeshBasicMaterial::create(
                    Material::Parameters().add("map", texture).add("wireframe", true));

            auto mesh = Mesh::create(geometry, material);

            scene->add(mesh);
            renderer->render(*scene, *camera);
            scene->remove(mesh);

            renderer->deallocateObject(*mesh);
            renderer->deallocateTexture(*texture);
            renderer->deallocateMaterial(*material);

            return running;
        },
        2000);
}

int main(int argc, char* argv[])
{

    auto onQuit = defer(sdl::quit);

    RendererParameters parameters;

    if (!sdl::init(parameters) || !glew::init(parameters))
    {
        return 0;
    }

    auto renderer = GLRenderer::create(parameters);
    if (!renderer)
    {
        return 0;
    }

    test_memory(renderer);

    return 0;
}