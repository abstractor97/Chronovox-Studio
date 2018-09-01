#include "engine\utils\global\GScreen.h"
#include "engine\utils\global\event\GMouse.h"
#include "engine\utils\logger\Logger.h"

std::string GScreen::m_windowTitle = "";
std::string GScreen::m_appVersion = "";
float GScreen::m_fps = 0;
float GScreen::m_fov = 0;
float GScreen::m_deltaTime = 0;
Sint8 GScreen::m_exitting = 0;
bool GScreen::m_developer = false;

GLFWwindow *GScreen::m_window = 0;
Vector2<Sint32> GScreen::m_monitorSize = {};
Vector2<Sint32> GScreen::m_windowPos = {};
bool GScreen::m_maximized = false;
bool GScreen::m_iconified = false;
bool GScreen::m_focused = true;
Vector2<Sint32> GScreen::m_screenSize = {};
Vector2<Sint32> GScreen::m_smallScreen = {};
bool GScreen::m_draggingWindow = false;
Vector2<Sint32> GScreen::m_dragStart = {};
Vector2<Sint32> GScreen::m_dragDistance = {};
Vector2<Sint32> GScreen::m_initWindowSize = {};
Vector2<Sint32> GScreen::m_resizeMousePos = {};
bool GScreen::m_resizing = false;
bool GScreen::m_finishedResize = false;
bool GScreen::m_shadows = false;

GScreen::WindowCommand GScreen::m_windowCommand = NONE;

void GScreen::initWindow(GLFWwindow *p_window) {
	m_window = p_window;

	int x, y;
	glfwGetWindowPos(m_window, &x, &y);
	m_windowPos = Vector2<Sint32>(x, y);

	glfwGetWindowSize(m_window, &x, &y);
	m_smallScreen = m_screenSize = Vector2<Sint32>(x, y);
}

void GScreen::windowIconifyCallback(GLFWwindow* p_window, int iconified) {
	m_iconified = (iconified != 0);
}

void GScreen::windowFocusCallback(GLFWwindow* p_window, int focus) {
	if (focus == GLFW_TRUE) {
		m_focused = true;
	}
	else {
		m_focused = false;
	}
}

void GScreen::startWindowDrag() {
	m_draggingWindow = true;
	m_dragStart = GMouse::getMousePos();

}
bool GScreen::isDraggingWindow() {
	return m_draggingWindow;
}
void GScreen::endWindowDrag() {
	if (m_draggingWindow) {
		Vector2<Sint32> wpos, wsize;
		glfwGetWindowPos(GScreen::m_window, &wpos.x, &wpos.y);
		glfwGetWindowSize(GScreen::m_window, &wsize.x, &wsize.y);
		HMONITOR hMon = MonitorFromPoint({ wpos.x + wsize.x / 2, wpos.y + wsize.y / 2 }, MONITOR_DEFAULTTOPRIMARY);
		MONITORINFO info;
		info.cbSize = sizeof(MONITORINFO);
		GetMonitorInfo(hMon, &info);

		glfwGetWindowPos(m_window, &wpos.x, &wpos.y);
		if (wpos.y + GMouse::getMousePos().y <= info.rcWork.top) {
			GScreen::m_windowCommand = WindowCommand::RESIZE;
		}
		m_draggingWindow = false;
		m_dragDistance = {};
	}
}

void GScreen::startResizing() {
	m_initWindowSize = m_screenSize;
	m_resizeMousePos = GMouse::getMousePos();
	m_resizing = true;
}
void GScreen::stopResizing() {
	m_resizing = false;
}

void GScreen::updateWindow() {
	if (isDraggingWindow() && !isMaximized()) {
		m_dragDistance = m_dragStart - GMouse::getMousePos();
		m_windowPos = m_windowPos - m_dragDistance;
		glfwSetWindowPos(m_window, m_windowPos.x, m_windowPos.y);
		m_dragStart = GMouse::getMousePos() + m_dragDistance;
	}
	if (m_finishedResize) {
		m_finishedResize = false;
	}
	if (m_resizing) {
		Vector2<Sint32> _size = m_screenSize;
		m_screenSize.x = (Sint32)std::fmaxf(800.f, (GLfloat)(m_initWindowSize.x - (m_resizeMousePos.x - GMouse::getMousePos().x)));
		m_screenSize.y = (Sint32)std::fmaxf(700.f, (GLfloat)(m_initWindowSize.y - (m_resizeMousePos.y - GMouse::getMousePos().y)));
		if (!(_size == m_screenSize))
			m_finishedResize = true;
	}
}
