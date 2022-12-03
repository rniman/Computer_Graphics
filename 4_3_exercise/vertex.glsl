#version 330 core
layout (location = 0) in vec3 vPos; //--- ���� ���α׷����� �޾ƿ� ���� ��ǥ��
layout (location = 1) in vec3 vNormal;

uniform mat4 modelTransform;		//--- �𵨸� ��ȯ ���: uniform ������ ����
uniform mat4 viewTransform;
uniform mat4 projectionTransform;

out vec3 out_Normal;
out vec3 FragPos;

void main()
{
	gl_Position = projectionTransform * viewTransform * modelTransform * vec4(vPos, 1.0); //--- ��ǥ���� modelTransform ��ȯ�� �����Ѵ�.

	FragPos = vec3(modelTransform * vec4(vPos, 1.0));

	out_Normal = vec3(modelTransform * vec4(vNormal, 0.0));
}