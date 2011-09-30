//HappyEngine Copyright (C) 2011  Bastian Damman, Sebastiaan Sprengers
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

#include "Light.h"
#include "HappyNew.h"
#include "HappyEngine.h"
#include "Assert.h"
#include "Circle.h"

namespace happyengine {
namespace graphics {

#pragma region PointLight
//RectI PointLight::getScissor(const Camera* pCamera) const
//{
//    RectI scissor;
//
//    math::Vector3 planeNormal(math::normalize(pCamera->getPosition() - position));
//
//    //math::Vector3 up(math::normalize(math::cross(planeNormal, pCamera->getRight())));
//    //math::Vector3 right(math::normalize(math::cross(up, planeNormal)));
//
//    math::Matrix m(math::Vector4(pCamera->getLook(), 0), math::Vector4(pCamera->getUp(), 0), math::Vector4(pCamera->getRight(), 0), math::Vector4(0, 0, 0, 1));
//
//    math::Vector3 pos2D(m * position);
//    math::Vector3 posCam2D(m * pCamera->getPosition());
//    math::shapes::Circle c1(math::Vector2(pos2D.x, pos2D.y), endAttenuation);
//    math::Vector2 midPoint((posCam2D.x + pos2D.x) / 2.0f, (posCam2D.y + pos2D.y) / 2.0f);
//    math::shapes::Circle c2(midPoint, length(math::Vector2(pos2D.x, pos2D.y) - midPoint));
//
//    std::pair<math::Vector2, math::Vector2> tangents(c1.intersect(c2));
//    //r^2 - x^2 - y^2 = z^2
//    float z1(sqrtf( math::sqr(endAttenuation) - math::sqr(tangents.first.x) - math::sqr(tangents.first.y) ));
//    float z2(sqrtf( math::sqr(endAttenuation) - math::sqr(tangents.second.x) - math::sqr(tangents.second.y) ));
//
//    math::Vector4 p0(0, 0, 0, 1);
//
//
//    return scissor;
//}
RectI PointLight::getOldScissor(const Camera* pCamera) const
{
    RectI scissor(GRAPHICS->getViewport());
    math::Vector2 min(FLT_MAX, FLT_MAX);
    math::Vector2 max(FLT_MIN, FLT_MIN);
    if (lengthSqr(pCamera->getPosition() - position) > endAttenuation * endAttenuation)
    {
        std::vector<math::Vector4> points;
        points.push_back(math::Vector4(position + math::Vector3(-1, -1, -1) * endAttenuation, 1));
        points.push_back(math::Vector4(position + math::Vector3(1, -1, -1) * endAttenuation, 1));
        points.push_back(math::Vector4(position + math::Vector3(-1, 1, -1) * endAttenuation, 1));
        points.push_back(math::Vector4(position + math::Vector3(1, 1, -1) * endAttenuation, 1));
        points.push_back(math::Vector4(position + math::Vector3(-1, -1, 1) * endAttenuation, 1));
        points.push_back(math::Vector4(position + math::Vector3(1, -1, 1) * endAttenuation, 1));
        points.push_back(math::Vector4(position + math::Vector3(-1, 1, 1) * endAttenuation, 1));
        points.push_back(math::Vector4(position + math::Vector3(1, 1, 1) * endAttenuation, 1));

        std::for_each(points.begin(), points.end(), [&](math::Vector4& p)
        {
            p = pCamera->getViewProjection() * p;
            p.x = 1 - (p.x / p.w + 1) * 0.5f;
            p.y = (p.y / p.w + 1) * 0.5f;
        });
        std::for_each(points.begin(), points.end(), [&](math::Vector4& p)
        {
            min.x = math::min(min.x, p.x);
            min.y = math::min(min.y, p.y);
            max.x = math::max(max.x, p.x);
            max.y = math::max(max.y, p.y);
        });
        min.x = math::clamp(min.x, 0.0f, 1.0f);
        min.y = math::clamp(min.y, 0.0f, 1.0f);
        max.x = math::clamp(max.x, 0.0f, 1.0f);
        max.y = math::clamp(max.y, 0.0f, 1.0f);
    }
    else
    {
        min.x = 0.0f;
        max.x = 1.0f;
        min.y = 0.0f;
        max.y = 1.0f;
    }
    scissor.x += static_cast<int>(scissor.width * min.x);
    scissor.y += static_cast<int>(scissor.height * min.y);
    scissor.width = static_cast<int>(scissor.width * (max.x - min.x));
    scissor.height = static_cast<int>(scissor.height * (max.y - min.y));

    ASSERT(scissor.width >= 0, "must be >= 0");
    ASSERT(scissor.height >= 0, "must be >= 0");

    return scissor;
}
RectI PointLight::getScissor(const Camera* pCamera) const
{
    //math::Matrix m(math::Matrix::createLookAtLH(pCamera->getPosition(),            // x = cam right
    //                pCamera->getPosition() + math::normalize(pCamera->getLook()),  // y = cam up
    //                math::normalize(pCamera->getUp())));                           // z = cam look
    const math::Matrix& m(pCamera->getView());
   
    math::Vector3 posCamSpace(m * position);

    //from right
    math::shapes::Circle c1(math::Vector2(posCamSpace.z, posCamSpace.y), endAttenuation);
    math::Vector2 midPoint12((0 + posCamSpace.z) / 2.0f, (0 + posCamSpace.y) / 2.0f);
    math::shapes::Circle c2(midPoint12, length(math::Vector2(posCamSpace.z, posCamSpace.y) - midPoint12));

    //from top
    math::shapes::Circle c3(math::Vector2(posCamSpace.z, posCamSpace.x), endAttenuation);
    math::Vector2 midPoint34((0 + posCamSpace.z) / 2.0f, (0 + posCamSpace.x) / 2.0f);
    math::shapes::Circle c4(midPoint34, length(math::Vector2(posCamSpace.z, posCamSpace.x) - midPoint34));

    std::vector<math::Vector2> tangents12(c1.intersect(c2));
    std::vector<math::Vector2> tangents34(c3.intersect(c4));
    if (tangents12.size() == 2 && tangents34.size() == 2) //check, it could be that we are in the pointlight
    {
	    math::Vector3 tan1(0, tangents12[0].y, tangents12[0].x);
        tan1.x = sqrtf( math::max(0.0f, math::sqr(endAttenuation) - math::sqr(tan1.y - posCamSpace.y) - math::sqr(tan1.z - posCamSpace.z)) ) + posCamSpace.x;

	    math::Vector3 tan2(0, tangents12[1].y, tangents12[1].x);
        tan2.x = sqrtf( math::max(0.0f, math::sqr(endAttenuation) - math::sqr(tan2.y - posCamSpace.y) - math::sqr(tan2.z - posCamSpace.z)) ) + posCamSpace.x;

	    math::Vector3 tan3(tangents34[0].y, 0, tangents34[0].x);
        tan3.y = sqrtf( math::max(0.0f, math::sqr(endAttenuation) - math::sqr(tan3.x - posCamSpace.x) - math::sqr(tan3.z - posCamSpace.z)) ) + posCamSpace.y;

	    math::Vector3 tan4(tangents34[1].y, 0, tangents34[1].x);
        tan4.y = sqrtf( math::max(0.0f, math::sqr(endAttenuation) - math::sqr(tan4.x - posCamSpace.x) - math::sqr(tan4.z - posCamSpace.z)) ) + posCamSpace.y;

        math::Matrix mInv(m.inverse());
	    tan1 = mInv * tan1;
	    tan2 = mInv * tan2;
	    tan3 = mInv * tan3;
	    tan4 = mInv * tan4;

	    math::Vector4 projTan1(pCamera->getViewProjection() * math::Vector4(tan1, 1));
	    math::Vector4 projTan2(pCamera->getViewProjection() * math::Vector4(tan2, 1));
	    math::Vector4 projTan3(pCamera->getViewProjection() * math::Vector4(tan3, 1));
	    math::Vector4 projTan4(pCamera->getViewProjection() * math::Vector4(tan4, 1));

	    projTan1.x = 1-(projTan1.x / projTan1.w + 1) * 0.5f;
	    projTan1.y = (projTan1.y / projTan1.w + 1) * 0.5f;
	    projTan2.x = 1-(projTan2.x / projTan2.w + 1) * 0.5f;
	    projTan2.y = (projTan2.y / projTan2.w + 1) * 0.5f;
	    projTan3.x = 1-(projTan3.x / projTan3.w + 1) * 0.5f;
	    projTan3.y = (projTan3.y / projTan3.w + 1) * 0.5f;
	    projTan4.x = 1-(projTan4.x / projTan4.w + 1) * 0.5f;
	    projTan4.y = (projTan4.y / projTan4.w + 1) * 0.5f;
                
        math::Vector2 min(0.0f, 0.0f), 
                      max(0.0f, 0.0f);
        min.x = math::clamp(math::min(math::min(math::min(projTan1.x, projTan2.x), projTan3.x), projTan4.x), 0.0f, 1.0f) * GRAPHICS->getViewport().width;
        min.y = math::clamp(math::min(math::min(math::min(projTan1.y, projTan2.y), projTan3.y), projTan4.y), 0.0f, 1.0f) * GRAPHICS->getViewport().height;
        max.x = math::clamp(math::max(math::max(math::max(projTan1.x, projTan2.x), projTan3.x), projTan4.x), 0.0f, 1.0f) * GRAPHICS->getViewport().width;
        max.y = math::clamp(math::max(math::max(math::max(projTan1.y, projTan2.y), projTan3.y), projTan4.y), 0.0f, 1.0f) * GRAPHICS->getViewport().height;

        return RectI(static_cast<int>(min.x), static_cast<int>(min.y), static_cast<int>(max.x - min.x), static_cast<int>(max.y - min.y));
    }
    else
    {
        return GRAPHICS->getViewport();
    }
}
#if _DEBUG
void PointLight::debugDraw(const Camera* pCamera) const
{
    {
        RectI rect(getScissor(pCamera));
        int p2(GRAPHICS->getViewport().height - (rect.y + rect.height));
        int p1(GRAPHICS->getViewport().height - rect.y);
        rect.y = p2;
        rect.height = p1 - p2;
        HE2D->setColor(0, 1, 0);
        HE2D->drawRectangle(math::Vector2((float)rect.x, (float)rect.y), math::Vector2((float)rect.width, (float)rect.height));
    }
    {
        RectI rect(getOldScissor(pCamera));
        int p2(GRAPHICS->getViewport().height - (rect.y + rect.height));
        int p1(GRAPHICS->getViewport().height - rect.y);
        rect.y = p2;
        rect.height = p1 - p2;
        HE2D->setColor(1, 0, 0);
        HE2D->drawRectangle(math::Vector2((float)rect.x, (float)rect.y), math::Vector2((float)rect.width, (float)rect.height));
    }
}
#endif
#pragma endregion

#pragma region SpotLight
RectI SpotLight::getScissor(const Camera* pCamera) const
{
    RectI scissor(GRAPHICS->getViewport());
    math::Vector2 min(FLT_MAX, FLT_MAX);
    math::Vector2 max(FLT_MIN, FLT_MIN);
    if (lengthSqr(pCamera->getPosition() - position) > endAttenuation * endAttenuation)
    {
        std::vector<math::Vector4> points;
        points.push_back(math::Vector4(position + math::Vector3(-1, -1, -1) * endAttenuation, 1));
        points.push_back(math::Vector4(position + math::Vector3(1, -1, -1) * endAttenuation, 1));
        points.push_back(math::Vector4(position + math::Vector3(-1, 1, -1) * endAttenuation, 1));
        points.push_back(math::Vector4(position + math::Vector3(1, 1, -1) * endAttenuation, 1));
        points.push_back(math::Vector4(position + math::Vector3(-1, -1, 1) * endAttenuation, 1));
        points.push_back(math::Vector4(position + math::Vector3(1, -1, 1) * endAttenuation, 1));
        points.push_back(math::Vector4(position + math::Vector3(-1, 1, 1) * endAttenuation, 1));
        points.push_back(math::Vector4(position + math::Vector3(1, 1, 1) * endAttenuation, 1));

        std::for_each(points.begin(), points.end(), [&](math::Vector4& p)
        {
            p = pCamera->getViewProjection() * p;
            p.x = 1 - (p.x / p.w + 1) * 0.5f;
            p.y = (p.y / p.w + 1) * 0.5f;
        });
        std::for_each(points.begin(), points.end(), [&](math::Vector4& p)
        {
            min.x = math::min(min.x, p.x);
            min.y = math::min(min.y, p.y);
            max.x = math::max(max.x, p.x);
            max.y = math::max(max.y, p.y);
        });
        min.x = math::clamp(min.x, 0.0f, 1.0f);
        min.y = math::clamp(min.y, 0.0f, 1.0f);
        max.x = math::clamp(max.x, 0.0f, 1.0f);
        max.y = math::clamp(max.y, 0.0f, 1.0f);
    }
    else
    {
        min.x = 0.0f;
        max.x = 1.0f;
        min.y = 0.0f;
        max.y = 1.0f;
    }
    scissor.x += static_cast<int>(scissor.width * min.x);
    scissor.y += static_cast<int>(scissor.height * min.y);
    scissor.width = static_cast<int>(scissor.width * (max.x - min.x));
    scissor.height = static_cast<int>(scissor.height * (max.y - min.y));

    ASSERT(scissor.width >= 0, "must be >= 0");
    ASSERT(scissor.height >= 0, "must be >= 0");

    return scissor;
}

#if _DEBUG
void SpotLight::debugDraw(const Camera* pCamera) const
{
    RectI rect(getScissor(pCamera));
    int p2(GRAPHICS->getViewport().height - (rect.y + rect.height));
    int p1(GRAPHICS->getViewport().height - rect.y);
    rect.y = p2;
    rect.height = p1 - p2;
    HE2D->drawRectangle(math::Vector2((float)rect.x, (float)rect.y), math::Vector2((float)rect.width, (float)rect.height));
}
#endif
#pragma endregion

} } //end namespace