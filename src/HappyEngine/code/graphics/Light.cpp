//HappyEngine Copyright (C) 2011 - 2012  Bastian Damman, Sebastiaan Sprengers 
//
//This file is part of HappyEngine.
//
//    HappyEngine is free software: you can redistribute it and/or modify
//    it under the terms of the GNU Lesser General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    HappyEngine is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU Lesser General Public License for more details.
//
//    You should have received a copy of the GNU Lesser General Public License
//    along with HappyEngine.  If not, see <http://www.gnu.org/licenses/>.
//
//Author:  Bastian Damman
//Created: 19/09/2011
#include "HappyPCH.h" 

#include "Light.h"
#include "HappyNew.h"
#include "HappyEngine.h"
#include "HeAssert.h"
#include "Circle.h"

namespace he {
namespace gfx {

//#pragma region PointLight
//RectI PointLight::getOldScissor(const Camera* pCamera) const
//{
//    RectI scissor(GRAPHICS->getViewport());
//    vec2 min(FLT_MAX, FLT_MAX);
//    vec2 max(FLT_MIN, FLT_MIN);
//    if (lengthSqr(pCamera->getPosition() - position) > endAttenuation * endAttenuation)
//    {
//        std::vector<vec4> points;
//        points.push_back(vec4(position + vec3(-1, -1, -1) * endAttenuation, 1));
//        points.push_back(vec4(position + vec3(1, -1, -1) * endAttenuation, 1));
//        points.push_back(vec4(position + vec3(-1, 1, -1) * endAttenuation, 1));
//        points.push_back(vec4(position + vec3(1, 1, -1) * endAttenuation, 1));
//        points.push_back(vec4(position + vec3(-1, -1, 1) * endAttenuation, 1));
//        points.push_back(vec4(position + vec3(1, -1, 1) * endAttenuation, 1));
//        points.push_back(vec4(position + vec3(-1, 1, 1) * endAttenuation, 1));
//        points.push_back(vec4(position + vec3(1, 1, 1) * endAttenuation, 1));
//
//        std::for_each(points.begin(), points.end(), [&](vec4& p)
//        {
//            p = pCamera->getViewProjection() * p;
//            p.x = 1 - (p.x / p.w + 1) * 0.5f;
//            p.y = (p.y / p.w + 1) * 0.5f;
//        });
//        std::for_each(points.begin(), points.end(), [&](vec4& p)
//        {
//            min.x = min(min.x, p.x);
//            min.y = min(min.y, p.y);
//            max.x = max(max.x, p.x);
//            max.y = max(max.y, p.y);
//        });
//        min.x = clamp(min.x, 0.0f, 1.0f);
//        min.y = clamp(min.y, 0.0f, 1.0f);
//        max.x = clamp(max.x, 0.0f, 1.0f);
//        max.y = clamp(max.y, 0.0f, 1.0f);
//    }
//    else
//    {
//        min.x = 0.0f;
//        max.x = 1.0f;
//        min.y = 0.0f;
//        max.y = 1.0f;
//    }
//    scissor.x += static_cast<int>(scissor.width * min.x);
//    scissor.y += static_cast<int>(scissor.height * min.y);
//    scissor.width = static_cast<int>(scissor.width * (max.x - min.x));
//    scissor.height = static_cast<int>(scissor.height * (max.y - min.y));
//
//    HE_ASSERT(scissor.width >= 0, "must be >= 0");
//    HE_ASSERT(scissor.height >= 0, "must be >= 0");
//
//    return scissor;
//}
//RectI PointLight::getScissor(const Camera* pCamera) const
//{
//    const mat44& m(pCamera->getView());
//   
//    vec3 posCamSpace(m * position);
//
//    //from right
//    shapes::Circle c1(vec2(posCamSpace.z, posCamSpace.y), endAttenuation);
//    vec2 midPoint12((0 + posCamSpace.z) / 2.0f, (0 + posCamSpace.y) / 2.0f);
//    shapes::Circle c2(midPoint12, length(vec2(posCamSpace.z, posCamSpace.y) - midPoint12));
//
//    //from top
//    shapes::Circle c3(vec2(posCamSpace.z, posCamSpace.x), endAttenuation);
//    vec2 midPoint34((0 + posCamSpace.z) / 2.0f, (0 + posCamSpace.x) / 2.0f);
//    shapes::Circle c4(midPoint34, length(vec2(posCamSpace.z, posCamSpace.x) - midPoint34));
//
//    std::vector<vec2> tangents12(c1.intersect(c2));
//    std::vector<vec2> tangents34(c3.intersect(c4));
//    if (tangents12.size() == 2 && tangents34.size() == 2) //check, it could be that we are in the pointlight
//    {
//	    vec3 tan1(0, tangents12[0].y, tangents12[0].x);
//        tan1.x = sqrtf( max(0.0f, sqr(endAttenuation) - sqr(tan1.y - posCamSpace.y) - sqr(tan1.z - posCamSpace.z)) ) + posCamSpace.x;
//
//	    vec3 tan2(0, tangents12[1].y, tangents12[1].x);
//        tan2.x = sqrtf( max(0.0f, sqr(endAttenuation) - sqr(tan2.y - posCamSpace.y) - sqr(tan2.z - posCamSpace.z)) ) + posCamSpace.x;
//
//	    vec3 tan3(tangents34[0].y, 0, tangents34[0].x);
//        tan3.y = sqrtf( max(0.0f, sqr(endAttenuation) - sqr(tan3.x - posCamSpace.x) - sqr(tan3.z - posCamSpace.z)) ) + posCamSpace.y;
//
//	    vec3 tan4(tangents34[1].y, 0, tangents34[1].x);
//        tan4.y = sqrtf( max(0.0f, sqr(endAttenuation) - sqr(tan4.x - posCamSpace.x) - sqr(tan4.z - posCamSpace.z)) ) + posCamSpace.y;
//
//        mat44 mInv(m.inverse());
//	    tan1 = mInv * tan1;
//	    tan2 = mInv * tan2;
//	    tan3 = mInv * tan3;
//	    tan4 = mInv * tan4;
//
//	    vec4 projTan1(pCamera->getViewProjection() * vec4(tan1, 1));
//	    vec4 projTan2(pCamera->getViewProjection() * vec4(tan2, 1));
//	    vec4 projTan3(pCamera->getViewProjection() * vec4(tan3, 1));
//	    vec4 projTan4(pCamera->getViewProjection() * vec4(tan4, 1));
//
//	    projTan1.x = 1-(projTan1.x / projTan1.w + 1) * 0.5f;
//	    projTan1.y = (projTan1.y / projTan1.w + 1) * 0.5f;
//	    projTan2.x = 1-(projTan2.x / projTan2.w + 1) * 0.5f;
//	    projTan2.y = (projTan2.y / projTan2.w + 1) * 0.5f;
//	    projTan3.x = 1-(projTan3.x / projTan3.w + 1) * 0.5f;
//	    projTan3.y = (projTan3.y / projTan3.w + 1) * 0.5f;
//	    projTan4.x = 1-(projTan4.x / projTan4.w + 1) * 0.5f;
//	    projTan4.y = (projTan4.y / projTan4.w + 1) * 0.5f;
//                
//        vec2 min(0.0f, 0.0f), 
//                      max(0.0f, 0.0f);
//        min.x = clamp(min(min(min(projTan1.x, projTan2.x), projTan3.x), projTan4.x), 0.0f, 1.0f) * GRAPHICS->getViewport().width;
//        min.y = clamp(min(min(min(projTan1.y, projTan2.y), projTan3.y), projTan4.y), 0.0f, 1.0f) * GRAPHICS->getViewport().height;
//        max.x = clamp(max(max(max(projTan1.x, projTan2.x), projTan3.x), projTan4.x), 0.0f, 1.0f) * GRAPHICS->getViewport().width;
//        max.y = clamp(max(max(max(projTan1.y, projTan2.y), projTan3.y), projTan4.y), 0.0f, 1.0f) * GRAPHICS->getViewport().height;
//
//        return RectI(static_cast<int>(min.x), static_cast<int>(min.y), static_cast<int>(max.x - min.x), static_cast<int>(max.y - min.y));
//    }
//    else
//    {
//        return GRAPHICS->getViewport();
//    }
//}
//#if _DEBUG
//void PointLight::debugDraw(const Camera* pCamera) const
//{
//    {
//        RectI rect(getScissor(pCamera));
//        int p2(GRAPHICS->getViewport().height - (rect.y + rect.height));
//        int p1(GRAPHICS->getViewport().height - rect.y);
//        rect.y = p2;
//        rect.height = p1 - p2;
//        GUI->setColor(0, 1, 0);
//        GUI->drawRectangle(vec2((float)rect.x, (float)rect.y), vec2((float)rect.width, (float)rect.height));
//    }
//    {
//        RectI rect(getOldScissor(pCamera));
//        int p2(GRAPHICS->getViewport().height - (rect.y + rect.height));
//        int p1(GRAPHICS->getViewport().height - rect.y);
//        rect.y = p2;
//        rect.height = p1 - p2;
//        GUI->setColor(1, 0, 0);
//        GUI->drawRectangle(vec2((float)rect.x, (float)rect.y), vec2((float)rect.width, (float)rect.height));
//    }
//}
//#endif
//#pragma endregion

//#pragma region SpotLight
//RectI SpotLight::getScissor(const Camera* pCamera) const
//{
//    const mat44& m(pCamera->getView());
//   
//    vec3 posCamSpace(m * position);
//
//    //from right
//    shapes::Circle c1(vec2(posCamSpace.z, posCamSpace.y), endAttenuation);
//    vec2 midPoint12((0 + posCamSpace.z) / 2.0f, (0 + posCamSpace.y) / 2.0f);
//    shapes::Circle c2(midPoint12, length(vec2(posCamSpace.z, posCamSpace.y) - midPoint12));
//
//    //from top
//    shapes::Circle c3(vec2(posCamSpace.z, posCamSpace.x), endAttenuation);
//    vec2 midPoint34((0 + posCamSpace.z) / 2.0f, (0 + posCamSpace.x) / 2.0f);
//    shapes::Circle c4(midPoint34, length(vec2(posCamSpace.z, posCamSpace.x) - midPoint34));
//
//    std::vector<vec2> tangents12(c1.intersect(c2));
//    std::vector<vec2> tangents34(c3.intersect(c4));
//    if (tangents12.size() == 2 && tangents34.size() == 2) //check, it could be that we are in the pointlight
//    {
//	    vec3 tan1(0, tangents12[0].y, tangents12[0].x);
//        tan1.x = sqrtf( max(0.0f, sqr(endAttenuation) - sqr(tan1.y - posCamSpace.y) - sqr(tan1.z - posCamSpace.z)) ) + posCamSpace.x;
//
//	    vec3 tan2(0, tangents12[1].y, tangents12[1].x);
//        tan2.x = sqrtf( max(0.0f, sqr(endAttenuation) - sqr(tan2.y - posCamSpace.y) - sqr(tan2.z - posCamSpace.z)) ) + posCamSpace.x;
//
//	    vec3 tan3(tangents34[0].y, 0, tangents34[0].x);
//        tan3.y = sqrtf( max(0.0f, sqr(endAttenuation) - sqr(tan3.x - posCamSpace.x) - sqr(tan3.z - posCamSpace.z)) ) + posCamSpace.y;
//
//	    vec3 tan4(tangents34[1].y, 0, tangents34[1].x);
//        tan4.y = sqrtf( max(0.0f, sqr(endAttenuation) - sqr(tan4.x - posCamSpace.x) - sqr(tan4.z - posCamSpace.z)) ) + posCamSpace.y;
//
//        mat44 mInv(m.inverse());
//	    tan1 = mInv * tan1;
//	    tan2 = mInv * tan2;
//	    tan3 = mInv * tan3;
//	    tan4 = mInv * tan4;
//
//	    vec4 projTan1(pCamera->getViewProjection() * vec4(tan1, 1));
//	    vec4 projTan2(pCamera->getViewProjection() * vec4(tan2, 1));
//	    vec4 projTan3(pCamera->getViewProjection() * vec4(tan3, 1));
//	    vec4 projTan4(pCamera->getViewProjection() * vec4(tan4, 1));
//
//	    projTan1.x = 1-(projTan1.x / projTan1.w + 1) * 0.5f;
//	    projTan1.y = (projTan1.y / projTan1.w + 1) * 0.5f;
//	    projTan2.x = 1-(projTan2.x / projTan2.w + 1) * 0.5f;
//	    projTan2.y = (projTan2.y / projTan2.w + 1) * 0.5f;
//	    projTan3.x = 1-(projTan3.x / projTan3.w + 1) * 0.5f;
//	    projTan3.y = (projTan3.y / projTan3.w + 1) * 0.5f;
//	    projTan4.x = 1-(projTan4.x / projTan4.w + 1) * 0.5f;
//	    projTan4.y = (projTan4.y / projTan4.w + 1) * 0.5f;
//                
//        vec2 min(0.0f, 0.0f), 
//                      max(0.0f, 0.0f);
//        min.x = clamp(min(min(min(projTan1.x, projTan2.x), projTan3.x), projTan4.x), 0.0f, 1.0f) * GRAPHICS->getViewport().width;
//        min.y = clamp(min(min(min(projTan1.y, projTan2.y), projTan3.y), projTan4.y), 0.0f, 1.0f) * GRAPHICS->getViewport().height;
//        max.x = clamp(max(max(max(projTan1.x, projTan2.x), projTan3.x), projTan4.x), 0.0f, 1.0f) * GRAPHICS->getViewport().width;
//        max.y = clamp(max(max(max(projTan1.y, projTan2.y), projTan3.y), projTan4.y), 0.0f, 1.0f) * GRAPHICS->getViewport().height;
//
//        return RectI(static_cast<int>(min.x), static_cast<int>(min.y), static_cast<int>(max.x - min.x), static_cast<int>(max.y - min.y));
//    }
//    else
//    {
//        return GRAPHICS->getViewport();
//    }
//}
//
//#if _DEBUG
//void SpotLight::debugDraw(const Camera* pCamera) const
//{
//    RectI rect(getScissor(pCamera));
//    int p2(GRAPHICS->getViewport().height - (rect.y + rect.height));
//    int p1(GRAPHICS->getViewport().height - rect.y);
//    rect.y = p2;
//    rect.height = p1 - p2;
//    GUI->drawRectangle(vec2((float)rect.x, (float)rect.y), vec2((float)rect.width, (float)rect.height));
//}
//#endif
//#pragma endregion

} } //end namespace