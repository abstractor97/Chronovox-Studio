#include "engine\gfx\gui\button\ButtonToggle.h"

CButtonToggle::CButtonToggle(std::string p_compName, std::string p_title, Vector2<Sint32> p_pos, Vector2<Sint32> p_size, Sint8 p_state, function p_func)
{
	m_compName = p_compName;
	m_title = p_title;
	m_pos = p_pos;
	m_size = p_size;
	m_colorTheme = m_colorThemes[ACTION];
	m_selected = p_state;

	m_texType = 0;
	m_function = p_func;

	m_soundClick.setSound(MBuffer::getInstance().getUnit("gui\\Click.wav"));
	m_soundHover.setSound(MBuffer::getInstance().getUnit("gui\\Hover.wav"));

	m_numValue = new Sint32(0);
}
CButtonToggle::CButtonToggle(std::string p_compName, Texture p_buttonTex, Vector2<Sint32> p_pos, Vector2<Sint32> p_size, Sint8 p_state, function p_func)
{
	m_compName = p_compName;
	m_title = "";
	m_pos = p_pos;
	m_size = p_size;
	m_colorTheme = m_colorThemes[ACTION];
	m_selected = p_state;

	m_function = p_func;

	m_soundClick.setSound(MBuffer::getInstance().getUnit("gui\\Click.wav"));
	m_soundHover.setSound(MBuffer::getInstance().getUnit("gui\\Hover.wav"));

	m_buttonTex[0] = p_buttonTex;
	m_texType = 1;

	m_numValue = new Sint32(0);
}
CButtonToggle::CButtonToggle(std::string p_compName, Texture p_activeTex, Texture p_inactiveTex, Vector2<Sint32> p_pos, Vector2<Sint32> p_size, Sint8 p_state, function p_func)
{
	m_compName = p_compName;
	m_title = "";
	m_pos = p_pos;
	m_size = p_size;
	m_colorTheme = m_colorThemes[ACTION];
	m_selected = p_state;

	m_function = p_func;

	m_soundClick.setSound(MBuffer::getInstance().getUnit("gui\\Click.wav"));
	m_soundHover.setSound(MBuffer::getInstance().getUnit("gui\\Hover.wav"));

	m_buttonTex[0] = p_activeTex;
	m_buttonTex[1] = p_inactiveTex;
	m_texType = 2;

	m_numValue = new Sint32(0);
}

void CButtonToggle::input(Sint8& p_interactFlags, Sint8* p_keyStates, Sint8* p_mouseStates, Vector2<Sint32> p_mousePos)
{
	p_mousePos = p_mousePos - m_pos;
	*m_numValue = 0;
	if((p_interactFlags & EVENT_MOUSEOVER) &&
		p_mousePos.x >= 0 && p_mousePos.x <= m_size.x &&
		p_mousePos.y >= 0 && p_mousePos.y <= m_size.y)
	{
		addTooltip();
		if(!m_hover)
		{
			m_soundHover.play2d();
			m_hover = true;
		}
		if(p_mouseStates[GLFW_MOUSE_BUTTON_LEFT] & MouseStates::MOUSE_PRESS)
		{
			*m_numValue = 1;
			m_selected = !m_selected;
			if(m_function != 0)
			{
				m_soundClick.play2d();
				m_function();
			}
		}
		p_interactFlags -= EVENT_MOUSEOVER;
	}
	else
	{
		resetTooltip();
	}
}
void CButtonToggle::update(GLfloat p_deltaUpdate)
{

}
void CButtonToggle::render()
{
	glPushMatrix();
	{
		Component::renderBack();
		Component::renderFill();
		glTranslatef(GLfloat(m_pos.x), GLfloat(m_pos.y), 0);
		if(m_texType == 0)
		{
			if(m_selected && m_texType != 2)
				m_colorTheme.m_active.useColor();
			else
			{
				if(m_hover)
					((m_colorTheme.m_active / 2 + m_colorTheme.m_fore / 2)).useColor();
				else
					m_colorTheme.m_fore.useColor();
			}
			glBegin(GL_QUADS);
			{
				glVertex2f(0, 0);
				glVertex2f(GLfloat(m_size.x), 0);
				glVertex2f(GLfloat(m_size.x), GLfloat(m_size.y));
				glVertex2f(0, GLfloat(m_size.y));
			}
			glEnd();
		}

		glTranslatef(GLfloat(m_size.x / 2), GLfloat(m_size.y / 2), 0);
		if(m_texType != 0)
		{
			if(m_texType == 1)
			{
				if(m_selected && m_texType != 2)
					m_colorTheme.m_active.useColor();
				else
				{
					if(m_hover)
						((m_colorTheme.m_active / 2 + m_colorTheme.m_fore / 2)).useColor();
					else
						m_colorTheme.m_fore.useColor();
				}
			}
			else
				glColor3f(1, 1, 1);
			if(m_selected != 0 || m_texType == 1)
				glBindTexture(GL_TEXTURE_2D, m_buttonTex[0].getId());
			else
				glBindTexture(GL_TEXTURE_2D, m_buttonTex[1].getId());
			glBegin(GL_QUADS);
			{
				glTexCoord2f(0, 1);
				if(m_selected != 0 || m_texType == 1) glVertex2f(-GLfloat(m_buttonTex[0].getSize().x) / 2, -GLfloat(m_buttonTex[0].getSize().y) / 2);
				else glVertex2f(-GLfloat(m_buttonTex[1].getSize().x) / 2, -GLfloat(m_buttonTex[1].getSize().y) / 2);
				glTexCoord2f(1, 1);
				if(m_selected != 0 || m_texType == 1) glVertex2f(GLfloat(m_buttonTex[0].getSize().x) / 2, -GLfloat(m_buttonTex[0].getSize().y) / 2);
				else glVertex2f(GLfloat(m_buttonTex[1].getSize().x) / 2, -GLfloat(m_buttonTex[1].getSize().y) / 2);
				glTexCoord2f(1, 0);
				if(m_selected != 0 || m_texType == 1) glVertex2f(GLfloat(m_buttonTex[0].getSize().x) / 2, GLfloat(m_buttonTex[0].getSize().y) / 2);
				else glVertex2f(GLfloat(m_buttonTex[1].getSize().x) / 2, GLfloat(m_buttonTex[1].getSize().y) / 2);
				glTexCoord2f(0, 0);
				if(m_selected != 0 || m_texType == 1) glVertex2f(-GLfloat(m_buttonTex[0].getSize().x) / 2, GLfloat(m_buttonTex[0].getSize().y) / 2);
				else glVertex2f(-GLfloat(m_buttonTex[1].getSize().x) / 2, GLfloat(m_buttonTex[1].getSize().y) / 2);
			}
			glEnd();
		}
		m_colorTheme.m_text.useColor();
		Font::getInstance().setAlignment(ALIGN_CENTER);
		Font::getInstance().print(m_title, 0, 0);
	}
	glPopMatrix();

	m_hover = false;
}