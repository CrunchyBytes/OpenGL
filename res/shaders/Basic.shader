#shader vertex
#version 330 core

/* Leer de este vector de 4 elementos las posiciones */
layout(location = 0) in vec4 position;
/* Nuevos elementos que metimos al buffer 
* Guarda los siguientes 2 elementos que lea, para cada uno de los vértices
 */
layout(location = 1) in vec2 texCoord;

/* Lo que lees en el Vertex, guárdalos en memoria, y lo va sacar en este elemento */
out vec2 v_TexCoord;

uniform mat4 u_MVP; // Model View Projection uniform
uniform mat4 u_MVR; // Rotation uniform

void main()
{
	/* Transform original object's position using the MVP uniform, and then the rotation uniform  */
	gl_Position = u_MVR * u_MVP * position;
	/* texCoord, obtenido del buffer--vector de 2 elementos--guárdalo en v_TexCoord que va de salida a la siguiente etapa de máquina de estados/pipeline (Fragment Shader)*/
	v_TexCoord = texCoord;
};

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

/* Recupera esto de la salida del vertex shader */
in vec2 v_TexCoord;

uniform vec4 u_Color;
/* Necesito designar cuál es mi uniform: varaibles donde se guarda valor */
uniform sampler2D u_Texture;

void main()
{
	/* Se le asigna color a la textura. Lo que hayas leído en el sahder, la parte del vertex, es lo que le asignarás de acuerdo al valor del uniform <- Applications, al guardarlo en slot 0*/
	/* Interpreto textura como color -> pintar en la imagen después del Draw Call, y corresponder lo que hayas encontrado en el buffer con el valor de u_Texture (uniform)*/
	vec4 texColor = texture(u_Texture, v_TexCoord);
	
	/* El color del cuadro es reemplazado por el que se recibe por la textura */
	/* Si se multiplica por u_Color, éste se combina con los colores obtenidos de la interpretación de la textura. */
	color = texColor * u_Color;	
};