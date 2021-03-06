#include "engine\utils\math\ModelMath.h"
#include "engine\editor\camera\Camera.h"

bool ModelMath::castRayMatrix(glm::vec3 p_start, glm::vec3 p_direction, Matrix* p_matrix, GLfloat &p_near, GLfloat &p_far) {
	GLfloat _c = 0.00001f;
	GLfloat _mNear, _mFar, _vNear, _vFar;
	GLfloat _iNear = p_near, _iFar = p_far;
	Sint8 _side = 0;
	glm::ivec3 m_selectedVoxel, m_selectedVoxelOffset;
	_mNear = _iNear;
	_mFar = _iFar;
	Math::castRay3d(p_start, p_direction, p_matrix->getPos(), p_matrix->getSize(), _mNear, _mFar, _side);
	if (_mNear < 1) {
		_vNear = _mNear, _vFar = _mFar;
		castRayVoxel(p_start, p_direction, p_matrix, _vNear, _vFar, m_selectedVoxel, _side, m_selectedVoxelOffset);
		p_near = _mNear + ((_mNear - _mFar) * _vNear);
		p_far = _mFar;
	}
	return (p_near <= 1);
}

bool ModelMath::castRayMatrices(glm::vec3 p_start, glm::vec3 p_direction, std::vector<Matrix*> p_matrices, Sint16 &p_selectedMatrix, GLfloat &p_near, GLfloat &p_far) {
	// Small correction value
	GLfloat _c = 0.00001f;
	// Matrix, Voxel, Current
	GLfloat _mNear, _mFar, _vNear, _vFar, _cNear = 1 - _c, _cFar = 0;
	GLfloat _near, _far;
	// Store initial values of p_near and p_far
	GLfloat _iNear = p_near, _iFar = p_far;
	Sint8 _side = 0;
	Sint32 _cMatrix = -1;
	glm::ivec3 m_selectedVoxel, m_selectedVoxelOffset;
	for (Uint16 i = 0; i < p_matrices.size(); i++) {
		_mNear = _iNear;
		_mFar = _iFar;
		Math::castRay3d(p_start, p_direction, p_matrices[i]->getPos(), p_matrices[i]->getSize(), _mNear, _mFar, _side);
		if (_mNear < _cNear) {
			_vNear = _mNear, _vFar = _mFar;
			castRayVoxel(p_start, p_direction, p_matrices[i], _vNear, _vFar, m_selectedVoxel, _side, m_selectedVoxelOffset);
			if (_vFar == 1) _near = 1.f - _c;
			else _near = _mNear + ((_mFar - _mNear) * _vNear);
			_far = _mFar;
			if (_near < _cNear + _c) {
				_cMatrix = p_matrices[i]->getId();
				_cNear = _near + _c;
				_cFar = _far;
				p_near = _mNear + ((_mNear - _mFar) * _vNear);
				p_far = _cFar;
			}
		}
	}
	p_selectedMatrix = _cMatrix;
	return (p_selectedMatrix != -1);
}

bool ModelMath::castRayVoxel(glm::vec3 p_start, glm::vec3 p_direction, Matrix *p_matrix, GLfloat &p_near, GLfloat &p_far, glm::ivec3 &p_selectedVoxel, Sint8 &p_side, glm::ivec3 &p_selectedVoxelOffset) {
	GLfloat _near = 0, _far = 1;
	GLfloat _cNear = 0, _cFar = 1;
	Sint32 _close = -1;
	Vector2<GLfloat> _closest = { 1, 1 };
	glm::ivec3 _size = p_matrix->getSize(), _pos = p_matrix->getPos();
	p_side = 0;

	p_selectedVoxel = p_selectedVoxelOffset = { -1, -1, -1 };

	p_near -= 0.000001f;
	p_far += 0.000001f;
	p_start = p_start - p_matrix->getPos() + p_direction * p_near;
	p_direction = p_direction * (p_far - p_near);

	Math::castRay3d(p_start, p_direction, glm::floor(p_start), { 1, 1, 1 }, _cNear, _cFar, p_side);
	glm::vec3 _floor = glm::floor(p_start);
	int i = 500;
	do {
		if (_floor.x >= 0 && _floor.y >= 0 && _floor.z >= 0 &&
			_floor.x < _size.x && _floor.y < _size.y && _floor.z < _size.z) {
			if (p_matrix->getVoxel(_floor).interactionType != 0) {
				p_near = _near;
				p_far = _near;
				p_selectedVoxelOffset = p_selectedVoxel;
				p_selectedVoxel = _floor;
				return true;
			}
		}
		p_selectedVoxel = _floor;
		_near = _cFar + 0.00001f;
		_cNear = 0, _cFar = 1;
		Math::castRay3d(p_start, p_direction, glm::floor(p_start + p_direction * (_near)), { 1, 1, 1 }, _cNear, _cFar, p_side);
		_floor = glm::floor(p_start + p_direction * _near);
		i--;
	} while (_cNear < 1 && _near < _far && i > 0);
	p_selectedVoxelOffset = p_selectedVoxel;
	p_selectedVoxel = _floor;
	switch (p_side) {
	case FACE_NORTH:	p_selectedVoxelOffset.x++; break;
	case FACE_SOUTH:	p_selectedVoxelOffset.x--; break;
	case FACE_TOP:		p_selectedVoxelOffset.y++; break;
	case FACE_BOTTOM:	p_selectedVoxelOffset.y--; break;
	case FACE_EAST:		p_selectedVoxelOffset.z++; break;
	case FACE_WEST:		p_selectedVoxelOffset.z--; break;
	}
	p_near = p_far = 1;
	return false;
}
bool ModelMath::castRayScale(glm::vec3 p_start, glm::vec3 p_direction, Matrix* p_matrix, GLfloat &p_near, GLfloat &p_far, glm::vec3 &p_scalePos, Sint8 &p_scale) {
	if (!GMouse::mouseDown(GLFW_MOUSE_BUTTON_LEFT)) {
		GLfloat _near = 0, _far = 1;
		Sint8 _side = 0;
		Sint32 _close = -1;
		Vector2<GLfloat> _closest = { 1, 1 };
		p_scale = 0;
		GLfloat size = 0.2f;
		GLfloat length = 2.f;

		glm::vec3 s = glm::vec3(p_matrix->getSize()) / glm::vec3(2);
		glm::vec3 _offset = p_matrix->getPos() + s;

		glm::vec3 dist = _offset - Camera::getPosition();
		GLfloat dlen = sqrt(dist.x * dist.x + dist.y * dist.y + dist.z * dist.z) / 25.f + 1;
		glm::vec3 scalar = glm::vec3(dlen, dlen, dlen);
		size = size * scalar.x;
		length = length * scalar.x;

		Math::castRay3d(p_start, p_direction, _offset + glm::vec3{ -(s.x + length), -size, -size }, { length, size * 2, size * 2 }, _near, _far, _side);
		if (_near < _closest.x) {
			p_scale = FACE_SOUTH;
			_closest = { _near, _far };
		}
		_near = 0;
		_far = 1;
		Math::castRay3d(p_start, p_direction, _offset + glm::vec3{ (s.x), -size, -size }, { length, size * 2, size * 2 }, _near, _far, _side);
		if (_near < _closest.x) {
			p_scale = FACE_NORTH;
			_closest = { _near, _far };
		}
		_near = 0;
		_far = 1;
		Math::castRay3d(p_start, p_direction, _offset + glm::vec3{ -size, -(s.y + length), -size }, { size * 2, length, size * 2 }, _near, _far, _side);
		if (_near < _closest.x) {
			p_scale = FACE_BOTTOM;
			_closest = { _near, _far };
		}
		_near = 0;
		_far = 1;
		Math::castRay3d(p_start, p_direction, _offset + glm::vec3{ -size, (s.y), -size }, { size * 2, length, size * 2 }, _near, _far, _side);
		if (_near < _closest.x) {
			p_scale = FACE_TOP;
			_closest = { _near, _far };
		}
		_near = 0;
		_far = 1;
		Math::castRay3d(p_start, p_direction, _offset + glm::vec3{ -size, -size, -(s.z + length) }, { size * 2, size * 2, length }, _near, _far, _side);
		if (_near < _closest.x) {
			p_scale = FACE_WEST;
			_closest = { _near, _far };
		}
		_near = 0;
		_far = 1;
		Math::castRay3d(p_start, p_direction, _offset + glm::vec3{ -size, -size, (s.z) }, { size * 2, size * 2, length }, _near, _far, _side);
		if (_near < _closest.x) {
			p_scale = FACE_EAST;
			_closest = { _near, _far };
		}

		if (p_scale != 0) {
			p_scalePos = p_start + p_direction * _closest.x;
		}
		return (p_scale != 0);
	}
	return false;
}

bool ModelMath::castRayVoxelPlane(glm::vec3 p_start, glm::vec3 p_direction, Matrix *p_matrix, GLfloat &p_near, GLfloat &p_far, glm::ivec3 p_planePoint, Sint8 p_side, glm::ivec3 &p_selectedVoxel, glm::ivec3 &p_selectedVoxelOffset) {
	GLfloat _near = 0, _far = 1;
	GLfloat _cNear = 0, _cFar = 1;
	Sint32 _close = -1;
	Vector2<GLfloat> _closest = { 1, 1 };
	glm::ivec3 _size = p_matrix->getSize(), _pos = p_matrix->getPos();
	Sint8 _side = 0;

	p_selectedVoxel = p_selectedVoxelOffset = { -1, -1, -1 };

	p_near -= 0.000001f;
	p_far += 0.000001f;
	p_start = p_start - p_matrix->getPos() + p_direction * p_near;
	p_direction = p_direction * (p_far - p_near);

	Math::castRay3d(p_start, p_direction, glm::floor(p_start), { 1, 1, 1 }, _cNear, _cFar, _side);
	glm::vec3 _floor = glm::floor(p_start);
	int i = 1000;
	do {
		if (_floor.x >= 0 && _floor.y >= 0 && _floor.z >= 0
			&& _floor.x < _size.x && _floor.y < _size.y && _floor.z < _size.z
			&& p_side == _side) {
			switch (p_side) {
			case FACE_NORTH:
			case FACE_SOUTH:
				if (_floor.x == p_planePoint.x) {
					p_near = _near;
					p_far = _near;
					p_selectedVoxelOffset = p_selectedVoxel;
					p_selectedVoxel = _floor;
					return true;
				}
				break;
			case FACE_TOP:
			case FACE_BOTTOM:
				if (_floor.y == p_planePoint.y) {
					p_near = _near;
					p_far = _near;
					p_selectedVoxelOffset = p_selectedVoxel;
					p_selectedVoxel = _floor;
					return true;
				}
				break;
			case FACE_WEST:
			case FACE_EAST:
				if (_floor.z == p_planePoint.z) {
					p_near = _near;
					p_far = _near;
					p_selectedVoxelOffset = p_selectedVoxel;
					p_selectedVoxel = _floor;
					return true;
				}
				break;
			}
		}
		p_selectedVoxel = _floor;
		_near = _cFar + 0.00001f;
		_cNear = 0, _cFar = 1;
		Math::castRay3d(p_start, p_direction, glm::floor(p_start + p_direction * (_near)), { 1, 1, 1 }, _cNear, _cFar, _side);
		_floor = glm::floor(p_start + p_direction * _near);
		i--;
	} while (_cNear < 1 && _near < _far && i > 0);
	p_selectedVoxel = p_selectedVoxelOffset = { -1, -1, -1 };
	return false;
}
