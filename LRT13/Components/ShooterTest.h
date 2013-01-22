#ifndef SHOOTER_TEST_H_
#define SHOOTER_TEST_H_

#include "../Utils/UnitTest.h"
#include "Shooter.h"

class ShooterTest : public UnitTest
{
public:
	ShooterTest();
	~ShooterTest();
	
	void Run();
	
private:
	Shooter* m_shooterComponent;
};

#endif
