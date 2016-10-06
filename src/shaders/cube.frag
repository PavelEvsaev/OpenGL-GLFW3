#version 410 core

struct Material
{
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
}; 
 
struct SpotLight
{
    vec3 position;
    vec3 direction;

    float outerCutOff;
    float innerCutOff;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic; 
};

uniform SpotLight light;  
uniform Material material;
uniform vec3 viewPos;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

out vec4 color;

void main()
{
	// Ambient
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
    vec3 result = ambient;

    //cos(theta)
    vec3 lightDir = normalize(light.position - FragPos);
    float theta = max(dot(light.direction, -lightDir), 0.f);

    if (theta > light.outerCutOff)
    {
        //soft edges
        float e = (light.innerCutOff - light.outerCutOff);
        float i = (theta - light.outerCutOff) / e;

        //attenuation
        float distance = length(FragPos - light.position);
        float attenuation = 1 / ((light.constant) + (light.linear * distance) + (light.quadratic * distance * distance));

        // Diffuse 
        float diff = max(dot(Normal, lightDir), 0.0);
        vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));  
        
        // Specular
        vec3 viewDir = normalize(viewPos - FragPos);
        vec3 reflectDir = reflect(-lightDir, Normal);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
        vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));

        diffuse  *= i * attenuation;
        specular *= i * attenuation;

        result += diffuse + specular;
    }

    color = vec4(result, 1.0f);
}