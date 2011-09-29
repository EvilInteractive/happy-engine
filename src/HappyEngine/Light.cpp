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
RectI PointLight::getScissor(const Camera* pCamera) const
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
void PointLight::debugDraw(const Camera* pCamera) const
{
    /*RectI rect(getScissor(pCamera));
    int p2(GRAPHICS->getViewport().height - (rect.y + rect.height));
    int p1(GRAPHICS->getViewport().height - rect.y);
    rect.y = p2;
    rect.height = p1 - p2;
    HE2D->drawRectangle(math::Vector2((float)rect.x, (float)rect.y), math::Vector2((float)rect.width, (float)rect.height));*/

    math::Matrix m(math::Vector4(pCamera->getLook(), 0), math::Vector4(pCamera->getUp(), 0), math::Vector4(pCamera->getRight(), 0), math::Vector4(0, 0, 0, 1));

    math::Vector3 pos2D(m * position);
    math::Vector3 posCam2D(m * pCamera->getPosition());

    math::shapes::Circle c1(math::Vector2(pos2D.x, pos2D.y), endAttenuation);
    math::Vector2 midPoint((posCam2D.x + pos2D.x) / 2.0f, (posCam2D.y + pos2D.y) / 2.0f);
    math::shapes::Circle c2(midPoint, length(math::Vector2(pos2D.x, pos2D.y) - midPoint));

    std::vector<math::Vector2> tangents(c1.intersect(c2));
    if (tangents.size() == 2)
	{
		float z1(sqrtf( math::sqr(endAttenuation) - math::sqr(tangents[0].x - pos2D.x) - math::sqr(tangents[0].y - pos2D.y) ) + pos2D.z);
		float z2(sqrtf( math::sqr(endAttenuation) - math::sqr(tangents[1].x - pos2D.x) - math::sqr(tangents[1].y - pos2D.y) ) + pos2D.z);

		math::Vector4 tan1(tangents[0].x, tangents[0].y, z1, 1);
		tan1 = m.inverse() * tan1;
		math::Vector4 tan2(tangents[1].x, tangents[1].y, z2, 1);
		tan2 = m.inverse() * tan2;

		math::Vector4 projTan1(pCamera->getViewProjection() * tan1);
		math::Vector4 projTan2(pCamera->getViewProjection() * tan2);

		projTan1.x = 1 - (projTan1.x / projTan1.w + 1) * 0.5f;
		projTan1.y = (projTan1.y / projTan1.w + 1) * 0.5f;
		projTan2.x = 1 - (projTan2.x / projTan2.w + 1) * 0.5f;
		projTan2.y = (projTan2.y / projTan2.w + 1) * 0.5f;

		std::vector<math::Vector2> points;
		points.push_back(math::Vector2(projTan1.x * GRAPHICS->getViewport().width, projTan1.y * GRAPHICS->getViewport().height));
		points.push_back(math::Vector2(projTan2.x * GRAPHICS->getViewport().width, projTan2.y * GRAPHICS->getViewport().height));

		HE2D->drawPolygon(points, 2, false);
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