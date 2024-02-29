# scop

## keys
| key | action |
| --- | ------ |
| ESC | close the window |
| F1 | change the display mode between wire and fill |
| F2 | change the texture mode between color and texture |
| WASD | movements |
| Space/Left Shift | up and down |
| YU | rotate object on the X axis |
| HJ | rotate object on the Y axis |
| NM | rotate object on the Z axis |
| scroll button | zoom/dezoom |

## declaration of an object file (.obj)
| symbol | meaning | declaration |
| ------ | ------- | ----------- |
| # | commentary | #[commentary] |
| v | vertex | v xyz[w] |
| f | face | f [vertex index, at least 3] |
| s | smooth shading | s [on/off/0/1]
| mtllib | material library | mtllib [path to .mtl file] |
| usemtl | use material | usemtl [material name] |

### for bonus
| symbol | meaning | declaration |
| ------ | ------- | ----------- |
| vt | vertex texture | vt uv[w] |
| f | face | f [vertex index/texture vertex index, at least 3] |

smooth shading can be declared but won't be take into account

## declaration of a material file (.mtl)
| symbol | meaning | declaration |
| ------ | ------- | ----------- |
| # | commentary | #[commentary] |
| Ns | Specular Exponent | Ns [float value 0-100] |
| Ka | Ambiant color | Ka [R G B float value 0-1]
| Kd | diffuse color | Kd [R G B float value 0-1]
| Ks | specular color | Ks [R G B float value 0-1]
| Ni | refraction index | Ni [float value 0-10] |
| d | opacity | d [float value 0-1] |
| illum | illumination model | illum [integer value 0-10]

only Ka is take into account to define the color if the texture isn't display

## useful links

openGL documentation: https://docs.gl/__
the tutorial I followed for this project: https://learnopengl.com/
