set (CODE_MATH_FILES    AABB.cpp                        AABB.h
                        CatmullRomSpline.h
                        Circle.cpp                      Circle.h
                        ISpline.h
                        LinearSpline.h
                        mat33.cpp                       mat33.h
                        mat44.cpp                       mat44.h
                        MathConstants.h
                        MathFunctions.h
                        Polygon.cpp                     Polygon.h
                        Random.cpp                      Random.h
                        Ray.cpp                         Ray.h
                        Rect.cpp                        Rect.h
                        Sphere.cpp                      Sphere.h
                        Triangulator.cpp                Triangulator.h
                        vec2.cpp                        vec2.h
                        vec3.cpp                        vec3.h
                        vec4.cpp                        vec4.h)
                        
foreach(f ${CODE_MATH_FILES}) 
    LIST(APPEND HAPPYENGINE_CODE_MATH_SRCS code/math/${f})
endforeach(f) 

source_group (src\\math FILES ${HAPPYENGINE_CODE_MATH_SRCS})
