#include "engine\editor\model\command\MMoveCommand.h"

MMoveCommand::MMoveCommand(Matrix* m, glm::vec3 p_from, glm::vec3 p_to) {
	m_commandType = "MoveCommand";
	m_matrix = m;
	from = p_from;
	to = p_to;
}
void MMoveCommand::terminate() {

}

void MMoveCommand::undo() {
	m_matrix->setPosition(from);
}
void MMoveCommand::redo() {
	m_matrix->setPosition(to);
}
