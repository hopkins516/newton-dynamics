/* Copyright (c) <2003-2019> <Julio Jerez, Newton Game Dynamics>
* 
* This software is provided 'as-is', without any express or implied
* warranty. In no event will the authors be held liable for any damages
* arising from the use of this software.
* 
* Permission is granted to anyone to use this software for any purpose,
* including commercial applications, and to alter it and redistribute it
* freely, subject to the following restrictions:
* 
* 1. The origin of this software must not be misrepresented; you must not
* claim that you wrote the original software. If you use this software
* in a product, an acknowledgment in the product documentation would be
* appreciated but is not required.
* 
* 2. Altered source versions must be plainly marked as such, and must not be
* misrepresented as being the original software.
* 
* 3. This notice may not be removed or altered from any source distribution.
*/

#ifndef __D_BODY_SPH_FLUID_H__
#define __D_BODY_SPH_FLUID_H__

#include "ndNewtonStdafx.h"
#include "ndBodyParticleSet.h"

#define D_RADIX_DIGIT_SIZE	10

D_MSV_NEWTON_ALIGN_32
class ndBodySphFluid: public ndBodyParticleSet
{
	public:
	D_NEWTON_API ndBodySphFluid();
	D_NEWTON_API ndBodySphFluid(const nd::TiXmlNode* const xmlNode, const dTree<const ndShape*, dUnsigned32>& shapesCache);
	D_NEWTON_API virtual ~ndBodySphFluid ();

	virtual ndBodySphFluid* GetAsBodySphFluid();
	D_NEWTON_API virtual void Save(nd::TiXmlElement* const rootNode, const char* const assetPath, dInt32 nodeid, const dTree<dUnsigned32, const ndShape*>& shapesCache) const;

	D_NEWTON_API virtual void AddParticle(const dFloat32 mass, const dVector& position, const dVector& velocity);

	D_NEWTON_API virtual void GenerateIsoSurface(const ndWorld* const world);

	const dIsoSurface& GetIsoSurface() const;

	protected:
	D_NEWTON_API virtual void Update(const ndWorld* const world, dFloat32 timestep);
	virtual dFloat32 RayCast(ndRayCastNotify& callback, const dFastRayTest& ray, const dFloat32 maxT) const;

	private:
	enum ndGridType
	{
		ndHomeGrid = 0,
		ndAdjacentGrid = 1,
	};

	class ndGridHash
	{
		public:
		ndGridHash()
		{
		}

		ndGridHash(const dVector& grid, dInt32 particelIndex, ndGridType cellType)
		{
			dAssert(grid.m_x >= dFloat32 (0.0f));
			dAssert(grid.m_y >= dFloat32 (0.0f));
			dAssert(grid.m_z >= dFloat32 (0.0f));
			dAssert(grid.m_x < dFloat32 (1<<20));
			dAssert(grid.m_y < dFloat32 (1<<20));
			dAssert(grid.m_z < dFloat32 (1<<20));

			dVector hash(grid.GetInt());

			m_gridHash = 0;
			m_x = hash.m_ix;
			m_y = hash.m_iy;
			m_z = hash.m_iz;
			m_cellType = cellType;
			m_particleIndex = particelIndex;
		}

		union
		{
			struct
			{
				dUnsigned64 m_x : D_RADIX_DIGIT_SIZE * 2;
				dUnsigned64 m_y : D_RADIX_DIGIT_SIZE * 2;
				dUnsigned64 m_z : D_RADIX_DIGIT_SIZE * 2;
			};
			struct
			{
				dUnsigned64 m_xLow  : D_RADIX_DIGIT_SIZE;
				dUnsigned64 m_xHigh : D_RADIX_DIGIT_SIZE;
				dUnsigned64 m_yLow  : D_RADIX_DIGIT_SIZE;
				dUnsigned64 m_yHigh : D_RADIX_DIGIT_SIZE;
				dUnsigned64 m_zLow  : D_RADIX_DIGIT_SIZE;
				dUnsigned64 m_zHigh : D_RADIX_DIGIT_SIZE;
			};

			dUnsigned64 m_gridHash;
		};
		dInt32 m_particleIndex;
		ndGridType m_cellType;
	};

	class ndContext
	{
		public:
		ndBodySphFluid* m_fluid;
		dInt32 m_pass;
		dInt32 m_scan[1 << (D_RADIX_DIGIT_SIZE + 1)];
		dInt32 m_histogram[D_MAX_THREADS_COUNT][1 << (D_RADIX_DIGIT_SIZE + 1)];
	};

	void SortGrids(const ndWorld* const world);
	void CreateGrids(const ndWorld* const world);
	void AddCounters(const ndWorld* const world, ndContext& context) const;
	void CaculateAABB(const ndWorld* const world, dVector& boxP0, dVector& boxP1) const;

	void SortParallel(const ndWorld* const world);
	void SortSingleThreaded(const ndWorld* const world);

	dVector m_box0;
	dVector m_box1;
	dArray<ndGridHash> m_hashGridMap;
	dArray<ndGridHash> m_hashGridMapScratchBuffer;
	dInt32 m_upperDigisIsValid[3];
	dIsoSurface m_isoSurcase;
} D_GCC_NEWTON_ALIGN_32 ;

inline dFloat32 ndBodySphFluid::RayCast(ndRayCastNotify& callback, const dFastRayTest& ray, const dFloat32 maxT) const
{
	return dFloat32(1.2f);
}

inline ndBodySphFluid* ndBodySphFluid::GetAsBodySphFluid()
{ 
	return this; 
}

inline const dIsoSurface& ndBodySphFluid::GetIsoSurface() const
{
	return m_isoSurcase;
}

#endif 


