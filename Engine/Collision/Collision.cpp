#include <vector>
#include "Collision.h"



using std::vector;
using namespace Engine;


vector<Object*>	m_CollisionObjects;




float DegreesToRadians(float i_Degrees)
{
	static const float DegToRad = float(M_PI) / 180.0f;

	return i_Degrees * DegToRad;
}


//float RandInRange(float i_Min, float i_Max)
//{
//	return i_Min + (static_cast<float>(rand()) / (RAND_MAX + 1) * (i_Max - i_Min + 1.0f));
//}


Vector3<float> GenerateRandomVector(const Vector3<float>& i_Min, const Vector3<float>& i_Max)
{
	return Vector3<float>(
		RandInRange(i_Min[0], i_Max[0]),
		RandInRange(i_Min[1], i_Max[1]),
		RandInRange(i_Min[2], i_Max[2]));
}


bool DetectCrossTimes(float i_Center, float i_Extent, float i_Point, float i_Travel, float& o_tEnter, float& o_tExit)
{
	// Extent must be a positive scalar
	assert(i_Extent > 0.0f);

	// calculate left (assumed) and right extents 
	float i_Start = i_Center - i_Extent;
	float i_End = i_Center + i_Extent;

	// handle case where comparator is not moving along this axis
	if (IsZero(i_Travel))
	{
		// if check point is outside of extents it will never be in collision state
		// otherwise it will always be in collision state
		if ((i_Point < i_Start) || (i_Point > i_End))
			return false;
		else
		{
			o_tEnter = 0.0f;
			o_tExit = 1.0f;
			return true;
		}
	}

	// calculate time to enter collision state and time to exit collision state
	o_tEnter = (i_Start - i_Point) / i_Travel;
	o_tExit = (i_End - i_Point) / i_Travel;

	// if time to enter collision state is AFTER time to exit collision state we need to reverse them
	// as we assumed comparator was on left (more negative) side and moving in a positive direction.
	// reversing the values fixes this
	if (o_tEnter > o_tExit)
		Swap(o_tEnter, o_tExit);

	return !((o_tEnter >= 1.0f) || (o_tExit <= 0.0f));
}


bool CheckSeparationAlongAxis(const Vector3<float>& i_CollisionAxis, 
	const Object& i_Obj1, const Vector3<float>& i_Obj1BBCenterInWorld,
	const Object& i_Obj2, const Vector3<float>& i_Obj2BBCenterInWorld, float i_dt,
	float& i_tEnter, float& i_tExit)
{

	float Obj1BBCenterOnAxis = i_Obj1BBCenterInWorld.Dot(i_CollisionAxis); 
	float Obj2BBCenterOnAxis = i_Obj2BBCenterInWorld.Dot(i_CollisionAxis);

	float Obj1ProjectedExtents = float(
		fabs(i_CollisionAxis.Dot(i_Obj1.m_Orientation.GetCol(0).GetNorm() * i_Obj1.m_BB.extent[0])) +
		fabs(i_CollisionAxis.Dot(i_Obj1.m_Orientation.GetCol(1).GetNorm() * i_Obj1.m_BB.extent[1])) +
		fabs(i_CollisionAxis.Dot(i_Obj1.m_Orientation.GetCol(2).GetNorm() * i_Obj1.m_BB.extent[2])));

	float Obj2ProjectedExtents = float(
		fabs(i_CollisionAxis.Dot(i_Obj2.m_Orientation.GetCol(0).GetNorm() * i_Obj2.m_BB.extent[0])) +
		fabs(i_CollisionAxis.Dot(i_Obj2.m_Orientation.GetCol(1).GetNorm() * i_Obj2.m_BB.extent[1])) +
		fabs(i_CollisionAxis.Dot(i_Obj2.m_Orientation.GetCol(2).GetNorm() * i_Obj2.m_BB.extent[2])));


	float Obj2ExpandedExtents = Obj2ProjectedExtents + Obj1ProjectedExtents;

	float Obj1VelOnAxis = i_Obj1.m_Velocity.Dot(i_CollisionAxis);
	float Obj2VelOnAxis = i_Obj2.m_Velocity.Dot(i_CollisionAxis);

	float Obj1RelVel = Obj1VelOnAxis - Obj2VelOnAxis;

	float Obj1TravelAlongAxis = Obj1RelVel * i_dt;

	i_tEnter = 0.0f;
	i_tExit = 1.0f;

	return !DetectCrossTimes(Obj2BBCenterOnAxis, Obj2ExpandedExtents, Obj1BBCenterOnAxis, Obj1TravelAlongAxis, i_tEnter, i_tExit);
}


CollisionPair FindCollision(float i_dt)
{
	CollisionPair CurrentCollision = { { 0, 0 }, 0.0f };

	const size_t count = m_CollisionObjects.size();

	for (size_t i = 0; i < (count - 1); i++)
	{
		for (size_t j = i + 1; j < count; j++)
		{
			assert(m_CollisionObjects[i]);
			assert(m_CollisionObjects[j]);

			float tCollision = 0.0f;

			if (CheckCollision(*m_CollisionObjects[i], *m_CollisionObjects[j], i_dt, tCollision))
			{
				if (CurrentCollision.m_pObjects[0])
				{
					if (tCollision < CurrentCollision.m_CollisionTime)
					{
						CurrentCollision.m_pObjects[0] = m_CollisionObjects[i];
						CurrentCollision.m_pObjects[1] = m_CollisionObjects[j];
						CurrentCollision.m_CollisionTime = tCollision;
					}
				}
				else
				{
					CurrentCollision.m_pObjects[0] = m_CollisionObjects[i];
					CurrentCollision.m_pObjects[1] = m_CollisionObjects[j];
					CurrentCollision.m_CollisionTime = tCollision;
				}
			}
		}
	}
	return CurrentCollision;
}


bool CheckCollision(const Object& i_Obj1, const Object& i_Obj2, float i_dt, float& o_tCollision)
{

	Matrix4<float> Obj1ToWorld = Matrix4<float>::CreateTranslation(i_Obj1.m_Position) * i_Obj1.m_Orientation.ToTransform();
	Matrix4<float> Obj2ToWorld = Matrix4<float>::CreateTranslation(i_Obj2.m_Position) * i_Obj2.m_Orientation.ToTransform();

	Vector3<float> Obj1BBCenterInWorld = Obj1ToWorld.TransformPoint(i_Obj1.m_BB.center);
	Vector3<float> Obj2BBCenterInWorld = Obj2ToWorld.TransformPoint(i_Obj2.m_BB.center);

	float tEnter = 0.0f;
	float tExit = 1.0f;

	float axisEnter = 0.0f;
	float axisExit = 1.0f;

	Vector3<float> CollisionAxis;

	CollisionAxis = i_Obj1.m_Orientation.GetCol(0).GetNorm();
	if (CheckSeparationAlongAxis(CollisionAxis, i_Obj1, Obj1BBCenterInWorld, i_Obj2, Obj2BBCenterInWorld, i_dt, axisEnter, axisExit))
	{
		DEBUG_PRINT("Found Separation along Obj1 X Axis \n");
		return false;
	}
	else
	{
		DEBUG_PRINT("Found Overlap along Obj1 X Axis from %f to %f \n", axisEnter, axisExit);
		if (axisEnter > tEnter)
		{
			DEBUG_PRINT("Updating tEnter to %f\n", axisEnter);
			tEnter = axisEnter;
		}
		if (axisExit < tExit)
		{
			DEBUG_PRINT("Updating tExit to %f\n", axisEnter);
			tExit = axisExit;
		}
	}

	CollisionAxis = i_Obj1.m_Orientation.GetCol(1).GetNorm();
	if (CheckSeparationAlongAxis(CollisionAxis, i_Obj1, Obj1BBCenterInWorld, i_Obj2, Obj2BBCenterInWorld, i_dt, axisEnter, axisExit))
	{
		DEBUG_PRINT("Found Separation along Obj1 Y Axis \n");
		return false;
	}
	else
	{
		DEBUG_PRINT("Found Overlap along Obj1 Y Axis from %f to %f \n", axisEnter, axisExit);
		if (axisEnter > tEnter)
		{
			DEBUG_PRINT("Updating tEnter to %f\n", axisEnter);
			tEnter = axisEnter;
		}
		if (axisExit < tExit)
		{
			DEBUG_PRINT("Updating tExit to %f\n", axisEnter);
			tExit = axisExit;
		}
	}

	CollisionAxis = i_Obj1.m_Orientation.GetCol(2).GetNorm();
	if (CheckSeparationAlongAxis(CollisionAxis, i_Obj1, Obj1BBCenterInWorld, i_Obj2, Obj2BBCenterInWorld, i_dt, axisEnter, axisExit))
	{
		DEBUG_PRINT("Found Separation along Obj1 Z Axis \n");
		return false;
	}
	else
	{
		DEBUG_PRINT("Found Overlap along Obj1 Z Axis from %f to %f \n", axisEnter, axisExit);
		if (axisEnter > tEnter)
		{
			DEBUG_PRINT("Updating tEnter to %f\n", axisEnter);
			tEnter = axisEnter;
		}
		if (axisExit < tExit)
		{
			DEBUG_PRINT("Updating tExit to %f\n", axisEnter);
			tExit = axisExit;
		}
	}

	CollisionAxis = i_Obj2.m_Orientation.GetCol(0).GetNorm();
	if (CheckSeparationAlongAxis(CollisionAxis, i_Obj2, Obj2BBCenterInWorld, i_Obj1, Obj1BBCenterInWorld, i_dt, axisEnter, axisExit))
	{
		DEBUG_PRINT("Found Separation along Obj2 X Axis \n");
		return false;
	}
	else
	{
		DEBUG_PRINT("Found Overlap along Obj2 X Axis from %f to %f \n", axisEnter, axisExit);
		if (axisEnter > tEnter)
		{
			DEBUG_PRINT("Updating tEnter to %f\n", axisEnter);
			tEnter = axisEnter;
		}
		if (axisExit < tExit)
		{
			DEBUG_PRINT("Updating tExit to %f\n", axisEnter);
			tExit = axisExit;
		}
	}

	CollisionAxis = i_Obj2.m_Orientation.GetCol(1).GetNorm();
	if (CheckSeparationAlongAxis(CollisionAxis, i_Obj2, Obj2BBCenterInWorld, i_Obj1, Obj1BBCenterInWorld, i_dt, axisEnter, axisExit))
	{
		DEBUG_PRINT("Found Separation along Obj2 Y Axis \n");
		return false;
	}
	else
	{
		DEBUG_PRINT("Found Overlap along Obj2 Y Axis from %f to %f \n", axisEnter, axisExit);
		if (axisEnter > tEnter)
		{
			DEBUG_PRINT("Updating tEnter to %f\n", axisEnter);
			tEnter = axisEnter;
		}
		if (axisExit < tExit)
		{
			DEBUG_PRINT("Updating tExit to %f\n", axisEnter);
			tExit = axisExit;
		}
	}

	CollisionAxis = i_Obj2.m_Orientation.GetCol(2).GetNorm();
	if (CheckSeparationAlongAxis(CollisionAxis, i_Obj2, Obj2BBCenterInWorld, i_Obj1, Obj1BBCenterInWorld, i_dt, axisEnter, axisExit))
	{
		DEBUG_PRINT("Found Separation along Obj1 Z Axis\n");
		return false;
	}
	else
	{
		DEBUG_PRINT("Found Overlap along Obj1 Z Axis from %f to %f\n", axisEnter, axisExit);
		if (axisEnter > tEnter)
		{
			DEBUG_PRINT("Updating tEnter to %f\n", axisEnter);
			tEnter = axisEnter;
		}
		if (axisExit < tExit)
		{
			DEBUG_PRINT("Updating tExit to %f\n", axisEnter);
			tExit = axisExit;
		}
	}

	CollisionAxis = i_Obj1.m_Orientation.GetCol(0).GetNorm().Cross(i_Obj2.m_Orientation.GetCol(0).GetNorm());
	if (!IsZero(CollisionAxis.Length()))
	{
		if (CheckSeparationAlongAxis(CollisionAxis, i_Obj1, Obj1BBCenterInWorld, i_Obj2, Obj2BBCenterInWorld, i_dt, axisEnter, axisExit))
		{
			DEBUG_PRINT("Found Separation along Obj1 X x Obj2 X Axis\n");
			return false;
		}
		else
		{
			DEBUG_PRINT("Found Overlap along Obj1 X x Obj2 X Axis from %f to %f\n", axisEnter, axisExit);
			if (axisEnter > tEnter)
			{
				DEBUG_PRINT("Updating tEnter to %f\n", axisEnter);
				tEnter = axisEnter;
			}
			if (axisExit < tExit)
			{
				DEBUG_PRINT("Updating tExit to %f\n", axisEnter);
				tExit = axisExit;
			}
		}
	}

	CollisionAxis = i_Obj1.m_Orientation.GetCol(0).GetNorm().Cross(i_Obj2.m_Orientation.GetCol(1).GetNorm());
	if (!IsZero(CollisionAxis.Length()))
	{

		if (CheckSeparationAlongAxis(CollisionAxis, i_Obj1, Obj1BBCenterInWorld, i_Obj2, Obj2BBCenterInWorld, i_dt, axisEnter, axisExit))
		{
			DEBUG_PRINT("Found Separation along Obj1 X x Obj2 Y Axis\n");
			return false;
		}
		else
		{
			DEBUG_PRINT("Found Overlap along Obj1 X x Obj2 Y Axis from %f to %f\n", axisEnter, axisExit);
			if (axisEnter > tEnter)
			{
				DEBUG_PRINT("Updating tEnter to %f\n", axisEnter);
				tEnter = axisEnter;
			}
			if (axisExit < tExit)
			{
				DEBUG_PRINT("Updating tExit to %f\n", axisEnter);
				tExit = axisExit;
			}
		}
	}

	CollisionAxis = i_Obj1.m_Orientation.GetCol(0).GetNorm().Cross(i_Obj2.m_Orientation.GetCol(2).GetNorm());
	if (!IsZero(CollisionAxis.Length()))
	{
		if (CheckSeparationAlongAxis(CollisionAxis, i_Obj1, Obj1BBCenterInWorld, i_Obj2, Obj2BBCenterInWorld, i_dt, axisEnter, axisExit))
		{
			DEBUG_PRINT("Found Separation along Obj1 X x Obj2 Z Axis\n");
			return false;
		}
		else
		{
			DEBUG_PRINT("Found Overlap along Obj1 X x Obj2 Z Axis from %f to %f\n", axisEnter, axisExit);
			if (axisEnter > tEnter)
			{
				DEBUG_PRINT("Updating tEnter to %f\n", axisEnter);
				tEnter = axisEnter;
			}
			if (axisExit < tExit)
			{
				DEBUG_PRINT("Updating tExit to %f\n", axisEnter);
				tExit = axisExit;
			}
		}
	}

	CollisionAxis = i_Obj1.m_Orientation.GetCol(1).GetNorm().Cross(i_Obj2.m_Orientation.GetCol(0).GetNorm());
	if (!IsZero(CollisionAxis.Length()))
	{
		if (CheckSeparationAlongAxis(CollisionAxis, i_Obj1, Obj1BBCenterInWorld, i_Obj2, Obj2BBCenterInWorld, i_dt, axisEnter, axisExit))
		{
			DEBUG_PRINT("Found Separation along Obj1 Y x Obj2 X Axis\n");
			return false;
		}
		else
		{
			DEBUG_PRINT("Found Overlap along Obj1 Y x Obj2 X Axis from %f to %f\n", axisEnter, axisExit);
			if (axisEnter > tEnter)
			{
				DEBUG_PRINT("Updating tEnter to %f\n", axisEnter);
				tEnter = axisEnter;
			}
			if (axisExit < tExit)
			{
				DEBUG_PRINT("Updating tExit to %f\n", axisEnter);
				tExit = axisExit;
			}
		}
	}

	CollisionAxis = i_Obj1.m_Orientation.GetCol(1).GetNorm().Cross(i_Obj2.m_Orientation.GetCol(1).GetNorm());
	if (!IsZero(CollisionAxis.Length()))
	{
		if (CheckSeparationAlongAxis(CollisionAxis, i_Obj1, Obj1BBCenterInWorld, i_Obj2, Obj2BBCenterInWorld, i_dt, axisEnter, axisExit))
		{
			DEBUG_PRINT("Found Separation along Obj1 Y x Obj2 Y Axis\n");
			return false;
		}
		else
		{
			DEBUG_PRINT("Found Overlap along Obj1 Y x Obj2 Y Axis from %f to %f\n", axisEnter, axisExit);
			if (axisEnter > tEnter)
			{
				DEBUG_PRINT("Updating tEnter to %f\n", axisEnter);
				tEnter = axisEnter;
			}
			if (axisExit < tExit)
			{
				DEBUG_PRINT("Updating tExit to %f\n", axisEnter);
				tExit = axisExit;
			}
		}
	}

	CollisionAxis = i_Obj1.m_Orientation.GetCol(1).GetNorm().Cross(i_Obj2.m_Orientation.GetCol(2).GetNorm());
	if (!IsZero(CollisionAxis.Length()))
	{
		if (CheckSeparationAlongAxis(CollisionAxis, i_Obj1, Obj1BBCenterInWorld, i_Obj2, Obj2BBCenterInWorld, i_dt, axisEnter, axisExit))
		{
			DEBUG_PRINT("Found Separation along Obj1 Y x Obj2 Z Axis\n");
			return false;
		}
		else
		{
			DEBUG_PRINT("Found Overlap along Obj1 Y x Obj2 Z Axis from %f to %f\n", axisEnter, axisExit);
			if (axisEnter > tEnter)
			{
				DEBUG_PRINT("Updating tEnter to %f\n", axisEnter);
				tEnter = axisEnter;
			}
			if (axisExit < tExit)
			{
				DEBUG_PRINT("Updating tExit to %f\n", axisEnter);
				tExit = axisExit;
			}
		}
	}

	CollisionAxis = i_Obj1.m_Orientation.GetCol(2).GetNorm().Cross(i_Obj2.m_Orientation.GetCol(0).GetNorm());
	if (!IsZero(CollisionAxis.Length()))
	{
		if (CheckSeparationAlongAxis(CollisionAxis, i_Obj1, Obj1BBCenterInWorld, i_Obj2, Obj2BBCenterInWorld, i_dt, axisEnter, axisExit))
		{
			DEBUG_PRINT("Found Separation along Obj1 Z x Obj2 X Axis\n");
			return false;
		}
		else
		{
			DEBUG_PRINT("Found Overlap along Obj1 Z x Obj2 X Axis from %f to %f\n", axisEnter, axisExit);
			if (axisEnter > tEnter)
			{
				DEBUG_PRINT("Updating tEnter to %f\n", axisEnter);
				tEnter = axisEnter;
			}
			if (axisExit < tExit)
			{
				DEBUG_PRINT("Updating tExit to %f\n", axisEnter);
				tExit = axisExit;
			}
		}
	}

	CollisionAxis = i_Obj1.m_Orientation.GetCol(2).GetNorm().Cross(i_Obj2.m_Orientation.GetCol(1).GetNorm());
	if (!IsZero(CollisionAxis.Length()))
	{
		if (CheckSeparationAlongAxis(CollisionAxis, i_Obj1, Obj1BBCenterInWorld, i_Obj2, Obj2BBCenterInWorld, i_dt, axisEnter, axisExit))
		{
			DEBUG_PRINT("Found Separation along Obj1 Z x Obj2 Y Axis\n");
			return false;
		}
		else
		{
			DEBUG_PRINT("Found Overlap along Obj1 Z x Obj2 Y Axis from %f to %f\n", axisEnter, axisExit);
			if (axisEnter > tEnter)
			{
				DEBUG_PRINT("Updating tEnter to %f\n", axisEnter);
				tEnter = axisEnter;
			}
			if (axisExit < tExit)
			{
				DEBUG_PRINT("Updating tExit to %f\n", axisEnter);
				tExit = axisExit;
			}
		}
	}

	CollisionAxis = i_Obj1.m_Orientation.GetCol(2).GetNorm().Cross(i_Obj2.m_Orientation.GetCol(2).GetNorm());
	if (!IsZero(CollisionAxis.Length()))
	{
		if (CheckSeparationAlongAxis(CollisionAxis, i_Obj1, Obj1BBCenterInWorld, i_Obj2, Obj2BBCenterInWorld, i_dt, axisEnter, axisExit))
		{
			DEBUG_PRINT("Found Separation along Obj1 Z x Obj2 Z Axis\n");
			return false;
		}
		else
		{
			DEBUG_PRINT("Found Overlap along Obj1 Z x Obj2 Z Axis from %f to %f\n", axisEnter, axisExit);
			if (axisEnter > tEnter)
			{
				DEBUG_PRINT("Updating tEnter to %f\n", axisEnter);
				tEnter = axisEnter;
			}
			if (axisExit < tExit)
			{
				DEBUG_PRINT("Updating tExit to %f\n", axisEnter);
				tExit = axisExit;
			}
		}
	}

	if (tEnter < tExit)
	{
		o_tCollision = tEnter;
		return true;
	}
	else
		return false;
}


bool Check2DCollision(const Object& i_Obj1, const Object& i_Obj2, float i_dt, float& i_tCollision, float& o_)
{

	Matrix4<float> Obj1ToWorld = Matrix4<float>::CreateTranslation(i_Obj1.m_Position) * i_Obj1.m_Orientation.ToTransform();
	Matrix4<float> Obj2ToWorld = Matrix4<float>::CreateTranslation(i_Obj2.m_Position) * i_Obj2.m_Orientation.ToTransform();

	Vector3<float> Obj1BBCenterInWorld = Obj1ToWorld.TransformPoint(i_Obj1.m_BB.center);
	Vector3<float> Obj2BBCenterInWorld = Obj2ToWorld.TransformPoint(i_Obj2.m_BB.center);

	float tEnter = 0.0f;
	float tExit = 1.0f;

	// Obj2 vs Obj1
	Vector3<float> Obj2RelVel = i_Obj2.m_Velocity - i_Obj1.m_Velocity;
	Vector3<float> Obj2Travel = Obj2RelVel * i_dt;

	// Obj1 X Axis
	{
		Vector3<float> Obj1XAxisInWorld = i_Obj1.m_Orientation.GetCol(0).GetNorm();

		float Obj1BBCenterOnAxis = Obj1BBCenterInWorld.Dot(Obj1XAxisInWorld);
		float Obj2BBCenterOnAxis = Obj2BBCenterInWorld.Dot(Obj1XAxisInWorld);

		float Obj2ProjectedExtents = float(
			fabs(Obj1XAxisInWorld.Dot(i_Obj2.m_Orientation.GetCol(0).GetNorm() * i_Obj2.m_BB.extent[0])) +
			fabs(Obj1XAxisInWorld.Dot(i_Obj2.m_Orientation.GetCol(1).GetNorm() * i_Obj2.m_BB.extent[1])) +
			fabs(Obj1XAxisInWorld.Dot(i_Obj2.m_Orientation.GetCol(2).GetNorm() * i_Obj2.m_BB.extent[2])));

		float Obj1ExpandedExtents = i_Obj1.m_BB.extent[0] + Obj2ProjectedExtents;

		float Obj2TravelAlongAxis = Obj2Travel.Dot(Obj1XAxisInWorld);

		float axisEnter = 0.0f;
		float axisExit = 1.0f;

		if (!DetectCrossTimes(Obj1BBCenterOnAxis, Obj1ExpandedExtents, Obj2BBCenterOnAxis, Obj2TravelAlongAxis, axisEnter, axisExit))
			return false;

		if (axisEnter > tEnter)
			tEnter = axisEnter;
		if (axisExit < tExit)
			tExit = axisExit;
	}

	// Obj1 Y Axis
	{
		Vector3<float> Obj1YAxisInWorld = i_Obj1.m_Orientation.GetCol(1).GetNorm();

		float Obj1BBCenterOnAxis = Obj1BBCenterInWorld.Dot(Obj1YAxisInWorld);
		float Obj2BBCenterOnAxis = Obj2BBCenterInWorld.Dot(Obj1YAxisInWorld);

		float Obj2ProjectedExtents = float(
			fabs(Obj1YAxisInWorld.Dot(i_Obj2.m_Orientation.GetCol(0).GetNorm() * i_Obj2.m_BB.extent[0])) +
			fabs(Obj1YAxisInWorld.Dot(i_Obj2.m_Orientation.GetCol(1).GetNorm() * i_Obj2.m_BB.extent[1])) +
			fabs(Obj1YAxisInWorld.Dot(i_Obj2.m_Orientation.GetCol(2).GetNorm() * i_Obj2.m_BB.extent[2])));

		float Obj1ExpandedExtents = i_Obj1.m_BB.extent[1] + Obj2ProjectedExtents;

		float Obj2TravelAlongAxis = Obj2Travel.Dot(Obj1YAxisInWorld);

		float axisEnter = 0.0f;
		float axisExit = 1.0f;

		if (!DetectCrossTimes(Obj1BBCenterOnAxis, Obj1ExpandedExtents, Obj2BBCenterOnAxis, Obj2TravelAlongAxis, axisEnter, axisExit))
			return false;

		if (axisEnter > tEnter)
			tEnter = axisEnter;
		if (axisExit < tExit)
			tExit = axisExit;
	}

	// Obj1 Z Axis
	{
		Vector3<float> Obj1ZAxisInWorld = i_Obj1.m_Orientation.GetCol(2).GetNorm();

		float Obj1BBCenterOnAxis = Obj1BBCenterInWorld.Dot(Obj1ZAxisInWorld);
		float Obj2BBCenterOnAxis = Obj2BBCenterInWorld.Dot(Obj1ZAxisInWorld);

		float Obj2ProjectedExtents = float(
			fabs(Obj1ZAxisInWorld.Dot(i_Obj2.m_Orientation.GetCol(0).GetNorm() * i_Obj2.m_BB.extent[0])) +
			fabs(Obj1ZAxisInWorld.Dot(i_Obj2.m_Orientation.GetCol(1).GetNorm() * i_Obj2.m_BB.extent[1])) +
			fabs(Obj1ZAxisInWorld.Dot(i_Obj2.m_Orientation.GetCol(2).GetNorm() * i_Obj2.m_BB.extent[2])));

		float Obj1ExpandedExtents = i_Obj1.m_BB.extent[2] + Obj2ProjectedExtents;

		float Obj2TravelAlongAxis = Obj2Travel.Dot(Obj1ZAxisInWorld);

		float axisEnter = 0.0f;
		float axisExit = 1.0f;

		if (!DetectCrossTimes(Obj1BBCenterOnAxis, Obj1ExpandedExtents, Obj2BBCenterOnAxis, Obj2TravelAlongAxis, axisEnter, axisExit))
			return false;

		if (axisEnter > tEnter)
			tEnter = axisEnter;
		if (axisExit < tExit)
			tExit = axisExit;
	}

	// Obj1 vs Obj2
	Vector3<float> Obj1RelVel = i_Obj1.m_Velocity - i_Obj2.m_Velocity;
	Vector3<float> Obj1Travel = Obj1RelVel * i_dt;

	// Obj2 X Axis
	{
		Vector3<float> Obj2XAxisInWorld = i_Obj2.m_Orientation.GetCol(0).GetNorm();

		float Obj2BBCenterOnAxis = Obj2BBCenterInWorld.Dot(Obj2XAxisInWorld);
		float Obj1BBCenterOnAxis = Obj1BBCenterInWorld.Dot(Obj2XAxisInWorld);

		float Obj1ProjectedExtents = float(
			fabs(Obj2XAxisInWorld.Dot(i_Obj1.m_Orientation.GetCol(0).GetNorm() * i_Obj1.m_BB.extent[0])) +
			fabs(Obj2XAxisInWorld.Dot(i_Obj1.m_Orientation.GetCol(1).GetNorm() * i_Obj1.m_BB.extent[1])) +
			fabs(Obj2XAxisInWorld.Dot(i_Obj1.m_Orientation.GetCol(2).GetNorm() * i_Obj1.m_BB.extent[2])));

		float Obj2ExpandedExtents = i_Obj2.m_BB.extent[0] + Obj1ProjectedExtents;

		float Obj1TravelAlongAxis = Obj1Travel.Dot(Obj2XAxisInWorld);

		float axisEnter = 0.0f;
		float axisExit = 1.0f;

		if (!DetectCrossTimes(Obj2BBCenterOnAxis, Obj2ExpandedExtents, Obj1BBCenterOnAxis, Obj1TravelAlongAxis, axisEnter, axisExit))
			return false;

		if (axisEnter > tEnter)
			tEnter = axisEnter;
		if (axisExit < tExit)
			tExit = axisExit;
	}

	// Obj2 Y Axis
	{
		Vector3<float> Obj2YAxisInWorld = i_Obj2.m_Orientation.GetCol(1).GetNorm();

		float Obj2BBCenterOnAxis = Obj2BBCenterInWorld.Dot(Obj2YAxisInWorld);
		float Obj1BBCenterOnAxis = Obj1BBCenterInWorld.Dot(Obj2YAxisInWorld);

		float Obj1ProjectedExtents = float(
			fabs(Obj2YAxisInWorld.Dot(i_Obj1.m_Orientation.GetCol(0).GetNorm() * i_Obj1.m_BB.extent[0])) +
			fabs(Obj2YAxisInWorld.Dot(i_Obj1.m_Orientation.GetCol(1).GetNorm() * i_Obj1.m_BB.extent[1])) +
			fabs(Obj2YAxisInWorld.Dot(i_Obj1.m_Orientation.GetCol(2).GetNorm() * i_Obj1.m_BB.extent[2])));

		float Obj2ExpandedExtents = i_Obj2.m_BB.extent[1] + Obj1ProjectedExtents;

		float Obj1TravelAlongAxis = Obj1Travel.Dot(Obj2YAxisInWorld);

		float axisEnter = 0.0f;
		float axisExit = 1.0f;

		if (!DetectCrossTimes(Obj2BBCenterOnAxis, Obj2ExpandedExtents, Obj1BBCenterOnAxis, Obj1TravelAlongAxis, axisEnter, axisExit))
			return false;

		if (axisEnter > tEnter)
			tEnter = axisEnter;
		if (axisExit < tExit)
			tExit = axisExit;
	}

	// Obj2 Z Axis
	{
		Vector3<float> Obj2ZAxisInWorld = i_Obj2.m_Orientation.GetCol(2).GetNorm();

		float Obj2BBCenterOnAxis = Obj2BBCenterInWorld.Dot(Obj2ZAxisInWorld);
		float Obj1BBCenterOnAxis = Obj1BBCenterInWorld.Dot(Obj2ZAxisInWorld);

		float Obj1ProjectedExtents = float(
			fabs(Obj2ZAxisInWorld.Dot( i_Obj1.m_Orientation.GetCol(0).GetNorm() * i_Obj1.m_BB.extent[0])) +
			fabs(Obj2ZAxisInWorld.Dot( i_Obj1.m_Orientation.GetCol(1).GetNorm() * i_Obj1.m_BB.extent[1])) +
			fabs(Obj2ZAxisInWorld.Dot( i_Obj1.m_Orientation.GetCol(2).GetNorm() * i_Obj1.m_BB.extent[2])));

		float Obj2ExpandedExtents = i_Obj2.m_BB.extent[2] + Obj1ProjectedExtents;

		float Obj1TravelAlongAxis = Obj1Travel.Dot(Obj2ZAxisInWorld);

		float axisEnter = 0.0f;
		float axisExit = 1.0f;

		if (!DetectCrossTimes(Obj2BBCenterOnAxis, Obj2ExpandedExtents, Obj1BBCenterOnAxis, Obj1TravelAlongAxis, axisEnter, axisExit))
			return false;

		if (axisEnter > tEnter)
			tEnter = axisEnter;
		if (axisExit < tExit)
			tExit = axisExit;
	}

	// Obj1X x Obj2X
	{
		Vector3<float> Obj1XAxisInWorld = i_Obj1.m_Orientation.GetCol(0);
		Vector3<float> Obj2XAxisInWorld = i_Obj2.m_Orientation.GetCol(0);

		Vector3<float> CollisionAxis = Obj1XAxisInWorld.Cross(Obj2XAxisInWorld).GetNorm();

		float Obj1BBCenterOnAxis = Obj1BBCenterInWorld.Dot( CollisionAxis);
		float Obj2BBCenterOnAxis = Obj2BBCenterInWorld.Dot( CollisionAxis);

		float Obj1ProjectedExtents = float(
			fabs(CollisionAxis.Dot( i_Obj1.m_Orientation.GetCol(0).GetNorm() * i_Obj1.m_BB.extent[0])) +
			fabs(CollisionAxis.Dot( i_Obj1.m_Orientation.GetCol(1).GetNorm() * i_Obj1.m_BB.extent[1])) +
			fabs(CollisionAxis.Dot( i_Obj1.m_Orientation.GetCol(2).GetNorm() * i_Obj1.m_BB.extent[2])));

		float Obj2ProjectedExtents = float(
			fabs(CollisionAxis.Dot( i_Obj2.m_Orientation.GetCol(0).GetNorm() * i_Obj2.m_BB.extent[0])) +
			fabs(CollisionAxis.Dot( i_Obj2.m_Orientation.GetCol(1).GetNorm() * i_Obj2.m_BB.extent[1])) +
			fabs(CollisionAxis.Dot( i_Obj2.m_Orientation.GetCol(2).GetNorm() * i_Obj2.m_BB.extent[2])));

		float Obj2ExpandedExtents = Obj2ProjectedExtents + Obj1ProjectedExtents;

		float Obj1VelOnAxis = i_Obj1.m_Velocity.Dot(CollisionAxis);
		float Obj2VelOnAxis = i_Obj2.m_Velocity.Dot(CollisionAxis);

		float Obj1RelVel = Obj1VelOnAxis - Obj2VelOnAxis;

		float Obj1TravelAlongAxis = Obj1RelVel * i_dt;

		float axisEnter = 0.0f;
		float axisExit = 1.0f;

		if (!DetectCrossTimes(Obj2BBCenterOnAxis, Obj2ExpandedExtents, Obj1BBCenterOnAxis, Obj1TravelAlongAxis, axisEnter, axisExit))
			return false;

		if (axisEnter > tEnter)
			tEnter = axisEnter;
		if (axisExit < tExit)
			tExit = axisExit;
	}

	return tEnter < tExit;
}


void SingleCollisionCheck()
{

	Matrix4<float> mRot = Matrix4<float>::CreateZRotation(DegreesToRadians(90.0f));
	Matrix4<float> mTrans = Matrix4<float>::CreateTranslation(0.0f, 100.0f, 0.0f);
	Matrix4<float> mToWorld = mTrans * mRot;

	Vector4<float> VXAxis(1.0f, 0.0f, 0.0f, 1.0f);

	Vector4<float> VWorld =  mToWorld * VXAxis;

	Matrix4<float> mFromWorld = mToWorld.GetInverseRotTrans();
	Vector4<float> VBack = mFromWorld * VWorld;

	Object	Obj1;

	Obj1.m_Position = Vector3<float>(-1.0f, 0.0f, 0.0f);
	Obj1.m_Velocity = Vector3<float>(1.0f, 0.0f, 0.0f);
	Obj1.m_Orientation = Matrix3<float>::CreateZRotation(45.0f);
	Obj1.m_BB.center = Vector3<float>(0.0f, 0.0f, 0.0f);
	Obj1.m_BB.extent = Vector3<float>(0.5f, 0.5f, 0.5f);

	Object	Obj2;

	Obj2.m_Position = Vector3<float>(1.0f, 0.0f, 0.0f);
	Obj2.m_Velocity = Vector3<float>(-1.0f, 0.0f, 0.0f);
	Obj2.m_Orientation = Matrix3<float>::CreateXRotation(45.0f);
	Obj2.m_BB.center = Vector3<float>(0.0f, 0.0f, 0.0f);
	Obj2.m_BB.extent = Vector3<float>(0.5f, 0.5f, 0.5f);

	float tCollision;

	bool bCollided = CheckCollision(Obj1, Obj2, 1.0f, tCollision);

	printf("Collided: %s\n", bCollided ? "true" : "false");
}


void CollisionPoolCheck()
{

	const float AreaExtent = 10.0f;
	Vector3<float> PositionMin(-AreaExtent, -AreaExtent, -AreaExtent);
	Vector3<float> PositionMax(AreaExtent, AreaExtent, AreaExtent);

	Vector3<float> VelocityMin(-1.0f, -1.0f, -1.0f);
	Vector3<float> VelocityMax(1.0f, 1.0f, 1.0f);

	const size_t numObjects = 100;

	for (size_t i = 0; i < numObjects; i++)
	{
		Object* pObj = new Object;

		pObj->m_Position = GenerateRandomVector(PositionMin, PositionMax);
		pObj->m_Velocity = GenerateRandomVector(VelocityMin, VelocityMax);

		switch (rand() & 0x03)
		{
		case 01:
			pObj->m_Orientation = Matrix3<float>::CreateXRotation(RandInRange(0.0f, 90.0f));
			break;
		case 02:
			pObj->m_Orientation = Matrix3<float>::CreateYRotation(RandInRange(0.0f, 90.0f));
			break;
		case 03:
			pObj->m_Orientation = Matrix3<float>::CreateZRotation(RandInRange(0.0f, 90.0f));
			break;
		default:
			pObj->m_Orientation = Matrix3<float>::CreateIdentity();
			break;
		}

		pObj->m_BB.center = Vector3<float>(0.0f, 0.0f, 0.0f);
		pObj->m_BB.extent = Vector3<float>(0.5f, 0.5f, 0.5f);

		m_CollisionObjects.push_back(pObj);
	}


	for (int i = 0; i < 10; i++)
	{
		CollisionPair Pair = FindCollision(RandInRange(0.1f, 1.0f));
	}

	//while (1)
	//	CollisionPair Pair = FindCollision(RandInRange(0.1f, 1.0f));
}


