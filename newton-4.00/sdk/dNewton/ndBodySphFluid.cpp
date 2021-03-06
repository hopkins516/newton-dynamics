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

#include "dCoreStdafx.h"
#include "ndNewtonStdafx.h"
#include "ndWorld.h"
#include "ndBodySphFluid.h"


/*
{
dFloat32 xxx[6][6][6];
for (dInt32 i = 0; i < 6 * 6 * 6; i++)
{
dFloat32* yyy = &xxx[0][0][0];
yyy[i] = 1.0f;
}
for (dInt32 i = 0; i < uniqueCount; i++)
{
dInt32 x = m_hashGridMap[i].m_x;
dInt32 y = m_hashGridMap[i].m_y;
dInt32 z = m_hashGridMap[i].m_z;

xxx[z][y][x] = 0.0f;
}

dIsoSurfaceOld isoSurcase;
isoSurcase.GenerateSurface(&xxx[0][0][0], 0.5f, 5, 5, 5, gridSize, gridSize, gridSize);
cellCount *= 1;
}
*/

ndBodySphFluid::ndBodySphFluid()
	:ndBodyParticleSet()
	,m_box0(dFloat32(-1e10f))
	,m_box1(dFloat32(1e10f))
	,m_hashGridMap(1024)
	,m_hashGridMapScratchBuffer(1024)
{
}

ndBodySphFluid::ndBodySphFluid(const nd::TiXmlNode* const xmlNode, const dTree<const ndShape*, dUnsigned32>& shapesCache)
	:ndBodyParticleSet(xmlNode->FirstChild("ndBodyKinematic"), shapesCache)
	,m_box0(dFloat32(-1e10f))
	,m_box1(dFloat32(1e10f))
	,m_hashGridMap()
	,m_hashGridMapScratchBuffer()
{
	// nothing was saved
	dAssert(0);
}

ndBodySphFluid::~ndBodySphFluid()
{
}

void ndBodySphFluid::Save(nd::TiXmlElement* const rootNode, const char* const assetPath, dInt32 nodeid, const dTree<dUnsigned32, const ndShape*>& shapesCache) const
{
	dAssert(0);
	nd::TiXmlElement* const paramNode = CreateRootElement(rootNode, "ndBodySphFluid", nodeid);
	ndBodyParticleSet::Save(paramNode, assetPath, nodeid, shapesCache);
}

void ndBodySphFluid::AddParticle(const dFloat32 mass, const dVector& position, const dVector& velocity)
{
	dVector point(position);
	point.m_w = dFloat32(1.0f);
	m_posit.PushBack(point);
}

void ndBodySphFluid::CaculateAABB(const ndWorld* const world, dVector& boxP0, dVector& boxP1) const
{
	D_TRACKTIME();
	dVector box0(dFloat32(1e20f));
	dVector box1(dFloat32(-1e20f));
	for (dInt32 i = m_posit.GetCount() - 1; i >= 0; i--)
	{
		box0 = box0.GetMin(m_posit[i]);
		box1 = box1.GetMax(m_posit[i]);
	}
	boxP0 = box0;
	boxP1 = box1;
}

void ndBodySphFluid::Update(const ndWorld* const world, dFloat32 timestep)
{
	dVector boxP0;
	dVector boxP1;
	CaculateAABB(world, boxP0, boxP1);
	m_box0 = boxP0 - dVector(m_radius * dFloat32(2.0f));
	m_box1 = boxP1 + dVector(m_radius * dFloat32(2.0f));

	CreateGrids(world);
	SortGrids(world);
}

void ndBodySphFluid::CreateGrids(const ndWorld* const world)
{
	D_TRACKTIME();
	if (m_hashGridMap.GetCount() < m_posit.GetCount() * 16)
	{
		m_hashGridMap.SetCount(m_posit.GetCount() * 16);
	}

	class ndCreateGrids: public ndScene::ndBaseJob
	{
		public:
		class ndContext
		{
			public:
			ndBodySphFluid* m_fluid;
			dAtomic<dInt32> m_iterator;
		};

		#define D_SCRATCH_BUFFER_SIZE (1024 * 2)

		virtual void Execute()
		{
			D_TRACKTIME();

			dVector m_neighborkDirs[8];

			ndBodySphFluid* const fluid = ((ndContext*)m_context)->m_fluid;
			const dFloat32 radius = fluid->m_radius;

			m_neighborkDirs[0] = dVector(-radius, -radius, -radius, dFloat32(0.0f));
			m_neighborkDirs[1] = dVector(radius, -radius, -radius, dFloat32(0.0f));
			m_neighborkDirs[2] = dVector(-radius, radius, -radius, dFloat32(0.0f));
			m_neighborkDirs[3] = dVector(radius, radius, -radius, dFloat32(0.0f));
			m_neighborkDirs[4] = dVector(-radius, -radius, radius, dFloat32(0.0f));
			m_neighborkDirs[5] = dVector(radius, -radius, radius, dFloat32(0.0f));
			m_neighborkDirs[6] = dVector(-radius, radius, radius, dFloat32(0.0f));
			m_neighborkDirs[7] = dVector(radius, radius, radius, dFloat32(0.0f));

			const dInt32 threadIndex = GetThreadId();
			const dInt32 threadCount = m_owner->GetThreadCount();
			const dInt32 particleCount = fluid->m_posit.GetCount();
			const dInt32 step = particleCount / threadCount;
			const dInt32 start = threadIndex * step;
			const dInt32 count = ((threadIndex + 1) < threadCount) ? step : particleCount - start;

			const dFloat32 diameter = radius * dFloat32(2.0f);
			const dFloat32 gridSize = diameter * dFloat32(1.0625f);

			const dVector origin(fluid->m_box0);
			const dVector invGridSize(dFloat32(1.0f) / gridSize);

			const dVector* const posit = &fluid->m_posit[0];

			dInt32 scratchBufferCount = 0;
			ndGridHash scratchBuffer[D_SCRATCH_BUFFER_SIZE + 128];

			dAtomic<dInt32>& iterator = ((ndContext*)m_context)->m_iterator;
			for (dInt32 i = 0; i < count; i++)
			{
				dVector r(posit[start + i] - origin);
				dVector p(r * invGridSize);
			
				ndGridHash hashKey(p, i, ndHomeGrid);
				scratchBuffer[scratchBufferCount] = hashKey;
				scratchBufferCount++;
				fluid->m_upperDigisIsValid[0] |= hashKey.m_xHigh;
				fluid->m_upperDigisIsValid[1] |= hashKey.m_yHigh;
				fluid->m_upperDigisIsValid[2] |= hashKey.m_zHigh;
			
				for (dInt32 j = 0; j < sizeof(m_neighborkDirs) / sizeof(m_neighborkDirs[0]); j++)
				{
					ndGridHash neighborKey(p + m_neighborkDirs[j], i, ndAdjacentGrid);
					if (neighborKey.m_gridHash != hashKey.m_gridHash)
					{
						scratchBuffer[scratchBufferCount] = neighborKey;
						scratchBufferCount++;
					}
				}
			
				if (scratchBufferCount > D_SCRATCH_BUFFER_SIZE)
				{
					dInt32 entry = iterator.fetch_add(scratchBufferCount);
					dAssert(iterator.load() < fluid->m_hashGridMap.GetCount());
					memcpy(&fluid->m_hashGridMap[entry], scratchBuffer, scratchBufferCount * sizeof(ndGridHash));
					scratchBufferCount = 0;
				}
			}
			
			if (scratchBufferCount)
			{
				dInt32 entry = iterator.fetch_add(scratchBufferCount);
				dAssert(iterator.load() < fluid->m_hashGridMap.GetCount());
				memcpy(&fluid->m_hashGridMap[entry], scratchBuffer, scratchBufferCount * sizeof(ndGridHash));
			}
		}
	};

	if (m_hashGridMap.GetCount() < m_posit.GetCount() * 16)
	{
		m_hashGridMap.Resize(m_posit.GetCount() * 16);
		m_hashGridMap.SetCount(m_posit.GetCount() * 16);
	}

	ndScene* const scene = world->GetScene();
	m_upperDigisIsValid[0] = 0;
	m_upperDigisIsValid[1] = 0;
	m_upperDigisIsValid[2] = 0;

	ndCreateGrids::ndContext context;
	context.m_fluid = this;
	scene->SubmitJobs<ndCreateGrids>(&context);
	m_hashGridMap.SetCount(context.m_iterator.load());
}

void ndBodySphFluid::SortSingleThreaded(const ndWorld* const world)
{
	const dInt32 count = m_hashGridMap.GetCount();

	//const dInt32 size = count / threadCount;
	//const dInt32 start = threadId * size;
	//const dInt32 batchSize = (threadId == threadCount - 1) ? count - start : size;
	//const dInt32 start = 0;
	//const dInt32 size = count;
	//const dInt32 batchSize = count;

	dInt32 histogram0[1 << (D_RADIX_DIGIT_SIZE + 1)];
	dInt32 histogram1[5][1 << D_RADIX_DIGIT_SIZE];
	memset(histogram0, 0, sizeof(histogram0));
	memset(histogram1, 0, sizeof(histogram1));

	ndGridHash* srcArray = &m_hashGridMap[0];
	for (dInt32 i = 0; i < count; i++)
	{
		const ndGridHash& entry = srcArray[i];
	
		const dInt32 xlow = dInt32(entry.m_xLow * 2 + entry.m_cellType);
		histogram0[xlow] = histogram0[xlow] + 1;
	
		const dInt32 xHigh = entry.m_xHigh;
		histogram1[0][xHigh] = histogram1[0][xHigh] + 1;
	
		const dInt32 ylow = entry.m_yLow;
		histogram1[1][ylow] = histogram1[1][ylow] + 1;
	
		const dInt32 yHigh = entry.m_yHigh;
		histogram1[2][yHigh] = histogram1[2][yHigh] + 1;
	
		const dInt32 zlow = entry.m_zLow;
		histogram1[3][zlow] = histogram1[3][zlow] + 1;
	
		const dInt32 zHigh = entry.m_zHigh;
		histogram1[4][zHigh] = histogram1[4][zHigh] + 1;
	}
	
	dInt32 acc0 = 0;
	for (dInt32 i = 0; i < (1 << (D_RADIX_DIGIT_SIZE + 1)); i++)
	{
		const dInt32 n = histogram0[i];
		histogram0[i] = acc0;
		acc0 += n;
	}
	dInt32 acc[5];
	memset(acc, 0, sizeof(acc));
	for (dInt32 i = 0; i < (1 << D_RADIX_DIGIT_SIZE); i++)
	{
		for (dInt32 j = 0; j < 5; j++)
		{
			const dInt32 n = histogram1[j][i];
			histogram1[j][i] = acc[j];
			acc[j] += n;
		}
	}
	
	dInt32 shiftbits = 0;
	dUnsigned64 mask = ~dUnsigned64(dInt64(-1 << D_RADIX_DIGIT_SIZE));
	ndGridHash* dstArray = &m_hashGridMapScratchBuffer[0];
	for (dInt32 i = 0; i < count; i++)
	{
		const ndGridHash& entry = srcArray[i];
		const dInt32 key = dUnsigned32((entry.m_gridHash & mask) >> shiftbits) * 2 + entry.m_cellType;
		const dInt32 index = histogram0[key];
		dstArray[index] = entry;
		histogram0[key] = index + 1;
	}
	mask <<= D_RADIX_DIGIT_SIZE;
	shiftbits += D_RADIX_DIGIT_SIZE;
	dSwap(dstArray, srcArray);
	
	if (m_upperDigisIsValid[0]) 
	{
		dInt32* const scan2 = &histogram1[0][0];
		for (dInt32 i = 0; i < count; i++)
		{
			const ndGridHash& entry = dstArray[i];
			const dInt32 key = dUnsigned32((entry.m_gridHash & mask) >> shiftbits);
			const dInt32 index = scan2[key];
			srcArray[index] = entry;
			scan2[key] = index + 1;
		}
		dSwap(dstArray, srcArray);
	}
	mask <<= D_RADIX_DIGIT_SIZE;
	shiftbits += D_RADIX_DIGIT_SIZE;
	
	for (dInt32 radix = 0; radix < 2; radix++)
	{
		dInt32* const scan0 = &histogram1[radix * 2 + 1][0];
		for (dInt32 i = 0; i < count; i++)
		{
			const ndGridHash& entry = srcArray[i];
			const dInt32 key = dUnsigned32((entry.m_gridHash & mask) >> shiftbits);
			const dInt32 index = scan0[key];
			dstArray[index] = entry;
			scan0[key] = index + 1;
		}
		mask <<= D_RADIX_DIGIT_SIZE;
		shiftbits += D_RADIX_DIGIT_SIZE;
		dSwap(dstArray, srcArray);
	
		if (m_upperDigisIsValid[radix + 1])
		{
			dInt32* const scan1 = &histogram1[radix * 2 + 2][0];
			for (dInt32 i = 0; i < count; i++)
			{
				const ndGridHash& entry = dstArray[i];
				const dInt32 key = dUnsigned32((entry.m_gridHash & mask) >> shiftbits);
				const dInt32 index = scan1[key];
				srcArray[index] = entry;
				scan1[key] = index + 1;
			}
			dSwap(dstArray, srcArray);
		}
		mask <<= D_RADIX_DIGIT_SIZE;
		shiftbits += D_RADIX_DIGIT_SIZE;
	}
	if (srcArray != &m_hashGridMap[0])
	{
		m_hashGridMap.Swap(m_hashGridMapScratchBuffer);
	}
	dAssert(srcArray == &m_hashGridMap[0]);
}

void ndBodySphFluid::AddCounters(const ndWorld* const world, ndContext& context) const
{
	D_TRACKTIME();

	dInt32 acc[1 << 11];
	memset(acc, 0, sizeof(acc));
	
	const dInt32 threadCount = world->GetThreadCount();
	for (dInt32 threadId = 0; threadId < threadCount; threadId++)
	{
		for (dInt32 i = 0; i < sizeof(context.m_scan) / sizeof(dInt32); i++)
		{
			dInt32 a = context.m_histogram[threadId][i];
			context.m_histogram[threadId][i] = acc[i] + context.m_scan[i];
			acc[i] += a;
		}
	}
}

void ndBodySphFluid::SortParallel(const ndWorld* const world)
{
	D_TRACKTIME();
	class ndBodySphFluidCountDigits : public ndScene::ndBaseJob
	{
		virtual void Execute()
		{
			D_TRACKTIME();

			ndWorld* const world = m_owner->GetWorld();
			ndContext* const context = (ndContext*)m_context;
			ndBodySphFluid* const fluid = context->m_fluid;
			const dInt32 threadId = GetThreadId();
			const dInt32 threadCount = world->GetThreadCount();
			
			const dInt32 count = fluid->m_hashGridMap.GetCount();
			const dInt32 size = count / threadCount;
			const dInt32 start = threadId * size;
			const dInt32 batchSize = (threadId == threadCount - 1) ? count - start : size;
			
			ndGridHash* const hashArray = &fluid->m_hashGridMap[start];
			dInt32* const histogram = context->m_histogram[threadId];
			if (context->m_pass)
			{
				memset(histogram, 0, sizeof(context->m_scan)/2);
				dInt32 shiftbits = context->m_pass * D_RADIX_DIGIT_SIZE;
				dUnsigned64 mask = ~dUnsigned64(dInt64(-1 << D_RADIX_DIGIT_SIZE));
				mask = mask << shiftbits;

				for (dInt32 i = 0; i < batchSize; i++)
				{
					const ndGridHash& entry = hashArray[i];
					const dInt32 key = dUnsigned32((entry.m_gridHash & mask) >> shiftbits);
					histogram[key] += 1;
				}
			}
			else
			{
				memset(histogram, 0, sizeof(context->m_scan));
				for (dInt32 i = 0; i < batchSize; i++)
				{
					const ndGridHash& entry = hashArray[i];
					const dInt32 xlow = dInt32(entry.m_xLow * 2 + entry.m_cellType);
					histogram[xlow] += 1;
				}
			}
		}
	};

	class ndBodySphFluidAddPartialSum : public ndScene::ndBaseJob
	{
		virtual void Execute()
		{
			D_TRACKTIME();
			ndWorld* const world = m_owner->GetWorld();
			ndContext* const context = (ndContext*)m_context;
			const dInt32 threadId = GetThreadId();
			const dInt32 threadCount = world->GetThreadCount();
			
			const dInt32 count = context->m_pass ? sizeof (context->m_scan) / sizeof (dInt32) : sizeof(context->m_scan) / (2 * sizeof(dInt32));
			const dInt32 size = count / threadCount;
			const dInt32 start = threadId * size;
			const dInt32 batchSize = (threadId == threadCount - 1) ? count - start : size;

			dInt32* const scan = context->m_scan;
			for (dInt32 i = 0; i < batchSize; i++)
			{
				dInt32 acc = 0;
				for (dInt32 j = 0; j < threadCount; j++)
				{
					acc += context->m_histogram[j][i + start];
				}
				scan[i + start] = acc;
			}
		}
	};

	class ndBodySphFluidReorderBuckets: public ndScene::ndBaseJob
	{
		virtual void Execute()
		{
			D_TRACKTIME();
			ndWorld* const world = m_owner->GetWorld();
			ndContext* const context = (ndContext*)m_context;
			ndBodySphFluid* const fluid = context->m_fluid;
			const dInt32 threadId = GetThreadId();
			const dInt32 threadCount = world->GetThreadCount();

			const dInt32 count = fluid->m_hashGridMap.GetCount();
			const dInt32 size = count / threadCount;
			const dInt32 start = threadId * size;
			const dInt32 batchSize = (threadId == threadCount - 1) ? count - start : size;

			ndGridHash* const srcArray = &fluid->m_hashGridMap[start];
			ndGridHash* const dstArray = &fluid->m_hashGridMapScratchBuffer[0];

			dInt32 shiftbits = context->m_pass * D_RADIX_DIGIT_SIZE;
			dUnsigned64 mask = ~dUnsigned64(dInt64(-1 << D_RADIX_DIGIT_SIZE));
			mask = mask << shiftbits;
			dInt32* const histogram = context->m_histogram[threadId];
			if (context->m_pass)
			{
				for (dInt32 i = 0; i < batchSize; i++)
				{
					const ndGridHash& entry = srcArray[i];
					const dInt32 key = dUnsigned32((entry.m_gridHash & mask) >> shiftbits);
					const dInt32 index = histogram[key];
					dstArray[index] = entry;
					histogram[key] = index + 1;
				}
			}
			else
			{
				for (dInt32 i = 0; i < batchSize; i++)
				{
					const ndGridHash& entry = srcArray[i];
					const dInt32 key = dUnsigned32((entry.m_gridHash & mask) >> shiftbits) * 2 + entry.m_cellType;
					const dInt32 index = histogram[key];
					dstArray[index] = entry;
					histogram[key] = index + 1;
				}
			}
		}
	};

	ndContext context;
	context.m_fluid = this;
	ndScene* const scene = world->GetScene();
	for (dInt32 pass = 0; pass < 6; pass++)
	{
		if (!(pass & 1) || m_upperDigisIsValid[pass >> 1])
		{
			context.m_pass = pass;
			scene->SubmitJobs<ndBodySphFluidCountDigits>(&context);
			scene->SubmitJobs<ndBodySphFluidAddPartialSum>(&context);

			dInt32 acc = 0;
			for (dInt32 i = 0; i < sizeof(context.m_scan) / sizeof(dInt32); i++)
			{
				dInt32 sum = context.m_scan[i];
				context.m_scan[i] = acc;
				acc += sum;
			}
			AddCounters(world, context);

			scene->SubmitJobs<ndBodySphFluidReorderBuckets>(&context);
			m_hashGridMap.Swap(m_hashGridMapScratchBuffer);
		}
	}	
}

void ndBodySphFluid::SortGrids(const ndWorld* const world)
{
	const dInt32 threadCount = world->GetThreadCount();
	m_hashGridMapScratchBuffer.SetCount(m_hashGridMap.GetCount());
	if (threadCount <= 1)
	{
		dAssert(threadCount == 1);
		SortSingleThreaded(world);
	}
	else
	{
		SortParallel(world);
	}

#ifdef _DEBUG
	for (dInt32 i = 0; i < (m_hashGridMap.GetCount() - 1); i++)
	{
		const ndGridHash& entry0 = m_hashGridMap[i + 0];
		const ndGridHash& entry1 = m_hashGridMap[i + 1];
		dUnsigned64 gridHashA = entry0.m_gridHash * 2 + entry0.m_cellType;
		dUnsigned64 gridHashB = entry1.m_gridHash * 2 + entry1.m_cellType;
		dAssert(gridHashA <= gridHashB);
	}
#endif
}

D_NEWTON_API void ndBodySphFluid::GenerateIsoSurface(const ndWorld* const world)
{
	D_TRACKTIME();
	dVector boxP0;
	dVector boxP1;
	CaculateAABB(world, boxP0, boxP1);

	dFloat32 gridSize = m_radius * dFloat32(2.0f);
	const dVector invGridSize (dFloat32 (1.0f) / gridSize);

	dVector padd(dFloat32(2.0f) * gridSize);
	boxP0 -= padd & dVector::m_triplexMask;
	boxP1 += padd & dVector::m_triplexMask;

	m_hashGridMap.SetCount(m_posit.GetCount());
	m_hashGridMapScratchBuffer.SetCount(m_posit.GetCount());
	const dVector* const posit = &m_posit[0];
	
	for (dInt32 i = m_posit.GetCount() - 1; i >= 0; i--)
	{
		dVector r(posit[i] - boxP0);
		dVector p(r * invGridSize);
		ndGridHash hashKey(p, i, ndHomeGrid);
		m_hashGridMap[i] = hashKey;
	}

	ndContext context;
	context.m_fluid = this;

	SortSingleThreaded(world);
	dInt32 uniqueCount = 0;
	for (dInt32 i = 0; i < m_hashGridMap.GetCount();)
	{
		dUnsigned64 key0 = m_hashGridMap[i].m_gridHash;
		m_hashGridMap[uniqueCount].m_gridHash = m_hashGridMap[i].m_gridHash;
		uniqueCount++;
		for (i ++; (i < m_hashGridMap.GetCount()) && (key0 == m_hashGridMap[i].m_gridHash); i++);
	}

	ndGridHash hashBox0(dVector::m_zero, 0, ndHomeGrid);
	ndGridHash hashBox1((boxP1 - boxP0) * invGridSize, 0, ndHomeGrid);

	dUnsigned64 cellCount = (hashBox1.m_z - hashBox0.m_z) * (hashBox1.m_y - hashBox0.m_y) * (hashBox1.m_x - hashBox0.m_x);

	//if (cellCount <= 128)
	if (cellCount <= 256)
	{
		dAssert((hashBox1.m_z - hashBox0.m_z) > 1);
		dAssert((hashBox1.m_y - hashBox0.m_y) > 1);
		dAssert((hashBox1.m_x - hashBox0.m_x) > 1);

		const int x_ = 6;
		const int y_ = 6;
		const int z_ = 20;
		dFloat32 xxx[z_][y_][x_];
		memset(xxx, 0, sizeof(xxx));
		for (dInt32 i = 0; i < uniqueCount; i++)
		{
			dInt32 x = m_hashGridMap[i].m_x;
			dInt32 y = m_hashGridMap[i].m_y;
			dInt32 z = m_hashGridMap[i].m_z;
			xxx[z][y][x] = 1.0f;
		}
		
		dInt32 gridCountX = dInt32(hashBox1.m_x - hashBox0.m_x) + 32;
		dInt32 gridCountY = dInt32(hashBox1.m_y - hashBox0.m_y) + 32;
		dInt32 gridCountZ = dInt32(hashBox1.m_z - hashBox0.m_z) + 32;
		m_isoSurcase.Begin(boxP0, dFloat32(0.5f), gridSize, gridCountX, gridCountY, gridCountZ);
		
		dIsoSurface::dIsoCell cell;
		for (dInt32 z = 0; z < z_-1; z++)
		{
			cell.m_z = z;
			for (dInt32 y = 0; y < y_-1; y++)
			{
				cell.m_y = y;
				for (dInt32 x = 0; x < x_-1; x++)
				{
					cell.m_x = x;
					cell.m_isoValues[0][0][0] = xxx[z + 0][y + 0][x + 0];
					cell.m_isoValues[0][0][1] = xxx[z + 0][y + 0][x + 1];
					cell.m_isoValues[0][1][0] = xxx[z + 0][y + 1][x + 0];
					cell.m_isoValues[0][1][1] = xxx[z + 0][y + 1][x + 1];
					cell.m_isoValues[1][0][0] = xxx[z + 1][y + 0][x + 0];
					cell.m_isoValues[1][0][1] = xxx[z + 1][y + 0][x + 1];
					cell.m_isoValues[1][1][0] = xxx[z + 1][y + 1][x + 0];
					cell.m_isoValues[1][1][1] = xxx[z + 1][y + 1][x + 1];
					m_isoSurcase.ProcessCell(cell);
				}
			}
		}
		m_isoSurcase.End();
	}
}