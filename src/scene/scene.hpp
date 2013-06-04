#ifndef SCENE_HPP
#define SCENE_HPP

#include <memory>
#include <vector>
#include <map>
#include <FTGL/ftgl.h>

#include <GL/glew.h>
#include <GL/gl.h>

#include <Cg/cg.h>
#include <Cg/cgGL.h>

#include "static_object.hpp"
#include "camera.hpp"

namespace qts {
class scene
{
public:
    typedef long long object_reference;

    scene();

    void render(const qts::camera& camera);
    bool initialize();

    const object_reference append(static_object&);

private:
    std::vector<static_object> static_objects;
    //std::vector<std::tuple<instance_reference, object_reference>> static_object_instances;

    //uint instance_reference_cardinality = 0;

    uint frame_id = 0;

    FTGLPixmapFont font;

    CGcontext cg_context;
    CGprogram cg_vertex_program;
    CGprogram cg_fragment_program;

    CGprofile vertex_profile;
    CGprofile fragment_profile;

	struct opengl_buffers_t {
		GLuint vertex_buffer;
		GLuint triangle_buffer;
	};

    std::map<size_t, opengl_buffers_t>  opengl_buffers;
};

}

#endif // SCENE_HPP
