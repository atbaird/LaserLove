#pragma once

#ifndef COMMONVARIABLES_HPP
#define COMMONVARIABLES_HPP

class ShaderProgram;

class CommonVariables
{
private:
	static ShaderProgram* s_collisionRadiusShaderProgram;
public:
	static const size_t s_backgroundLayer;
	static const size_t s_explosionLayer;
	static const size_t s_playerLayer;
	static const size_t s_enemyLayer;
	static const size_t s_bulletLayer;
	static const size_t s_textLayer;

	static void SetCollisionShaderProgram(ShaderProgram* prog);
	static ShaderProgram* GetCollisionShaderProgram();
};
#endif