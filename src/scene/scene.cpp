#include "scene.hpp"

#include <iostream>
#include <algorithm>

#include <glm/ext.hpp>

#include <boost/format.hpp>
#include <boost/log/trivial.hpp>


//struct instance_sorter {
//    typedef std::tuple<qts::scene::instance_reference, qts::scene::object_reference> instance_ref_pair;
//
//    bool operator() (const instance_ref_pair& a, const instance_ref_pair& b) const {
//        return std::get<1>(a) < std::get<1>(b);
//    }
//};

qts::scene::scene()
	: frame_id(0)
    , font("/usr/share/fonts/truetype/ubuntu-font-family/UbuntuMono-R.ttf")
{


}

void qts::scene::render(const qts::camera& camera)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glColor3f(1.0f, 0.0f, 0.0f);

    glMatrixMode(GL_MODELVIEW);

    glm::mat4 MVM = glm::mat4_cast(-camera.rotation) * glm::translate(glm::mat4(1.0f), -camera.eye);

    glLoadMatrixf(glm::value_ptr(MVM));

    cgGLBindProgram(cg_vertex_program);
    cgGLEnableProfile(vertex_profile);

    CGparameter modelViewProj = cgGetNamedParameter(cg_vertex_program, "modelViewProj");
    CGparameter lightColor = cgGetNamedParameter(cg_vertex_program, "lightColor");
    CGparameter lightPosition = cgGetNamedParameter(cg_vertex_program, "lightPosition");
    CGparameter eyePosition = cgGetNamedParameter(cg_vertex_program, "eyePosition");
    CGparameter Ke = cgGetNamedParameter(cg_vertex_program, "Ke");
    CGparameter Ka = cgGetNamedParameter(cg_vertex_program, "Ka");
    CGparameter Kd = cgGetNamedParameter(cg_vertex_program, "Kd");
    CGparameter Ks = cgGetNamedParameter(cg_vertex_program, "Ks");

    glm::vec3 lp = glm::sin(frame_id / 100.0f) * glm::vec3(1200.0f, 0.0f, 0.0f) + glm::vec3(0.0f, 200.0f, 0.0f);
    static_objects[0].transform = glm::translate(glm::mat4(1.0f), lp);
    static_objects[0].transform = glm::scale(static_objects[0].transform, glm::vec3(10.0f, 10.0f, 10.0f));

    cgGLSetParameter3f(lightColor, 0.5f, 0.5f, 0.5f);
    cgGLSetParameter3f(lightPosition, lp.x, lp.y, lp.z);
    cgGLSetParameter3f(eyePosition, camera.eye.x, camera.eye.y, camera.eye.z);
    cgGLSetParameter3f(Ke, 0.0f, 0.0f, 0.0f);
    cgGLSetParameter3f(Ka, 0.1f, 0.1f, 0.1f);
    cgGLSetParameter3f(Kd, 0.5f, 0.5f, 0.5f);
    cgGLSetParameter3f(Ks, 1.0f, 1.0f, 1.0f);

    cgGLBindProgram(cg_fragment_program);
    cgGLEnableProfile(fragment_profile);

    glEnableClientState(GL_VERTEX_ARRAY);
    //glEnableClientState(GL_NORMAL_ARRAY);

    for (auto& object: static_objects) {
    	if (opengl_buffers.find(object.object_mesh.hash) == opengl_buffers.end()) {
    		BOOST_LOG_TRIVIAL(warning) << "No OpenGL buffers found for object with hash " << object.object_mesh.hash;

    		continue;
    	}

    	BOOST_LOG_TRIVIAL(warning) << opengl_buffers[object.object_mesh.hash].vertex_buffer;

		glBindBuffer(GL_ARRAY_BUFFER, opengl_buffers[object.object_mesh.hash].vertex_buffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, opengl_buffers[object.object_mesh.hash].triangle_buffer);

		glVertexPointer(3, GL_FLOAT, 0, nullptr);
    	//glNormalPointer(GL_FLOAT, sizeof(vertex_normal), reinterpret_cast<const GLvoid*>(object.object_mesh.vertex_normals.data()));

    	glPushMatrix();
    	glMultMatrixf(glm::value_ptr(object.transform));

    	cgGLSetStateMatrixParameter(modelViewProj, CG_GL_MODELVIEW_PROJECTION_MATRIX, CG_GL_MATRIX_IDENTITY);
        glDrawElements(GL_TRIANGLES, object.object_mesh.triangles.size(), GL_UNSIGNED_INT, nullptr);

        glPopMatrix();
    }

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    //glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);

    cgGLDisableProfile(vertex_profile);
    cgGLDisableProfile(fragment_profile);

    glm::vec3 prograd = glm::normalize((camera.forward * camera.rotation));

    font.Render((boost::format("Frame %1%") % frame_id++).str().c_str(), -1, FTPoint(50.0f, 60.0f, 0.0f));
    font.Render((boost::format("Pro %1% %2% %3%") % prograd.x % prograd.y % prograd.z).str().c_str(), -1, FTPoint(50.0f, 40.0f, 0.0f));
    font.Render((boost::format("Cam %1% %2% %3%") % camera.eye.x % camera.eye.y % camera.eye.z).str().c_str(), -1, FTPoint(50.0f, 20.0f, 0.0f));
}

void MyErrorCallback(void)
{

  const char* errorString = cgGetErrorString(cgGetError());

  BOOST_LOG_TRIVIAL(error) << "Cg error: " << errorString;

}

bool qts::scene::initialize() {
	glewInit();

    if (font.Error())
        return false;

    font.FaceSize(14);

    glm::mat4 projection_matrix = glm::perspective(56.25f, 16.0f / 10.0f, 0.1f, 10000.0f);

    glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    glCullFace(GL_BACK);
    glShadeModel(GL_SMOOTH);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glLoadMatrixf(glm::value_ptr(projection_matrix));

    cgSetErrorCallback(MyErrorCallback);
    cg_context = cgCreateContext();

    vertex_profile = cgGLGetLatestProfile(CG_GL_VERTEX);
    fragment_profile = cgGLGetLatestProfile(CG_GL_FRAGMENT);

    cgGLSetOptimalOptions(vertex_profile);
    cg_vertex_program = cgCreateProgramFromFile(cg_context, CG_SOURCE, "shaders/first.vert.cg", vertex_profile, "main", NULL);
    cgGLLoadProgram(cg_vertex_program);

    cgGLSetOptimalOptions(fragment_profile);
    cg_fragment_program = cgCreateProgramFromFile(cg_context, CG_SOURCE, "shaders/first.frag.cg", fragment_profile, "main", NULL);
    cgGLLoadProgram(cg_fragment_program);

    return true;
}

const qts::scene::object_reference qts::scene::append(static_object& object) {
	static_objects.push_back(std::move(object));

	if (opengl_buffers.find(object.object_mesh.hash) == opengl_buffers.end()) {
		GLuint vbo[2];
		glGenBuffers(1, vbo);

		glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
		glBufferData(GL_ARRAY_BUFFER,
				object.object_mesh.vertices.size() * sizeof(decltype(object.object_mesh.vertices)::value_type),
				object.object_mesh.vertices.data(),
				GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[1]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER,
				object.object_mesh.triangles.size() * sizeof(decltype(object.object_mesh.triangles)::value_type),
				object.object_mesh.triangles.data(),
				GL_STATIC_DRAW);

		opengl_buffers[object.object_mesh.hash].vertex_buffer = vbo[0];
		opengl_buffers[object.object_mesh.hash].triangle_buffer = vbo[1];
	}

	return qts::scene::object_reference(static_objects.size() - 1);
}

//qts::object_loader& qts::scene::get_object_loader() {
//    return *(object_loader_.get());
//}
//
//qts::scene::object_reference qts::scene::add_object(static_object object) {

//}
//
//qts::scene::instance_reference qts::scene::add_instance(qts::scene::object_reference object_ref) {
//    static_object_instances.push_back(std::make_tuple(instance_reference_cardinality, object_ref));
//
//    return qts::scene::instance_reference(instance_reference_cardinality++);
//}


