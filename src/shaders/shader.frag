#version 410 core

struct SpotLight
{
    vec3 position;
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float outerCutOff;
    float innerCutOff;

    float constant;
    float linear;
    float quadratic;
};

struct Material
{
    sampler2D diffuse1;
    sampler2D specular1;
    float     shininess;
}; 

uniform SpotLight light;
uniform Material material;
uniform vec3 viewPos;

in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;

out vec4 color;

vec3 calc_spot_light(SpotLight light, vec3 normal, vec3 viewDir);

void main()
{   
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 result_color = calc_spot_light(light, Normal, viewDir);
    color = vec4(result_color, 1.f);
}

vec3 calc_spot_light(SpotLight light, vec3 normal, vec3 viewDir)
{
    // Ambient
    vec3 ambient = light.ambient * vec3(texture(material.diffuse1, TexCoords));
    vec3 result = ambient;

    //cos(theta)
    vec3 lightDir = normalize(FragPos - light.position);
    float theta = max(dot(light.direction, lightDir), 0.f);

    if (theta > light.outerCutOff)
    {
        // Diffuse 
        float diff = max(dot(Normal, -lightDir), 0.0);
        
        // Specular
        vec3 reflectDir = reflect(lightDir, Normal);
        float spec = pow(max(dot(-viewDir, reflectDir), 0.0), material.shininess);

        //soft edges
        float epsilon   = light.innerCutOff - light.outerCutOff;
        float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);    
        
        //attenuation
        float distance = length(FragPos - light.position);
        float attenuation = 1 / ((light.constant) + (light.linear * distance) + (light.quadratic * distance * distance));

        vec3 diffuse  = intensity * attenuation * light.diffuse * diff * vec3(texture(material.diffuse1, TexCoords));
        vec3 specular = intensity * attenuation * light.specular * spec * vec3(texture(material.specular1, TexCoords));

        result += diffuse + specular;
    }

    return result;
}