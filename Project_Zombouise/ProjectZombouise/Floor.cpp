#include "pch.h"
#include "Floor.h"

Floor::Floor(int token, FloorRenderable* renderable)
	: m_Type(token), m_pRenderable(renderable)
{
}
