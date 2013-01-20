#include "RobotData.h"

#warning proposed code and needs implementation

using namespace data;

int RobotData::GetMissedPacketsInLifetime() { return 0; }
void RobotData::IncrementMissedPacketsInLifetime() { return; }

int RobotData::GetCycleCount() { return 0; }
void RobotData::IncrementCycleCount() { return; }

RobotData::RobotState RobotData::GetCurrentState() { return m_state; }
void RobotData::SetRobotState(RobotData::RobotState state) { m_state = state; }

int RobotData::AllocateKey() { return 0; }
void RobotData::AddValue(int key, string type, string serialized) { return; }
