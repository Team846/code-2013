/*
 * AutoClimb.cpp
 *
 *  Created on: Apr 12, 2013
 *      Author: Tony
 */

#include "AutoClimb.h"

AutoClimb* AutoClimb::m_instance = NULL;

AutoClimb* AutoClimb::Instance()
{
	if(m_instance == NULL)
		m_instance = new AutoClimb();
	
	return m_instance;
}

AutoClimb::AutoClimb()
	: AsyncProcess("AutoClimb"),
	  Configurable()
{
	m_componentData = ComponentData::GetInstance();
	m_climber = m_componentData->climberData;
	
	m_imu = IMU::Instance();
	
	m_climbLevel = 0;
	m_climbRequested = m_climbForce = false;
}

AutoClimb::~AutoClimb()
{
	if(m_isRunning)
		Abort();
}

void AutoClimb::ClimbOneLevel(bool force)
{
	if(m_aborted)
	{
		AsyncPrinter::Printf("AutoClimb was aborted.  To reset, reset AutoClimb.\n");
		return;
	}
	
	m_climbRequested = true;
	m_climbForce = force;
}

INT32 AutoClimb::Tick()
{
	m_climber->enableDebug();
	
//	while(true)
//	{
//		if(m_climbRequested)
//		{
//			if(m_climbLevel == 3 && !m_climbForce)
//			{
//				AsyncPrinter::Printf("Already at the highest level!\n");
//				continue;
//			}
//			
//			m_climbRequested = false;
//			m_climbForce = false;
//			
//			m_climber->extendHooks();
//		
//			// do some prep
//			m_componentData->shooterData->SetLauncherAngleLow();
//			
//			// we only want to disengage the PTO if we're on the ground
//			if(m_climbLevel == 0)
//				m_climber->DisengagePTO();
//			else
//			{
//				m_climber->extendArm();
//				// give it some time
//				Wait(1.0);
//				
//				// start the winch pawl
//				double startTime = Timer::GetFPGATimestamp();
//				
//				m_climber->winchPawlUp();
//
//				int overCount = 0;
//				
//				while(true)
//				{
//					if(Timer::GetFPGATimestamp() - startTime > 5.0) // TODO: don't hardcode this
//					{
//						AsyncPrinter::Printf("AutoClimb::Tick(): Winch pawl timeout! Aborting...\n");
//						m_climber->winchPawlInactive();
//						// shut down
//						AbortClimb(WINCH_PAWL_TIMEOUT, false, ABORT_NEXT_LEVEL);
//						m_isRunning = false;
//						return WINCH_PAWL_TIMEOUT;
//					}
//					
//					if(m_climber->winchPawlCurrent() > m_winchPawlClimbThreshold)
//					{
//						if(m_climber->winchPawlCurrent() > m_winchPawlAbortThreshold)
//						{
//							AsyncPrinter::Printf("AutoClimb::Tick(): Winch pawl over-current! Aborting...\n");
//							m_climber->winchPawlInactive();
//							// shut down
//							AbortClimb(WINCH_PAWL_OVER_CURRENT, false, ABORT_NEXT_LEVEL);
//							m_isRunning = false;
//							return WINCH_PAWL_OVER_CURRENT;
//						}
//						
//						if(++overCount > 10)
//						{
//							m_climber->winchPawlInactive();
//							break;
//						}
//					}
//				}
//			}
//			
//			// point of no return...
//			m_canAbort = false;
//			
//			m_canAbort = true;
//		}
//		
//		Wait(0.1);
//	}
//	
//	Abort();
	
	return 0;
}

void AutoClimb::Configure()
{
	
}

void AutoClimb::Reset()
{
	m_aborted = false;
	Start();
}

void AutoClimb::UserAbort(bool force, ABORT_MODE severity)
{	
	AbortClimb(USER_ABORT, force);
}

void AutoClimb::AbortClimb(ABORT_CODE errorCode, bool force, ABORT_MODE severity)
{
	if(!CanAbort())
	{
		if(force)
		{
			AsyncPrinter::Printf("Forced abort requested. Here we go...\n");
		}
		else
		{
			AsyncPrinter::Printf("Can't abort at this time.\n");
			return;
		}
	}
	
	m_aborted = true;
	
	Abort(errorCode, 0.005);
	
	if(severity == LOCKDOWN)
	{
		
	}
}

bool AutoClimb::CanAbort()
{
	return m_canAbort;
}
