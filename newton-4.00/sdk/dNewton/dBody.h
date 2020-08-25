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

#ifndef _D_BODY_H_
#define _D_BODY_H_

#include "dNewtonStdafx.h"

class dNewton;

D_MSC_VECTOR_ALIGNMENT
class dBody: public dClassAlloc  
{
	public:
	D_NEWTON_API dBody();
	D_NEWTON_API virtual ~dBody();

	D_NEWTON_API virtual void ApplyExternalForces(dInt32 threadID, dFloat32 tiemstep) = 0;
	dNewton* GetNewton() const;

	private:
	dList<dBody*>::dListNode* GetNewtonNode() const;
	void SetNewtonNode(dNewton* const newton, dList<dBody*>::dListNode* const node);

	dNewton* m_newton;
	dList<dBody*>::dListNode* m_newtonNode;
	friend class dNewton;
} D_GCC_VECTOR_ALIGNMENT;


inline dNewton* dBody::GetNewton() const
{
	return m_newton;
}

inline void dBody::SetNewtonNode(dNewton* const newton, dList<dBody*>::dListNode* const node)
{
	m_newton = newton;
	m_newtonNode = node;
}

inline dList<dBody*>::dListNode* dBody::GetNewtonNode() const
{
	return m_newtonNode;
}

#endif 
