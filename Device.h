
#pragma once


#include <SetupAPI.h>


#pragma comment(lib, "setupapi.lib")


#define INSTANCEID_MAXSIZE	50
#define DEVICEID_MAXSIZE	31

// USB ����� GUID�� �Դϴ�.
DEFINE_GUID(GUID_DEVCLASS_USB, 0x36fc9e60L, 0xc465, 0x11cf, 0x80, 0x56, 0x44, 0x45, 0x53, 0x54, 0x00, 0x00);


namespace device {
	// ��ġ���� ENUM
	enum DEVICE_STATE {
		DV_DEFAULT	= 0x0000,
		DV_MOUNT	= 0x0001,
		DV_UNMOUNT	= 0x0002,
		DV_REMOVE	= 0x0012,
		DV_DISCONNECT = 0x0022
	};

	typedef struct tagEXC_LISTW {
		TCHAR szHardwareId[DEVICEID_MAXSIZE];
		TCHAR szReqDate[11];
		TCHAR szDesc[131];
		BYTE  uState;
	} EXC_LISTW, *PEXC_LISTW;

	typedef struct tagEXC_INFO {
		ULONG		ulSize;
		PEXC_LISTW	pExcList;
		UINT		uPolicy;
	} EXC_INFO, *PEXC_INFO;

	typedef struct tagCONNECT_DEV {
		HDEVINFO hInfo;
		PSP_DEVINFO_DATA pInfoData;
	} CONN_DEV, *PCONN_DEV;


	class CDeviceControl
	{
	public:

		CDeviceControl()
		{
			m_cpGuid = NULL;
		}


	protected:

		// ��ġ�� GUID
		// �ڽ�Ŭ�������� �߰��� �ʱ�ȭ�մϴ�.
		const GUID *m_cpGuid;


		// ������ ��ġ�� ���¸� �����մϴ�.
		BOOL ChangeState(DWORD IN dwState, PCONN_DEV IN OUT pConnDev);
		// ��ġ������ �����ϱ� ���� �ڵ鿡 ����մϴ�.
		// ����ϱ� �� pConnDev�� �޸𸮰� �Ҵ�Ǿ� �־�� �մϴ�.
		LSTATUS Attach(PCONN_DEV OUT pConnDev);
		// ����� ��ġ�� �и��մϴ�.
		void Detach(PCONN_DEV IN pConnDev = NULL);
		// ��ġ���� �Լ��Դϴ�.
		DEVICE_STATE EnumDevice(LPARAM IN OUT lParam = NULL);
		// ��ġ���� �Լ����� ������ ���� ���⿡ �����մϴ�.
		virtual DEVICE_STATE EnumProc(PCONN_DEV IN OUT pConnDev, LPARAM IN OUT lParam) = 0;
	};
}
