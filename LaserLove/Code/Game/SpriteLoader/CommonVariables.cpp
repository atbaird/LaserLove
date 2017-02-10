#include "Game/SpriteLoader/CommonVariables.hpp"

//Variables
ShaderProgram* CommonVariables::s_collisionRadiusShaderProgram = nullptr;
const size_t CommonVariables::s_backgroundLayer = 0;
const size_t CommonVariables::s_explosionLayer = 2;
const size_t CommonVariables::s_playerLayer = 3;
const size_t CommonVariables::s_enemyLayer = 1;
const size_t CommonVariables::s_bulletLayer = 4;
const size_t CommonVariables::s_textLayer = 5;

void CommonVariables::SetCollisionShaderProgram(ShaderProgram* prog)
{
	s_collisionRadiusShaderProgram = nullptr;
	s_collisionRadiusShaderProgram = prog;
}
ShaderProgram* CommonVariables::GetCollisionShaderProgram()
{
	return s_collisionRadiusShaderProgram;
}