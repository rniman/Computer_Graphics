#version 330
//out_Color: ���ؽ� ���̴����� �Է¹޴� ���� ��
//FragColor: ����� ������ ������ ������ ���۷� ���� ��. 

//���ؽ� ���̴����Լ� ���� ����
in vec3 out_Normal;
in vec3 FragPos;

//���� ���
out vec4 FragColor;

uniform vec3 objectColor;                                                   //--- ���� ���α׷����� ������ ��ü�� ���� 
uniform vec3 lightColor;													//--- ���� ���α׷����� ������ ���� ����
uniform vec3 lightPos;
uniform vec3 cameraEye;
uniform float ambientLight;

void main(void) 
{
	vec3 Normal = out_Normal;

	//�ں��Ʈ (�ֺ�����(��������))
	//float ambientLight = 0.5;                                               //--- �ֺ� ���� ���: 0.0 �� ambientLight �� 1.0 
	vec3 ambient = ambientLight * lightColor;							    //--- �ֺ� ������

	//��ǻ�� (��� �ݻ� ����)
	vec3 normalVector = normalize(Normal);							// �븻�� ����ȭ

	vec3 lightDir = normalize(lightPos - FragPos);					// ǥ��� ���� ��ġ�� ���� ���� ����	
	
	float diffuseLight = max(dot(normalVector, lightDir), 0.0);		// N�� L�� ���� ������ ���� ���� (���� ���� ���� �� ���� �Ѵ�.) 
	vec3 diffuse = diffuseLight * lightColor;						// ����ݻ������� = ����ݻ簪 * ��������
	
	//����ŧ�� (�ſ� �ݻ� ����)
	int shininess = 256;											// ����(�Ų�����) ���
	vec3 viewDir = normalize(cameraEye - FragPos);					// ������ ���� ����
	vec3 reflectDir = reflect(-lightDir, normalVector);				// �ݻ� ����: reflect �Լ� - �Ի� ������ �ݻ� ���� ��� 
	float specularLight = max(dot(viewDir, reflectDir), 0.0);		// ���� ����� �ݻ� ������ ���� ������ ���� ����
	specularLight = pow(specularLight, shininess);					// shininess ������ ���̶���Ʈ ǥ��
	vec3 specular = specularLight * lightColor;						// �ſ� �ݻ� ������

	vec3 result = (ambient + diffuse + specular) * objectColor;		// ��ü�� ���� �ֺ��������� ���Ͽ� ���� ��ü ���� ����

	FragColor = vec4 (result , 1.0);
}