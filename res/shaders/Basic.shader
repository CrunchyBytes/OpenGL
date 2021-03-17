#shader vertex
#version 330 core

/* Leer de este vector de 4 elementos las posiciones */
layout(location = 0) in vec4 position;
/* Nuevos elementos que metimos al buffer 
 * Guarda los siguientes 2 elementos que lea, para cada uno de los v�rtices
 */
layout(location = 1) in vec2 texCoord;

/* Lo que lees en el Vertex, gu�rdalos en memoria, y lo va sacar en este elemento */
out vec2 v_TexCoord;

uniform mat4 u_M; // Model uniform
uniform mat4 u_V; // View uniform
uniform mat4 u_P; // Projection uniform

void main()
{
	/* Transform original object's position using the MVP uniform, and then the rotation uniform  */
	gl_Position = u_P * u_V * u_M * position;
	/* texCoord, obtenido del buffer--vector de 2 elementos--gu�rdalo en v_TexCoord que va de salida a la siguiente etapa de m�quina de estados/pipeline (Fragment Shader) */
	v_TexCoord = texCoord;
};

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

/* Recupera esto de la salida del vertex shader */
in vec2 v_TexCoord;

uniform vec4 u_Color;
/* Necesito designar cu�l es mi uniform: varaibles donde se guarda valor */
uniform sampler2D u_Texture;

void main()
{
	/* Se le asigna color a la textura. Lo que hayas le�do en el sahder, la parte del vertex, es lo que le asignar�s de acuerdo al valor del uniform <- Applications, al guardarlo en slot 0 */
	/* Interpreto textura como color -> pintar en la imagen despu�s del Draw Call, y corresponder lo que hayas encontrado en el buffer con el valor de u_Texture (uniform) */
	vec4 texColor = texture(u_Texture, v_TexCoord);
	
	/* El color del cuadro es reemplazado por el que se recibe por la textura */
	/* Si se multiplica por u_Color, �ste se combina con los colores obtenidos de la interpretaci�n de la textura. */
	color = texColor * u_Color;	
};