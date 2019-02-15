#pragma once
#include "engine\utils\Utilities.h"
#include "engine\gfx\texture\MTexture.h"
#include "engine\gfx\mesh\MMesh.h"
#include "camera\Camera.h"

class TEMode {
protected:
	std::string m_directory;
	std::string m_name;

	virtual void inputEditor(Sint8 p_guiFlags);
	virtual void updateEditor(GLfloat p_deltaUpdate);
	virtual void renderEditor();
	virtual void renderEditorShadow();
public:
	TEMode();

	virtual bool hasChanged();
	virtual void setChanged(bool p_changed);

	void setPath(std::string p_path);
	std::string getName();
	std::string getDirectory();
	std::string getPath();

	void input(Sint8 p_guiFlags);
	void update(GLfloat p_deltaUpdate);
	void render();
	void renderShadow();

	virtual void fileNew();
	virtual void fileOpen();
	virtual bool fileSave();
	virtual bool fileSaveAs();
	virtual void fileExit();

	virtual void editUndo();
	virtual void editRedo();
};
