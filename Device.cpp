
#include "stdafx.h"
#include "Device.h"


using namespace device;


/*///////////////////////////////////////////////////////////////////////////////////

	�� ��ġ������ �����ϱ� ���� �ڵ��� ����մϴ�.
	�� �������� �� �Լ��� ȣ���ؾ� �մϴ�.
	�� ��� ����� ��ģ �� Detach�� ȣ���Ͽ� ���ҽ��� ��ȯ�ؾ��մϴ�.

/*///////////////////////////////////////////////////////////////////////////////////
LSTATUS CDeviceControl::Attach
(
	PCONN_DEV pConnDev
)
{
	LSTATUS lsRet = NO_ERROR;

	if (!pConnDev) {
		return lsRet;
	}

	pConnDev->hInfo = SetupDiGetClassDevs(m_cpGuid, 0, 0, DIGCF_PRESENT);

	if (pConnDev->hInfo == INVALID_HANDLE_VALUE) {
		Detach(pConnDev);
		return lsRet;
	}

	pConnDev->pInfoData = new SP_DEVINFO_DATA;
	
	if (!pConnDev->pInfoData) {
		Detach(pConnDev);
		return lsRet;
	}

	pConnDev->pInfoData->cbSize = sizeof(SP_DEVINFO_DATA);

	return lsRet;
}


/*///////////////////////////////////////////////////////////////////////////////////

	 �� ��ġ���Ÿ� ����� �� ���ҽ��� ��ȯ�� �� �� �Լ��� ȣ���մϴ�.

/*///////////////////////////////////////////////////////////////////////////////////
void CDeviceControl::Detach
(
	PCONN_DEV pConnDev
)
{
	if (pConnDev) {
		if ((pConnDev->hInfo) &&
			pConnDev->hInfo != INVALID_HANDLE_VALUE) {
			SetupDiDestroyDeviceInfoList(pConnDev->hInfo);
			pConnDev->hInfo = NULL;
		}

		if (pConnDev->pInfoData) {
			delete pConnDev->pInfoData;
			pConnDev->pInfoData = NULL;
		}

		delete pConnDev;
		pConnDev = NULL;
	}
}


/*///////////////////////////////////////////////////////////////////////////////////

	 �� ��ġ ���/�̻������ ������ �� �ֽ��ϴ�.
	 �� DICS_START/DICS_STOP

/*///////////////////////////////////////////////////////////////////////////////////
BOOL CDeviceControl::ChangeState
(
	DWORD		dwState,
	PCONN_DEV	pConnDev
)
{
	BOOL				 bSuccess = TRUE;
	SP_PROPCHANGE_PARAMS param;


	param.ClassInstallHeader.cbSize = sizeof(SP_CLASSINSTALL_HEADER);
	param.ClassInstallHeader.InstallFunction = DIF_PROPERTYCHANGE;	// �Ӽ� ���� �Լ��� ���
	param.Scope = DICS_FLAG_CONFIGSPECIFIC;	// ���� ���� ��ġ���� ����
	param.StateChange = dwState;			// ��ġ ���� ����
	param.HwProfile = 0;

	if (!SetupDiSetClassInstallParams(
		pConnDev->hInfo, pConnDev->pInfoData,
		(PSP_CLASSINSTALL_HEADER)&param, sizeof(param))) {
		bSuccess = FALSE;
	}

	// ���泻�� ����
	if (!SetupDiCallClassInstaller(
		DIF_PROPERTYCHANGE,
		pConnDev->hInfo, pConnDev->pInfoData)) {
		// 0xE0000235 64bit�� ���α׷����� �缳ġ �� ���
		// 0xE000020B �߸��� �ڵ� ��
		// 0xE0000215 �߸��� �Ķ���� ��
		bSuccess = FALSE;
	}
	
	return bSuccess;
}


/*///////////////////////////////////////////////////////////////////////////////////

	 �� ��ġ�� �����մϴ�.
	 �� ���� �� ���� ������ ������ ���� EnumProc�� �����մϴ�.
		EnumProc�� CDeviceControl�� ���������Լ��� ��ӹ��� �ڽ�Ŭ�������� �߰���
		�����ؾ� �մϴ�.

/*///////////////////////////////////////////////////////////////////////////////////
DEVICE_STATE CDeviceControl::EnumDevice
(
	LPARAM lParam
)
{
	PCONN_DEV	 pConnDev = new CONN_DEV;
	DEVICE_STATE dsRet = DV_DEFAULT;


	if (Attach(pConnDev) != NO_ERROR) {
		return DV_DEFAULT;
	}

	// Ŭ���� ����� i��° ���� ����
	for (UINT i = 0; SetupDiEnumDeviceInfo(pConnDev->hInfo, i, pConnDev->pInfoData); ++i) {
		DEVICE_STATE dvStt = EnumProc(pConnDev, lParam);

		if (dvStt != DV_DEFAULT) {
			dsRet = dvStt;
		}
	}

	Detach(pConnDev);

	return dsRet;
}
