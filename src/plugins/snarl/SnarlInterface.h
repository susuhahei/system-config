#ifndef SNARL_INTERFACE
#define SNARL_INTERFACE

#include <tchar.h>
#include <windows.h>
#include <strsafe.h>


namespace Snarl {

	static const LPCTSTR SNARL_GLOBAL_MSG = _T("SnarlGlobalEvent");
	static const LPCTSTR SNARL_APP_MSG    = _T("SnarlAppMessage");

	static const int SNARL_STRING_LENGTH = 1024;
	static const int SNARL_UNICODE_LENGTH = SNARL_STRING_LENGTH / 2;

        static const intptr_t SNARL_LAUNCHED = 1;                // Snarl has just started running
        static const intptr_t SNARL_QUIT = 2;                    // Snarl is about to stop running
        static const intptr_t SNARL_ASK_APPLET_VER = 3;          // (R1.5) Reserved for future use
        static const intptr_t SNARL_SHOW_APP_UI = 4;             // (R1.6) Application should show its UI

        static const intptr_t SNARL_NOTIFICATION_CLICKED = 32;   // notification was right-clicked by user
        static const intptr_t SNARL_NOTIFICATION_TIMED_OUT = 33;
        static const intptr_t SNARL_NOTIFICATION_ACK = 34;       // notification was left-clicked by user
        static const intptr_t SNARL_NOTIFICATION_MENU = 35;           // V39 - menu item selected
        static const intptr_t SNARL_NOTIFICATION_MIDDLE_BUTTON = 36;  // V39 - notification middle-clicked by user
        static const intptr_t SNARL_NOTIFICATION_CLOSED = 37;         // V39 - user clicked the close gadget

        static const intptr_t SNARL_NOTIFICATION_CANCELLED = SNARL_NOTIFICATION_CLICKED;  // Added in R1.6

	static const DWORD WM_SNARLTEST = WM_USER + 237;    // note hardcoded WM_USER value!

	// --------------------------------------------------------------------
	
        enum M_RESULT {
		M_ABORTED         = 0x80000007,
		M_ACCESS_DENIED   = 0x80000009,
		M_ALREADY_EXISTS  = 0x8000000C,
		M_BAD_HANDLE      = 0x80000006,
		M_BAD_POINTER     = 0x80000005,
		M_FAILED          = 0x80000008,
		M_INVALID_ARGS    = 0x80000003,
		M_NO_INTERFACE    = 0x80000004,
		M_NOT_FOUND       = 0x8000000B,
		M_NOT_IMPLEMENTED = 0x80000001,
		M_OK              = 0x00000000,
		M_OUT_OF_MEMORY   = 0x80000002,
		M_TIMED_OUT       = 0x8000000A
	} ;

	enum SNARL_COMMANDS {
		SNARL_SHOW = 1,
		SNARL_HIDE,
		SNARL_UPDATE,
		SNARL_IS_VISIBLE,
		SNARL_GET_VERSION,
		SNARL_REGISTER_CONFIG_WINDOW,
		SNARL_REVOKE_CONFIG_WINDOW,

		/* R1.6 onwards */
		SNARL_REGISTER_ALERT,
		SNARL_REVOKE_ALERT,   // for future use
		SNARL_REGISTER_CONFIG_WINDOW_2,
		SNARL_GET_VERSION_EX,
		SNARL_SET_TIMEOUT,
		
		/* following introduced in Snarl V39 (R2.1) */
		SNARL_SET_CLASS_DEFAULT,
		SNARL_CHANGE_ATTR,
		SNARL_REGISTER_APP,
		SNARL_UNREGISTER_APP,
		SNARL_ADD_CLASS,

		/* extended commands (all use SNARLSTRUCTEX) */
		SNARL_EX_SHOW = 0x20,
		SNARL_SHOW_NOTIFICATION                // V39
	};
	
	static const SNARL_COMMANDS SNARL_GET_REVISION = SNARL_REVOKE_ALERT;
	
        enum SNARL_APP_FLAGS {
		SNARL_APP_HAS_PREFS = 1,
		SNARL_APP_HAS_ABOUT = 2
	};
	
        static const intptr_t SNARL_APP_PREFS = 1;
        static const intptr_t SNARL_APP_ABOUT = 2;

	
	/* --------------- V39 additions --------------- */
	
	/* snAddClass() flags */
	enum SNARL_CLASS_FLAGS {
		SNARL_CLASS_ENABLED = 0,
		SNARL_CLASS_DISABLED = 1,
		SNARL_CLASS_NO_DUPLICATES = 2,         // means Snarl will suppress duplicate notifications
		SNARL_CLASS_DELAY_DUPLICATES = 4       // means Snarl will suppress duplicate notifications within a pre-set time period
	};

	/* Class attributes */
        enum SNARL_ATTRIBUTES {
		SNARL_ATTRIBUTE_TITLE = 1,
		SNARL_ATTRIBUTE_TEXT,
		SNARL_ATTRIBUTE_ICON,
		SNARL_ATTRIBUTE_TIMEOUT,
		SNARL_ATTRIBUTE_SOUND,
		SNARL_ATTRIBUTE_ACK,               // file to run on ACK
		SNARL_ATTRIBUTE_MENU
	};

	/* ------------------- end of V39 additions ------------------ */
	
	struct SNARLSTRUCT {
		SNARL_COMMANDS Cmd;
                intptr_t Id;
                intptr_t Timeout;
                intptr_t LngData2;
		char Title[SNARL_STRING_LENGTH];
		char Text[SNARL_STRING_LENGTH];
		char Icon[SNARL_STRING_LENGTH];
	};

	struct SNARLSTRUCTEX {
		SNARL_COMMANDS Cmd;
                intptr_t Id;
                intptr_t Timeout;
                intptr_t LngData2;
		char Title[SNARL_STRING_LENGTH];
		char Text[SNARL_STRING_LENGTH];
		char Icon[SNARL_STRING_LENGTH];

		char Class[SNARL_STRING_LENGTH];
		char Extra[SNARL_STRING_LENGTH];
		char Extra2[SNARL_STRING_LENGTH];
                intptr_t Reserved1;
                intptr_t Reserved2;
	};


	
	// ------------------------------------------------------------------------
	// SnarlInterface class definition
	// ------------------------------------------------------------------------
	
	class SnarlInterface {
		public:
			SnarlInterface();
			~SnarlInterface();

			static HWND   GetSnarlWindow();		
                        static intptr_t GetGlobalMsg();

			
			LPTSTR AllocateString(size_t n) { return new TCHAR[n]; }
			void FreeString(LPCTSTR str)    { delete [] str; str = NULL; }
			

                        intptr_t  ShowMessage(LPCSTR szTitle, LPCSTR szText, intptr_t timeout = 0, LPCSTR szIconPath = "", HWND hWndReply = NULL, WPARAM uReplyMsg = 0);
                        intptr_t  ShowMessage(LPCWSTR szTitle, LPCWSTR szText, intptr_t timeout = 0, LPCWSTR szIconPath = L"", HWND hWndReply = NULL, WPARAM uReplyMsg = 0);
                        intptr_t  ShowMessageEx(LPCSTR szClass, LPCSTR szTitle, LPCSTR szText, intptr_t timeout = 0, LPCSTR szIconPath = "", HWND hWndReply = NULL, WPARAM uReplyMsg = 0, LPCSTR szSoundFile = "");
                        intptr_t  ShowMessageEx(LPCWSTR szClass, LPCWSTR szTitle, LPCWSTR szText, intptr_t timeout = 0, LPCWSTR szIconPath = L"", HWND hWndReply = NULL, WPARAM uReplyMsg = 0, LPCWSTR szSoundFile = L"");

			LPCTSTR GetAppPath();    // ** Remember to FreeString when finished with the string !
			LPCTSTR GetIconsPath();  // ** Remember to FreeString when finished with the string !

			BOOL      GetVersion(WORD* Major, WORD* Minor);
                        intptr_t    GetVersionEx();
			BOOL      HideMessage();
                        BOOL      HideMessage(intptr_t Id);
			BOOL      IsMessageVisible();
                        BOOL      IsMessageVisible(intptr_t Id);
			M_RESULT  RegisterAlert(LPCSTR szAppName, LPCSTR szClass);
			M_RESULT  RegisterAlert(LPCWSTR szAppName, LPCWSTR szClass);
                        M_RESULT  RegisterConfig(HWND hWnd, LPCSTR szAppName, intptr_t replyMsg);
                        M_RESULT  RegisterConfig(HWND hWnd, LPCWSTR szAppName, intptr_t replyMsg);
                        M_RESULT  RegisterConfig2(HWND hWnd, LPCSTR szAppName, intptr_t replyMsg, LPCSTR szIcon);
                        M_RESULT  RegisterConfig2(HWND hWnd, LPCWSTR szAppName, intptr_t replyMsg, LPCWSTR szIcon);
			M_RESULT  RevokeConfig(HWND hWnd);
                        M_RESULT  SetTimeout(intptr_t Timeout);
                        M_RESULT  SetTimeout(intptr_t Id, intptr_t Timeout);
			M_RESULT  UpdateMessage(LPCSTR szTitle, LPCSTR szText, LPCSTR szIconPath = "");
			M_RESULT  UpdateMessage(LPCWSTR szTitle, LPCWSTR szText, LPCWSTR szIconPath = L"");
                        M_RESULT  UpdateMessage(intptr_t Id, LPCSTR szTitle, LPCSTR szText, LPCSTR szIconPath = "");
                        M_RESULT  UpdateMessage(intptr_t Id, LPCWSTR szTitle, LPCWSTR szText, LPCWSTR szIconPath = L"");
			
			/* V39 */
                        M_RESULT  AddClass(LPCSTR Class, LPCSTR Description = NULL, SNARL_CLASS_FLAGS Flags = SNARL_CLASS_ENABLED, LPCSTR DefaultTitle = NULL, LPCSTR DefaultIcon = NULL, intptr_t DefaultTimeout = 0);
                        M_RESULT  AddClass(LPCWSTR Class, LPCWSTR Description = NULL, SNARL_CLASS_FLAGS Flags = SNARL_CLASS_ENABLED, LPCWSTR DefaultTitle = NULL, LPCWSTR DefaultIcon = NULL, intptr_t DefaultTimeout = 0);
			M_RESULT  ChangeAttribute(SNARL_ATTRIBUTES Attr, LPCSTR Value);
			M_RESULT  ChangeAttribute(SNARL_ATTRIBUTES Attr, LPCWSTR Value);
                        M_RESULT  ChangeAttribute(intptr_t Id, SNARL_ATTRIBUTES Attr, LPCSTR Value);
                        M_RESULT  ChangeAttribute(intptr_t Id, SNARL_ATTRIBUTES Attr, LPCWSTR Value);
                        intptr_t    GetAppMsg();
                        intptr_t    GetRevision();
			
                        M_RESULT  RegisterApp(LPCSTR Application, LPCSTR SmallIcon, LPCSTR LargeIcon, HWND hWnd = 0, intptr_t ReplyMsg = 0);
                        M_RESULT  RegisterApp(LPCWSTR Application, LPCWSTR SmallIcon, LPCWSTR LargeIcon, HWND hWnd = 0, intptr_t ReplyMsg = 0);
			void      SetAsSnarlApp(HWND hWndOwner, SNARL_APP_FLAGS Flags = (SNARL_APP_FLAGS)(SNARL_APP_HAS_ABOUT | SNARL_APP_HAS_PREFS));
			M_RESULT  SetClassDefault(LPCSTR Class, SNARL_ATTRIBUTES Attr, LPCSTR Value);
			M_RESULT  SetClassDefault(LPCWSTR Class, SNARL_ATTRIBUTES Attr, LPCWSTR Value);
                        intptr_t    ShowNotification(LPCSTR Class, LPCSTR Title = NULL, LPCSTR Text = NULL, intptr_t Timeout = 0, LPCSTR Icon = NULL, HWND hWndReply = NULL, intptr_t uReplyMsg = 0, LPCSTR Sound = NULL);
                        intptr_t    ShowNotification(LPCWSTR Class, LPCWSTR Title = NULL, LPCWSTR Text = NULL, intptr_t Timeout = 0, LPCWSTR Icon = NULL, HWND hWndReply = NULL, intptr_t uReplyMsg = 0, LPCWSTR Sound = NULL);
			M_RESULT  UnregisterApp();
			
                        intptr_t    GetLastMessageId() { return m_nLastMessageId; }

		private:
                        template <class T> intptr_t Send(T ss);
			LPSTR  WideToUTF8(LPCWSTR szWideStr);
			
                        intptr_t m_nLastMessageId;
			HWND   m_hwndFrom; // set during snRegisterConfig() or snRegisterConfig2()

	};

}

#endif // SNARL_INTERFACE
