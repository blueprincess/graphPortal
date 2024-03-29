/**
 * This file is in the public domain.
 * Contributors: Martin Kraus, Sylvain Beucler
 */
#version 120
varying vec4 position;  // position of the vertex (and fragment) in world space
varying vec3 varyingNormalDirection;  // surface normal vector in world space
uniform mat4 m, v, p;
uniform mat4 v_inv;

struct lightSource
{
  vec4 position;
  vec4 diffuse;
  vec4 specular;
  float invLinearAttenuation;
  float spotCutoff, spotExponent;
  vec3 spotDirection;
};
lightSource light0 = lightSource(
  vec4(0.0,  3.0,  0.0, 1.0),
  vec4(1.0,  1.0,  1.0, 1.0),
  vec4(1.0,  1.0,  1.0, 1.0),
  2.0,
  180.0, 0.0,
  vec3(0.0, 0.0, 0.0)
);
vec4 scene_ambient = vec4(0.2, 0.2, 0.2, 1.0);

struct material
{
  vec4 ambient;
  vec4 diffuse;
  vec4 specular;
  float shininess;
};
material frontMaterial = material(
  vec4(0.2, 0.2, 0.2, 1.0),
  vec4(1.0, 0.8, 0.8, 1.0),
  vec4(1.0, 1.0, 1.0, 1.0),
  5.0
);

void main()
{
  vec3 normalDirection = normalize(varyingNormalDirection);
  vec3 viewDirection = normalize(vec3(v_inv * vec4(0.0, 0.0, 0.0, 1.0) - position));
  vec3 lightDirection;
  float attenuation;
  
  // initialize total lighting with ambient lighting
  vec3 totalLighting = vec3(scene_ambient) * vec3(frontMaterial.ambient);
  
	  vec3 positionToLightSource = vec3(light0.position - position);
	  float distanceSqr = dot(positionToLightSource, positionToLightSource);
	  lightDirection = inversesqrt(distanceSqr) * positionToLightSource;
	  attenuation = light0.invLinearAttenuation * inversesqrt(distanceSqr);
      
      vec3 diffuseReflection = attenuation 
	* vec3(light0.diffuse) * vec3(frontMaterial.diffuse)
	* max(0.0, dot(normalDirection, lightDirection));
      
      vec3 specularReflection;
      if (dot(normalDirection, lightDirection) < 0.0) // light source on the wrong side?
	{
	  specularReflection = vec3(0.0, 0.0, 0.0); // no specular reflection
	}
      else // light source on the right side
	{
	  specularReflection = attenuation * vec3(light0.specular) * vec3(frontMaterial.specular)
	    * pow(max(0.0, dot(reflect(-lightDirection, normalDirection), viewDirection)), frontMaterial.shininess);
	}

      totalLighting = totalLighting + diffuseReflection + specularReflection;
  
  gl_FragColor = vec4(totalLighting, 1.0);
}
