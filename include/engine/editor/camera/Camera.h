#pragma once
#include "engine\utils\Utilities.h"
#include "engine\gfx\texture\MTexture.h"

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>

class Camera {
private:
	static glm::vec3 m_position, m_rotation;
	static GLfloat m_zoom, m_tarZoom, m_zoomSpeed;
	static GLfloat m_autoRotation;
	static Texture* m_skyTexture;
	static glm::mat4 m_projectionMatrix;
	static bool m_draggingRight, m_draggingMiddle;

	static void zoom(GLfloat p_scroll);
	static void turn(Vector2<Sint32> p_mouseMove);
	static void pan(Vector3<GLfloat> p_panDir);

public:
	static void init();
	static void reset();

	static void setProjectionMatrix(glm::mat4 p_projectionMatrix);

	static void resetZoom();
	static void addZoom(GLfloat p_zoom);
	static GLfloat getZoom() { return m_zoom; }

	static void resetAutoRotation();
	static void addAutoRotation(GLfloat p_speed);

	static void setPosition(glm::vec3 p_position);

	static glm::vec3 getFocus();
	static glm::vec3 getPosition();
	static glm::vec3 getRotation();
	static glm::vec3 getDirection();
	static glm::vec3 getMousePosition();
	static glm::vec3 getMouseDirection();

	static glm::mat4 getViewMatrix();
	static glm::mat4 getModelViewProjection();

	static void applyLightDirection();
	static void applyTransformation();

	static void input(Sint8 p_guiFlags);
	static void update(GLfloat p_deltaUpdate);
	static void renderSkybox();
	static void renderFocus();
};
