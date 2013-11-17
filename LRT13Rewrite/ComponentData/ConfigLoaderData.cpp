#include "ConfigLoaderData.h"

using namespace data::configloader;

ConfigLoaderData::ConfigLoaderData()
{
	m_loadRequested = m_saveRequested = m_applyRequested = false;
}

bool ConfigLoaderData::IsLoadRequested()
{
	return m_loadRequested;
}

bool ConfigLoaderData::IsSaveRequested()
{
	return m_saveRequested;
}

bool ConfigLoaderData::IsApplyRequested()
{
	return m_applyRequested;
}

void ConfigLoaderData::RequestLoad()
{
	m_loadRequested = true;
}

void ConfigLoaderData::RequestSave()
{
	m_saveRequested = true;
}

void ConfigLoaderData::RequestApply()
{
	m_applyRequested = true;
}

void ConfigLoaderData::RemoveLoadRequest()
{
	m_loadRequested = false;
}

void ConfigLoaderData::RemoveSaveRequest()
{
	m_saveRequested = false;
}

void ConfigLoaderData::RemoveApplyRequest()
{
	m_applyRequested = false;
}
