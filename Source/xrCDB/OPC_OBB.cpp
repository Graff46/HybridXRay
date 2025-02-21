﻿///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Contains OBB-related code.
 *	\file		IceOBB.cpp
 *	\author		Pierre Terdiman
 *	\date		January, 29, 2000
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	An Oriented Bounding Box (OBB).
 *	\class		OBB
 *	\author		Pierre Terdiman
 *	\version	1.0
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Precompiled Header
#include "stdafx.h"
#pragma hdrstop

using namespace Meshmerizer;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Tests if a point is contained within the OBB.
 *	\param		p	[in] the world point to test
 *	\return		true if inside the OBB
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool OBB::ContainsPoint(const Point& p) const
{
    // Point in OBB test using lazy evaluation and early exits

    // Translate to box space
    Point RelPoint = p - mCenter;

    // Point * mRot maps from box space to world space
    // mRot * Point maps from world space to box space (what we need here)

    float f        = mRot2.m[0][0] * RelPoint.x + mRot2.m[0][1] * RelPoint.y + mRot2.m[0][2] * RelPoint.z;
    if (f >= mExtents.x || f <= -mExtents.x)
        return false;

    f = mRot2.m[1][0] * RelPoint.x + mRot2.m[1][1] * RelPoint.y + mRot2.m[1][2] * RelPoint.z;
    if (f >= mExtents.y || f <= -mExtents.y)
        return false;

    f = mRot2.m[2][0] * RelPoint.x + mRot2.m[2][1] * RelPoint.y + mRot2.m[2][2] * RelPoint.z;
    if (f >= mExtents.z || f <= -mExtents.z)
        return false;
    return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Builds an OBB from an AABB and a world transform.
 *	\param		aabb	[in] the aabb
 *	\param		mat		[in] the world transform
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void OBB::Create(const AABB& aabb, const Matrix4x4& mat)
{
    // Note: must be coherent with Rotate()

    aabb.GetCenter(mCenter);
    aabb.GetExtents(mExtents);
    // Here we have the same as OBB::Rotate(mat) where the obb is (mCenter, mExtents, Identity).

    // So following what's done in Rotate:
    // - x-form the center
    mCenter *= mat;
    // - combine rotation with identity, i.e. just use given matrix
    mRot2 = mat;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Computes the obb planes.
 *	\param		planes	[out] 6 box planes
 *	\return		true if success
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool OBB::ComputePlanes(Plane* planes) const
{
    // Checkings
    if (!planes)
        return false;

    Point Axis0 = *mRot2[0];
    Point Axis1 = *mRot2[1];
    Point Axis2 = *mRot2[2];

    // Writes normals
    planes[0].n = Axis0;
    planes[1].n = -Axis0;
    planes[2].n = Axis1;
    planes[3].n = -Axis1;
    planes[4].n = Axis2;
    planes[5].n = -Axis2;

    // Compute a point on each plane
    Point p0    = mCenter + Axis0 * mExtents.x;
    Point p1    = mCenter - Axis0 * mExtents.x;
    Point p2    = mCenter + Axis1 * mExtents.y;
    Point p3    = mCenter - Axis1 * mExtents.y;
    Point p4    = mCenter + Axis2 * mExtents.z;
    Point p5    = mCenter - Axis2 * mExtents.z;

    // Compute d
    planes[0].d = -(planes[0].n | p0);
    planes[1].d = -(planes[1].n | p1);
    planes[2].d = -(planes[2].n | p2);
    planes[3].d = -(planes[3].n | p3);
    planes[4].d = -(planes[4].n | p4);
    planes[5].d = -(planes[5].n | p5);

    return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Computes the obb points.
 *	\param		pts	[out] 8 box points
 *	\return		true if success
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool OBB::ComputePoints(Point* pts) const
{
    // Checkings
    if (!pts)
        return false;

    Point Axis0 = *mRot2[0];
    Point Axis1 = *mRot2[1];
    Point Axis2 = *mRot2[2];

    pts[0]      = mCenter - Axis0 * mExtents.x + Axis1 * mExtents.y + Axis2 * mExtents.z;
    pts[1]      = mCenter + Axis0 * mExtents.x + Axis1 * mExtents.y + Axis2 * mExtents.z;
    pts[2]      = mCenter + Axis0 * mExtents.x - Axis1 * mExtents.y + Axis2 * mExtents.z;
    pts[3]      = mCenter - Axis0 * mExtents.x - Axis1 * mExtents.y + Axis2 * mExtents.z;
    pts[4]      = mCenter - Axis0 * mExtents.x + Axis1 * mExtents.y - Axis2 * mExtents.z;
    pts[5]      = mCenter + Axis0 * mExtents.x + Axis1 * mExtents.y - Axis2 * mExtents.z;
    pts[6]      = mCenter + Axis0 * mExtents.x - Axis1 * mExtents.y - Axis2 * mExtents.z;
    pts[7]      = mCenter - Axis0 * mExtents.x - Axis1 * mExtents.y - Axis2 * mExtents.z;

    return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Returns a list of indices used to draw the OBB.
 *	\return		48 indices for the list returned by ComputePoints()
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const udword* OBB::GetLineIndices() const
{
    static udword Indices[] = {
        0,
        1,
        1,
        2,
        2,
        3,
        3,
        0,
        1,
        5,
        5,
        6,
        6,
        2,
        2,
        1,
        7,
        6,
        6,
        5,
        5,
        4,
        4,
        7,
        3,
        7,
        7,
        4,
        4,
        0,
        0,
        3,
        4,
        5,
        5,
        1,
        1,
        0,
        0,
        4,
        3,
        2,
        2,
        6,
        6,
        7,
        7,
        3,
    };
    return Indices;
}
