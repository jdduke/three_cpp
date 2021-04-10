#include "common.hpp"

#include <three/cameras/perspective_camera.hpp>
#include <three/core/geometry.hpp>
#include <three/lights/point_light.hpp>
#include <three/materials/line_basic_material.hpp>
#include <three/objects/line.hpp>
#include <three/renderers/renderer_parameters.hpp>
#include <three/renderers/gl_renderer.hpp>
#include <three/scenes/fog.hpp>

#include <three/extras/utils/font.hpp>

#include <array>

using namespace three;

template <typename T>
inline void concat(std::vector<T>& a, const std::vector<T>& b)
{
    a.insert(a.end(), b.begin(), b.end());
}

std::vector<Vector3> hilbert3D(Vector3 center,
                               float side,
                               int iterations,
                               int v0, int v1,
                               int v2, int v3,
                               int v4, int v5,
                               int v6, int v7)
{

    const auto half = side / 2;

    const std::array<Vector3, 8> vec_s = {
        Vector3(center.x - half, center.y + half, center.z - half),
        Vector3(center.x - half, center.y + half, center.z + half),
        Vector3(center.x - half, center.y - half, center.z + half),
        Vector3(center.x - half, center.y - half, center.z - half),
        Vector3(center.x + half, center.y - half, center.z - half),
        Vector3(center.x + half, center.y - half, center.z + half),
        Vector3(center.x + half, center.y + half, center.z + half),
        Vector3(center.x + half, center.y + half, center.z - half)
    };

    const std::array<Vector3, 8> vec = {
        vec_s[v0], vec_s[v1], vec_s[v2], vec_s[v3], vec_s[v4], vec_s[v5], vec_s[v6], vec_s[v7]
    };

    if (--iterations >= 0)
    {

        std::vector<Vector3> tmp;

        concat(tmp, hilbert3D(vec[0], half, iterations, v0, v3, v4, v7, v6, v5, v2, v1));
        concat(tmp, hilbert3D(vec[1], half, iterations, v0, v7, v6, v1, v2, v5, v4, v3));
        concat(tmp, hilbert3D(vec[2], half, iterations, v0, v7, v6, v1, v2, v5, v4, v3));
        concat(tmp, hilbert3D(vec[3], half, iterations, v2, v3, v0, v1, v6, v7, v4, v5));
        concat(tmp, hilbert3D(vec[4], half, iterations, v2, v3, v0, v1, v6, v7, v4, v5));
        concat(tmp, hilbert3D(vec[5], half, iterations, v4, v3, v2, v5, v6, v1, v0, v7));
        concat(tmp, hilbert3D(vec[6], half, iterations, v4, v3, v2, v5, v6, v1, v0, v7));
        concat(tmp, hilbert3D(vec[7], half, iterations, v6, v5, v2, v1, v0, v3, v4, v7));

        return tmp;
    }

    return std::vector<Vector3>(vec.begin(), vec.end());
}


void lines_cubes(GLRenderer::Ptr renderer)
{

    auto scene = Scene::create();

    auto camera = PerspectiveCamera::create(
        33, (float)renderer->width() / renderer->height(), 1, 10000);
    camera->position.z = 700;

    auto geometry = Geometry::create();
    geometry->vertices = hilbert3D(Vector3(0, 0, 0), 200.0, 4, 0, 1, 2, 3, 4, 5, 6, 7);

    /////////////////////////////////////////////////////////////////////////

    auto addLine = [&scene](Vector3 pos, float scale, Geometry::Ptr geometry, Material::Ptr material) {
        auto line = Line::create(geometry, material);
        line->scale.x = line->scale.y = line->scale.z = scale;
        line->position = pos;
        scene->add(line);
    };

    const auto scale = 0.15f, d = 125.f;
    const auto c1 = Color(0x553300), c2 = Color(0x555555), c3 = Color(0x992800);
    auto m1 = LineBasicMaterial::create(Material::Parameters()
                                            .add("color", c1)
                                            .add("opacity", 0.5f)
                                            .add("blending", THREE::AdditiveBlending)
                                            .add("transparent", true));
    auto m2 = LineBasicMaterial::create(Material::Parameters()
                                            .add("color", c2)
                                            .add("opacity", 0.5f)
                                            .add("blending", THREE::AdditiveBlending)
                                            .add("transparent", true));
    auto m3 = LineBasicMaterial::create(Material::Parameters()
                                            .add("color", c3)
                                            .add("opacity", 0.5f)
                                            .add("blending", THREE::AdditiveBlending)
                                            .add("transparent", true));

    addLine(Vector3(0, 0, 0), scale, geometry, m3);
    addLine(Vector3(d, 0, 0), scale, geometry, m2);
    addLine(Vector3(-d, 0, 0), scale, geometry, m2);
    addLine(Vector3(0, d, 0), scale, geometry, m2);
    addLine(Vector3(d, d, 0), scale, geometry, m2);
    addLine(Vector3(-d, d, 0), scale, geometry, m2);
    addLine(Vector3(0, -d, 0), scale, geometry, m2);
    addLine(Vector3(d, -d, 0), scale, geometry, m2);
    addLine(Vector3(-d, -d, 0), scale, geometry, m2);
    addLine(Vector3(d * 2, 0, 0), scale, geometry, m1);
    addLine(Vector3(-d * 2, 0, 0), scale, geometry, m1);
    addLine(Vector3(d * 2, d, 0), scale, geometry, m1);
    addLine(Vector3(-d * 2, d, 0), scale, geometry, m1);
    addLine(Vector3(d * 2, -d, 0), scale, geometry, m1);
    addLine(Vector3(-d * 2, -d, 0), scale, geometry, m1);

    //////////////////////////////////////////////////////////////////////////

    auto running = true, renderStats = true;
    sdl::addEventListener(SDL_KEYDOWN, [&](const sdl::Event& e) {
        switch (e.key.keysym.sym)
        {
        case SDLK_q:
        case SDLK_ESCAPE:
            running = false;
            break;
        default:
            renderStats = !renderStats;
            break;
        };
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

    stats::Stats stats(*renderer);
    auto time = 0.f;

    anim::gameLoop(

        [&](float dt) -> bool {
            time += dt;

            camera->position.x += (-500.f * mouseX - camera->position.x) * 3 * dt;
            camera->position.y += (500.f * mouseY + 200 - camera->position.y) * 3 * dt;
            camera->lookAt(scene->position);

            for (size_t i = 0; i < scene->children.size(); i++)
            {
                if (scene->children[i]->type() == THREE::Line)
                    scene->children[i]->rotation.y = time * (i % 2 ? 1 : -1);
            }

            renderer->render(*scene, *camera);

            stats.update(dt, renderStats);

            return running;
        }

    );
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

    lines_cubes(renderer);

    return 0;
}